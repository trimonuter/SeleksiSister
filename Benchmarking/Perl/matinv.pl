use strict;
use warnings;
use File::Slurp;

sub read_matrix {
    my ($filename) = @_;
    my @matrix;
    
    my $data = read_file($filename);
    my @lines = split /\n/, $data;
    my $n = shift @lines;
    
    foreach my $line (@lines) {
        my @row = split ' ', $line;
        push @matrix, \@row;
    }
    
    return ($n, \@matrix);
}

sub write_matrix_to_file {
    my ($filename, $matrix) = @_;
    my $n = scalar @$matrix;
    my @lines;
    
    foreach my $row (@$matrix) {
        push @lines, join(' ', @$row);
    }
    
    write_file($filename, join("\n", @lines) . "\n");
}

sub gauss_jordan_inverse {
    my ($matrix, $n) = @_;
    my @augmented;
    
    # Create an augmented matrix with the identity matrix on the right
    for my $i (0..$n-1) {
        push @augmented, [ @{$matrix->[$i]}, (0) x $n ];
        $augmented[$i][$i + $n] = 1;
    }
    
    # Perform Gauss-Jordan elimination
    for my $i (0..$n-1) {
        my $pivot = $augmented[$i][$i];
        if ($pivot == 0) {
            my $swap_row = -1;
            for my $j ($i + 1 .. $n - 1) {
                if ($augmented[$j][$i] != 0) {
                    $swap_row = $j;
                    last;
                }
            }
            die "Matrix is singular and cannot be inverted.\n" if $swap_row == -1;
            
            # Swap rows
            @augmented[$i, $swap_row] = @augmented[$swap_row, $i];
            $pivot = $augmented[$i][$i];
        }
        
        # Normalize the pivot row
        for my $j (0 .. 2 * $n - 1) {
            $augmented[$i][$j] /= $pivot;
        }
        
        # Eliminate the current column in all other rows
        for my $j (0 .. $n - 1) {
            if ($j != $i) {
                my $factor = $augmented[$j][$i];
                for my $k (0 .. 2 * $n - 1) {
                    $augmented[$j][$k] -= $factor * $augmented[$i][$k];
                }
            }
        }
    }
    
    # Extract the inverse matrix from the augmented matrix
    my @inverse;
    for my $i (0..$n-1) {
        push @inverse, [ @{$augmented[$i]}[$n..2*$n-1] ];
    }    
    return \@inverse;
}

sub main {
    my ($n, $matrix) = read_matrix("../random_matrix.txt");
    
    my $inverse = gauss_jordan_inverse($matrix, $n);
    
    write_matrix_to_file("./inverse_matrix.txt", $inverse);
}

main();