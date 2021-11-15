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
        std::map<std::string , std::string> configEditor;
        std::map<std::string , std::string> configMetadata;
        std::map<std::string , std::string> configDifficulty;
    private:
        std::string filename;
        bool stringCompare(std::string, std::string);
        std::pair<std::string, std::string> parseKeyValue(std::string text, bool hasSpaceBefore, bool hasSpaceAfter);
};
