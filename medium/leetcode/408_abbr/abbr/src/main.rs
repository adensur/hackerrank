// https://leetcode.com/problems/valid-word-abbreviation/

pub fn valid_word_abbreviation(word: String, abbr: String) -> bool {
    let abbr = abbr.as_bytes();
    let mut word_iter = word.as_bytes().iter();
    let mut abbr_idx = 0;
    while abbr_idx < abbr.len() {
        let ch = abbr[abbr_idx] as char;
        match ch {
            letter if (letter >= 'a' && letter <= 'z') => {
                if let Some(ch2) = word_iter.next() {
                    if *ch2 as char == letter {
                        abbr_idx += 1;
                        continue;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            }
            digit if (digit >= '0' && digit <= '9') => {
                if digit == '0' {
                    // leading zero detected!
                    return false;
                }
                let mut buf = String::new();
                buf.push(digit);
                abbr_idx += 1;
                while abbr_idx < abbr.len() {
                    let ch = abbr[abbr_idx] as char;
                    if ch < '0' || ch > '9' {
                        break;
                    }
                    buf.push(ch);
                    abbr_idx += 1;
                }
                // abbr_idx now points to the first non-digit char, or outside of the slice
                let number: i32 = buf.parse().unwrap();
                for _ in 0..number {
                    if let Some(_) = word_iter.next() {
                        // pass
                    } else {
                        return false;
                    }
                }
            }
            _ => panic!("Unexpected letter"),
        }
    }
    if let Some(_) = word_iter.next() {
        // have some trailing chars!
        return false;
    }
    return true;
}

fn main() {
    println!("Hello, world!");
}
