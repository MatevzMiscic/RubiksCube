from cube import Cube

corner_to_triple = [(45, 8, 15), (47, 17, 24), (53, 26, 33), (51, 35, 6), (42, 9, 2), (44, 18, 11), (38, 27, 20), (36, 0, 29)]
triple_to_corner = {}

for i in  range(8):
    a, b, c = corner_to_triple[i]
    triple_to_corner[(a // 9, b // 9, c // 9)] = i

edge_to_pair = [(46, 16), (50, 25), (52, 34), (48, 7), (5, 12), (21, 14), (23, 30), (3, 32), (43, 10), (41, 19), (37, 28), (39, 1)]
pair_to_edge = {}

for i in  range(12):
    a, b = edge_to_pair[i]
    pair_to_edge[(a // 9, b // 9)] = i

#print(triple_to_corner)

def tocube(array):
    corner = [-1 for _ in range(8)]
    cornerori = [-1 for _ in range(8)]
    edge = [0 for _ in range(12)]
    edgeori = [0 for _ in range(12)]
    for i in range(8):
        #print(i)
        a, b, c = corner_to_triple[i]
        a, b, c = array[a], array[b], array[c]
        if (a, b, c) in triple_to_corner:
            origin = triple_to_corner[(a, b, c)]
            corner[origin] = i
            cornerori[origin] = 0
        elif (c, a, b) in triple_to_corner:
            origin = triple_to_corner[(c, a, b)]
            corner[origin] = i
            cornerori[origin] = 1
        elif (b, c, a) in triple_to_corner:
            origin = triple_to_corner[(b, c, a)]
            corner[origin] = i
            cornerori[origin] = 2
        else:
            return None
    for i in range(12):
        #print(i)
        a, b = edge_to_pair[i]
        a, b = array[a], array[b]
        if (a, b) in pair_to_edge:
            origin = pair_to_edge[(a, b)]
            edge[origin] = i
            edgeori[origin] = 0
        elif (b, a) in pair_to_edge:
            origin = pair_to_edge[(b, a)]
            edge[origin] = i
            edgeori[origin] = 1
        else:
            return None
    return Cube(corner, cornerori, edge, edgeori)

"""
colors = [i // 9 for i in range(54)]
cube = tocube(colors)
print(cube)
"""

colors = [1, 3, 3, 4, 0, 2, 5, 1, 4, 4, 3, 1, 3, 1, 5, 1, 0, 3, 2, 5, 0, 3, 2, 0, 0, 5, 4, 5, 2, 2, 5, 3, 2, 0, 0, 3, 5, 1, 1, 0, 4, 2, 2, 4, 4, 0, 1, 5, 4, 5, 1, 2, 4, 3]
c = tocube(colors)