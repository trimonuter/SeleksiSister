def read_matrix(filename):
    with open(filename, 'r') as f:
        # Read the size of the matrix
        n = int(f.readline())
        
        # Read the matrix elements
        matrix = [[float(elem) for elem in f.readline().split()] for _ in range(n)]
    
    return matrix

def write_matrix_to_file(filename, matrix):
    with open(filename, 'w') as f:
        for row in matrix:
            f.write(" ".join(f"{elem}" for elem in row) + "\n")

def gauss_jordan_inverse(matrix):
    # Get matrix length
    n = len(matrix)
    
    # Create an augmented matrix with the identity matrix on the right
    augmented_matrix = [row + [1 if i == j else 0 for j in range(n)] for i, row in enumerate(matrix)]
    
    # Perform Gauss-Jordan elimination
    for i in range(n):
        # Find the pivot element
        pivot = augmented_matrix[i][i]
        if pivot == 0:
            # Swap with a row below that has a non-zero pivot
            for j in range(i + 1, n):
                if augmented_matrix[j][i] != 0:
                    augmented_matrix[i], augmented_matrix[j] = augmented_matrix[j], augmented_matrix[i]
                    pivot = augmented_matrix[i][i]
                    break
            else:
                raise ValueError("Matrix is singular and cannot be inverted.")
        
        # Normalize the pivot row
        for j in range(2 * n):
            augmented_matrix[i][j] /= pivot
        
        # Eliminate the current column in all other rows
        for j in range(n):
            if j != i:
                factor = augmented_matrix[j][i]
                for k in range(2 * n):
                    augmented_matrix[j][k] -= factor * augmented_matrix[i][k]
    
    # Extract the inverse matrix from the augmented matrix
    inverse_matrix = [row[n:] for row in augmented_matrix]
    
    return inverse_matrix

# Main program
matrix = read_matrix("../random_matrix.txt")
inverse_matrix = gauss_jordan_inverse(matrix)

write_matrix_to_file("./inverse_matrix.txt", inverse_matrix)