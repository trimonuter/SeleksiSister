const fs = require('fs');

function readMatrix(filename) {
    const data = fs.readFileSync(filename, 'utf8').split('\n').filter(line => line.trim() !== '');
    const n = parseInt(data[0].trim());
    const matrix = [];

    for (let i = 1; i <= n; i++) {
        const row = data[i].trim().split(' ').map(Number);
        matrix.push(row);
    }

    return matrix;
}

function writeMatrixToFile(filename, matrix) {
    const n = matrix.length;
    const lines = matrix.map(row => row.join(' '));
    fs.writeFileSync(filename, lines.join('\n') + '\n', 'utf8');
}

function gaussJordanInverse(matrix) {
    const n = matrix.length;
    const augmented = matrix.map(row => [...row, ...Array(n).fill(0)]);
    
    for (let i = 0; i < n; i++) {
        augmented[i][i + n] = 1;
    }

    for (let i = 0; i < n; i++) {
        let pivot = augmented[i][i];
        if (pivot === 0) {
            let swapRow = -1;
            for (let j = i + 1; j < n; j++) {
                if (augmented[j][i] !== 0) {
                    swapRow = j;
                    break;
                }
            }
            if (swapRow === -1) {
                throw new Error('Matrix is singular and cannot be inverted.');
            }

            [augmented[i], augmented[swapRow]] = [augmented[swapRow], augmented[i]];
            pivot = augmented[i][i];
        }

        for (let j = 0; j < 2 * n; j++) {
            augmented[i][j] /= pivot;
        }

        for (let j = 0; j < n; j++) {
            if (j !== i) {
                const factor = augmented[j][i];
                for (let k = 0; k < 2 * n; k++) {
                    augmented[j][k] -= factor * augmented[i][k];
                }
            }
        }
    }

    const inverse = augmented.map(row => row.slice(n, 2 * n));
    return inverse;
}

function main() {
    try {
        const matrix = readMatrix('../random_matrix.txt');
        const inverse = gaussJordanInverse(matrix);
        writeMatrixToFile('./inverse_matrix.txt', inverse);
    } catch (error) {
        console.error('Error:', error.message);
    }
}

main();
