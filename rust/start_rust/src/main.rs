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
  // for i in 0..ys.len() {  // ys.len > xs.len, so after i = 5 func cout too far
  //   match xs.get(i) { // match func is the roll of Option type matching
  //     // if i > xs's index range, return None
  //     Some(xval) => println!("{}: {}", i, xval),
  //     None => println!("Slow down! {} is too far!", i),
  //   }
  // }

  let empty_array: [u32; 1] = [3];  //[type, arr length]
  let comp_array: [u32; 1] = [1];
  /*
  assert_eq, 두 값을 비교하는 매크로, 다른 경우 패닉 발생 시킴
  패닉(panic), Rust에서 프로그램 실행 중에 복구할 수 없는 오류가 발생했을 때, 프로그램을 강제로 중단시키는 메커니즘
    panic!(""); 이용해 프로그래머가 명시적으로 발생 시킬 수도 있다
  */
  // assert_eq!(empty_array, comp_array); // 배열의 값들을 복사해서 비교하기에 배열 큰 경우 부하 큼
  /*
  output
    assertion `left == right` failed
    left: [3]
    right: [1]
  */

  let empty_array: [u32; 0] = [];
  assert_eq!(&empty_array, &[]); // 주소 값이 아니라 참조를 비교, 배열의 복사 과정이 없기에 더 효율적
}

/*
아래 Unit struct는 태깅, 마크 목적으로 의미 있는 구분과 메모리 절약을 목적으로 사용한다
  데이터를 식별하고자 하는 목적으로 사용하며 필드를 담고 있지 않아 메모리를 사용하지 않는다
*/
struct Unit;

struct Point {
  x: f32,
  y: f32,
}

/*
enum 내부 구조체 형식 할당 가능
*/
enum WebEvent {
  Click { x: i64, y: i64 },
  KeyPress(char),
  Paste(String),
}

enum Stage {
  Beginner,
  Advanced,
}

enum Role {
  Student,
  Teacher,
}

fn struct_enum() {
  println!("=================================================");
  println!("struct_enum");
  println!("=================================================");
  println!("=================================================");
  let another_point: Point = Point { x: 5.2, y: 0.2 };

  // Access the fields of the point
  println!("point coordinates: ({}, {})", another_point.x, another_point.y);
  
  // Make a new point by using struct update syntax to use the fields of our
  // other one
  let bottom_right = Point { x: 5.2, ..another_point };
  println!("point coordinates: ({}, {})", bottom_right.x, bottom_right.y);

  let event = WebEvent::Click { x: 10, y: 20 };

  /*
  match는 switch-case문의 상위 호환
    switch문과 다르게 
      1. 패턴 매칭: 여러 패턴을 매칭하여 값에 따라 다르게 처리할 수 있습니다.
      2. 구조 분해: 구조체나 튜플을 패턴으로 분해하여 쉽게 접근할 수 있습니다.
      3. 범위 패턴: 값의 범위에 따라 분기할 수 있습니다.
      4. 가드 조건: 패턴에 추가적인 조건을 붙일 수 있습니
  */
  match event {
      WebEvent::Click { x, y } => println!("Clicked at x={}, y={}", x, y),
      WebEvent::KeyPress(c) => println!("Key pressed: '{}'", c),
      WebEvent::Paste(s) => println!("Pasted text: '{}'", s),
      _ => println!("default case"),  // Default case
  }

  // define enum's value to use enum value directly
  use crate::Stage::{Beginner, Advanced};
  use crate::Role::*;

  let stage = Beginner;
  let role = Student;

  match stage {
    // Note the lack of scoping because of the explicit `use` above.
    Beginner => println!("Beginners are starting their learning journey!"),
    Advanced => println!("Advanced learners are mastering their subjects..."),
  } 

  match role {
    // Note again the lack of scoping.
    Student => println!("Students are acquiring knowledge!"),
    Teacher => println!("Teachers are spreading knowledge!"),
  }
}

/*
In Rust, enum acts more than a constant(like tagging)
use impl keyword to define fn, so we could use object oriented-programming
*/

use crate::List::*;

// choose Cons or Nil
enum List {
  Cons(u32, Box<List>),
  Nil,
}

impl List {
  fn new() -> List {
    Nil
  }

  fn prepend(self, elem: u32) -> List {
    Cons(elem, Box::new(self))
  }

  /*
  &self : not copy but ref current object
  _ : ignore value
  ref tail : ref data, so in code ref tail is means list-object's second value(=box pointer)
  */
  fn len(&self) -> u32 {
    match *self {
      Cons(_, ref tail) => 1 + tail.len(),
      Nil => 0
    }
  }

  fn stringify(&self) -> String {
    match *self {
      Cons(head, ref tail) => {
        format!("{}, {}", head, tail.stringify())
      },
      Nil => {
        format!("Nil")
      },
    }
  }
}

fn enum_detail() {
  println!("=================================================");
  println!("enum_detail");
  println!("=================================================");
  println!("=================================================");
  let mut list = List::new();
  
  list = list.prepend(1);
  list = list.prepend(2);
  list = list.prepend(3);

  println!("linked list has length: {}", list.len());
  println!("{}", list.stringify());
}

fn main() {

  format_traits();

  primitive_types();

  struct_enum();

  enum_detail();
}
