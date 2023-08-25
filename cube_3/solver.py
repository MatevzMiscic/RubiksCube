from cube import Cube
from inout import read
from combination import combrank1, combrank2
from permutation import rank, rank_sign, mul, inv, sign
from convert import tocube

# read preprocessed data
stage1 = read("cube_3/stage1.bin")
stage2 = read("cube_3/stage2.bin")
stage3 = read("cube_3/stage3.bin")
stage4 = read("cube_3/stage4.bin")

# hashes and allowed moves for all four stages
moves1 = [
    Cube.B, Cube.B2, Cube.B3, Cube.R, Cube.R2, Cube.R3, Cube.U, Cube.U2, Cube.U3, 
    Cube.F, Cube.F2, Cube.F3, Cube.L, Cube.L2, Cube.L3, Cube.D, Cube.D2, Cube.D3
]
def hash1(cube):
    code = 0
    for i in range(12):
        code |= (cube.edgeori[i] << cube.edge[i])
    return code & 2047

moves2 = [
    Cube.B, Cube.B2, Cube.B3, Cube.R2, Cube.U, Cube.U2, Cube.U3, 
    Cube.F, Cube.F2, Cube.F3, Cube.L2, Cube.D, Cube.D2, Cube.D3
]
def hash2(cube):
    ori = [0 for _ in range(8)]
    for i in range(8):
        ori[cube.corner[i]] = cube.cornerori[i]
    a = 0
    for i in range(7):
        a = 3*a + ori[i]
    mask = (1 << cube.edge[4]) | (1 << cube.edge[5]) | (1 << cube.edge[6]) | (1 << cube.edge[7])
    b = combrank1[mask]
    return 2187*b + a

moves3 = [
    Cube.B2, Cube.R2, Cube.U, Cube.U2, Cube.U3, 
    Cube.F2, Cube.L2, Cube.D, Cube.D2, Cube.D3
]
#n = 40320
#h3_compressed = read("cube_3/hash3.bin")
#h3 = [(h3_compressed[2*i] << 8) | h3_compressed[2*i] for i in range(n)]
h3 = read("cube_3/hash3.bin")
def hash3(cube):
    idx = rank(cube.corner)
    a = (h3[2*idx] << 8) | h3[2*idx + 1]
    mask = (1 << cube.edge[0]) | (1 << cube.edge[2]) | (1 << cube.edge[8]) | (1 << cube.edge[10])
    mask = (mask >> 4) | (mask & 15)
    b = combrank2[mask]
    return 420*b + a

moves4 = [
    Cube.B2, Cube.R2, Cube.U2,
    Cube.F2, Cube.L2, Cube.D2
]
def hash4(cube):
    p = cube.corner
    p1 = [p[0] // 2, p[2] // 2, p[5] // 2, p[7] // 2]
    p2 = [p[1] // 2, p[3] // 2, p[4] // 2, p[6] // 2]
    fun = [0, 0, 1, 1, 0, 1, 2, 3, 2, 2, 3, 3]
    q = cube.edge
    q1 = [fun[q[0]], fun[q[2]], fun[q[8]], fun[q[10]]]
    q2 = [fun[q[4]], fun[q[5]], fun[q[6]], fun[q[7]]]
    q3 = [fun[q[1]], fun[q[3]], fun[q[9]], fun[q[11]]]
    a = rank(p1) # up to 24
    b = mul(inv(p1), p2)[0] # # up to 4
    c = rank(q1) # up to 24
    d = rank(q2) # up to 24
    e = rank_sign(q3, (sign(q1) + sign(q2)) % 2) # up to 12
    return 12*(24*(24*(4*a + b) + c) + d) + e



# returns element at index idx in the given compressed bytearray
def get(array, idx):
    if idx % 2 == 0:
        return array[idx // 2] >> 4
    else:
        return array[idx // 2] & 15






# returns moves that take cube to coset of identity
def solve_stage(cube, data, hashf, allowed_moves):
    moves = []
    code = hashf(cube)
    dist = get(data, code)
    while dist > 0:
        for move in allowed_moves:
            v = cube * move
            c = hashf(v)
            d = get(data, c)
            if d < dist:
                cube = v
                dist = d
                moves.append(move)
                break
    return moves

# solves the cube using Thistlethwaite's algorithm
def solve(cube):
    clone = cube.clone()
    hashes = [hash1, hash2, hash3, hash4]
    stages = [stage1, stage2, stage3, stage4]
    allowed_moves = [moves1, moves2, moves3, moves4]
    out = []
    for i in range(4):
        #print(i)
        moves = solve_stage(clone, stages[i], hashes[i], allowed_moves[i])
        clone.applyall(moves)
        out.extend(moves)
    return out


# dictionary that assigns to each move its name
movenames = {}
for i in range(18):
    movenames[Cube.moves[i].hash()] = Cube.names[i]

# returns list of names of given moves
def tonames(moves):
    return [movenames[move.hash()] for move in moves]

"""
n = 10
for i in range(n):
    print("Test " + str(i) + ":")
    c = Cube.shuffled()
    print(c)
    moves = solve(c)
    print(tonames(moves))
    print(len(moves))
    c.applyall(moves)
    print(c)
"""


#colors = [1, 3, 3, 4, 0, 2, 5, 1, 4, 4, 3, 1, 3, 1, 5, 1, 0, 3, 2, 5, 0, 3, 2, 0, 0, 5, 4, 5, 2, 2, 5, 3, 2, 0, 0, 3, 5, 1, 1, 0, 4, 2, 2, 4, 4, 0, 1, 5, 4, 5, 1, 2, 4, 3]
#colors = [0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5]
colors = [2, 0, 2, 2, 0, 2, 0, 2, 0, 3, 1, 1, 3, 1, 1, 3, 3, 1, 0, 2, 0, 0, 2, 0, 2, 0, 2, 3, 3, 1, 3, 3, 1, 3, 1, 1, 4, 5, 5, 4, 4, 5, 4, 5, 5, 4, 4, 5, 4, 5, 5, 4, 4, 5]
c = tocube(colors)
print(c)
moves = solve(c)
print(tonames(moves))
print(len(moves))
c.applyall(moves)
print(c)
