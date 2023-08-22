from inout import compress, read, write

filename = "cube_3/stage2.bin"
"""
with open(filename, 'br') as file:
    stage2 = file.read()

freq = [0 for _ in range(20)]
for d in stage2:
    freq[d] += 1
print(freq)
"""
data = read(filename)
data = compress(data)
write("cube_3/stage2_compressed.bin", data)