package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"mime/multipart"
	"net/http"
	"os"
	"strconv"
	"time"
)

func send_file(fileType string) {
	// func send_file(id int, fileType string) {
	var filePath, url string
	// 멀티파트 폼 데이터 생성
	body := &bytes.Buffer{}
	writer := multipart.NewWriter(body)

	url = "http://localhost:8080/request"
	// 파일 타입별 경로 및 서버 URL 설정
	switch fileType {
	case "text":
		// filePath = fmt.Sprintf("text_file/example.txt", id)
		filePath = "file/example.txt"
		writer.WriteField("description", "This is a test file")
		writer.WriteField("File-Type", "Text")
	case "image":
		filePath = "file/Tk0IajNeOfOdEUWDV-tTZn4tOPT0kRPtme8PAiL6fm7NQ_YV7CKYEWlNDJJ9D_om7rzbkqgpe9lBhyKojZ8lxEkJX-chNzEiuKhnprlUQ61-ZkWqpx8w_5gmKLdYaSJ6rD93RuJGnqIkiXRPchCwJQ.webp"
		writer.WriteField("description", "This is a image file")
		writer.WriteField("File-Type", "Image")
	default:
		fmt.Println("지원되지 않는 파일 타입:", fileType)
		return
	}

	// 파일 열기
	file, err := os.Open(filePath)
	if err != nil {
		fmt.Println("파일 열기 실패:", err)
		return
	}
	defer file.Close()

	// 파일 첨부
	part, err := writer.CreateFormFile("file", filePath)
	if err != nil {
		fmt.Println("폼 파일 생성 실패:", err)
		return
	}
	io.Copy(part, file)

	// 추가 필드 작성

	writer.Close()

	// HTTP 요청 생성
	req, err := http.NewRequest("POST", url, body)
	if err != nil {
		fmt.Println("요청 생성 실패:", err)
		return
	}
	req.Header.Set("Content-Type", writer.FormDataContentType())

	// HTTP 요청 실행
	client := &http.Client{}
	resp, err := client.Do(req)
	if err != nil {
		fmt.Println("요청 실패:", err)
		return
	}
	defer resp.Body.Close()

	fmt.Println("응답 상태 코드:", resp.Status)
}

func send_file_url() {
	i := 0

	for {
		message := "test_" + strconv.Itoa(i)
		data := map[string]interface{}{
			"ID":      i,
			"message": message,
		}

		fmt.Printf("Job : %d, %s\n", data["ID"], data["message"])

		jsonData, err := json.Marshal(data)
		if err != nil {
			fmt.Println("JSON 변환 실패:", err)
			return
		}

		url := "http://localhost:8080/request"
		req, err := http.NewRequest("POST", url, bytes.NewBuffer(jsonData))
		if err != nil {
			fmt.Println("요청 생성 실패:", err)
			return
		}

		req.Header.Set("Content-Type", "application/json")

		client := &http.Client{}
		resp, err := client.Do(req)
		if err != nil {
			fmt.Println("요청 실패:", err)
			return
		}
		defer resp.Body.Close()

		// 응답 확인
		fmt.Println("응답 상태 코드:", resp.Status)
		i += 1
	}
}

func send_image_file() {

}

func main() {

	for {
		send_file("text")
		// send_file("image")
		// send_file_url()
		time.Sleep(2 * time.Second)
	}

	// send_file_url()

}
