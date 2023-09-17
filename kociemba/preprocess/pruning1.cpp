#include <iostream>
#include <array>
#include <vector>
#include <deque>
#include "../Settings.h"
#include "../moves/moves.h"
#include "../io/io.h"
#include "table.h"

using namespace std;


struct coord{

    static std::vector<ushort> twistmt;
    static std::vector<ushort> flipmt;
    static std::vector<ushort> slicemt;

    // in the least significant 9 bits slice coordinate is stored
    // in the next 12 bits twist coordinate is stored
    // in the next 11 bits flip coordinate is stored
    uint val;
    
    coord(uint val){
        this->val = val;
    }
    coord(int slice, int twist, int flip){
        this->val = (flip << 21) | (twist << 9) | slice;
    }
    coord(Cube& cube){
        this->val = (cube.flip_coord() << 21) | (cube.twist_coord() << 9) | cube.slice_coord();
    }

    inline int slice(){
        return val & 511;
    }
    inline int twist(){
        return (val >> 9) & 4095;
    }
    inline int flip(){
        return (val >> 21) & 2047;
    }

    inline int index(){
        return 2187 * ((slice() << 11) | flip()) + twist();
    }

    static void compute_mts(){
        twistmt = twist_movetable();
        flipmt = flip_movetable();
        slicemt = slice_movetable();
    }

    inline coord neigh(int i){
        return coord(slicemt[18*slice() + i], twistmt[18*twist() + i], flipmt[18*flip() + i]);
    }

    void print(){
        printf("%d, idx: %d, slice: %d, twist: %d, flip: %d\n", val, index(), slice(), flip(), twist());
    }
};

std::vector<ushort> coord::twistmt;
std::vector<ushort> coord::flipmt;
std::vector<ushort> coord::slicemt;




void printb(int n){
    for(int i = 31; i >= 0; --i){
        if(n & (1 << i)){
            printf("1 ");
        }else{
            printf("0 ");
        }
    }
    printf("\n");
}





// 2217093120
void pruning_table(table& arr){
    coord::compute_mts();
    printf("Move table calculated.\n");

    int distance = 0;
    Cube id;
    coord goal(id);
    deque<coord> queue = {goal};
    arr.set(goal.index(), 0);

    int count = 0;

    int d2 = 0;

    while(!queue.empty()){
        count += 1;
        coord u = queue.front();
        queue.pop_front();
        uint index_u = u.index();

        
        int dm3 = arr.get(index_u);
        if(d2 % 3 != dm3){
            printf("dist %d: count %d\n", d2, count-1);
            d2 += 1;
            count = 1;
            //if(d2 == 7) break;
        }

        int dist = arr.get(index_u) + 1;
        if(dist == 3) dist = 0;
        if(distance % 3 != dist){
            distance += 1;
            //printf("dist: %d, distance: %d\n", dist, distance);
            //if(distance == 7) break;
        }
        //printf("u = %d, dist = %d\n", index_u, distance-1);
        for(int i = 0; i < 18; ++i){
            coord v = u.neigh(i);
            uint index = v.index();
            if(arr.get(index) == 3){
                //printf("  node %d found\n", index);
                arr.set(index, dist);
                if(distance < 8) queue.push_back(v);
            }
        }
    }
    printf("count = %d\n", count);
}


uint idx(Cube& cube){
    return 2187 * ((cube.slice_coord() << 11) | cube.flip_coord()) + cube.twist_coord();
}

Cube neigh(Cube& c, int i){
    return c * Cube::moves[i];
}

void pruning_table2(table& arr){
    arr.fill(3);
    printf("Table filled.\n");

    int distance = 0;
    Cube id;
    deque<Cube> queue = {id};
    arr.set(idx(id), 0);

    int count = 0;

    while(!queue.empty()){
        count += 1;
        Cube u = queue.front();
        queue.pop_front();
        uint index_u = idx(u);
        int dist = arr.get(index_u) + 1;
        if(dist == 3) dist = 0;
        if(distance % 3 != dist){
            distance += 1;
            //printf("dist: %d, distance: %d\n", dist, distance);
            if(distance == 4) break;
        }
        printf("u = %d, dist = %d\n", index_u, distance-1);
        for(int i = 0; i < 18; ++i){
            Cube v = neigh(u, i);
            uint index = idx(v);
            if(arr.get(index) == 3){
                //printf("  node %d found\n", index);
                arr.set(index, dist);
                queue.push_back(v);
            }
        }
    }
    printf("count = %d\n", count-1);
}





