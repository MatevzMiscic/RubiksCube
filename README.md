# Rubiks Cube

In this repository a few methods of solving a rubik's cube are implemented.
In `cube_2` folder an algorithm that can optimally solve a 2 by 2 by 2 Rubik's cube can be found.
In `cube_3` folder there is an implementation of Thistlethwaite's algorithm that is capable of solving a Rubik's cube in at most 45 moves.
Finally in `kociemba` folder there is an implementation of Kociemba's two phase algorithm that can solve a Rubik's cube in at most 30 moves.
Those numbers are worst case scenarios, on average both algorithms perform much better.

Optimal solver for smaller cube and Thistlethwaite's algorithm are implemented in python, whereas Kociemba's algorithm is implemented in c++ for performance reasons.

## Optimal 2 by 2 solver

One can solve a cube using a `solve2` function defined in `solver.py`:
```moves = solve2(cube)```
This function returns a list of moves that solve the given cube. To transform those moves to their names a `tonames` function can be used:
```solution = tonames(moves)```
which returns a list of strings that correspond to moves.

The cube must be given abstraclty in terms of permutation and orientations of pieces. As of now, I have not implemented method that would transform array of colors to such abstract representation.

## Thistlethwaite's algorithm

One can solve a cube in the following manner:
```python
moves = solve(cube)
solution = tonames(moves)
```
using functions from `solver.py` in `cube_3` folder.

Here one can also transform an array of colors to cube in abstract representation:
```python
cube = tocube(colors)
```
using function `tocube` from `convert.py`.

I have also created a simple gui in Tkinter that can help solve a cube.
You can see a screenshot bellow.

![The screenshot of gui](/cube_3/screenshots/image.png)

## Kociemba's algorithm

Solving can again be done in a simmilar way:
```cpp
int time_in_ms = 1000;
Cube cube;
cnv::to_cube(colors, cube);
vector<ubyte> moves = solver::timelimit_solve(copy, time_in_ms);
```
where colors is a ```array<ubyte, 54>``` array of colors.
Now ```moves``` is a vector of indeces to ```Cube::moves``` array, that can colve the cube.

## Further work

- At the moment, only second phase of kociemba's algorithm uses symmetry reduction, to save space. I have to implement it in Thistlethwaite's algorithm and in the first phase.
- Use distance modulo 3 in pruning tables of Thistlethwaite's algorithm.
- Compress 5 positions to byte instead of 4 in pruning tables. Currently each position takes 2 bits.
- Add performance to readme