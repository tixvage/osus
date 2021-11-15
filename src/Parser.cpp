#include "Parser.h"



int parse(){
    std::ifstream ifs("../beatmaps/aaa.osu");
	std::string line;
	if (ifs.is_open()){
		std::getline(ifs, line);
	}
	return 0;
}