void pruning_table3(table& arr, int start, int end){
    std::vector<ushort> twistmt = twist_movetable();
    std::vector<ushort> flipmt = flip_movetable();
    std::vector<ushort> slicemt = slice_movetable();
    printf("Move table calculated.\n");

    int distance = 0;

    Cube id;
    coord goal(id);
    arr.set(goal.index(), 0);

    for(int d = start; d <= end; ++d){
        int prev = (d + 2) % 3;
        int dist = d % 3;
        printf("prev = %d, dist = %d\n", prev, dist);
        uint i = 0;
        for(uint slice = 0; slice < 495; ++slice){
            if(slice % 5 == 0) printf("%d / 100\n", slice / 5);
            for(uint flip = 0; flip < 2048; ++flip){
                for(uint twist = 0; twist < 2187; ++twist){
                    if(arr.get(i) == prev){
                        for(int j = 0; j < 18; ++j){
                            uint index = 2187 * ((slicemt[18*slice + j] << 11) | flipmt[18*flip + j]) + twistmt[18*twist + j];
                            if(arr.get(index) == 3){
                                arr.set(index, dist);
                                //printf("found node %d\n", index);
                            }
                        }
                    }
                    i += 1;
                }
            }
        }
    }
}




void pruning_table4(table& arr, int start, int end){
    std::vector<ushort> twistmt = twist_movetable();
    std::vector<ushort> flipmt = flip_movetable();
    std::vector<ushort> slicemt = slice_movetable();
    printf("Move table calculated.\n");

    int distance = 0;

    Cube id;
    coord goal(id);
    arr.set(goal.index(), 0);
    printf("goal: %d, dist: %d\n\n", goal.index(), arr.get(goal.index()));

    for(int d = start; d <= end; ++d){
        int prev = (d + 2) % 3;
        int dist = d % 3;
        uint i = 0;
        for(uint slice = 0; slice < 495; ++slice){
            if(slice % 5 == 0) printf("%d / 100\n", slice / 5);
            for(uint flip = 0; flip < 2048; ++flip){
                for(uint twist = 0; twist < 2187; ++twist){
                    /*
                    if(i != 2187 * ((slice << 11) | flip) + twist){
                        printf("i = %d, idx = %d\n", i, 2187 * ((slice << 11) | flip) + twist);
                    }
                    */
                    //*
                    if(arr.get(i) == 3){
                        for(int j = 0; j < 18; ++j){
                            uint index = 2187 * ((slicemt[18*slice + j] << 11) | flipmt[18*flip + j]) + twistmt[18*twist + j];
                            if(arr.get(index) == prev){
                                arr.set(i, dist);
                                //printf("found node %d\n", i);
                                continue;
                            }
                        }
                    }
                    //*/
                    i += 1;
                }
            }
        }
    }
}



vector<int> solve(Cube& cube, table& arr){
    vector<int> moves;
    coord c(cube);
    bool run = true;
    while(run){
        run = false;
        int dist = (arr.get(c.index()) + 2) % 3;
        printf("c = %d, dist = %d, sl: %d, fl; %d, tw: %d\n", c.index(), arr.get(c.index()), c.slice(), c.flip(), c.twist());
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


int main(){
    cmb::calcbin();
    table arr(2217093120);
    arr.fill(3);
    printf("Table filled.\n");
    pruning_table(arr);
    pruning_table3(arr, 9, 10);
    pruning_table4(arr, 11, 12);
    io::write("stage1.bin", arr.bits, (arr.len + 3) / 4);
    

    // testing
    /*
    Cube c;
    c *= Cube::L * Cube::F * Cube::R * Cube::F;
    solve(c, arr);
    //*/
}