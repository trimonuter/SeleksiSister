import Foundation

func readMatrix(from filename: String) -> [[Double]]? {
    guard let fileHandle = FileHandle(forReadingAtPath: filename) else {
        print("Error opening file")
        return nil
    }
    
    let data = fileHandle.readDataToEndOfFile()
    guard let content = String(data: data, encoding: .utf8) else {
        print("Error reading file content")
        return nil
    }
    
    let lines = content.split(separator: "\n").map { $0.trimmingCharacters(in: .whitespacesAndNewlines) }
    guard let n = Int(lines[0]) else {
        print("Error parsing matrix size")
        return nil
    }
    
    var matrix = [[Double]](repeating: [Double](repeating: 0.0, count: n), count: n)
    
    for i in 0..<n {
        let row = lines[i + 1].split(separator: " ").compactMap { Double($0) }
        if row.count == n {
            matrix[i] = row
        } else {
            print("Error parsing matrix row")
            return nil
        }
    }
    
    return matrix
}

func writeMatrix(to filename: String, matrix: [[Double]]) {
    let n = matrix.count
    var content = ""
    
    for i in 0..<n {
        let row = matrix[i].map { String($0) }.joined(separator: " ")
        content += row + "\n"
    }
    
    do {
        try content.write(toFile: filename, atomically: true, encoding: .utf8)
    } catch {
        print("Error writing file: \(error)")
    }
}

func gaussJordanInverse(matrix: [[Double]]) -> [[Double]]? {
    let n = matrix.count
    var augmented = [[Double]](repeating: [Double](repeating: 0.0, count: 2 * n), count: n)
    
    // Create augmented matrix with the identity matrix on the right
    for i in 0..<n {
        for j in 0..<n {
            augmented[i][j] = matrix[i][j]
            augmented[i][j + n] = (i == j) ? 1.0 : 0.0
        }
    }
    
    // Perform Gauss-Jordan elimination
    for i in 0..<n {
        var pivot = augmented[i][i]
        if pivot == 0.0 {
            var swapRow = -1
            for j in (i + 1)..<n {
                if augmented[j][i] != 0.0 {
                    swapRow = j
                    break
                }
            }
            if swapRow == -1 {
                print("Matrix is singular and cannot be inverted.")
                return nil
            }
            
            // Swap rows
            augmented.swapAt(i, swapRow)
            pivot = augmented[i][i]
        }
        
        // Normalize the pivot row
        for j in 0..<2 * n {
            augmented[i][j] /= pivot
        }
        
        // Eliminate the current column in all other rows
        for j in 0..<n {
            if j != i {
                let factor = augmented[j][i]
                for k in 0..<2 * n {
                    augmented[j][k] -= factor * augmented[i][k]
                }
            }
        }
    }
    
    // Extract the inverse matrix from the augmented matrix
    var inverse = [[Double]](repeating: [Double](repeating: 0.0, count: n), count: n)
    for i in 0..<n {
        for j in 0..<n {
            inverse[i][j] = augmented[i][j + n]
        }
    }
    
    return inverse
}

// Main function
func main() {
    guard let matrix = readMatrix(from: "../random_matrix.txt") else {
        return
    }
    
    guard let inverse = gaussJordanInverse(matrix: matrix) else {
        return
    }
    
    writeMatrix(to: "./inverse_matrix.txt", matrix: inverse)
}

// Call main
main()
