#include <stdio.h>
#include <stdlib.h>

void read_matrix(const char *filename, double ***matrix, int *n);
void gauss_jordan_inverse(double **matrix, double **inverse, int n);
void free_matrix(double **matrix, int n);
void write_matrix_to_file(const char *filename, double **matrix, int n);

int main() {
    int n;
    double **matrix;
    double **inverse;

    // Read n and matrix elements from file
    read_matrix("../random_matrix.txt", &matrix, &n);

    // Allocate memory for the inverse matrix
    inverse = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        inverse[i] = (double *)malloc(n * sizeof(double));
    }

    // Compute the inverse matrix
    gauss_jordan_inverse(matrix, inverse, n);

    // Write the inverse matrix to a file
    write_matrix_to_file("./inverse_matrix.txt", inverse, n);

    // Free allocated memory
    free_matrix(matrix, n);
    free_matrix(inverse, n);

    return 0;
}

void read_matrix(const char *filename, double ***matrix, int *n) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", n);

    // Allocate memory for the matrix
    *matrix = (double **)malloc(*n * sizeof(double *));
    for (int i = 0; i < *n; i++) {
        (*matrix)[i] = (double *)malloc(*n * sizeof(double));
    }

    // Read the matrix elements
    for (int i = 0; i < *n; i++) {
        for (int j = 0; j < *n; j++) {
            fscanf(file, "%lf", &(*matrix)[i][j]);
        }
    }

    fclose(file);
}

void write_matrix_to_file(const char *filename, double **matrix, int n) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(file, "%f ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}


void gauss_jordan_inverse(double **matrix, double **inverse, int n) {
    double **augmented = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        augmented[i] = (double *)malloc(2 * n * sizeof(double));
    }

    // Create an augmented matrix with the identity matrix on the right
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            augmented[i][j] = matrix[i][j];
            augmented[i][j + n] = (i == j) ? 1 : 0;
        }
    }

    // Perform Gauss-Jordan elimination
    for (int i = 0; i < n; i++) {
        // Find the pivot element
        double pivot = augmented[i][i];
        if (pivot == 0) {
            // Swap with a row below that has a non-zero pivot
            int swap_row = -1;
            for (int j = i + 1; j < n; j++) {
                if (augmented[j][i] != 0) {
                    swap_row = j;
                    break;
                }
            }
            if (swap_row == -1) {
                fprintf(stderr, "Matrix is singular and cannot be inverted.\n");
                exit(EXIT_FAILURE);
            }

            // Swap rows
            for (int k = 0; k < 2 * n; k++) {
                double temp = augmented[i][k];
                augmented[i][k] = augmented[swap_row][k];
                augmented[swap_row][k] = temp;
            }
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

    // Free allocated memory for augmented matrix
    for (int i = 0; i < n; i++) {
        free(augmented[i]);
    }
    free(augmented);
}

void free_matrix(double **matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}