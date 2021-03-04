# Mul div
a / b
= a * 2^n / b * 2^n
= (a * 2^n / b) / 2^n
= (a * 2^n / b) >> n
= (a * g) >> n // (g = 2^n / b)
n ~= 2log2(b)

# Example
a / 10
10 = 1010_2
n = 11
