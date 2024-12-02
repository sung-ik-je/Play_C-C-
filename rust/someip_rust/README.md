# SOME/IP DTC 개요
1. event handler 등록 : 실행하는 바이너리 추적 목적

2. 데이터 선언
	1. 구조체 선언
		구조체와 유사한 데이터 타입 뭐가 있는지?
	2. enum 선언, enum은?

3. 프로그램 실행 과정
		Cli console에서 진행되는 작업 
	1. 1번 무한 루프 : 테스트 케이스 선택 메뉴 출력
		테스트 종료하는 선택 필수
		테스트 케이스는 숫자 입력만 가능하며 그 외 입력은 무시
			1. 바이너리 실행하는 테스트 케이스 
				입력 시 테스트 케이스 바이너리 실행
					1. 바이너리에 argument를 넣어서 실행
						1. leep하면서 출력하는 경우
						2. api를 이용해 특정 데이터 가져오거나 시간 출력하는 등의 작업
				바이너리는 메인 프로세스로부터 종료 이벤트를 수신할 수 있어야 한다	
			1. 2번 무한 루프 : 하위 테스트 케이스 선택 메뉴 출력
				1. 상위 테스트 케이스로 돌아갈 수 있어야 한다
				2. switch문과 유사한 선택 사항이 있는지 여부
					1. json file을 수정할 수 있는지?
					2. batch script 실행 가능한지?
			1, 2번과 병렬로 json file을 읽어서 패킷을 형성 및 전송하는 작업 

<br><br>

# Directory
project/src/main.rs : SOME/IP DTC 메인 바이너리 소스 코드<br>
project/src/bin/work_bin.rs : 특정 작업 수행하는 바이너리 소스 코드<br>
project/src/bin/loop_bin.rs : 무한 루프 목적 바이너리 소스 코드<br>

**참고** src/bin 경로가 의무는 아니며 Cargo.toml file에 bin 경로를 명시해주면 된다.
```
[[bin]]
name = "loop-bin"
path = "src/bin/loop_bin.rs"


[[bin]]
name = "work-bin"
path = "src/bin/work_bin.rs"
```

## 복수 개의 bin 생성 시 참고
cargo new 키워드로 pj 생성 시 기본적으로 project/src/main.rs 소스 코드를 기반으로 bin 생성



