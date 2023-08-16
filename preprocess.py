import os
from cube import Cube2
from permutation import fact, rank
from collections import deque

"""
folder = os.getcwd()
filename = "data.bin"
path = os.path.join(folder, filename)

with open(path, 'bw') as file:
    array = bytearray([10, 4, 1, 6])
    print(type(array))
    file.write(array)
    
with open(path, 'br') as file:
    array = file.read()
    print(type(array))
    for b in array:
        print(type(b))
        print(b)
    print(array[0])
"""


n = fact[7] * (3 ** 6)
dist = bytearray(n)
for i in range(n):
    dist[i] = 15

cube = Cube2.solved()
code = cube.hash()
queue = deque([(cube, code)])
dist[code] = 0


# BFS
count = 1
perc = 36741
while len(queue) > 0:
    u, c = queue.popleft()
    #print(c, dist[c])
    for v in u.neighbours():
        d = v.hash()
        if d >= n:
            print(v)
        if dist[d] == 15:
            dist[d] = dist[c] + 1
            queue.append((v, d))
            count += 1
            if count % perc == 0:
                print(count // perc)

data = bytearray(n // 2)
for i in range(n // 2):
    data[i] = (dist[2*i] << 4) | dist[2*i + 1]


# save distances to binary file
folder = os.getcwd()
filename = "data.bin"
path = os.path.join(folder, filename)

with open(path, 'bw') as file:
    file.write(data)
