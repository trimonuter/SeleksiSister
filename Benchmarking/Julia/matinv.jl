# Function to read the matrix from a file
function read_matrix(filename::String)
    matrix = []
    open(filename, "r") do file
        n = parse(Int, readline(file))  # Read the size of the matrix
        matrix = [parse.(Float64, split(readline(file))) for _ in 1:n]
    end
    return matrix
end

# Function to write matrix to file
function write_matrix_to_file(filename::String, matrix::Vector{Vector{Float64}})
    open(filename, "w") do file
        for row in matrix
            println(file, join(map(x -> string(x), row), " "))
        end
    end
end

# Gauss-Jordan inversion
function gauss_jordan_inverse(matrix::Vector{Vector{Float64}})
    n = length(matrix)
    augmented = [hcat(matrix[i], [i == j ? 1.0 : 0.0 for j in 1:n]) for i in 1:n]

    # Perform Gauss-Jordan elimination
    for i in 1:n
        pivot = augmented[i][i]
        if pivot == 0.0
            swap_row = -1
            for j in i+1:n
                if augmented[j][i] != 0.0
                    swap_row = j
                    break
                end
            end
            if swap_row == -1
                throw(ArgumentError("Matrix is singular and cannot be inverted."))
            end

            # Swap rows
            augmented[i], augmented[swap_row] = augmented[swap_row], augmented[i]
            pivot = augmented[i][i]
        end

        # Normalize the pivot row
        for j in 1:2n
            augmented[i][j] /= pivot
        end

        # Eliminate the current column in all other rows
        for j in 1:n
            if j != i
                factor = augmented[j][i]
                for k in 1:2n
                    augmented[j][k] -= factor * augmented[i][k]
                end
            end
        end
    end

    # Extract the inverse matrix from the augmented matrix
    inverse = [augmented[i][n+1:end] for i in 1:n]
    return inverse
end


# Main function
function main()
    # Read the matrix from a file
    matrix = read_matrix("../random_matrix.txt")

    # Compute the inverse
    inverse = gauss_jordan_inverse(matrix)

    # Write the inverse matrix to a file
    write_matrix_to_file("inverse_matrix.txt", inverse)
end

# Run the main function
main()