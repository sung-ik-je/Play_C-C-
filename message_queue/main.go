package main

import (
	"fmt"
	"log"
	"net/http"
)

func job_handler(w http.ResponseWriter, r *http.Request) {
	fmt.Println("call job handler")
}

func main() {
	http.HandleFunc("/add_job", job_handler)

	fmt.Println("Server is listening on http://localhost:8080")
	err := http.ListenAndServe(":8080", nil)
	if err != nil {
		log.Fatal("Error starting server:", err)
	}
}
