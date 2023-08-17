from cube import Cube2


moves = [
    Cube2.B, 
    Cube2.B2, 
    Cube2.B3, 
    Cube2.R, 
    Cube2.R2, 
    Cube2.R3, 
    Cube2.U, 
    Cube2.U2, 
    Cube2.U3
]

perm = [
    Cube2.solved(), 
    Cube2.D3*Cube2.U, 
    Cube2.D2*Cube2.U2, 
    Cube2.D*Cube2.U3, 
    Cube2.F2*Cube2.B2 * Cube2.D3*Cube2.U, 
    Cube2.F2*Cube2.B2, 
    Cube2.F2*Cube2.B2 * Cube2.D*Cube2.U3, 
    Cube2.R2*Cube2.L2
]

R = Cube2.F3*Cube2.B * Cube2.D3*Cube2.U
rot = [Cube2.solved(), R, R*R]


def remove_symm(cube):
    #print(cube)
    #c = cube.clone()
    A = perm[cube.perm[0]]
    cube.apply(A)
    #print(cube)
    B = rot[cube.orient[0]]
    cube.apply(B)
    #print(c*A*B)
    #c.apply(A*B)
    #print(cube, c)
    return A*B





c = Cube2.solved()
c.apply(Cube2.L3)
#remove_symm(c)

"""
for i in range(30):
    cube = Cube2.shuffled(10)
    X = remove_symm(cube)
    print(cube)
#"""

"""
for move in Symmetry.moves:
    print(move)

print()

X = Symmetry.perm[4] * Symmetry.R
for move in Symmetry.moves:
    print(X*move*X.inv())
#"""