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

sub multiply {
    # Get function arguments
    my ($a, $b) = @_;

    # Determine if result is negative
    my $neg = 0;
    if ($a < 0) {
        $a = int(sprintf("%d", subtract(0, $a)));
        $neg = !$neg;
    }
    if ($b < 0) {
        $b = int(sprintf("%d", subtract(0, $b)));
        $neg = !$neg;
    }

    # Start multiplication loop
    my $sum = 0;
    loop:
        if ($b > 0) {
            $sum = add($sum, $a);
            $b = subtract($b, 1);
            goto loop;
        }

        # Set result sign
        if ($neg) {$sum = add(~$sum, 1);}

        return $sum;
}

sub divide {
    # Get function arguments
    my ($a, $b) = @_;
    
    # Determine if result is negative
    my $neg = 0;
    if ($a < 0) {
        $a = int(sprintf("%d", subtract(0, $a)));
        $neg = !$neg;
    }
    if ($b < 0) {
        $b = int(sprintf("%d", subtract(0, $b)));
        $neg = !$neg;
    }

    # Start division loop
    my $div = 0;
    loop:
        if ($a >= $b) {
            $a = subtract($a, $b);
            $div = add($div, 1);
            goto loop;
        }

        # Set result sign
        if ($neg) {$div = add(~$div, 1);}

        return $div;
}

sub power {
    # Get function arguments
    my ($a, $b) = @_;

    # Return 0 if b is negative
    if ($b < 0) {return 0};

    # Start multiplication loop
    my $prod = 1;
    loop:
        if ($b > 0) {
            $prod = int(sprintf("%d", multiply($prod, $a)));
            $b = subtract($b, 1);

            goto loop;
        }

        return $prod;
}

sub fast_inv_sqrt {
    my ($number) = @_;

    # Constants (without using multiplication)
    my $magic = 0x5f3759df;    # Magic constant for approximation

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

# Trim whitespaces of a string
sub trim {
    my ($str) = @_;
    $str =~ s/^\s+|\s+$//g;
    return $str;
}

# Convert integer string to int if not an operator
sub to_int {
    # Get arg string
    my ($exp) = @_;

    # If an operator, don't convert
    if (
        $exp eq '+' 
        || $exp eq '-' 
        || $exp eq '*' 
        || $exp eq '/'
        || $exp eq '^'
        || $exp eq ''
    ) {return $exp};

    # Convert to int
    return int($exp);
}

# Convert a string of operations to an array of tokens
sub parse_exp {
    # Get arg string
    my ($expression) = @_;

    # Convert to tokens using regex delimiter 
    my @tokens = split /(\s*[\+\-\*\/\^]\s*)/, $expression;
    #*;

    # Apply trim to each token
    my $i = 0;
    trim:
        $tokens[$i] = trim($tokens[$i]);
        
        $i++;
        if ($i < scalar @tokens) {goto trim;}

    # Convert tokens to int if needed
    $i = 0;
    convert:
        $tokens[$i] = to_int($tokens[$i]);
        
        $i++;
        if ($i < scalar @tokens) {goto convert;}
    
    return @tokens;
}

sub evaluate {
    # Get arg string
    my ($exp) = @_;

    # Convert to tokens
    my @tokens = parse_exp($exp);
    # foreach my $token (@tokens) {
    #     print "$token\n";
    # }

    # Evaluate expression
    my $result = 0;         # Result of the expression
    my $op = '+';           # Current operator
    my $neg = 0;            # Negative flag
    my $last_is_op = 1;     # Last token was an operator
    my $i = 0;              # Token index
    
    loop:
        if ($i < scalar @tokens) {
            my $token = $tokens[$i];
            
            if ($token eq '+') {$op = '+'; $last_is_op = 1;} 
            elsif ($token eq '-') {
                if ($last_is_op) {
                    $tokens[$i + 1] = 
                    int(
                        sprintf("%d", subtract(0, $tokens[$i + 1]))
                    );
                } else {
                    $op = '-';
                    $last_is_op = 1;
                }
            } 
            elsif ($token eq '*') {$op = '*'; $last_is_op = 1;} 
            elsif ($token eq '/') {$op = '/'; $last_is_op = 1;}
            elsif ($token eq '^') {$op = '^'; $last_is_op = 1;}
            elsif ($token eq '') {}
            else {
                $last_is_op = 0;

                if ($op eq '+') {
                    $result = add($result, $token);
                } elsif ($op eq '-') {
                    $result = subtract($result, $token);
                } elsif ($op eq '*') {
                    $result = multiply($result, $token);
                } elsif ($op eq '/') {
                    $result = divide($result, $token);
                } elsif ($op eq '^') {
                    $result = power($result, $token);
                }

                $result = int(sprintf("%d", $result));
            }

            $i++;
            goto loop;
        }

        return $result;
}

# Main program
my $isq = 0;
main:
    # User prompt
    print "('help' for commands) [Enter an expression]: ";

    # Get user input
    my $expression = <STDIN>;

    # Remove newline character
    chomp $expression;

    # Check for commands
    if ($expression eq 'exit') {exit;}
    if ($expression eq 'help') {
        print "\n================================================================\n";
        print "|| Commands:                                                  ||\n";
        print "||   - 'exit' to quit the program                             ||\n";
        print "||   - 'help' to display this message                         ||\n";
        print "||   - 'isq' to enable/disable inverse square root mode       ||\n";
        print "||      (calculate and display 1/sqrt(x) of final result)     ||\n";
        print "||                                                            ||\n";
        print "|| Operators:                                                 ||\n";
        print "||   - '+' for addition                                       ||\n";
        print "||   - '-' for subtraction/negative sign                      ||\n";
        print "||   - '*' for multiplication                                 ||\n";
        print "||   - '/' for division (floor for pos / ceil for neg)        ||\n";
        print "||   - '^' for power                                          ||\n";
        print "||                                                            ||\n";
        print "|| Note: no order of operations, all operations done          ||\n";
        print "||       left-to-right                                        ||\n";
        print "================================================================\n\n";
        goto main;
    } elsif ($expression eq 'isq') {
        $isq = !$isq;
        print "Inverse square root mode is now: ";
        if ($isq) {print "enabled\n\n";} else {print "disabled\n\n";}
        goto main;
    }

    # Evaluate expression
    my $result = sprintf("%d", evaluate($expression));

    # Print result
    print "Result: $result\n";
    if ($isq) {
        my $inv_sqrt = fast_inv_sqrt($result);
        print "Inverse square root of $result is approximately: $inv_sqrt\n\n";
    }

    # Repeat
    goto main;