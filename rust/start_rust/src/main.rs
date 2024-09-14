
fn format_traits() {
  println!("Hello, world!");

  format!("stdout string");
  
  let name = "ik";
  let age = 30;
  
  let stdout = format!("name : {} year : {}", name, age); // {} : 기본 출력, display trait

  println!("{}", stdout);
  print!("same as format!, printed to the console");
  println!("same as print!, append newline");

  eprint!("smae as print, but printed to the stderr");
  eprintln!("smae as eprint, append newline");

  println!("{subject} {verb} {object}",
             object="the lazy dog",
             subject="the quick brown fox",
             verb="jumps over");
  
  #[derive(Debug)]
  struct Person {
    name: String,
    age: u32,
  }

  let person = Person {
    name: "Bob".to_string(),
    age: 25,
  };
  println!("{:?}", person); // {:?} : debug trait
  println!("{:#?}", person); // {:?}와 유사하지만 들여쓰기를 포함해 출력

  // {:.precision}:, rust에서 중괄호 내 :는 형식 지정자를 정의하는데 사용된다
  println!("Base 10:               {}",   69420); // 69420
  println!("Base 2 (binary):       {:b}", 69420); // 10000111100101100
  println!("Base 8 (octal):        {:o}", 69420); // 207454
  println!("Base 16 (hexadecimal): {:x}", 69420); // 10f2c

  // print시에 정렬, 패딩
  println!("{number:>5}", number=1);  // "    1"
  println!("{number:0>5}", number=1); // "00001", >(우측 정렬), 1을 어디로 정렬할지, 0을 채운다는 의미
  println!("{number:05}", number=1);  // 위에 {:0>5}와 마찬가지로 우측 정렬 및 0으로 패딩
  println!("{number:0<5}", number=1); // "10000", <(좌측 정렬)
  println!("{number:0>width$}", number=1, width=5); // "00001", $은 매크로에서 변수의 값이 들어갈 자리임을 나타냄

  // 형식 지정자는 print 매크로와 함께 출력할 변수를 서식화할 때 뿐만 아니라 변수와 함께 형식을 동적으로 지정 가능
  let number: f64 = 1.0;
  let width: usize = 5;
  println!("{number:>width$}"); // "    1"

  // rust는 기본적으로 선언되었지만 사용되지 않는 코드(dead_code)의 대해 경고를 표시하게 되어 있다
  // dead_code allow 목적(경고 표시 안나게)
  #[allow(dead_code)] // disable `dead_code` which warn against unused module
  struct Structure(i32);
}
fn main() {

  format_traits();
}
