import tkinter as tk
from tkinter  import Frame, Button, Label

from cube import Cube
from convert import tocube, tocolors
from solver import solve, tonames


window = tk.Tk()
window.title("Rubik's Cube Solver")
window.config(bg="skyblue")
window.columnconfigure(0, weight=1)
window.rowconfigure((0, 2), weight=1)


n = 3
face_size = 150 # size of a face
sticker_size = face_size/n
margin = 2 # space around sticker
space = int(sticker_size/2) # space between colors in picker
border = int(sticker_size/2) # space around cube net
highlightthickness = 2

faces = ["L", "F", "R", "B", "U", "D"]
color_names = ["red", "green", "orange", "blue", "yellow", "white"]
colors = [(238, 31, 35), (105, 189, 69), (245, 128, 33), (56, 83, 163), (242, 235, 23), (254, 254, 254)]

def rgb(r, g, b):
    return f'#{r:02x}{g:02x}{b:02x}'

def rgb_tuple(color):
    r, g, b = color
    return f'#{r:02x}{g:02x}{b:02x}'





# color picker setup
picked = 6
boxes = []

def box_clicked(event, index):
    global picked
    if picked < 6:
        picker.itemconfig(boxes[picked], width=1)
    if index == picked:
        picked = 6
    else:
        picked = index
        picker.itemconfig(boxes[picked], width=6)


picker = tk.Canvas(window, bg="steelblue", highlightthickness=highlightthickness, width=2*border + 4*face_size, height=2*sticker_size)
X = 2*face_size + space/2 - 3*(sticker_size + space)
Y = sticker_size/2
for i in range(6):
    x = X + i * (sticker_size + space)
    box = picker.create_rectangle(x, Y, x + sticker_size, Y + sticker_size, fill=rgb_tuple(colors[i]), width=1, outline="black")
    picker.tag_bind(box, "<Button-1>", lambda event, index=i: box_clicked(event, index))
    boxes.append(box)
picker.grid(row=0, column=0, padx=10, pady=10)






# cube net setup
load = [2, 0, 3, 5, 1, 5, 3, 2, 0, 4, 1, 0, 0, 4, 0, 4, 1, 5, 3, 1, 4, 4, 3, 4, 2, 2, 5, 1, 1, 5, 3, 5, 3, 0, 2, 0, 1, 0, 2, 3, 0, 2, 2, 4, 5, 1, 5, 3, 5, 2, 4, 4, 3, 1]
load = [i//9 for i in range(54)]

if load is not None:
    cube = load
else:
    cube = [i//9 for i in range(54)]
stickers = []

sides = [(0, 1), (1, 1), (2, 1), (3, 1), (1, 0), (1, 2)]
def net_pressed(event):
    if picked == 6:
        return
    event.x -= border
    event.y -= border
    X = event.x // face_size
    Y = event.y // face_size
    for i in range(6):
        if X == sides[i][0] and Y == sides[i][1]:
            index = (n**2) * i + n * ((event.y - Y * face_size) * n // face_size) + ((event.x - X * face_size) * n // face_size)
            net.itemconfig(stickers[index], fill=rgb_tuple(colors[picked]))
            cube[index] = picked
            #print(index)
    #print(X, Y)

net = tk.Canvas(window, bg="skyblue", highlightthickness=0, width=2*border + 4*face_size, height=2*border + 3*face_size)
for side in range(len(sides)):
    X = border + face_size * sides[side][0]
    Y = border + face_size * sides[side][1]
    net.create_rectangle(X, Y, X + face_size, Y + face_size, fill="black")
    for j in range(n):
        for i in range(n):
            x = X + i * sticker_size
            y = Y + j * sticker_size
            if load is None:
                sticker = net.create_rectangle(x + margin, y + margin, x + sticker_size - margin, y + sticker_size - margin, fill="light grey", width=0)
            else:
                sticker = net.create_rectangle(x + margin, y + margin, x + sticker_size - margin, y + sticker_size - margin, fill=rgb_tuple(colors[load[(n**2)*side + n*j + i]]), width=0)
            stickers.append(sticker)
    net.create_text(X + face_size/2, Y + face_size/2, font="arial 20",text=faces[side])
net.bind("<Button-1>", net_pressed)
net.grid(row=1, column=0, padx=10, pady=0)






# control panel setup
def get_button(parent, text, command, style):
    bg = [rgb(240, 245, 255), rgb(56, 83, 163)]
    fg = [rgb(56, 83, 163), rgb(255, 255, 255)]
    abg = [rgb(230, 240, 250), rgb(46, 73, 153)]
    afg = [rgb(46, 73, 153), rgb(255, 255, 255)]
    button = Button(parent, text=text, command=command, bd=1, bg=bg[style], fg=fg[style], activebackground=abg[style], activeforeground=afg[style], font="arial 20", width=10, height=1)
    return button

def clear_pressed():
    for i in range(54):
        net.itemconfig(stickers[i], fill="lightgrey")
        cube[i] = 6

def shuffle_pressed():
    global cube
    shuffle = Cube.shuffled()
    cube = tocolors(shuffle)
    for i in range(54):
        net.itemconfig(stickers[i], fill=rgb_tuple(colors[cube[i]]))

def solve_preesed():
    #print(cube)
    freq = [0 for i in range(7)]
    for color in cube:
        freq[color] += 1
    for i in range(6):
        if freq[i] != 9:
            text = "Invalid cube net."
            if freq[i] < 9:
                text += "\nNot enough " + color_names[i] + " stickers."
            else:
                text += "\nToo many " + color_names[i] + " stickers."
            label.config(text=text)
            return
    c = tocube(cube.copy())
    if c is None:
        label.config(text="Invalid cube net.")
        return
    #print(c)
    if not c.solvable():
        label.config(text="Possition is not solvable.")
        return
    if c.is_solved():
        label.config(text="The cube is solved.")
        return
    moves = solve(c)
    #print(tonames(moves))
    names = tonames(moves)
    text = ""
    for i in range(len(names)):
        if i > 0 and i % 11 == 0:
            text += "\n"
        text += names[i] + " "
    label.config(text=text)

panel = Frame(window, width=2*border + 4*face_size, height=3*face_size, bg="skyblue")
panel.grid(row=2, column=0, padx=10, pady=10)

clear_button = get_button(panel, "Clear", clear_pressed, 0)
clear_button.grid(row=0, column=0, padx=0)

shuffle_button = get_button(panel, "Shuffle", shuffle_pressed, 0)
shuffle_button.grid(row=1, column=0)

solve_button = get_button(panel, "Solve", solve_preesed, 1)
solve_button.grid(row=2, column=0)

label = Label(panel, font="arial 20", bg=rgb(191, 228, 244), width=29, height=5, text="")
label.grid(row=0, column=1, rowspan=3, padx=(10, 0), pady=0)



window.update()
print(window.winfo_width(), window.winfo_height())
window.minsize(window.winfo_width(), window.winfo_height())
window.mainloop()