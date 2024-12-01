## 새로운 프로젝트 생성
command : `cargo new my_project`

프로젝트 생성 시 기본적으로 project/src/main.rs 파일이 생성되며 cargo build 명령어에 따른 기본적으로 bin을 생성하는 소스 코드 파일이다.<br>

아래 2개의 파일은 Rust 프로젝트에서 패키지 관리와 빌드 설정을 관리하는 중요한 파일이다.
Cargo.toml : Rust 프로젝트의 설정 파일로 프로젝트의 정보를 정의하고 필요한 외부 라이브러리(크레이트)의 의존성을 선언하는 역할 
  - bin, lib를 포함해 스크립트, 빌드 명령어 등 다양한 추가 설정이 가능하다. 
  <br>

**참고** 
Rust에서 Package와 Crate
```
Rust에서 크레이트(Crate)는 라이브러리 또는 바이너리 실행 파일을 포함하는 컴파일 단위를 의미한다.
크레이트는 하나의 라이브러리 또는 하나의 바이너리 파일로 구성된다.

여러 크레이트를 모아 패키지라 지칭한다.
여러 크레이트를 묶어 관리하는 상위 개념으로 Cargo.toml 파일을 통해 패키지의 메타데이터와 의존성을 정의한다.

module < lib < crate < package 구조로 볼 수 있다.
1. module
  코드를 논리적으로 나눈 작은 단위
2. lib
  개별적인 기능 단위로 보통 코드 재사용을 위해 작성된다.
  Rust에서는 library crate로 간주된다.
3. crate
  Rust의 컴파일 단위로 하나의 바이너리 실행 파일 혹은 라이브리러로 구성된다.
  모듈로 나눠진 여러 코드들을 하나로 묶는 단위로 라이브러리나 실행 파일을 만들 수 있는 lib보다 더 큰 단위이다.
4. package
  여러 크레이트를 묶어서 관리하는 상위 단위로 프로젝트 수준의 단위를 의미한다.
  하나의 패키지에 최대 하나의 라이브러리 크레이트와 0개 이상의 바이너리 크레이트를 포함한다.
```


Cargo.lock : 프로젝트에서 사용하는 모든 의존성의 정확한 버전을 기록하는 파일이다. cargo가 의존성을 설치한 후 자동으로 생성된다.
  - cargo build 명령어를 실행하면 Cargo.lock에 기록된 대로 동일한 의존성이 설치된다.
    - cargo build 실행 시 Cargo.toml을 기반으로 Cargo.lock 업데이트된다.
  - 주로 binary 프로젝트에서 사용되며 라이브러리 프로젝트에서는 저장소에 포함하지 않는 경우가 많다.

<br><br>

## 프로젝트 빌드 (디버그 모드)
command : `cargo build`

<br><br>

## 프로젝트 실행
command : `cargo run`

<br><br>

## 최적화된 릴리스 빌드
command : `cargo build --release`

<br><br>

# 테스트 실행
command : `cargo test`
