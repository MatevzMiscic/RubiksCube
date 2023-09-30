import tkinter as tk
from tkinter  import Frame, Button, Label

from cube import Cube
from convert import tocube
from solver import solve, tonames


window = tk.Tk()
window.config(bg="skyblue")


n = 3
size = 150
margin = 2
space = size/6

colors = [(238, 31, 35), (105, 189, 69), (245, 128, 33), (56, 83, 163), (242, 235, 23), (254, 254, 254)]

def rgb(color):
    r, g, b = color
    return f'#{r:02x}{g:02x}{b:02x}'




# color picker setup
picked = 6
boxes = []

def box_clicked(event, index):
    print(index)
    global picked
    if picked < 6:
        picker.itemconfig(boxes[picked], width=1)
    if index == picked:
        picked = 6
    else:
        picked = index
        picker.itemconfig(boxes[picked], width=6)


picker = tk.Canvas(window, bg="skyblue", width=4*size, height=size)
X = 2*size + space/2 - 3*(size/n + space)
Y = size/n
for i in range(6):
    x = X + i * (size/n + space)
    box = picker.create_rectangle(x, Y, x + size/n, Y + size/n, fill=rgb(colors[i]), width=1, outline="black")
    picker.tag_bind(box, "<Button-1>", lambda event, index=i: box_clicked(event, index))
    boxes.append(box)
picker.grid(row=0, column=0)





# cube net setup
load = [2, 0, 3, 5, 1, 5, 3, 2, 0, 4, 1, 0, 0, 4, 0, 4, 1, 5, 3, 1, 4, 4, 3, 4, 2, 2, 5, 1, 1, 5, 3, 5, 3, 0, 2, 0, 1, 0, 2, 3, 0, 2, 2, 4, 5, 1, 5, 3, 5, 2, 4, 4, 3, 1]

if load is not None:
    cube = load
else:
    cube = [i//9 for i in range(54)]
stickers = []

sides = [(0, 1), (1, 1), (2, 1), (3, 1), (1, 0), (1, 2)]
def net_pressed(event):
    if picked == 6:
        return
    X = event.x // size
    Y = event.y // size
    for i in range(6):
        if X == sides[i][0] and Y == sides[i][1]:
            index = (n**2) * i + n * ((event.y - Y * size) * n // size) + ((event.x - X * size) * n // size)
            net.itemconfig(stickers[index], fill=rgb(colors[picked]))
            cube[index] = picked
            print(index)
    #print(X, Y)

net = tk.Canvas(window, bg="skyblue", width=4*size, height=3*size)
for side in range(len(sides)):
    X = size * sides[side][0]
    Y = size * sides[side][1]
    net.create_rectangle(X, Y, X + size, Y + size, fill="black")
    for j in range(n):
        for i in range(n):
            x = X + i * size/n
            y = Y + j * size/n
            if load is None:
                sticker = net.create_rectangle(x + margin, y + margin, x + size/n - margin, y + size/n - margin, fill="light grey", width=0)
            else:
                sticker = net.create_rectangle(x + margin, y + margin, x + size/n - margin, y + size/n - margin, fill=rgb(colors[load[(n**2)*side + n*j + i]]), width=0)
            stickers.append(sticker)
net.bind("<Button-1>", net_pressed)
net.grid(row=1, column=0)




# control panel setup
def solve_preesed():
    print(cube)
    c = tocube(cube.copy())
    print(c)
    moves = solve(c)
    print(tonames(moves))
    names = tonames(moves)
    text = ""
    for i in range(len(names)):
        text += names[i] + " "
        if i % 12 == 11:
            text += "\n"
    label.config(text=text)

panel = Frame(window, width=4*size, height=3*size, bg="skyblue")
panel.grid(row=2, column=0)
#"""
button = Button(panel, text="Solve", command=solve_preesed,
    bd=2,
    #relief="raised",
    compound=tk.CENTER,
    bg=rgb((56, 83, 163)),
    fg="white",
    activebackground=rgb((76, 103, 203)),
    activeforeground="white",
    font="arial 20",
    pady=0, padx=12
    )
button.grid(row=0, column=0)
#"""
label = Label(panel, font="arial 20", bg="skyblue", width=30, text="")
label.grid(row=0, column=1)



window.mainloop()