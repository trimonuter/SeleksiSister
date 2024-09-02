open System
open System.IO

let readMatrix (filename: string) =
    let lines = File.ReadAllLines(filename)
    let n = Int32.Parse(lines.[0])
    let matrix = Array.init n (fun _ -> Array.zeroCreate<double> n)

    for i in 0 .. (n - 1) do
        let values = lines.[i + 1].Split(' ') |> Array.map Double.Parse
        Array.blit values 0 matrix.[i] 0 n

    matrix

let writeMatrixToFile (filename: string) (matrix: double [][]) =
    let n = matrix.Length
    use writer = new StreamWriter(filename)
    for i in 0 .. (n - 1) do
        let row = matrix.[i]
        writer.WriteLine(String.Join(" ", row))

let gaussJordanInverse (matrix: double [][]) =
    let n = matrix.Length
    let augmented = Array.init n (fun i -> Array.init (2 * n) (fun j ->
        if j < n then matrix.[i].[j] else if j = i + n then 1.0 else 0.0))
    
    for i in 0 .. (n - 1) do
        let pivot = augmented.[i].[i]
        if pivot = 0.0 then
            let swapRow = Array.findIndex (fun j -> augmented.[j].[i] <> 0.0) (Array.init n id)
            if swapRow = -1 then
                failwith "Matrix is singular and cannot be inverted."
            else
                // Swap rows
                let temp = augmented.[i]
                augmented.[i] <- augmented.[swapRow]
                augmented.[swapRow] <- temp
        
        let pivot = augmented.[i].[i]
        // Normalize the pivot row
        for j in 0 .. (2 * n - 1) do
            augmented.[i].[j] <- augmented.[i].[j] / pivot
        
        // Eliminate the current column in all other rows
        for j in 0 .. (n - 1) do
            if j <> i then
                let factor = augmented.[j].[i]
                for k in 0 .. (2 * n - 1) do
                    augmented.[j].[k] <- augmented.[j].[k] - factor * augmented.[i].[k]

    // Extract the inverse matrix from the augmented matrix
    Array.init n (fun i -> Array.sub augmented.[i] n n)

[<EntryPoint>]
let main argv =
    try
        let matrix = readMatrix "../random_matrix.txt"
        let inverse = gaussJordanInverse matrix
        writeMatrixToFile "./inverse_matrix.txt" inverse
    with
    | ex -> printfn "An error occurred: %s" ex.Message
    0
