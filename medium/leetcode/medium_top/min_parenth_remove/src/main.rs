// https://leetcode.com/problems/minimum-remove-to-make-valid-parentheses/

use std::collections::HashSet;

pub fn min_remove_to_make_valid(s: String) -> String {
    let mut bytes = s.into_bytes();
    let mut count = 0i32;
    let mut to_remove: HashSet<usize> = HashSet::new();
    let mut opening_parenthesis: Vec<usize> = Vec::new();
    for i in 0..bytes.len() {
        let ch = bytes[i] as char;
        if ch == ')' {
            if count == 0 {
                // can't go any lower!
                to_remove.insert(i);
                continue;
            }
            count -= 1;
        } else if ch == '(' {
            count += 1;
            opening_parenthesis.push(i);
        }
    }
    while count > 0 {
        count -= 1;
        to_remove.insert(opening_parenthesis.pop().unwrap()); // to_remove is guaranteed to have enough elements
    }
    let mut write_idx = 0usize;
    for read_idx in 0..bytes.len() {
        if to_remove.contains(&read_idx) {
            continue;
        }
        bytes[write_idx] = bytes[read_idx];
        write_idx += 1;
    }
    bytes.resize(write_idx, 0);
    String::from_utf8(bytes).unwrap()
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn test1() {
        let input = "lee(t(c)o)de)";
        let expected = "lee(t(c)o)de";
        let output = min_remove_to_make_valid(String::from(input));
        assert_eq!(output, expected, "Expected {} got {}", expected, output);
    }
}

fn main() {
    println!("Hello, world!");
}
