package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"net/http"
	"strconv"
)

func main() {

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

		url := "http://localhost:8080/add_job"
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
