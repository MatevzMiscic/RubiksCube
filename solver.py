import os
from cube import Cube2
from symmetries import remove_symm


# read distances from binary file
folder = os.getcwd()
filename = "data.bin"
path = os.path.join(folder, filename)

with open(path, 'br') as file:
    array = file.read()



# returns distance of the position with given hash
def getDist(code):
    # IDK why, but on some systems there is additional byte in front of the bin file (TODO solve that shit)
    delta = len(array) - 1837080
    code += 2 * delta

    if code % 2 == 0:
        return array[code // 2] >> 4
    else:
        return array[code // 2] & 15


# returns the sequence of moves that solves given rubiks cube
# the cube must satisfy perm[0] = 0 and orient[0] = 0, which can be acheaved using rotations of the cube
def solve(cube):
    #neighbours = ['B', 'B2', 'B3', 'R', 'R2', 'R3', 'U', 'U2', 'U3']
    moves = []
    code = cube.hash()
    dist = getDist(code)
    while not cube.isSolved():
        for i, v in enumerate(cube.neighbours()):
            c = v.hash()
            d = getDist(c)
            if d < dist:
                cube = v
                dist = d
                moves.append(Cube2.moves[i])
                break
    return moves

# returns the sequence of moves that solves given rubiks cube
def solve2(cube):
    X = remove_symm(cube)
    moves = solve(cube)
    return [X*move*X.inv() for move in moves]



# dictionary that assigns to each move its name
movenames = {}
for i in range(18):
    movenames[Cube2.moves[i].hash()] = Cube2.names[i]

def tonames(moves):
    return [movenames[move.hash()] for move in moves]




# testing

#"""
c = Cube2.shuffled()
print(c)
sol = solve2(c)
print(sol)
print(tonames(sol))
#"""

"""
c = Cube2.solved()
c.apply(Cube2.R2)
c.apply(Cube2.U)
c.apply(Cube2.B3)
c.apply(Cube2.U2)
c.apply(Cube2.R3)
print(solve(c))
#"""

"""
for i in range(30):
    print(array[i])
"""

c = Cube2.solved()