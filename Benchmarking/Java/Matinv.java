import java.io.*;
import java.util.ArrayList;

public class Matinv {
    public static void main(String[] args) {
        ArrayList<ArrayList<Double>> matrix = new ArrayList<>();
        ArrayList<ArrayList<Double>> inverse = new ArrayList<>();

        // Read matrix elements from file
        readMatrix("../random_matrix.txt", matrix);

        // Resize the inverse matrix to the size of the original matrix
        int n = matrix.size();
        for (int i = 0; i < n; i++) {
            inverse.add(new ArrayList<>());
            for (int j = 0; j < n; j++) {
                inverse.get(i).add(0.0);
            }
        }

        // Compute the inverse matrix
        gaussJordanInverse(matrix, inverse);

        // Write the inverse matrix to a file
        writeMatrixToFile("./inverse_matrix.txt", inverse);
    }

    public static void readMatrix(String filename, ArrayList<ArrayList<Double>> matrix) {
        try (BufferedReader reader = new BufferedReader(new FileReader(filename))) {
            int n = Integer.parseInt(reader.readLine().trim());

            for (int i = 0; i < n; i++) {
                ArrayList<Double> row = new ArrayList<>();
                String[] elements = reader.readLine().trim().split("\\s+");
                for (String element : elements) {
                    row.add(Double.parseDouble(element));
                }
                matrix.add(row);
            }

        } catch (IOException e) {
            throw new RuntimeException("Error opening file", e);
        }
    }

    public static void writeMatrixToFile(String filename, ArrayList<ArrayList<Double>> matrix) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(filename))) {
            int n = matrix.size();
            for (ArrayList<Double> row : matrix) {
                for (Double element : row) {
                    writer.write(element + " ");
                }
                writer.newLine();
            }
        } catch (IOException e) {
            throw new RuntimeException("Error writing to file", e);
        }
    }

    public static void gaussJordanInverse(ArrayList<ArrayList<Double>> matrix, ArrayList<ArrayList<Double>> inverse) {
        int n = matrix.size();
        ArrayList<ArrayList<Double>> augmented = new ArrayList<>();

        // Create an augmented matrix with the identity matrix on the right
        for (int i = 0; i < n; i++) {
            ArrayList<Double> row = new ArrayList<>();
            for (int j = 0; j < n; j++) {
                row.add(matrix.get(i).get(j));
            }
            for (int j = 0; j < n; j++) {
                row.add(i == j ? 1.0 : 0.0);
            }
            augmented.add(row);
        }

        // Perform Gauss-Jordan elimination
        for (int i = 0; i < n; i++) {
            double pivot = augmented.get(i).get(i);
            if (pivot == 0) {
                int swapRow = -1;
                for (int j = i + 1; j < n; j++) {
                    if (augmented.get(j).get(i) != 0) {
                        swapRow = j;
                        break;
                    }
                }
                if (swapRow == -1) {
                    throw new RuntimeException("Matrix is singular and cannot be inverted.");
                }

                // Swap rows
                ArrayList<Double> temp = augmented.get(i);
                augmented.set(i, augmented.get(swapRow));
                augmented.set(swapRow, temp);
                pivot = augmented.get(i).get(i);
            }

            // Normalize the pivot row
            for (int j = 0; j < 2 * n; j++) {
                augmented.get(i).set(j, augmented.get(i).get(j) / pivot);
            }

            // Eliminate the current column in all other rows
            for (int j = 0; j < n; j++) {
                if (j != i) {
                    double factor = augmented.get(j).get(i);
                    for (int k = 0; k < 2 * n; k++) {
                        augmented.get(j).set(k, augmented.get(j).get(k) - factor * augmented.get(i).get(k));
                    }
                }
            }
        }

        // Extract the inverse matrix from the augmented matrix
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                inverse.get(i).set(j, augmented.get(i).get(j + n));
            }
        }
    }
}
