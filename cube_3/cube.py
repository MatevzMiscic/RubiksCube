from random import randint



class Cube:

    def __init__(self, corner, cornerori, edge, edgeori):
        self.corner = corner
        self.cornerori = cornerori
        self.edge = edge
        self.edgeori = edgeori

    @staticmethod
    def solved():
        return Cube([i for i in range(8)], [0 for _ in range(8)], [i for i in range(12)], [0 for _ in range(12)])

    @staticmethod
    def shuffled(n=50):
        cube = Cube.solved()
        for i in range(n):
            cube.apply(Cube.moves[randint(0, len(Cube.moves) - 1)])
        return cube

    def clone(self):
        return Cube(self.corner.copy(), self.cornerori.copy(), self.edge.copy(), self.edgeori.copy())

    def apply(self, move):
        for i in range(8):
            self.cornerori[i] = (move.cornerori[self.corner[i]] + self.cornerori[i]) % 3
            self.corner[i] = move.corner[self.corner[i]]
        for i in range(12):        
            self.edgeori[i] = (move.edgeori[self.edge[i]] + self.edgeori[i]) % 2
            self.edge[i] = move.edge[self.edge[i]]
    
    def applyall(self, moves):
        for move in moves:
            self.apply(move)

    def __mul__(self, other):
        cornerori = [(other.cornerori[self.corner[i]] + self.cornerori[i]) % 3 for i in range(8)]
        corner = [other.corner[self.corner[i]] for i in range(8)]
        #print(len(other.edgeori), len(self.edgeori), self.edge)
        edgeori = [(other.edgeori[self.edge[i]] + self.edgeori[i]) % 2 for i in range(12)]
        edge = [other.edge[self.edge[i]] for i in range(12)]
        return Cube(corner, cornerori, edge, edgeori)

    def neighbours(self):
        return [self*move for move in Cube.moves]

    def __repr__(self):
        return str(self.corner) + ", " + str(self.cornerori) + ", " + str(self.edge) + ", " + str(self.edgeori)



Cube.F = Cube([4, 0, 2, 3, 5, 1, 6, 7], [2, 1, 0, 0, 1, 2, 0, 0], [4, 1, 2, 3, 8, 0, 6, 7, 5, 9, 10, 11], [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
Cube.B = Cube([0, 1, 6, 2, 4, 5, 7, 3], [0, 0, 2, 1, 0, 0, 1, 2], [0, 1, 7, 3, 4, 5, 2, 10, 8, 9, 6, 11], [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
Cube.U = Cube([0, 1, 2, 3, 7, 4, 5, 6], [0, 0, 0, 0, 0, 0, 0, 0], [0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 8], [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
Cube.D = Cube([1, 2, 3, 0, 4, 5, 6, 7], [0, 0, 0, 0, 0, 0, 0, 0], [3, 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11], [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
Cube.L = Cube([3, 1, 2, 7, 0, 5, 6, 4], [1, 0, 0, 2, 2, 0, 0, 1], [0, 1, 2, 4, 11, 5, 6, 3, 8, 9, 10, 7], [0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1])
Cube.R = Cube([0, 5, 1, 3, 4, 6, 2, 7], [0, 2, 1, 0, 0, 1, 2, 0], [0, 6, 2, 3, 4, 1, 9, 7, 8, 5, 10, 11], [0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0])

Cube.F2 = Cube.F * Cube.F
Cube.B2 = Cube.B * Cube.B
Cube.U2 = Cube.U * Cube.U
Cube.D2 = Cube.D * Cube.D
Cube.L2 = Cube.L * Cube.L
Cube.R2 = Cube.R * Cube.R

Cube.F3 = Cube.F2 * Cube.F
Cube.B3 = Cube.B2 * Cube.B
Cube.U3 = Cube.U2 * Cube.U
Cube.D3 = Cube.D2 * Cube.D
Cube.L3 = Cube.L2 * Cube.L
Cube.R3 = Cube.R2 * Cube.R

Cube.moves = [
    Cube.B, Cube.B2, Cube.B3, Cube.R, Cube.R2, Cube.R3, Cube.U, Cube.U2, Cube.U3, 
    Cube.F, Cube.F2, Cube.F3, Cube.L, Cube.L2, Cube.L3, Cube.D, Cube.D2, Cube.D3
]


c = Cube.solved()