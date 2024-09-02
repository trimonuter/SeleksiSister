<!-- ![dog](./img/dog.jpg) -->
<div style="text-align:center;">
    <img src="./img/dog.jpg" alt="dog">
</div>

# Cara Menjalankan

`perl calc.pl` untuk menjalankan kalkulator.

`python3 randexp.py` untuk menjalankan *random expression generator* untuk ujicoba kalkulator.

# Cara Menggunakan

```
================================================================
|| Commands:                                                  ||
||   - 'exit' to quit the program                             ||
||   - 'help' to display this message                         ||
||   - 'isq' to enable/disable inverse square root mode       ||
||      (calculate and display 1/sqrt(x) of final result)     ||
||                                                            ||
|| Operators:                                                 ||
||   - '+' for addition                                       ||
||   - '-' for subtraction/negative sign                      ||
||   - '*' for multiplication                                 ||
||   - '/' for division (floor for pos / ceil for neg)        ||
||   - '^' for power                                          ||
||                                                            ||
|| Note: no order of operations, all operations done          ||
||       left-to-right                                        ||
================================================================
```

# Ujicoba Operator

## +
```
('help' for commands) [Enter an expression]: 10 + 20
Result: 30
('help' for commands) [Enter an expression]: 5 + 0
Result: 5
('help' for commands) [Enter an expression]: -5 + 17
Result: 12
('help' for commands) [Enter an expression]: -2 + -3 + -5 + -10
Result: -20
('help' for commands) [Enter an expression]: -99+100
Result: 1
```

## -
```
('help' for commands) [Enter an expression]: 5 -3
Result: 2
('help' for commands) [Enter an expression]: 9 - 7
Result: 2
('help' for commands) [Enter an expression]: 40 - 0
Result: 40
('help' for commands) [Enter an expression]: 1 - 999999
Result: -999998
('help' for commands) [Enter an expression]: -5 -7 -3 -10 -20 -34
Result: -79
```

## *
```
('help' for commands) [Enter an expression]: 5 * 1
Result: 5
('help' for commands) [Enter an expression]: 3 * 17
Result: 51
('help' for commands) [Enter an expression]: 15 * 0
Result: 0
('help' for commands) [Enter an expression]: 20 * -4
Result: -80
('help' for commands) [Enter an expression]: -13 * -13
Result: 169
```

## /
```
('help' for commands) [Enter an expression]: 9 / 3
Result: 3
('help' for commands) [Enter an expression]: 10 / 3
Result: 3
('help' for commands) [Enter an expression]: 43 / 11
Result: 3
('help' for commands) [Enter an expression]: 169/13
Result: 13
('help' for commands) [Enter an expression]: -97 / 3
Result: -32
```

## ^
```
('help' for commands) [Enter an expression]: 2 ^ 8
Result: 256
('help' for commands) [Enter an expression]: 5 ^ 3
Result: 125
('help' for commands) [Enter an expression]: 3 ^ 2 ^ 2
Result: 81
('help' for commands) [Enter an expression]: 8 ^ -2
Result: 0
('help' for commands) [Enter an expression]: 10 ^ 4
Result: 10000
```

## isq
```
('help' for commands) [Enter an expression]: isq
Inverse square root mode is now: enabled

('help' for commands) [Enter an expression]: 3 * 17 + 9
Result: 60
Inverse square root of 60 is approximately: 0.124683134257793

('help' for commands) [Enter an expression]: 9 * 7 -11
Result: 52
Inverse square root of 52 is approximately: 0.139991268515587

('help' for commands) [Enter an expression]: 20 * 7 + 11
Result: 151
Inverse square root of 151 is approximately: 0.0839116498827934

('help' for commands) [Enter an expression]: 9 * 22
Result: 198
Inverse square root of 198 is approximately: 0.0724370405077934

('help' for commands) [Enter an expression]: 7 * 19
Result: 133
Inverse square root of 133 is approximately: 0.0883061811327934
```

# fin.

![my beloved](./img/ruri.png)
<div style="display: flex; flex-direction: column; justify-content: center; align-items: center; margin:0px; padding:0px;">
  <h3><i>Ice age coming, ice age coming, let me hear both sides</i></h3>
  <h3><i>>pol</i></h3>
</div>