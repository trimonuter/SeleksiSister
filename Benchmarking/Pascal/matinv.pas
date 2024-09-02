program MatrixInverse;

type
  TMatrix = array of array of Real;

var
  matrix, inverse: TMatrix;
  n: Integer;

procedure ReadMatrix(const filename: string; var m: TMatrix);
var
  f: Text;
  i, j: Integer;
begin
  Assign(f, filename);
  Reset(f);

  ReadLn(f, n);
  SetLength(m, n, n);

  for i := 0 to n - 1 do
    for j := 0 to n - 1 do
      Read(f, m[i][j]);

  Close(f);
end;

procedure WriteMatrixToFile(const filename: string; const m: TMatrix);
var
  f: Text;
  i, j: Integer;
begin
  Assign(f, filename);
  Rewrite(f);

  for i := 0 to Length(m) - 1 do
  begin
    for j := 0 to Length(m[i]) - 1 do
      Write(f, m[i][j]:0:6, ' ');  // Adjusted to 6 decimal places
    WriteLn(f);
  end;

  Close(f);
end;

procedure GaussJordanInverse(const matrix: TMatrix; var inverse: TMatrix);
var
  i, j, k: Integer;
  pivot, factor: Real;
  augmented: TMatrix;
begin
  n := Length(matrix);
  SetLength(augmented, n, 2 * n);

  // Create an augmented matrix with the identity matrix on the right
  for i := 0 to n - 1 do
  begin
    for j := 0 to n - 1 do
    begin
      augmented[i][j] := matrix[i][j];
      if i = j then
        augmented[i][j + n] := 1.0
      else
        augmented[i][j + n] := 0.0;
    end;
  end;

  // Perform Gauss-Jordan elimination
  for i := 0 to n - 1 do
  begin
    pivot := augmented[i][i];
    if pivot = 0 then
    begin
      // Find a row to swap with
      k := i + 1;
      while (k < n) and (augmented[k][i] = 0) do
        Inc(k);

      if k = n then
      begin
        WriteLn('Matrix is singular and cannot be inverted.');
        Halt(1);
      end;

      // Swap rows
      for j := 0 to 2 * n - 1 do
      begin
        factor := augmented[i][j];
        augmented[i][j] := augmented[k][j];
        augmented[k][j] := factor;
      end;

      pivot := augmented[i][i];
    end;

    // Normalize the pivot row
    for j := 0 to 2 * n - 1 do
      augmented[i][j] := augmented[i][j] / pivot;

    // Eliminate the current column in all other rows
    for k := 0 to n - 1 do
    begin
      if k <> i then
      begin
        factor := augmented[k][i];
        for j := 0 to 2 * n - 1 do
          augmented[k][j] := augmented[k][j] - factor * augmented[i][j];
      end;
    end;
  end;

  // Extract the inverse matrix from the augmented matrix
  SetLength(inverse, n, n);
  for i := 0 to n - 1 do
    for j := 0 to n - 1 do
      inverse[i][j] := augmented[i][j + n];
end;

begin
  // Read matrix elements from file
  ReadMatrix('../random_matrix.txt', matrix);

  // Compute the inverse matrix
  SetLength(inverse, Length(matrix), Length(matrix[0]));
  GaussJordanInverse(matrix, inverse);

  // Write the inverse matrix to a file
  WriteMatrixToFile('./inverse_matrix.txt', inverse);
end.
