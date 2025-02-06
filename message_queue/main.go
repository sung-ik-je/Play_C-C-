package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"sync"
	"time"
)

type Job struct {
	ID      int    `json:"ID"`
	Message string `json:"message"`
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

func job_handler(w http.ResponseWriter, r *http.Request) {
	var job Job

	decoder := json.NewDecoder(r.Body)
	err := decoder.Decode(&job)
	if err != nil {
		http.Error(w, "잘못된 JSON 데이터", http.StatusBadRequest)
		return
	}

	jobQueue.Enqueue(job)

	fmt.Fprintf(w, "작업이 큐에 추가되었습니다: ID=%d, Message=%s", job.ID, job.Message)
	fmt.Println("큐에 작업 추가됨:", job)
}

func create_worker() {
	for i := 1; i <= numWorkers; i++ {
		wg.Add(1)
		go worker(i, jobQueue, &wg)
	}
}

func main() {
	jobQueue = NewJobQueue(100)

	// 서버 열기 전에 worker 초기화 필요
	create_worker()

	http.HandleFunc("/add_job", job_handler)

	fmt.Println("Server is listening on http://localhost:8080")
	err := http.ListenAndServe(":8080", nil)
	if err != nil {
		log.Fatal("Error starting server:", err)
	}

	wg.Wait()
}
