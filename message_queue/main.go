package main

import (
	"encoding/json"
	"fmt"
	"hash/fnv"
	"image"
	"image/jpeg"
	"image/png"
	"io"
	"log"
	"mime/multipart"
	"net/http"
	"os"
	"path/filepath"
	"strconv"
	"strings"
	"sync"
	"time"

	"github.com/chai2010/webp"
	"golang.org/x/image/draw"
)

var reauest_id int = 0

type JobID int
type JobState int
type FileType int
type FileExtension int

const (
	SUCCESS = iota
	FAIL
)

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

// 현재는 이미지 파일 이름 해시를 통한 디렉토리 생성
func hashFNV(s string) string {
	h := fnv.New32a()
	h.Write([]byte(s))
	return strconv.FormatUint(uint64(h.Sum32()), 10)
}

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
		fmt.Printf("Worker %d processing Job %d: %d\n", id, job.ID, job.JobId)
		time.Sleep(1 * time.Second) // 작업을 처리하는 시뮬레이션
	}
}

// jobData
func parsing_file_metadata(r *http.Request, j interface{}) int {
	file_type := r.FormValue("File-Type")
	file_extension := r.FormValue("File-Extension")

	if jobDataFileInstance, ok := j.(*JobDataFile); ok {
		if strings.HasPrefix(file_type, "Image") {
			fmt.Println("Image File")
			jobDataFileInstance.Type = Image

			width, err := strconv.Atoi(r.FormValue("Width"))
			if err != nil {
				// http.Error(w, "Invalid number", http.StatusBadRequest)
				return 1
			}

			height, err := strconv.Atoi(r.FormValue("Height"))
			if err != nil {
				// http.Error(w, "Invalid number", http.StatusBadRequest)
				return 1
			}

			jobDataFileInstance.Data = ImageMetaData{width, height}
		} else {
			jobDataFileInstance.Type = Text
		}

		switch file_extension {
		case "Text":
			jobDataFileInstance.Extension = Txt
		case "Pdf":
			jobDataFileInstance.Extension = Pdf
		case "Jpg":
			jobDataFileInstance.Extension = Jpg
		case "Png":
			jobDataFileInstance.Extension = Png
		}
	} else {
		fmt.Println("타입 어설션 실패 1")
	}

	return 0
}

func resizeImage(file multipart.File, outputPath, ext string, newWidth, newHeight int) error {
	fmt.Printf("Image Resizing : %dx%d\n", newWidth, newHeight)

	// file pointer를 초기화해줘야 resizeImage func의 여러 호출을 이용할 수 있다
	_, err := file.Seek(0, io.SeekStart)
	if err != nil {
		return fmt.Errorf("파일 포인터 초기화 실패: %v", err)
	}

	var img image.Image
	switch ext {
	case "jpg", "jpeg":
		img, err = jpeg.Decode(file)
		fmt.Println("Image Extention JPG, JPEG")
	case "png":
		img, err = png.Decode(file)
		fmt.Println("Image Extention PNG")
	case "webp":
		img, err = webp.Decode(file)
		fmt.Println("Image Extention WEBP")
	default:
		return fmt.Errorf("지원하지 않는 확장자: %s", ext)
	}

	if err != nil {
		fmt.Println("이미지 리사이징 실패")
		return fmt.Errorf("이미지 디코딩 실패: %v", err)
	}

	newImg := image.NewRGBA(image.Rect(0, 0, newWidth, newHeight))
	draw.CatmullRom.Scale(newImg, newImg.Bounds(), img, img.Bounds(), draw.Over, nil)

	outputPath += "/Image_" + strconv.Itoa(newWidth) + "_" + strconv.Itoa(newHeight) + "." + ext
	fmt.Println("outputPath : ", outputPath)
	outFile, err := os.Create(outputPath)
	if err != nil {
		fmt.Println("출력 파일 생성 실패")
		return fmt.Errorf("출력 파일 생성 실패: %v", err)
	}
	defer outFile.Close()

	switch ext {
	case "jpg", "jpeg":
		err = jpeg.Encode(outFile, newImg, &jpeg.Options{Quality: 90})
	case "png":
		err = png.Encode(outFile, newImg)
	case "webp":
		err = webp.Encode(outFile, newImg, &webp.Options{Lossless: false, Quality: 90})
	}

	if err != nil {
		fmt.Println("이미지 저장 실패")
		return fmt.Errorf("이미지 저장 실패: %v", err)
	}

	fmt.Println("리사이징 완료:", outputPath)
	return nil
}

func file_parser(r *http.Request, j interface{}) {
	result := parsing_file_metadata(r, j)
	if result == FAIL {
		// return nil
	}

	err := r.ParseMultipartForm(10 << 20)
	if err != nil {
		// http.Error(w, "파일 파싱 실패", http.StatusBadRequest)
		// return nil
	}
	file, header, err := r.FormFile("file")
	if err != nil {
		// http.Error(w, "파일 가져오기 실패", http.StatusBadRequest)
		// return nil
	}
	defer file.Close()

	file_name_h := hashFNV(header.Filename)
	savePath := filepath.Join("storage", file_name_h)
	if err := os.MkdirAll(savePath, os.ModePerm); err != nil {
		panic(err)
	}

	file_extension := r.FormValue("File-Extenstion")

	resizeImage(file, savePath, file_extension, 30, 30)
	resizeImage(file, savePath, file_extension, 100, 100)
	resizeImage(file, savePath, file_extension, 500, 500)
}

// 매개변수 j는 구조체 포인터 변수
func json_parser(r *http.Request, j interface{}) {
	decoder := json.NewDecoder(r.Body)

	if jobDataJsonInstance, ok := j.(*JobDataJson); ok {
		err := decoder.Decode(&jobDataJsonInstance)
		if err != nil {
			// http.Error(w, "잘못된 JSON 데이터", http.StatusBadRequest)
		}
	}
}

// func job_handler(w http.ResponseWriter, r *http.Request) {
func job_handler(r *http.Request, request_file_type string) {
	var job Job
	job.ID = reauest_id
	job.Status = Ready
	job.RequestTime = time.Now()

	switch request_file_type {
	case "json":
		job.JobId = FileDownload
		var jabDataJson JobDataJson
		job.JobData = &jabDataJson
		json_parser(r, &job.JobData)
	case "multipart":
		var jobDataFile JobDataFile
		job.JobData = &jobDataFile
		file_parser(r, job.JobData)

		if jobDataInstance, ok := job.JobData.(*JobDataFile); ok {
			if jobDataInstance.Type == Image {
				job.JobId = ImageProcess
			} else {
				job.JobId = TextAnalyze
			}
		} else {
			fmt.Println("타입 어설션 실패 2")
		}
	}

	jobQueue.Enqueue(job)

	// fmt.Fprintf(w, "작업이 큐에 추가되었습니다: ID=%d, Message=%s", job.ID, job.Message)
	// fmt.Println("큐에 작업 추가됨:", job)
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
		job_handler(r, "multipart")
	} else if strings.HasPrefix(contentType, "application/json") {
		job_handler(r, "json")
	} else {
		fmt.Println("지원되지 않는 요청 형식:", contentType)
		http.Error(w, "Unsupported Content-Type", http.StatusUnsupportedMediaType)
		return
	}
	reauest_id++
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
