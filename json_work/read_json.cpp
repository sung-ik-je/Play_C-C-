#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include "nlohmann/json.hpp"

struct tempStruct {
  int a;
  std::string b;
  std::string c;
};

int main() {
  std::ifstream inputFile("temp.json");
  if (!inputFile) {
    std::cerr << "Could not open the file!" << std::endl; // cerr : 표준 오류 스트림
    return 1;
  }

  nlohmann::json jsonData;
  inputFile >> jsonData;    // json 파일 읽어와서 jsonData에 저장

  tempStruct tmp;
  tmp.a = jsonData["first"];
  tmp.b = jsonData["second"];
  tmp.c = jsonData["third"];

  std::string a = jsonData["depth1"]["depth2-1"];
  std::string b = jsonData["depth1"]["depth2-2"];
  std::string c = jsonData["depth1"]["depth2-3"];

  std::cout << "a : " << tmp.a << std::endl;
  std::cout << "b : " << tmp.b << std::endl;
  std::cout << "c : " << tmp.c << std::endl;

  std::cout << "two depth" << std::endl;

  std::cout << "2 a : " << a << std::endl;
  std::cout << "2 b : " << b << std::endl;
  std::cout << "2 c : " << c << std::endl;


  std::stringstream ss;
  uint16_t aa;
  uint16_t bb;
  uint32_t cc;
  uint16_t dd = 0x12;
  
  ss << std::hex << a;  
  ss >> aa;
  ss.clear();  // 스트림 상태 플래그 초기화
  ss.str("");  // 스트림 버퍼 초기화
  ss << std::hex << b;
  ss >> bb;
  ss.clear();  // 스트림 상태 플래그 초기화
  ss.str("");  // 스트림 버퍼 초기화
  ss << std::hex << c;
  ss >> cc;

  std::cout << "change a : " << aa << std::endl;
  std::cout << "change b : " << std::hex << bb << std::endl;  // std::hex를 기준으로 전부 다 hex로 출력
                                      // 스트림 조작자가 스트림의 상태를 변경하는 방식으로 진행되는데 std::hex는 hex로 상태를 변경
  std::cout << "change c : " << cc << std::endl;
  std::cout << "dd : " << dd << std::endl;
}