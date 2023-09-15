#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include "../Settings.h"



namespace io{

    template<size_t n>
    bool read(std::string filename, std::array<byte, n>& data){
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

    template<size_t n>
    bool write(std::string filename, std::array<byte, n>& data){
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

    bool read(std::string filename, int* data, int n);
    bool write(std::string filename, int* data, int n);

}