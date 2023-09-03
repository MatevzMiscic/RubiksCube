#include "io.h"

using namespace std;

int main(){
    string filename = "data.bin";
    array<byte, 8> data = {10, 11, 12, 13, 14, 15};
    io::write(filename, data);
    io::read(filename, data);
    for(byte b : data){
        cout << (int)b << "\n";
    }
}