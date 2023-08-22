import os
from cube import Cube
from combination import combrank
from collections import deque


# G_0 = <F, B, U, D, L, R>  all states
# G_1 = <F, B, U, D, L2, R2>  edges oriented corectly
# G_2 = <F2, B2, U, D, L2, R2>  corners oriented corectly and pieces 4, 5, 6, 7 only permuted among themselves
# G_3 = <F2, B2, U2, D2, L2, R2>
# G_4 = <1>  solved state


def edgeori_hash(cube):
    code = 0
    for i in range(12):
        code |= (cube.edgeori[i] << cube.edge[i])
    return code & 2047

def preprocess1():
    n = 2**11
    dist = bytearray(n)
    for i in range(n):
        dist[i] = 15

    cube = Cube.solved()
    code = edgeori_hash(cube)
    queue = deque([(cube, code)])
    dist[code] = 0

    while len(queue) > 0:
        u, c = queue.popleft()
        for v in u.neighbours():
            d = edgeori_hash(v)
            if dist[d] == 15:
                dist[d] = dist[c] + 1
                queue.append((v, d))
    return dist

def solve1(cube, data):
    moves = []
    code = edgeori_hash(cube)
    dist = data[code]
    while dist > 0:
        for move in Cube.moves:
            v = cube*move
            c = edgeori_hash(v)
            d = data[c]
            if d < dist:
                cube = v
                dist = d
                moves.append(move)
                break
    return moves







def applyall(cube, moves):
    for move in moves:
        cube.apply(move)

"""
data = preprocess1()
c = Cube.solved()
applyall(c, [Cube.L, Cube.R, Cube.U, Cube.B, Cube.L2, Cube.B2, Cube.F])
moves = solve1(c, data)
applyall(c, moves)
"""

"""
data = preprocess1()
c = Cube.solved()
for move in [Cube.L, Cube.R, Cube.U, Cube.B, Cube.L2]:
    c.apply(move)
    print(c)
    code = edgeori_hash(c)
    print(code)
    print(data[code])

print("Checking neighbours...")
for n in c.neighbours():
    print(n)
    code = edgeori_hash(n)
    print(code)
    print(data[code])
"""


def hash2(cube):
    ori = [0 for _ in range(8)]
    for i in range(8):
        ori[cube.corner[i]] = cube.cornerori[i]
    a = 0
    for i in range(7):
        a = 3*a + ori[i]
    mask = (1 << cube.edge[4]) | (1 << cube.edge[5]) | (1 << cube.edge[6]) | (1 << cube.edge[7])
    b = combrank[mask]

    code = 2187*b + a
    if code >= 1082565:
        print("a =", a, "b =", b)
    
    return 2187*b + a

def preprocess2():
    moves = [
        [Cube.F, Cube.F2, Cube.F3], 
        [Cube.B, Cube.B2, Cube.B3], 
        [Cube.U, Cube.U2, Cube.U3], 
        [Cube.D, Cube.D2, Cube.D3], 
        [Cube.R2], 
        [Cube.L2]
    ]
    n = 2187 * 495 # 3**7 * 12C4

    count = 0
    perc = n // 100

    dist = bytearray(n)
    for i in range(n):
        dist[i] = 15

    cube = Cube.solved()
    code = hash2(cube)
    queue = deque([(cube, code, -1)])
    dist[code] = 0

    while len(queue) > 0:
        u, c, i = queue.popleft()

        count += 1
        if count % perc == 0:
            print(count // perc)
        for j in range(6):
            if j == i:
                continue
            for move in moves[j]:
                v = u * move
                d = hash2(v)
                if d >= n:
                    print(v)
                    print(d)
                if dist[d] == 15:
                    dist[d] = dist[c] + 1
                    if dist[d] <= 3:
                        queue.append((v, d, j))
    return dist

def solve2(cube, data):
    moves = []
    code = hash2(cube)
    dist = data[code]
    while dist > 0:
        for move in Cube.moves:
            v = cube*move
            c = hash2(v)
            d = data[c]
            if d < dist:
                cube = v
                dist = d
                moves.append(move)
                break
    return moves

data = preprocess2()

folder = os.getcwd()
filename = "stage2.bin"
path = os.path.join(folder, filename)

with open(path, 'bw') as file:
    file.write(data)

"""
#data1 = preprocess1()
data2 = preprocess2()
c = Cube.solved()
applyall(c, [Cube.R2, Cube.F3, Cube.U, Cube.L2, Cube.U])
print(c)
#applyall(c, solve1(c, data1))
#print(c)
moves = solve2(c, data2)
applyall(c, moves)
print(c)
#"""