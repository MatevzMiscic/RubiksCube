



# compute binomials
n = 15
binom = [[1]]
for i in range(1, n):
    binom.append([1])
    for j in range(i - 1):
        binom[i].append(binom[i - 1][j] + binom[i - 1][j + 1])
    binom[i].append(1)
print(binom)

# counts set bits among least significant 12 bits
def countbits(n):
    count = 0
    for i in range(12):
        count += 1 & (n >> i)
    return count

# enumerate 4 element subsets of {0,...,11}
n = 1 << 12
combrank = [0 for _ in range(n)]
count = 0
for i in range(n):
    combrank[i] = count
    if countbits(i) == 4:
        count += 1
