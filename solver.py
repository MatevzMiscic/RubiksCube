import os
from cube import Cube2



folder = os.getcwd()
filename = "data.bin"
path = os.path.join(folder, filename)

with open(path, 'br') as file:
    array = file.read()

def getDist(code):
    if code % 2 == 0:
        return array[code // 2] >> 4
    else:
        return array[code // 2] & 15

def solve(cube):
    neighbours = ['B', 'B2', 'B3', 'R', 'R2', 'R3', 'U', 'U2', 'U3']
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
                moves.append(neighbours[i])
                break
    return moves
    
c = Cube2.shuffled()
print(c)
sol = solve(c)
print(sol)