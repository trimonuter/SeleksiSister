local function read_matrix(filename)
    local matrix = {}
    local file = io.open(filename, "r")
    
    if not file then
        error("Error opening file")
    end
    
    -- Read matrix size
    local n = tonumber(file:read("*n"))
    
    -- Read the matrix elements
    for i = 1, n do
        matrix[i] = {}
        for j = 1, n do
            matrix[i][j] = tonumber(file:read("*n"))
        end
    end
    
    file:close()
    return matrix
end

local function write_matrix_to_file(filename, matrix)
    local file = io.open(filename, "w")
    
    if not file then
        error("Error opening file for writing")
    end
    
    for i = 1, #matrix do
        for j = 1, #matrix[i] do
            file:write(string.format("%.8f ", matrix[i][j]))
        end
        file:write("\n")
    end
    
    file:close()
end

local function gauss_jordan_inverse(matrix)
    local n = #matrix
    local augmented = {}
    
    -- Create an augmented matrix with the identity matrix on the right
    for i = 1, n do
        augmented[i] = {}
        for j = 1, n do
            augmented[i][j] = matrix[i][j]
            augmented[i][j + n] = (i == j) and 1 or 0
        end
    end
    
    -- Perform Gauss-Jordan elimination
    for i = 1, n do
        local pivot = augmented[i][i]
        if pivot == 0 then
            local swap_row = -1
            for j = i + 1, n do
                if augmented[j][i] ~= 0 then
                    swap_row = j
                    break
                end
            end
            if swap_row == -1 then
                error("Matrix is singular and cannot be inverted.")
            end
            
            -- Swap rows
            augmented[i], augmented[swap_row] = augmented[swap_row], augmented[i]
            pivot = augmented[i][i]
        end
        
        -- Normalize the pivot row
        for j = 1, 2 * n do
            augmented[i][j] = augmented[i][j] / pivot
        end
        
        -- Eliminate the current column in all other rows
        for j = 1, n do
            if j ~= i then
                local factor = augmented[j][i]
                for k = 1, 2 * n do
                    augmented[j][k] = augmented[j][k] - factor * augmented[i][k]
                end
            end
        end
    end
    
    -- Extract the inverse matrix from the augmented matrix
    local inverse = {}
    for i = 1, n do
        inverse[i] = {}
        for j = 1, n do
            inverse[i][j] = augmented[i][j + n]
        end
    end
    
    return inverse
end

-- Main function
local function main()
    local matrix = read_matrix("../random_matrix.txt")
    
    -- Compute the inverse matrix
    local inverse = gauss_jordan_inverse(matrix)
    
    -- Write the inverse matrix to a file
    write_matrix_to_file("./inverse_matrix.txt", inverse)
end

main()