#include "Parser.h"



void parse(){
    std::ifstream ifs("../beatmaps/BeautifulWorld.osu");
	std::string line;
	if (ifs.is_open()){
		//std::getline(ifs, line);
		//std::cout << line << std::endl;
		while(std::getline(ifs, line)){
			if(stringCompare(line, "[Colours]")){
				std::cout << "les goo" << std::endl;
				std::cout << line << std::endl;
			}
		}
	}	
}

bool stringCompare(std::string firstString, std::string secondString){
	bool ans = true;
	for(int i = 0; i < std::min(firstString.size(), secondString.size()); i++){
		if(firstString[i] != secondString[i]){
			ans = false;
			break;
		}
	}
	return ans;
}