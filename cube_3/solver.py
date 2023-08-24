from cube import Cube
from inout import read
from combination import combrank1, combrank2
from permutation import rank, rank_sign

# read preprocessed data
stage1 = read("cube_3/stage1.bin")
stage2 = read("cube_3/stage2.bin")
stage3 = read("cube_3/stage3.bin")
stage4 = read("cube_3/stage4.bin")

# hashes for all four stages
def hash1(cube):
    code = 0
    for i in range(12):
        code |= (cube.edgeori[i] << cube.edge[i])
    return code & 2047

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

h3 = read("cube_3/hash3.bin")
def hash3(cube):
    idx = rank(cube.corner)
    a = (h3[2*idx] << 8) | h3[2*idx + 1]
    mask = (1 << cube.edge[0]) | (1 << cube.edge[2]) | (1 << cube.edge[8]) | (1 << cube.edge[10])
    mask = (mask >> 4) | (mask & 15)
    b = combrank2[mask]
    return 420*b + a

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
def solve_stage(cube, data, hashf):
    moves = []
    code = hashf(cube)
    dist = get(data, code)
    while dist > 0:
        for move in Cube.moves:
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
    out = []
    for i in range(4):
        moves = solve_stage(clone, stages[i], hashes[i])
        clone.applyall(moves)
        out.extend(moves)
    return out

c = Cube.shuffled()
print(c)
#moves = solve(c)
#c.applyall(moves)
#print(c)
