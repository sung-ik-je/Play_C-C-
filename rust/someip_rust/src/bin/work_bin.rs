use std::net::TcpListener;
use std::io::Read;
use std::thread;
use std::time::Duration;
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;


/*
  spawn
    새로운 스레드를 생성하는 함수
    스레드에서 실행될 클로저(함수)를 인자로 받는다.
    소유권의 이전이 관건이다.

  move ||
    클로저 내부로 값을 이동시키는 역할
    move 키워드는 클로저 내부에서 외부 변수를 사용하는 방법을 정의하며 이 키워드를 사용하면 클로저가 외부 변수를 소유하게 된다.
      이는 더 이상 main에서 run_thread_clone을 사용하지 못한다는 의미
      만약 move를 사용하지 않으면 run_thread_clone에 대해 _handle 스레드 또한 참조할 수 있지만 소유권은 main func에 있다.
    move 선언 이후 사용되는 클로저 내부에서 사용되는 변수들에 대해서만 소유권을 이동시키며 다른 변수들은 영향을 받지 않는다.
*/
fn other_thread(run_thread: Arc<AtomicBool>) {
  let mut i = 0;
    while run_thread.load(Ordering::SeqCst) {
        println!("other thread: {}", i);
        i += 1;
        thread::sleep(Duration::from_millis(500)); // 0.5초 대기
    }
}

fn main() {
  /*
  Atomic Reference Counted, 멀티 스레드 환경에서 안전한 참조 카운팅 제공하는 스마트 포인터
    변수 값 자체를 복사하는 것이 아닌 변수를 Arc 객체로 감싸고 Arc 객체를 복사(clone)하며 Arc 객체의 참조 카운트를 증가시킨다.
      run_thread, run_thread_clone 각 Arc 객체가 하나의 변수를 가리키고 있다는 의미
    Arc 객체 자체는 불변이지만 그 안에 AtomicBool, AtomicI32 등 Atomic 타입의 경우 원자적 연산을 통해 안전하게 수정(store) 가능하다.
    store 시에 메모리 정렬 관련해서 매개변수를 설정한다. 

    그냥 clone 없이 run_thread 사용하면 되는거 아닌가?
      clone을 사용하는 것은 rust의 소유권 시스템과 관련된다.
      Arc는 내부적으로 rust의 소유권 공유할 수 있도록 설계되어 있다. 만약 clone이 아닌 run_thread를 직접 사용하는 것은 단순히 값을 복사하는 형태이며 
        이는 rust의 소유권을 위반하는 행위이다.
      run_thread 자체는 Arc 객체로 소유권을 이동하거나 값을 직접 복사할 수 없는 규칙이 있다.
  */
  let run_thread = Arc::new(AtomicBool::new(true));
  let run_thread_clone = Arc::clone(&run_thread);

  let _handle = thread::spawn(move || other_thread(run_thread_clone));

  let listener = TcpListener::bind("127.0.0.1:7878").expect("Failed to bind");

  for stream in listener.incoming() {
    println!("Waiting for packet...");
    let mut stream = stream.unwrap();
    let mut buffer = [0; 1024];

    // 실제로 읽은 바이트 수를 확인
    let bytes_read = stream.read(&mut buffer).unwrap();

    // 읽은 데이터의 유효 부분만 사용
    let command = String::from_utf8_lossy(&buffer[..bytes_read]);

    println!("packet: {}", command.trim());

    if command.trim() == "exit" {
      println!("Exit command received. Shutting down...");
      /*
      arc 값 저장, 스레드를 종료 신호 설정
      Ordering : 멀티 스레드 환경에서 다양한 스레드들의 메모리 접근 시 순서 보장 관련 개념
        */
      run_thread.store(false, Ordering::SeqCst);
      break;
    }
  }

  _handle.join().unwrap();
  println!("Server has stopped.");
}
