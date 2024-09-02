#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

using namespace std;

void read_matrix(const string& filename, vector<vector<double>>& matrix);
void gauss_jordan_inverse(const vector<vector<double>>& matrix, vector<vector<double>>& inverse);
void write_matrix_to_file(const string& filename, const vector<vector<double>>& matrix);

int main() {
    vector<vector<double>> matrix;
    vector<vector<double>> inverse;

    // Read matrix elements from file
    read_matrix("../random_matrix.txt", matrix);

    // Resize the inverse matrix to the size of the original matrix
    int n = matrix.size();
    inverse.resize(n, vector<double>(n));

    // Compute the inverse matrix
    gauss_jordan_inverse(matrix, inverse);

    // Write the inverse matrix to a file
    write_matrix_to_file("./inverse_matrix.txt", inverse);

    return 0;
}

void read_matrix(const string& filename, vector<vector<double>>& matrix) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Error opening file");
    }

    int n;
    file >> n;

    matrix.resize(n, vector<double>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file >> matrix[i][j];
        }
    }

    file.close();
}

void write_matrix_to_file(const string& filename, const vector<vector<double>>& matrix) {
    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Error opening file for writing");
    }

    int n = matrix.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file << matrix[i][j] << " ";
        }
        file << endl;
    }

    file.close();
}

void gauss_jordan_inverse(const vector<vector<double>>& matrix, vector<vector<double>>& inverse) {
    int n = matrix.size();
    vector<vector<double>> augmented(n, vector<double>(2 * n));

    // Create an augmented matrix with the identity matrix on the right
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            augmented[i][j] = matrix[i][j];
            augmented[i][j + n] = (i == j) ? 1 : 0;
        }
    }

    // Perform Gauss-Jordan elimination
    for (int i = 0; i < n; i++) {
        double pivot = augmented[i][i];
        if (pivot == 0) {
            int swap_row = -1;
            for (int j = i + 1; j < n; j++) {
                if (augmented[j][i] != 0) {
                    swap_row = j;
                    break;
                }
            }
            if (swap_row == -1) {
                throw runtime_error("Matrix is singular and cannot be inverted.");
            }

            // Swap rows
            swap(augmented[i], augmented[swap_row]);
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
