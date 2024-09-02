package main

import (
	"bufio"
	"errors"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	var matrix, inverse [][]float64

	// Read matrix elements from file
	err := readMatrix("../random_matrix.txt", &matrix)
	if err != nil {
		fmt.Println("Error reading matrix:", err)
		return
	}

	// Resize the inverse matrix to the size of the original matrix
	n := len(matrix)
	inverse = make([][]float64, n)
	for i := range inverse {
		inverse[i] = make([]float64, n)
	}

	// Compute the inverse matrix
	err = gaussJordanInverse(matrix, &inverse)
	if err != nil {
		fmt.Println("Error computing inverse:", err)
		return
	}

	// Write the inverse matrix to a file
	err = writeMatrixToFile("./inverse_matrix.txt", inverse)
	if err != nil {
		fmt.Println("Error writing inverse matrix to file:", err)
	}
}

func readMatrix(filename string, matrix *[][]float64) error {
	file, err := os.Open(filename)
	if err != nil {
		return errors.New("error opening file")
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	if !scanner.Scan() {
		return errors.New("error reading file")
	}
	n, err := strconv.Atoi(scanner.Text())
	if err != nil {
		return errors.New("invalid matrix size")
	}

	*matrix = make([][]float64, n)
	for i := 0; i < n; i++ {
		if !scanner.Scan() {
			return errors.New("error reading matrix data")
		}
		row := strings.Fields(scanner.Text())
		(*matrix)[i] = make([]float64, n)
		for j := 0; j < n; j++ {
			(*matrix)[i][j], err = strconv.ParseFloat(row[j], 64)
			if err != nil {
				return errors.New("invalid matrix value")
			}
		}
	}
	return nil
}

func writeMatrixToFile(filename string, matrix [][]float64) error {
	file, err := os.Create(filename)
	if err != nil {
		return errors.New("error opening file for writing")
	}
	defer file.Close()

	writer := bufio.NewWriter(file)
	for _, row := range matrix {
		for _, value := range row {
			_, err = writer.WriteString(fmt.Sprintf("%f ", value))
			if err != nil {
				return err
			}
		}
		_, err = writer.WriteString("\n")
		if err != nil {
			return err
		}
	}
	return writer.Flush()
}

func gaussJordanInverse(matrix [][]float64, inverse *[][]float64) error {
	n := len(matrix)
	augmented := make([][]float64, n)
	for i := 0; i < n; i++ {
		augmented[i] = make([]float64, 2*n)
		for j := 0; j < n; j++ {
			augmented[i][j] = matrix[i][j]
			augmented[i][j+n] = 0
			if i == j {
				augmented[i][j+n] = 1
			}
		}
	}

	// Perform Gauss-Jordan elimination
	for i := 0; i < n; i++ {
		pivot := augmented[i][i]
		if pivot == 0 {
			swapRow := -1
			for j := i + 1; j < n; j++ {
				if augmented[j][i] != 0 {
					swapRow = j
					break
				}
			}
			if swapRow == -1 {
				return errors.New("matrix is singular and cannot be inverted")
			}
			augmented[i], augmented[swapRow] = augmented[swapRow], augmented[i]
			pivot = augmented[i][i]
		}

		// Normalize the pivot row
		for j := 0; j < 2*n; j++ {
			augmented[i][j] /= pivot
		}

		// Eliminate the current column in all other rows
		for j := 0; j < n; j++ {
			if j != i {
				factor := augmented[j][i]
				for k := 0; k < 2*n; k++ {
					augmented[j][k] -= factor * augmented[i][k]
				}
			}
		}
	}

	// Extract the inverse matrix from the augmented matrix
	for i := 0; i < n; i++ {
		for j := 0; j < n; j++ {
			(*inverse)[i][j] = augmented[i][j+n]
		}
	}
	return nil
}
