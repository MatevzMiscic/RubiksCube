


class EncodedSet:

    # creates a set with numbers 0, 1,..., n-1
    def __init__(self, n):
        self.b = 0
        flag = 1
        while flag <= n:
            flag <<= 1
            self.b += 1
        self.n = n
        self.bits = 0
        self.mul = 0
        self.separ = 0
        for i in range(n):
            self.bits = (self.bits << (self.b + 1)) + (n - 1 - i)
            self.mul = (self.mul << (self.b + 1)) + 1
            #print(str(bin(self.bits))[2:])
            #print(str(bin(self.mul))[2:])
        self.separ = self.mul << self.b
        self.mod = (2 << self.b) - 1
        #print(self.b)
        #print(self.bits)
        #print("mul =", str(bin(self.mul))[2:])
        #print("fie =", str(bin(self.separ))[2:])
        #print("mod =", str(bin(self.mod))[2:])


    # returns number of elements of the set that are less than x
    # x must be in the set
    def rank(self, x):
        c = (self.mul * x) | self.separ # 
        #print("c =", str(bin(c))[2:])
        num = (c - self.bits) & self.separ # separator bits of elements less than or equal to x are set to one
        #print("num =", str(bin(num))[2:])
        return ((num >> self.b) % self.mod) - 1 # we return one less than the number of separators set to one

    # deletes element x from the set
    # x must be in the set
    def delete(self, x):
        r = self.rank(x)
        #print(self)
        #print("rank =", r)
        left = (self.bits >> ((r + 1) * (self.b + 1))) << (r * (self.b + 1))
        right = self.bits & ((1 << (r * (self.b + 1))) - 1)
        self.bits = left | right
        self.mul >>= self.b + 1
        self.separ >>= self.b + 1
        #print("left =", str(bin(left))[2:])
        #print("righ =", str(bin(right))[2:])
        #print("bits =", str(bin(self.bits))[2:])

    def __repr__(self):
        l = [str(((2 << self.b) - 1) & (self.bits >> (i * (self.b + 1)))) for i in range(self.n)]
        a =  " ".join(l)
        b = str(bin(self.bits))[2:]
        return a + "\n" + b
        #return str(bin(self.bits))[2:]



"""
s = EncodedSet(5)
for i in range(5):
    print(s.rank(i))
s.delete(3)
for i in range(5):
    if i != 3:
        print(s.rank(i))
"""

# computes factorials
n = 15
fact = [1 for _ in range(n)]
f = 1
for i in range(1, n):
    f *= i
    fact[i] = f


# returns the rank of the perm[i,...,n-1] on set
def rank_helper(perm, i, n, eset):
    if i >= n - 1:
        return 0
    #print("i =", i)
    #print(eset)
    a = eset.rank(perm[i])
    eset.delete(perm[i])
    #print(eset)
    b = rank_helper(perm, i + 1, n, eset)
    return a * fact[n - 1 - i] + b

# returns the lexicographic rank of the given permutation of integers 0, 1,..., len(perm) - 1
def rank(perm):
    n = len(perm)
    eset = EncodedSet(n)
    return rank_helper(perm, 0, n, eset)


# returns number of permutation of given sign less than perm[i,..., n-1] on set in lex order
def rank_sign_helper(perm, i, n, sign, eset):
    if i >= n - 1:
        return 0
    a = eset.rank(perm[i])
    if i == n - 2:
        return a * (1 - sign)
    eset.delete(perm[i])
    #print(eset)
    b = rank_sign_helper(perm, i + 1, n, (sign + a) % 2, eset)
    return a * fact[n - 1 - i] // 2 + b

def rank_sign(perm, sign):
    n = len(perm)
    eset = EncodedSet(n)
    return rank_sign_helper(perm, 0, n, sign, eset)
    
# returns product of permutations a and b where a is applied first
def mul(a, b):
    return [b[a[i]] for i in range(len(a))]

# returns inverse of given permutation
def inv(perm):
    out = [0 for _ in range(len(perm))]
    for i in range(len(perm)):
        out[perm[i]] = i
    return out

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


"""
import itertools

n = 4
rank = [0 for _ in range(fact[n])]

count = 0
even = 0
for perm in itertools.permutations([i for i in range(n)]):
    rank[count] = even
    reven = rank_sign(perm, 0)
    rodd = rank_sign(perm, 1)
    print(reven, even, reven + rodd, count, reven == even and reven + rodd == count)
    if sign(perm) == 0:
        even += 1
    count += 1
"""