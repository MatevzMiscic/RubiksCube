#include <iostream>
#include <array>
#include <deque>
#include "../Settings.h"
#include "../moves/moves.h"

using namespace std;


struct coord{

    static std::array<short, 18*2187> twistmt;
    static std::array<short, 18*2048> flipmt;
    static std::array<short, 18*495> slicemt;

    // in the least significant 9 bits slice coordinate is stored
    // in the next 12 bits twist coordinate is stored
    // in the next 11 bits flip coordinate is stored
    int val;
    
    coord(int val){
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
};



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


struct table{
    // table of length len, where all each entry has two bits

    int len;
    int* bits;
    
    table(int n){
        len = n;
        bits = new int[n/16 + 1];
    }
    ~table(){
        delete[] bits;
    }

    void fill(int a){
        int flag = 0;
        for(int i = 0; i < 16; ++i){
            flag = (flag << 2) | a;
        }
        //printb(a);
        //printb(flag);
        for(int i = 0; i < len/16 + 1; ++i){
            bits[i] = flag;
        }
    }

    inline int get(int index){
        return (bits[index >> 4] >> ((index & 15) << 1)) & 3;
    }

    inline void set(int index, int val){
        int shift = (index & 15) << 1;
        bits[index >> 4] &= ~(3 << shift);
        bits[index >> 4] |= val << shift;
    }

    void print(){
        for(int i = 0; i < len; ++i){
            printf("%d ", get(i));
        }
        printf("\n");
    }
};



// 2217093120
void pruning_table(table& arr){
    coord::compute_mts();
    printf("Move table calculated.\n");

    arr.fill(3);
    printf("Table filled.\n");

    int distance = 0;
    Cube id;
    coord goal(id);
    deque<coord> queue = {goal};
    int index = goal.index();
    arr.set(index, 0);

    int count = 0;

    while(!queue.empty()){
        count += 1;
        coord u = queue.front();
        queue.pop_front();
        int index_u = u.index();
        int dist = arr.get(index_u) + 1;
        if(distance % 3 != dist){
            distance += 1;
            if(distance == 3) break;
        }
        for(int i = 0; i < 18; ++i){
            coord v = u.neigh(i);
            int index = v.index();
            if(arr.get(index) == 3){
                if(dist == 3) dist = 0;
                arr.set(index, dist);
                queue.push_back(v);
            }
        }
    }
    printf("count = %d\n", count);
}


int main(){
    table arr(554273280);
    pruning_table(arr);
}