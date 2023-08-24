from inout import compress, read, write
from cube import Cube
from random import randint
from permutation import rank
from collections import deque
import itertools

"""
import sys
sys.path.insert(1, '/path/to/cube_2')
import permutation
"""
"""
filename = "cube_3/stage2.bin"

with open(filename, 'br') as file:
    stage2 = file.read()

freq = [0 for _ in range(20)]
for d in stage2:
    freq[d] += 1
print(freq)
"""

"""
c = Cube.solved()

def domove(cube):
    moves = [Cube.L2, Cube.R2, Cube.U2, Cube.D2, Cube.F2, Cube.B2]
    cube.apply(moves[randint(0, len(moves) - 1)])
    print(cube)
#"""

def decomp(perm):
    perm1 = [perm[0]//2, perm[2]//2, perm[5]//2, perm[7]//2]
    perm2 = [perm[1]//2, perm[3]//2, perm[4]//2, perm[6]//2]
    return perm1, perm2

def apply(a, b):
    return [b[a[i]] for i in range(len(a))]

def inv(perm):
    out = [0 for _ in range(len(perm))]
    for i in range(len(perm)):
        out[perm[i]] = i
    return out

perms = [
    [0, 1, 2, 3],
    [1, 0, 3, 2],
    [2, 3, 0, 1],
    [3, 2, 1, 0]
]
def inSub(perm):
    mask = (1 << perm[0]) | (1 << perm[2]) | (1 << perm[5]) | (1 << perm[7])
    if mask != 0b10100101:
        return False
    p1, p2 = decomp(perm)
    return apply(inv(p1), p2) in perms

# cosets are of the form Hx
def same_coset(p1, p2):
    return inSub(apply(p1, inv(p2)))

"""
moves = [Cube.L2, Cube.R2, Cube.U2, Cube.D2, Cube.F2, Cube.B2]
for move in moves:
    p1, p2 = decomp(move.corner)
    mul = apply(inv(p1), p2)
    print(move, p1, p2, mul)
#"""

"""
print()
moves = [Cube.L2, Cube.R2, Cube.U, Cube.D, Cube.F2, Cube.B2]
for move in moves:
    print(move)
#"""



"""
# finding a missing factor of 1/3 in the size of G_3

def hash1(perm):
    perm1 = [perm[0]//2, perm[2]//2, perm[5]//2, perm[7]//2]
    perm2 = [perm[1]//2, perm[3]//2, perm[4]//2, perm[6]//2]
    #print(perm, perm1, perm2)
    a = rank(perm1)
    b = rank(perm2)
    return 24*a + b


n = 24*24
count = 0
visited = bytearray(n)

moves = [
    [4, 0, 2, 3, 5, 1, 6, 7], 
    [0, 1, 6, 2, 4, 5, 7, 3], 
    [0, 1, 2, 3, 7, 4, 5, 6], 
    [1, 2, 3, 0, 4, 5, 6, 7], 
    [3, 1, 2, 7, 0, 5, 6, 4], 
    [0, 5, 1, 3, 4, 6, 2, 7], 
]
for i in range(6):
    moves[i] = apply(moves[i], moves[i])

n = 24*24

perm = [i for i in range(8)]
code = hash1(perm)
queue = deque([(perm, code, -1)])
visited[code] = 1

while len(queue) > 0:
    u, c, i = queue.popleft()
    count += 1
    
    for j in range(6):
        if j == i:
            continue
        v = apply(u, moves[j])
        d = hash1(v)
        if d >= n:
            print(v)
            print(d)
        if visited[d] == 0:
            visited[d] = 1
            queue.append((v, d, j))

print(count)
print()

for k in range(10):
    c = 0
    for i in range(24*k, 24*(k+1)):
        if visited[i]:
            print(i)
            c += 1
    print()
    print(c)
    print()

map1 = [0, 2, 5, 7]
map2 = [1, 3, 4, 6]

for perm1 in itertools.permutations([0, 1, 2, 3]):
    for perm2 in itertools.permutations([0, 1, 2, 3]):
        a = rank(perm1)
        b = rank(perm2)
        code = 24*a + b
        flag1 = visited[code]
        perm = [map1[perm1[0]], map2[perm2[0]], map1[perm1[1]], map2[perm2[1]], map2[perm2[2]], map1[perm1[2]], map2[perm2[3]], map1[perm1[3]]]
        flag2 = inSub(perm)
        print(flag1, flag2)
        if flag1 != flag2:
            print("Napaka xxxxxxxxxxxxxxxxxxxxx", perm)
#"""

#"""
# checking if hash_i is really  invariant for left multiplication by elements of stage i
from preprocess import *

c = Cube.shuffled()

# G_1 = <F, B, U, D, L2, R2>
G1 = [Cube.F, Cube.B, Cube.U, Cube.D, Cube.L2, Cube.R2]
print(edgeori_hash(c))
for m in G1:
    code = edgeori_hash(m*c)
    print(code)

print()

# G_2 = <F2, B2, U, D, L2, R2>
G2 = [Cube.F2, Cube.B2, Cube.U, Cube.D, Cube.L2, Cube.R2]
print(hash2(c))
for m in G2:
    code = hash2(m*c)
    print(code)

print()

G3 = [Cube.F2, Cube.B2, Cube.U2, Cube.D2, Cube.L2, Cube.R2]
print(hash3(c))
for m in G3:
    code = hash3(m*c)
    print(code)
#"""

"""
n = 40320
coset = [0 for i in range(n)]
represent = []

count = 0
for perm in itertools.permutations([0, 1, 2, 3, 4, 5, 6, 7]):
    i = 0
    while i < len(represent):
        if same_coset(perm, represent[i]):
            break
        i += 1
    if i == len(represent):
        represent.append(perm)
    coset[count] = i
    #if i == 0:
        #a, b = decomp(perm)
        #mask = (1 << perm[0]) | (1 << perm[2]) | (1 << perm[5]) | (1 << perm[7])
        #print(perm, a, b, bin(mask)[2:], "...", i)
    count += 1
    if count % 403 == 0:
        print(count // 403)

a = bytearray(2*n)
for i in range(n):
    a[2*i] = (coset[i] >> 8)
    a[2*i + 1] = coset[i] & 0b11111111

#write("cube_3/hash3.bin", a)
#"""

"""
freq = [0 for _ in range(10)]
represent = []

for perm1 in itertools.permutations([0, 2, 5, 7]):
    for perm2 in itertools.permutations([1, 3, 4, 6]):
        perm = [perm1[0], perm2[0], perm1[1], perm2[1], perm2[2], perm1[2], perm2[3], perm1[3]]
        a, b = decomp(perm)
        i = 0
        while i < len(represent):
            if same_coset(perm, represent[i]):
                break
            i += 1
        if i == len(represent):
            represent.append(perm)
        freq[i] += 1
        print(perm, a, b, "...", i)

print(freq)
#"""

