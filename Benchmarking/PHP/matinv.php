<?php

function readMatrix($filename, &$matrix) {
    $file = fopen($filename, "r");
    if (!$file) {
        throw new Exception("Error opening file");
    }

    $n = (int)fgets($file);
    $matrix = array_fill(0, $n, array_fill(0, $n, 0));

    for ($i = 0; $i < $n; $i++) {
        $line = fgets($file);
        $values = explode(' ', trim($line));
        for ($j = 0; $j < $n; $j++) {
            $matrix[$i][$j] = (float)$values[$j];
        }
    }

    fclose($file);
}

function writeMatrixToFile($filename, $matrix) {
    $file = fopen($filename, "w");
    if (!$file) {
        throw new Exception("Error opening file for writing");
    }

    $n = count($matrix);
    for ($i = 0; $i < $n; $i++) {
        fwrite($file, implode(' ', $matrix[$i]) . PHP_EOL);
    }

    fclose($file);
}

function gaussJordanInverse($matrix, &$inverse) {
    $n = count($matrix);
    $augmented = array_fill(0, $n, array_fill(0, 2 * $n, 0));

    // Create an augmented matrix with the identity matrix on the right
    for ($i = 0; $i < $n; $i++) {
        for ($j = 0; $j < $n; $j++) {
            $augmented[$i][$j] = $matrix[$i][$j];
            $augmented[$i][$j + $n] = ($i == $j) ? 1 : 0;
        }
    }

    // Perform Gauss-Jordan elimination
    for ($i = 0; $i < $n; $i++) {
        $pivot = $augmented[$i][$i];
        if ($pivot == 0) {
            $swapRow = -1;
            for ($j = $i + 1; $j < $n; $j++) {
                if ($augmented[$j][$i] != 0) {
                    $swapRow = $j;
                    break;
                }
            }
            if ($swapRow == -1) {
                throw new Exception("Matrix is singular and cannot be inverted.");
            }

            // Swap rows
            $temp = $augmented[$i];
            $augmented[$i] = $augmented[$swapRow];
            $augmented[$swapRow] = $temp;

            $pivot = $augmented[$i][$i];
        }

        // Normalize the pivot row
        for ($j = 0; $j < 2 * $n; $j++) {
            $augmented[$i][$j] /= $pivot;
        }

        // Eliminate the current column in all other rows
        for ($j = 0; $j < $n; $j++) {
            if ($j != $i) {
                $factor = $augmented[$j][$i];
                for ($k = 0; $k < 2 * $n; $k++) {
                    $augmented[$j][$k] -= $factor * $augmented[$i][$k];
                }
            }
        }
    }

    // Extract the inverse matrix from the augmented matrix
    $inverse = array_fill(0, $n, array_fill(0, $n, 0));
    for ($i = 0; $i < $n; $i++) {
        for ($j = 0; $j < $n; $j++) {
            $inverse[$i][$j] = $augmented[$i][$j + $n];
        }
    }
}

// Main function
try {
    $matrix = [];
    $inverse = [];

    // Read matrix elements from file
    readMatrix("../random_matrix.txt", $matrix);

    // Compute the inverse matrix
    gaussJordanInverse($matrix, $inverse);

    // Write the inverse matrix to a file
    writeMatrixToFile("./inverse_matrix.txt", $inverse);
} catch (Exception $e) {
    echo "Error: " . $e->getMessage() . "\n";
}
