import 'dart:io';

void main() {
  List<List<double>> matrix = [];
  List<List<double>> inverse = [];

  // Read matrix elements from file
  readMatrix('../random_matrix.txt', matrix);

  // Resize the inverse matrix to the size of the original matrix
  int n = matrix.length;
  inverse = List.generate(n, (_) => List.filled(n, 0.0));

  // Compute the inverse matrix
  gaussJordanInverse(matrix, inverse);

  // Write the inverse matrix to a file
  writeMatrixToFile('./inverse_matrix.txt', inverse);
}

void readMatrix(String filename, List<List<double>> matrix) {
  File file = File(filename);
  if (!file.existsSync()) {
    throw Exception("Error opening file");
  }

  List<String> lines = file.readAsLinesSync();
  int n = int.parse(lines[0]);

  matrix.clear();
  for (int i = 0; i < n; i++) {
    List<double> row = lines[i + 1].split(' ').map((e) => double.parse(e)).toList();
    matrix.add(row);
  }
}

void writeMatrixToFile(String filename, List<List<double>> matrix) {
  File file = File(filename);
  IOSink sink = file.openWrite();

  for (List<double> row in matrix) {
    sink.writeln(row.map((e) => e.toString()).join(' '));
  }

  sink.close();
}

void gaussJordanInverse(List<List<double>> matrix, List<List<double>> inverse) {
  int n = matrix.length;
  List<List<double>> augmented = List.generate(n, (_) => List.filled(2 * n, 0.0));

  // Create an augmented matrix with the identity matrix on the right
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      augmented[i][j] = matrix[i][j];
      augmented[i][j + n] = (i == j) ? 1.0 : 0.0;
    }
  }

  // Perform Gauss-Jordan elimination
  for (int i = 0; i < n; i++) {
    double pivot = augmented[i][i];
    if (pivot == 0) {
      int swapRow = -1;
      for (int j = i + 1; j < n; j++) {
        if (augmented[j][i] != 0) {
          swapRow = j;
          break;
        }
      }
      if (swapRow == -1) {
        throw Exception("Matrix is singular and cannot be inverted.");
      }

      // Swap rows
      List<double> temp = augmented[i];
      augmented[i] = augmented[swapRow];
      augmented[swapRow] = temp;

      pivot = augmented[i][i];
    }

    // Normalize the pivot row
    for (int j = 0; j < 2 * n; j++) {
      augmented[i][j] /= pivot;
    }

    // Eliminate the current column in all other rows
    for (int j = 0; j < n; j++) {
      if (j != i) {
        double factor = augmented[j][i];
        for (int k = 0; k < 2 * n; k++) {
          augmented[j][k] -= factor * augmented[i][k];
        }
      }
    }
  }

  // Extract the inverse matrix from the augmented matrix
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      inverse[i][j] = augmented[i][j + n];
    }
  }
}
