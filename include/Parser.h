#pragma once
#include <vector>
#include "stdio.h"
#include <string.h>
#include <fstream>
#include <iostream>
#include <map>

class Parser{
    public:
        Parser(std::string file);
        void parse();
        std::map<std::string , std::string> configGeneral;
    private:
        std::string filename;
        bool stringCompare(std::string, std::string);
};
