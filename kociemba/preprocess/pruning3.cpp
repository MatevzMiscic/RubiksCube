#include <iostream>
#include <array>
#include <vector>
#include <deque>
#include "../Settings.h"
#include "../moves/moves.h"
#include "../io/io.h"
#include "table.h"

using namespace std;



vector<ushort> cornermt;
vector<uint> layermt[2];



struct coord{
    ushort corner;
    uint layer;

    coord() : corner(0), layer(0){}
    coord(ushort corner, uint layer) : corner(corner), layer(layer){}

    inline int index(){
        return 483840 * corner + (layer >> 1);
    }

    inline coord neigh(int i){
        uint sign = layer & 1;
        return coord(cornermt[10*corner + i], (layermt[sign][10 * (layer >> 1) + i] << 1) | (1 ^ sign));
    }
};


void pruning_table(table& arr, int bound){
    int distance = 0;
    coord s;
    deque<coord> queue = {s};
    arr.fill(3);
    arr.set(s.index(), 0);

    int count = 0;

    while(!queue.empty()){
        count += 1;
        coord u = queue.front();
        queue.pop_front();

        int dist = arr.get(u.index()) + 1;
        if(dist == 3) dist = 0;
        if(distance % 3 != dist){
            distance += 1;
        }

        //printf("u = %d, dist = %d\n", index_u, distance-1);
        for(int i = 0; i < 10; ++i){
            coord v = u.neigh(i);
            uint index = v.index();
            if(arr.get(index) == 3){
                //printf("  node %d found\n", index);
                arr.set(index, dist);
                if(distance < bound) queue.push_back(v);
            }
        }
    }
    printf("count = %d\n", count);
}

/*
vector<int> solve(Cube& cube, table& arr){
    vector<int> moves;
    //coord c(cube);
    coord c;
    bool run = true;
    while(run){
        run = false;
        int dist = (arr.get(c.index()) + 2) % 3;
        //printf("c = %d, dist = %d, sl: %d, fl; %d, tw: %d\n", c.index(), arr.get(c.index()), c.slice(), c.flip(), c.twist());
        for(int i = 0; i < 18; ++i){
            coord d = c.neigh(i);
            if(arr.get(d.index()) == dist){
                printf("move %d\n", i);
                moves.push_back(i);
                run = true;
                c = d;
                break;
            }
        }
    }
    return moves;
}
*/

int main(){
    cornermt = corner_cls_movetable();
    layers_compressed_movetable(layermt[0], layermt[1]);

    table arr(2768 * 483840);
    pruning_table(arr, 4);
}