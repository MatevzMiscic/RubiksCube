from permutation import rank
from random import randint

"""
F ... front
B ... back
U ... up
D ... down
L ... left
R ... right

Cubie numbers:
0 FDL
1 FDR
2 BDL
3 BDR
4 FUL
5 FUR
6 BUL
7 BUR
"""

class Cube2:

    def __init__(self, perm, orient):
        # perm[i] is place where cubie that started in place i can be found
        self.perm = perm
        # orient[i] is the orientation of a cubie that is in place i in solved cube
        self.orient = orient

    @staticmethod
    def solved():
        return Cube2([i for i in range(8)], [0 for _ in range(8)])
    
    @staticmethod
    def shuffled(n=50):
        #moves = [Cube2.B, Cube2.B2, Cube2.B3, Cube2.R, Cube2.R2, Cube2.R3, Cube2.U, Cube2.U2, Cube2.U3]
        cube = Cube2.solved()
        for i in range(n):
            cube.apply(Cube2.moves[randint(0, len(Cube2.moves) - 1)])
        return cube
    
    def isSolved(self):
        for i in range(8):
            if self.perm[i] != i or self.orient[i] != 0:
                return False
        return True

    def clone(self):
        return Cube2(self.perm.copy(), self.orient.copy())

    def apply(self, move):
        for i in range(8):
            self.orient[i] = (move.orient[self.perm[i]] + self.orient[i]) % 3
            self.perm[i] = move.perm[self.perm[i]]

    def __mul__(self, other):
        perm = [other.perm[self.perm[i]] for i in range(8)]
        orient = [(other.orient[self.perm[i]] + self.orient[i]) % 3 for i in range(8)]
        return Cube2(perm, orient)

    def inv(self):
        perm = [0 for _ in range(8)]
        for i in range(8):
            perm[self.perm[i]] = i
        orient = [0 for _ in range(8)]
        for i in range(8):
            orient[i] = (3 - self.orient[perm[i]]) % 3
        return Cube2(perm, orient)

    def neighbours(self):
        n = []
        moves = [Cube2.B, Cube2.R, Cube2.U]
        for move in moves:
            for i in range(4):
                if i > 0:
                    n.append(self.clone())
                self.apply(move)
        return n

    def hash(self):
        a = 0
        for i in range(1, 7):
            a = 3 * a + self.orient[i]
        b = rank(self.perm)
        #print(a, b, (3**6) * b + a)
        return (3**6) * b + a

    def __repr__(self):
        return str(self.perm) + ", " + str(self.orient)

Cube2.F = Cube2([4, 0, 2, 3, 5, 1, 6, 7], [2, 1, 0, 0, 1, 2, 0, 0])
Cube2.B = Cube2([0, 1, 6, 2, 4, 5, 7, 3], [0, 0, 2, 1, 0, 0, 1, 2])
Cube2.U = Cube2([0, 1, 2, 3, 7, 4, 5, 6], [0, 0, 0, 0, 0, 0, 0, 0])
Cube2.D = Cube2([1, 2, 3, 0, 4, 5, 6, 7], [0, 0, 0, 0, 0, 0, 0, 0])
Cube2.L = Cube2([3, 1, 2, 7, 0, 5, 6, 4], [1, 0, 0, 2, 2, 0, 0, 1])
Cube2.R = Cube2([0, 5, 1, 3, 4, 6, 2, 7], [0, 2, 1, 0, 0, 1, 2, 0])

Cube2.F2 = Cube2.F * Cube2.F
Cube2.B2 = Cube2.B * Cube2.B
Cube2.U2 = Cube2.U * Cube2.U
Cube2.D2 = Cube2.D * Cube2.D
Cube2.L2 = Cube2.L * Cube2.L
Cube2.R2 = Cube2.R * Cube2.R

Cube2.F3 = Cube2.F2 * Cube2.F
Cube2.B3 = Cube2.B2 * Cube2.B
Cube2.U3 = Cube2.U2 * Cube2.U
Cube2.D3 = Cube2.D2 * Cube2.D
Cube2.L3 = Cube2.L2 * Cube2.L
Cube2.R3 = Cube2.R2 * Cube2.R

Cube2.moves = [
    Cube2.B, Cube2.B2, Cube2.B3, Cube2.R, Cube2.R2, Cube2.R3, Cube2.U, Cube2.U2, Cube2.U3, 
    Cube2.F, Cube2.F2, Cube2.F3, Cube2.L, Cube2.L2, Cube2.L3, Cube2.D, Cube2.D2, Cube2.D3
]
Cube2.names = [
    "B", "B2", "B3", "R", "R2", "R3", "U", "U2", "U3", "F", "F2", "F3", "L", "L2", "L3", "D", "D2", "D3"
]

F = Cube2([4, 0, 2, 3, 5, 1, 6, 7], [2, 1, 0, 0, 1, 2, 0, 0])
B = Cube2([0, 1, 6, 2, 4, 5, 7, 3], [0, 0, 2, 1, 0, 0, 1, 2])
U = Cube2([0, 1, 2, 3, 7, 4, 5, 6], [0, 0, 0, 0, 0, 0, 0, 0])
D = Cube2([1, 2, 3, 0, 4, 5, 6, 7], [0, 0, 0, 0, 0, 0, 0, 0])
L = Cube2([3, 1, 2, 7, 0, 5, 6, 4], [1, 0, 0, 2, 2, 0, 0, 1])
R = Cube2([0, 5, 1, 3, 4, 6, 2, 7], [0, 2, 1, 0, 0, 1, 2, 0])
c = Cube2.solved()