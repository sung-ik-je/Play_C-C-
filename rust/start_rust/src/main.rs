use std::mem;

fn format_traits() {
  println!("=================================================");
  println!("format_traits");
  println!("=================================================");
  println!("=================================================");
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


fn primitive_types() {
  println!("=================================================");
  println!("primitive_types");
  println!("=================================================");
  println!("=================================================");
  // signed int : i8~128, unsigned int : u8~128
  let s_int8: i8 = 12;
  let s_int128: i128 = 12;
  println!("size of i8 : {}", mem::size_of::<i8>()); // return 1byte
  println!("size of i128 : {}", mem::size_of::<i128>()); // return 16byte
  let u_int8: u8 = 12;
  let u_int128: u128 = 12;

  // rust is use i32 and f64 type for default
  let a_float: f64 = 1.0;  // Regular annotation
  // the case of addiotional specified type in value 
  let an_integer   = 5i32; // Suffix annotation, 5 is to store to i32 type

  // rust use immutable type in default
  let true_or_false: bool = true;
  // true_or_false = false; // impossible
  // so if change value you must use mut keyword
  let mut true_or_false: bool = true;
  true_or_false = false;

  println!("1e4 is {}, -2.5e-3 is {}", 1e4, -2.5e-3);

  let tuple_of_tuples = ((1u8, 2u16, 2u32), (4u64, -1i8), -2i16);

  // Tuples are printable.
  println!("tuple of tuples: {:?}", tuple_of_tuples);

  // array, [T; length]
  let xs: [i32; 5] = [1, 2, 3, 4, 5];
  let ys: [i32; 500] = [0; 500];

  // analyze_slice(&ys[1 .. 4]);

  /*
  Option Enum
    enum Option<T> {
      Some(T), // have a specific value
      None, // don't have a specific value
    } 
  
  for func range
    0..5 : 0~4
    0..=5 : 0~5
    5.. : 5~end, to infinite
    ..5 : first~4
    ..=5 : first~5
  */
  for i in 0..ys.len() {  // ys.len > xs.len, so after i = 5 func cout too far
    match xs.get(i) { // match func is the roll of Option type matching
      // if i > xs's index range, return None
      Some(xval) => println!("{}: {}", i, xval),
      None => println!("Slow down! {} is too far!", i),
    }
  }

  let empty_array: [u32; 0] = [];
  assert_eq!(&empty_array, &[]);
}

fn main() {

  format_traits();

  primitive_types();
}
