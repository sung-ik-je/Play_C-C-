use std::thread;
use std::time::Duration;

fn main() {
  let mut cnt: i32 = 0;
  loop {
    println!("5초마다 출력됩니다 : {}", cnt);
    thread::sleep(Duration::from_secs(5)); // 5초간 대기
    cnt += 1;
  }
}
