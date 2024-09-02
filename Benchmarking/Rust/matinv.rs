use std::fs::File;
use std::io::{self, BufRead, Write};

fn read_matrix(filename: &str) -> io::Result<Vec<Vec<f64>>> {
    let file = File::open(filename)?;
    let reader = io::BufReader::new(file);
    let mut lines = reader.lines();

    let n: usize = lines.next()
        .ok_or_else(|| io::Error::new(io::ErrorKind::Other, "Missing matrix size"))?
        .map_err(|e| io::Error::new(io::ErrorKind::InvalidData, e))?
        .trim()
        .parse()
        .map_err(|e| io::Error::new(io::ErrorKind::InvalidData, e))?;

    let mut matrix = vec![vec![0.0; n]; n];
    for (i, line) in lines.enumerate() {
        let line = line.map_err(|e| io::Error::new(io::ErrorKind::InvalidData, e))?;
        let values: Vec<f64> = line.split_whitespace()
            .map(|s| s.parse().map_err(|e| io::Error::new(io::ErrorKind::InvalidData, e)))
            .collect::<Result<_, _>>()?;
        matrix[i].copy_from_slice(&values);
    }
    Ok(matrix)
}

fn write_matrix_to_file(filename: &str, matrix: &[Vec<f64>]) -> io::Result<()> {
    let mut file = File::create(filename)?;
    for row in matrix {
        let line = row.iter()
            .map(|x| format!("{}", x))
            .collect::<Vec<String>>()
            .join(" ");
        writeln!(file, "{}", line)?;
    }
    Ok(())
}

fn gauss_jordan_inverse(matrix: &[Vec<f64>]) -> Vec<Vec<f64>> {
    let n = matrix.len();
    let mut augmented = vec![vec![0.0; 2 * n]; n];

    for i in 0..n {
        for j in 0..n {
            augmented[i][j] = matrix[i][j];
            augmented[i][j + n] = if i == j { 1.0 } else { 0.0 };
        }
    }

    for i in 0..n {
        let pivot = augmented[i][i];
        if pivot == 0.0 {
            let mut swap_row = None;
            for j in (i + 1)..n {
                if augmented[j][i] != 0.0 {
                    swap_row = Some(j);
                    break;
                }
            }
            if let Some(swap_row) = swap_row {
                augmented.swap(i, swap_row);
            } else {
                panic!("Matrix is singular and cannot be inverted.");
            }
        }

        for j in 0..2 * n {
            augmented[i][j] /= pivot;
        }

        for j in 0..n {
            if j != i {
                let factor = augmented[j][i];
                for k in 0..2 * n {
                    augmented[j][k] -= factor * augmented[i][k];
                }
            }
        }
    }

    let mut inverse = vec![vec![0.0; n]; n];
    for i in 0..n {
        for j in 0..n {
            inverse[i][j] = augmented[i][j + n];
        }
    }
    inverse
}

fn main() -> io::Result<()> {
    let matrix = read_matrix("../random_matrix.txt")?;
    let inverse = gauss_jordan_inverse(&matrix);
    write_matrix_to_file("./inverse_matrix.txt", &inverse)
}
