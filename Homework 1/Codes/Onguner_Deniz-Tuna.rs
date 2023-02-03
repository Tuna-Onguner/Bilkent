/*
 * Bilkent University
 * Fall 2022 - 2023
 * CS315 Programing Languages
 * Homework 1: Associative Arrays
 * "Rust HashMap"
 *
 * 18.11.2022, Friday
 *
 * Section 2
 * Deniz Tuna Onguner
 * ID: 22001788
 *
 * References:
 * https://doc.rust-lang.org/std/collections/struct.HashMap.html
 */

   
use std::collections::HashMap;

fn main() {
  
  println!("CS315 Homework 1\nRust HashMap");

  // Initialize a HashMap
  let mut movies_imdb = HashMap::new();

  // Adding new elemets
  movies_imdb.insert("Avatar", 7.8);
  movies_imdb.insert("Inception", 8.8);
  movies_imdb.insert("Interstellar", 6.6);
  movies_imdb.insert("Titanic", 9.4);
  movies_imdb.insert("Friends", 8.9);

  // Print the HashMap
  println!("\nPrinting the HashMap");
  for (key, value) in &movies_imdb {
    println!("{}: {}", key, value);
  }

  // Getting the element count
  let mut size = movies_imdb.len();
  println!("\nElement count of the HasHap is {size}");

  // Accessing elements
  let imdb_of_titanic = movies_imdb["Titanic"];
  println!("\nIMDb point of Titanic is {imdb_of_titanic}");
  
  // Modify an element
  *movies_imdb.get_mut("Interstellar").unwrap() = 8.6;

  // Remove an element
  movies_imdb.remove("Friends");

  // Print updated HashMap
  println!("\nPrinting the updated HashMap");
  for (key, value) in &movies_imdb {
    println!("{}: {}", key, value);
  }

  size = movies_imdb.len();
  println!("\nElement count of the updated HasHap is {size}");

  // Search in keys
  println!("\nSearch for a key");
  let key = "Avatar";
  if search_in_keys(movies_imdb.clone(), key.to_string()) {
    println!("Key exist");
  } else {
    println!("Key does not exist");
  }

  // Search in values
  println!("\nSearch for a value");
  let val = 9.4;
  if search_in_vals(movies_imdb.clone(), val) {
    println!("Value exist");
  } else {
    println!("Value does not exist");
  }

  // foo function
  println!("\nfoo function");
  foo(movies_imdb.clone());

  // Clear HashMap
  movies_imdb.clear();
  foo(movies_imdb.clone()); // Nothing to print
  
}

// search_in_keys
fn search_in_keys(map: HashMap<&str, f32>, key: String) -> bool {
  for (k, _) in &map {
    if k == &key {
      return true
    }
  }
  return false
}

// search_in_vals
fn search_in_vals(map: HashMap<&str, f32>, val: f32) -> bool {
  for (_, v) in &map {
    if v == &val {
      return true
    }
  }
  return false
}

// foo
fn foo(map: HashMap<&str, f32>) {
  for (k, v) in &map {
    println!("{k}: {v}");
  }
}

// End of file 

