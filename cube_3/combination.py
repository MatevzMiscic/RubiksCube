



# compute binomials
n = 15
binom = [[1]]
for i in range(1, n):
    binom.append([1])
    for j in range(i - 1):
        binom[i].append(binom[i - 1][j] + binom[i - 1][j + 1])
    binom[i].append(1)
#print(binom)

# counts set bits among least significant 12 bits
def countbits(n):
    count = 0
    for i in range(12):
        count += 1 & (n >> i)
    return count

# enumerate 4 element subsets of {0,...,11}
n = 1 << 12
combrank1 = [0 for _ in range(n)]
count1 = 0
for i in range(n):
    combrank1[i] = count1
    if countbits(i) == 4:
        count1 += 1

# enumerate 4 element subsets of {0,...,8}
n = 1 << 8
combrank2 = [0 for _ in range(n)]
count2 = 0
for i in range(n):
    combrank2[i] = count2
    if countbits(i) == 4:
        count2 += 1

# computes sign of permutation (0 means even, 1 means odd)
def sign(perm):
    n = len(perm)
    cycle = 0
    vis = [False for _ in range(n)]
    for i in range(n):
        if not vis[i]:
            vis[i] = True
            cycle += 1
            j = i
            #print(j, "...", cycle)
            while perm[j] != i:
                j = perm[j]
                vis[j] = True
    return (n - cycle) % 2

