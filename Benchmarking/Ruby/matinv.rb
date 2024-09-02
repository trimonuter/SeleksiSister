require 'matrix'

def read_matrix(filename)
  matrix = []
  File.open(filename, 'r') do |file|
    n = file.gets.to_i
    n.times do
      matrix << file.gets.split.map(&:to_f)
    end
  end
  Matrix[*matrix]
end

def write_matrix(filename, matrix)
  File.open(filename, 'w') do |file|
    matrix.row_count.times do |i|
      file.puts matrix.row(i).to_a.join(' ')
    end
  end
end

def gauss_jordan_inverse(matrix)
  n = matrix.row_count
  augmented = Matrix.build(n, 2 * n) do |i, j|
    if j < n
      matrix[i, j]
    else
      i == j - n ? 1 : 0
    end
  end

  # Perform Gauss-Jordan elimination
  (0...n).each do |i|
    pivot = augmented[i, i]
    if pivot == 0
      swap_row = (i + 1...n).find { |j| augmented[j, i] != 0 }
      raise "Matrix is singular and cannot be inverted." unless swap_row

      augmented[i, true], augmented[swap_row, true] = augmented[swap_row, true], augmented[i, true]
      pivot = augmented[i, i]
    end

    # Normalize the pivot row
    (0...2 * n).each do |j|
      augmented[i, j] /= pivot
    end

    # Eliminate the current column in all other rows
    (0...n).each do |j|
      next if j == i

      factor = augmented[j, i]
      (0...2 * n).each do |k|
        augmented[j, k] -= factor * augmented[i, k]
      end
    end
  end

  # Extract the inverse matrix from the augmented matrix
  Matrix.build(n, n) do |i, j|
    augmented[i, j + n]
  end
end

def main
  matrix = read_matrix('../random_matrix.txt')
  inverse = gauss_jordan_inverse(matrix)
  write_matrix('./inverse_matrix.txt', inverse)
end

main
