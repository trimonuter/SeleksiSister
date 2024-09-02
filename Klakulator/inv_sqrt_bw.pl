use strict;
use warnings;

sub add {
    # Get function arguments
    my ($a, $b) = @_;

    # Start addition loop
    loop:
        # Get carry
        my $carry = (($a & $b) << 1);

        # Get sum
        $a = ($a ^ $b);

        # Set b as carry
        if ($carry != 0) {
            $b = $carry;
            goto loop;
        }

        return $a;
}

sub subtract {
    # Get function arguments
    my ($a, $b) = @_;

    # Negate b using two's complement
    $b = add(~$b, 1);

    # Add a and -b
    return add($a, $b);
}

sub fast_inv_sqrt_bitwise {
    my ($number) = @_;

    # Constants (without using multiplication)
    my $magic = 0x5f3759df;    # Magic constant for approximation
    my $threehalfs_bits = 0x3fc00000;  # Approximate 1.5 in float bits

    # Step 1: Get the bit representation of the float
    my $i = unpack('L', pack('f', $number));  # Interpret float as int (32-bit)

    # Step 2: Approximate the inverse square root using bit manipulation
    $i = subtract($magic, $i >> 1);   # Apply the magic number and bit-shift

    # Step 3: Convert back to float
    my $y = unpack('f', pack('L', $i));

    # Step 4: Newton's method is omitted due to constraints
    # Here, we're skipping the refinement since we can't use multiplication or subtraction

    return $y;
}

# Example usage:
my $number = 1000;
my $inv_sqrt = fast_inv_sqrt_bitwise($number);
print "Fast Inverse Square Root of $number (approx) is: $inv_sqrt\n";

# Verify against actual inverse square root
my $actual_inv_sqrt = 1 / sqrt($number);
print "Actual Inverse Square Root of $number is: $actual_inv_sqrt\n";
