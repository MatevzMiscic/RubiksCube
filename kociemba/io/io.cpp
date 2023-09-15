#include "io.h"

using namespace std;

bool io::read(std::string filename, int* data, int n){
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if(!file){
        return false;
    }
    file.read((char*)&(data[0]), n);
    file.close();
    if(!file.good()){
        return false;
    }
    return true;
}

bool io::write(std::string filename, int* data, int n){
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    if(!file){
        return false;
    }
    file.write((char*)&(data[0]), n);
    file.close();
    if(!file.good()){
        return false;
    }
    return true;
}