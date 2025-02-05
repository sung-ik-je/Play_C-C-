package main

import (
	"fmt"
	"sync"
	"time"
)

type Job struct {
	ID      int
	Message string
}

type JobQueue struct {
	queue chan Job
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
		fmt.Printf("Worker %d processing Job %d: %s\n", id, job.ID, job.Message)
		time.Sleep(1 * time.Second)
	}
}

func main_test() {
	jobQueue := NewJobQueue(10)

	for i := 1; i <= 5; i++ {
		jobQueue.Enqueue(Job{ID: i, Message: fmt.Sprintf("Job %d", i)})
	}

	var wg sync.WaitGroup
	numWorkers := 3
	for i := 1; i <= numWorkers; i++ {
		wg.Add(1)
		go worker(i, jobQueue, &wg)
	}

	close(jobQueue.queue)

	wg.Wait()
}
