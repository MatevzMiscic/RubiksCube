import os

# compresses given bytearray to two times smaller bytearray
# array can only contain integers that fit to 4 bits, that is integers from 0 to 15
# array must have even size
def compress(array):
    n = len(array)
    data = bytearray(n // 2)
    for i in range(n // 2):
        data[i] = (array[2*i] << 4) | array[2*i + 1]
    return data

# returns element at index ixx in the given compressed bytearray
def get(array, idx):
    if idx % 2 == 0:
        return array[idx // 2] >> 4
    else:
        return array[idx // 2] & 15

# writes data in cwd/filename
def write(filename, data):
    folder = os.getcwd()
    path = os.path.join(folder, filename)
    with open(path, 'bw') as file:
        file.write(data)

# returns data from cwd/filename
def read(filename):
    folder = os.getcwd()
    path = os.path.join(folder, filename)
    with open(path, 'br') as file:
        data = file.read()
    return data