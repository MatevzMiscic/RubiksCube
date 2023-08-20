import os
from cube import Cube
#from permutation import fact, rank
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


data = preprocess1()
c = Cube.solved()
applyall(c, [Cube.L, Cube.R, Cube.U, Cube.B, Cube.L2, Cube.B2, Cube.F])
moves = solve1(c, data)
applyall(c, moves)

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