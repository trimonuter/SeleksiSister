using System;
using System.Collections.Generic;
using System.IO;

class Program
{
    static void Main()
    {
        List<List<double>> matrix = new List<List<double>>();
        List<List<double>> inverse = new List<List<double>>();

        // Read matrix elements from file
        ReadMatrix("../random_matrix.txt", matrix);

        // Resize the inverse matrix to the size of the original matrix
        int n = matrix.Count;
        for (int i = 0; i < n; i++)
        {
            inverse.Add(new List<double>(new double[n]));
        }

        // Compute the inverse matrix
        GaussJordanInverse(matrix, inverse);

        // Write the inverse matrix to a file
        WriteMatrixToFile("./inverse_matrix.txt", inverse);
    }

    static void ReadMatrix(string filename, List<List<double>> matrix)
    {
        using (StreamReader file = new StreamReader(filename))
        {
            if (!file.EndOfStream)
            {
                int n = int.Parse(file.ReadLine());

                for (int i = 0; i < n; i++)
                {
                    List<double> row = new List<double>();
                    string[] rowValues = file.ReadLine().Split(' ');

                    foreach (var value in rowValues)
                    {
                        row.Add(double.Parse(value));
                    }

                    matrix.Add(row);
                }
            }
            else
            {
                throw new Exception("Error opening file");
            }
        }
    }

    static void WriteMatrixToFile(string filename, List<List<double>> matrix)
    {
        using (StreamWriter file = new StreamWriter(filename))
        {
            int n = matrix.Count;
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    file.Write(matrix[i][j] + " ");
                }
                file.WriteLine();
            }
        }
    }

    static void GaussJordanInverse(List<List<double>> matrix, List<List<double>> inverse)
    {
        int n = matrix.Count;
        List<List<double>> augmented = new List<List<double>>(n);

        // Create an augmented matrix with the identity matrix on the right
        for (int i = 0; i < n; i++)
        {
            augmented.Add(new List<double>(new double[2 * n]));
            for (int j = 0; j < n; j++)
            {
                augmented[i][j] = matrix[i][j];
                augmented[i][j + n] = (i == j) ? 1 : 0;
            }
        }

        // Perform Gauss-Jordan elimination
        for (int i = 0; i < n; i++)
        {
            double pivot = augmented[i][i];
            if (pivot == 0)
            {
                int swapRow = -1;
                for (int j = i + 1; j < n; j++)
                {
                    if (augmented[j][i] != 0)
                    {
                        swapRow = j;
                        break;
                    }
                }
                if (swapRow == -1)
                {
                    throw new Exception("Matrix is singular and cannot be inverted.");
                }

                // Swap rows
                var temp = augmented[i];
                augmented[i] = augmented[swapRow];
                augmented[swapRow] = temp;

                pivot = augmented[i][i];
            }

            // Normalize the pivot row
            for (int j = 0; j < 2 * n; j++)
            {
                augmented[i][j] /= pivot;
            }

            // Eliminate the current column in all other rows
            for (int j = 0; j < n; j++)
            {
                if (j != i)
                {
                    double factor = augmented[j][i];
                    for (int k = 0; k < 2 * n; k++)
                    {
                        augmented[j][k] -= factor * augmented[i][k];
                    }
                }
            }
        }

        // Extract the inverse matrix from the augmented matrix
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                inverse[i][j] = augmented[i][j + n];
            }
        }
    }
}
