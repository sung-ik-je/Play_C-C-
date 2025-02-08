package main

import (
	"encoding/json"
	"fmt"
	"io"
	"log"
	"net/http"
	"os"
	"path/filepath"
	"strings"
	"sync"
	"time"
)

type JobID int
type JobState int
type FileType int
type FileExtension int

const (
	ImageProcess JobID = iota
	TextAnalyze
	FileDownload
)

const (
	Ready JobState = iota
	Complete
	Fail
	Retry
)

const (
	Txt FileExtension = iota
	Pdf
	Jpg
	Png
)

const (
	Image FileType = iota
	Text
)

type Job struct {
	ID          int
	JobId       JobID
	Status      JobState
	JobData     interface{}
	RequestTime time.Time
	RetryTime   time.Time
}

// JobData
type JobDataFile struct {
	Type      FileType
	Extension FileExtension
	Data      interface{}
}

// JobDataFile->Data
type ImageMetaData struct {
	width  int
	height int
}

// JobData, Json 직렬화 필요
type JobDataJson struct {
	FileUrl []string `json:"file_url"`
}

type JobQueue struct {
	queue chan Job
}

var wg sync.WaitGroup
var jobQueue *JobQueue
var numWorkers = 10

func NewJobQueue(bufferSize int) *JobQueue {
	return &JobQueue{
		queue: make(chan Job, bufferSize),
	}
}

func (jq *JobQueue) Enqueue(job Job) {
	jq.queue <- job
}

func (jq *JobQueue) Dequeue() Job {
	return <-jq.queue
}

func worker(id int, jobQueue *JobQueue, wg *sync.WaitGroup) {
	defer wg.Done()
	for job := range jobQueue.queue {
		fmt.Printf("Worker %d processing Job %d: %s\n", id, job.ID, job.Message)
		time.Sleep(1 * time.Second) // 작업을 처리하는 시뮬레이션
	}
}

// func job_handler(w http.ResponseWriter, r *http.Request) {
func job_handler(r *http.Request, request_file_type string) {
	var job Job

	var decoder *json.Decoder

	now := time.Now()

	switch request_file_type {
	case "json":
		decoder = json.NewDecoder(r.Body)
		err := decoder.Decode(&job)
		if err != nil {
			http.Error(w, "잘못된 JSON 데이터", http.StatusBadRequest)
			return
		}
	case "multipart":
		err := r.ParseMultipartForm(10 << 20)
		if err != nil {
			http.Error(w, "파일 파싱 실패", http.StatusBadRequest)
			return
		}
		file, header, err := r.FormFile("file")
		if err != nil {
			http.Error(w, "파일 가져오기 실패", http.StatusBadRequest)
			return
		}
		defer file.Close()

		// 저장할 파일 경로 설정
		savePath := filepath.Join("uploads", header.Filename)
		outFile, err := os.Create(savePath)
		if err != nil {
			http.Error(w, "파일 저장 실패", http.StatusInternalServerError)
			return
		}
		defer outFile.Close()

		// 파일 저장
		_, err = io.Copy(outFile, file)
		if err != nil {
			http.Error(w, "파일 저장 중 오류 발생", http.StatusInternalServerError)
			return
		}

		// 이미지 크기 가져오기
		width, height, err := getImageSize(savePath)
		if err != nil {
			http.Error(w, "이미지 크기 추출 실패", http.StatusInternalServerError)
			return
		}

		// Job 생성 후 큐에 추가
		job := Job{
			FilePath: savePath,
			Width:    width,
			Height:   height,
		}
		jobQueue.Enqueue(job)

		fmt.Fprintf(w, "파일이 업로드 및 큐에 추가됨: %s (%d x %d)", savePath, width, height)
	}

	decoder = json.NewDecoder(r.Body)

	jobQueue.Enqueue(job)

	// fmt.Fprintf(w, "작업이 큐에 추가되었습니다: ID=%d, Message=%s", job.ID, job.Message)
	fmt.Println("큐에 작업 추가됨:", job)
}

func create_worker() {
	for i := 1; i <= numWorkers; i++ {
		wg.Add(1)
		go worker(i, jobQueue, &wg)
	}
}

func requestHandler(w http.ResponseWriter, r *http.Request) {
	contentType := r.Header.Get("Content-Type")

	if strings.HasPrefix(contentType, "multipart/form-data") {
		fmt.Println("Multipart 요청")
		job_handler(r, "multipart")
	} else if strings.HasPrefix(contentType, "application/json") {
		fmt.Println("JSON 요청")
		job_handler(r, "json")
	} else {
		fmt.Println("⚠️ 지원되지 않는 요청 형식:", contentType)
		http.Error(w, "Unsupported Content-Type", http.StatusUnsupportedMediaType)
		return
	}

	w.WriteHeader(http.StatusOK)
	fmt.Fprintln(w, "요청이 성공적으로 처리되었습니다!")
}

func main() {
	jobQueue = NewJobQueue(100)

	// 서버 열기 전에 worker 초기화 필요
	create_worker()

	http.HandleFunc("/request", requestHandler)

	fmt.Println("Server is listening on http://localhost:8080")
	err := http.ListenAndServe(":8080", nil)
	if err != nil {
		log.Fatal("Error starting server:", err)
	}

	wg.Wait()
}
