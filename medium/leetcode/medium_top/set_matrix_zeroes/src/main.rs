// https://leetcode.com/explore/interview/card/top-interview-questions-medium/103/array-and-strings/777/

use std::collections::HashSet;
impl Solution {
    pub fn set_zeroes(matrix: &mut Vec<Vec<i32>>) {
        let mut columns_to_zero: HashSet<usize> = HashSet::new();
        let mut rows_to_zero: HashSet<usize> = HashSet::new();
        for row_idx in 0..matrix.len() {
            for col_idx in 0..matrix[row_idx].len() {
                if matrix[row_idx][col_idx] == 0 {
                    columns_to_zero.insert(col_idx);
                    rows_to_zero.insert(row_idx);
                }
            }
        }
        for row_idx in rows_to_zero {
            for col_idx in 0..matrix[row_idx].len() {
                matrix[row_idx][col_idx] = 0;
            }
        }
        for col_idx in columns_to_zero {
            for row_idx in 0..matrix.len() {
                matrix[row_idx][col_idx] = 0;
            }
        }
    }
}

fn main() {
    println!("Hello, world!");
}
