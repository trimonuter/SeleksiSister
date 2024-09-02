import java.io.File
import java.lang.RuntimeException
import java.util.Scanner

fun main() {
    val matrix = mutableListOf<MutableList<Double>>()
    val inverse = mutableListOf<MutableList<Double>>()

    // Read matrix elements from file
    readMatrix("../random_matrix.txt", matrix)

    // Resize the inverse matrix to the size of the original matrix
    val n = matrix.size
    for (i in 0 until n) {
        inverse.add(MutableList(n) { 0.0 })
    }

    // Compute the inverse matrix
    gaussJordanInverse(matrix, inverse)

    // Write the inverse matrix to a file
    writeMatrixToFile("./inverse_matrix.txt", inverse)
}

fun readMatrix(filename: String, matrix: MutableList<MutableList<Double>>) {
    val file = File(filename)
    if (!file.exists()) {
        throw RuntimeException("Error opening file")
    }

    val scanner = Scanner(file)
    val n = scanner.nextInt()

    for (i in 0 until n) {
        val row = mutableListOf<Double>()
        for (j in 0 until n) {
            row.add(scanner.nextDouble())
        }
        matrix.add(row)
    }

    scanner.close()
}

fun writeMatrixToFile(filename: String, matrix: List<List<Double>>) {
    val file = File(filename)
    file.bufferedWriter().use { writer ->
        for (row in matrix) {
            writer.write(row.joinToString(" ") + "\n")
        }
    }
}

fun gaussJordanInverse(matrix: List<List<Double>>, inverse: MutableList<MutableList<Double>>) {
    val n = matrix.size
    val augmented = Array(n) { DoubleArray(2 * n) }

    // Create an augmented matrix with the identity matrix on the right
    for (i in 0 until n) {
        for (j in 0 until n) {
            augmented[i][j] = matrix[i][j]
            augmented[i][j + n] = if (i == j) 1.0 else 0.0
        }
    }

    // Perform Gauss-Jordan elimination
    for (i in 0 until n) {
        var pivot = augmented[i][i]
        if (pivot == 0.0) {
            var swapRow = -1
            for (j in i + 1 until n) {
                if (augmented[j][i] != 0.0) {
                    swapRow = j
                    break
                }
            }
            if (swapRow == -1) {
                throw RuntimeException("Matrix is singular and cannot be inverted.")
            }

            // Swap rows
            augmented[i] = augmented[swapRow].also { augmented[swapRow] = augmented[i] }
            pivot = augmented[i][i]
        }

        // Normalize the pivot row
        for (j in 0 until 2 * n) {
            augmented[i][j] /= pivot
        }

        // Eliminate the current column in all other rows
        for (j in 0 until n) {
            if (j != i) {
                val factor = augmented[j][i]
                for (k in 0 until 2 * n) {
                    augmented[j][k] -= factor * augmented[i][k]
                }
            }
        }
    }

    // Extract the inverse matrix from the augmented matrix
    for (i in 0 until n) {
        for (j in 0 until n) {
            inverse[i][j] = augmented[i][j + n]
        }
    }
}
