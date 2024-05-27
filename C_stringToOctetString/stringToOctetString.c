#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 문자열을 octet string으로 변환하여 반환하는 함수, 일반적으로 ASCII 문자열인 경우 octet string으로 변환했을 때 길이가 같다
// 다른 인코딩 형태인 경우는 문자당 바이트 크기가 달라 변환 시 길이 바뀔 수 있다 
unsigned char* stringToOctetString(const char* str) {
    // 문자열의 길이 계산
    size_t len = strlen(str);

    /*
    octet string을 저장할 배열 동적 할당
    
    C에서 메모리를 동적할당하는 이유?
      main -> stringToOctetString처럼 지역 변수는 함수가 호출될 때 메모리가 할당되며 종료될 때 해당 메모리가 해제된다(지역 변수는 함수 범위 내에서만 유효)
      
      메모리를 직접적으로 할당해주는 이유?
        지역 변수는 전역 변수, 정적 지역 변수와는 다르게 프로그램이 시작될 때 자동으로 0 또는 NULL로 초기화되지 않는다
          쓰레기 값이 할당되며 바로 사용하는 경우 다른 메모리 주소에 영향 줄 수 있다
        때문에 동적으로 메모리 주소를 할당해준다
    */
    unsigned char* octet_string = (unsigned char*)malloc(len * sizeof(unsigned char));

    // 문자열을 ASCII 값으로 변환하여 octet string에 저장
    for (size_t i = 0; i < len; i++) {
        octet_string[i] = (unsigned char)str[i];
    }

    return octet_string;
}

int main() {
    char str[] = "Hello, world!"; // 입력 받을 문자열
    unsigned char* octet_str = stringToOctetString(str); // octet string으로 변환

    // octet string 출력
    printf("Octet String: ");
    for (size_t i = 0; i < strlen(str); i++) {
        printf("%02X ", octet_str[i]); // 각 octet을 16진수로 출력
    }
    printf("\n");

    // octet string 메모리 해제
    /*
    main 함수 종료 시 octet_str 변수는 main 함수의 지역 변수이기에 함수가 종료되면 스택에서 제거되지만 
    stringToOctetString 함수로부터 응답 받은 OctetString 형태가 저장된 메모리 부분(런타임 시 생성되었기에 힙에 존재)은 그대로 존재
    때문에 main 함수 종료 전 free를 통해 사용성을 잃은 데이터를 삭제해주는 것
    */
    free(octet_str);

    return 0;
}
