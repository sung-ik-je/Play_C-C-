use std::net::TcpStream;
use std::io::Write;
use std::io;


fn send_event(stream: &mut TcpStream) {
  let command: &str = "exit";
  stream.write(command.as_bytes()).expect("Failed to send command");
}

fn main() {
    let mut stream: TcpStream = TcpStream::connect("127.0.0.1:7878").expect("Failed to connect to server");

    let mut input: String = String::new(); // 사용자 입력을 저장할 변수

    loop {
      println!("Enter something: ");

      io::stdin()
          .read_line(&mut input) // 입력을 읽어 input에 저장
          .expect("Failed to read line");
      
      if input.trim() == "exit" { // trim()으로 줄바꿈 제거
        send_event(&mut stream);    // 소유권 이동하지 않고 가변 참조 전달
        break;
      }
    }
    println!("Exit command sent to b.");
}
