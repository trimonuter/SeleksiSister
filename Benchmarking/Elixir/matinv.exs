defmodule MatrixInverse do
  # Read matrix from a file
  def read_matrix(filename) do
    case File.read(filename) do
      {:ok, content} ->
        [n | rest] = String.split(content)
        matrix = for i <- 0..(String.to_integer(n) - 1) do
          Enum.map(rest |> Enum.slice(i * String.to_integer(n), String.to_integer(n)), &String.to_float/1)
        end
        matrix

      {:error, _} ->
        raise "Error opening file"
    end
  end

  # Write matrix to a file
  def write_matrix_to_file(filename, matrix) do
    matrix_content =
      matrix
      |> Enum.map(fn row -> Enum.join(row, " ") end)
      |> Enum.join("\n")

    case File.write(filename, matrix_content) do
      :ok -> :ok
      {:error, _} -> raise "Error writing file"
    end
  end

  # Gauss-Jordan elimination to calculate the inverse matrix
  def gauss_jordan_inverse(matrix) do
    n = length(matrix)
    augmented = Enum.map(0..(n-1), fn i ->
      Enum.concat(Enum.at(matrix, i), Enum.map(0..(n-1), fn j -> if i == j, do: 1.0, else: 0.0 end))
    end)

    augmented = Enum.reduce(0..(n-1), augmented, fn i, augmented ->
      pivot = Enum.at(Enum.at(augmented, i), i)

      augmented =
        if pivot == 0 do
          swap_row =
            Enum.find_index((i + 1)..(n - 1), fn j -> Enum.at(Enum.at(augmented, j), i) != 0 end)

          if swap_row == nil do
            raise "Matrix is singular and cannot be inverted"
          end

          List.replace_at(augmented, i, Enum.at(augmented, swap_row))
          |> List.replace_at(swap_row, Enum.at(augmented, i))
        else
          augmented
        end

      pivot = Enum.at(Enum.at(augmented, i), i)

      augmented =
        List.update_at(augmented, i, fn row ->
          Enum.map(row, fn x -> x / pivot end)
        end)

      Enum.reduce(0..(n-1), augmented, fn j, augmented ->
        if j != i do
          factor = Enum.at(Enum.at(augmented, j), i)

          List.update_at(augmented, j, fn row ->
            Enum.zip(row, Enum.at(augmented, i))
            |> Enum.map(fn {r1, r2} -> r1 - factor * r2 end)
          end)
        else
          augmented
        end
      end)
    end)

    for i <- 0..(n-1) do
      Enum.slice(Enum.at(augmented, i), n, n)
    end
  end
end

# Main execution to read, calculate inverse, and write result
defmodule Main do
  def run do
    matrix = MatrixInverse.read_matrix("../random_matrix.txt")
    inverse = MatrixInverse.gauss_jordan_inverse(matrix)
    MatrixInverse.write_matrix_to_file("./inverse_matrix.txt", inverse)
  end
end

Main.run()
