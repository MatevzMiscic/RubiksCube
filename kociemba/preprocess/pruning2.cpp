#include <iostream>
#include <vector>
#include <deque>
#include "table.h"
#include "../moves/moves.h"
#include "../io/io.h"


using namespace std;





void corner_pruning_table(table& arr){
    
    vector<ushort> cornermt = corner_movetable();
    printf("Move table calculated.\n");

    int distance = 0;
    Cube id;
    ushort corner = id.corner_coord();
    arr.fill(3);
    arr.set(corner, 0);
    deque<ushort> queue = {corner};

    int count = 0;

    while(!queue.empty()){
        count += 1;
        ushort u = queue.front();
        queue.pop_front();

        int dist = arr.get(u) + 1;
        if(dist == 3) dist = 0;

        for(int i = 0; i < 18; ++i){
            ushort v = cornermt[18*u + i];
            if(arr.get(v) == 3){
                arr.set(v, dist);
                queue.push_back(v);
            }
        }
    }
    printf("count = %d\n", count);
}



void layer_pruning_table(table& arr){
    std::vector<uint> layersmt(10*967680);
    bool succ = io::read("../moves/layers.move", (int*)(&layersmt[0]), 4*layersmt.size());
    printf("Move table read: %d.\n", succ);

    int distance = 0;
    arr.fill(3);
    arr.set(0, 0);
    deque<uint> queue = {0};

    uint count = 0;

    while(!queue.empty()){
        count += 1;
        if(count % 9677 == 0) printf("%d / 100\n", count / 9677);
        uint u = queue.front();
        queue.pop_front();

        int dist = arr.get(u) + 1;
        if(dist == 3) dist = 0;

        for(int i = 0; i < 10; ++i){
            uint v = layersmt[10*u + i];
            if(arr.get(v) == 3){
                arr.set(v, dist);
                queue.push_back(v);
            }
        }
    }
    printf("count = %d\n", count);
}



int main(){
    cmb::calcbin();
    table corner_arr(40320);
    corner_pruning_table(corner_arr);
    io::write("stage2_corner.bin", corner_arr.bits, (corner_arr.len + 3) / 4);
    //table layer_arr(967680);
    //layer_pruning_table(layer_arr);
    //io::write("stage2_layer.bin", layer_arr.bits, (layer_arr.len + 3) / 4);
    printf("done");
}