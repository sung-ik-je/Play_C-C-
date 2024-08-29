fn main() {
  println!("Hello, world!");

  format!("stdout string");
  
  let name = "ik";
  let age = 30;
  
  let stdout = format!("name : {} year : {}", name, age);

  println!("{}", stdout);
  print!("same as format!, printed to the console");
  println!("same as print!, append newline");

  eprint!("smae as print, but printed to the stderr");
  eprintln!("smae as eprint, append newline");
}
