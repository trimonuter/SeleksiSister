import random
import argparse

def main():
    # Get the size of the matrix from the user
    parser = argparse.ArgumentParser(description="Generate a random n x n matrix and write to a file.")
    parser.add_argument('size', type=int, help="Size of the matrix (n)")
    
    # Set matrix size and output filename
    n = parser.parse_args().size
    filename = "./random_matrix.txt"

    # Generate the random matrix
    matrix = [[random.uniform(-50, 50) for _ in range(n)] for _ in range(n)]

    # Write the matrix to the file
    with open(filename, 'w') as f:
        f.write(f"{n}")
        for row in matrix:
            f.write("\n" + " ".join(f"{elem:.2f}" for elem in row))
    
if __name__ == "__main__":
    main()
