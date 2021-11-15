#include "Parser.h"

Parser::Parser(std::string file){
	filename = file;
}

void Parser::parse(){
    std::ifstream ifs("../beatmaps/" + filename);
	std::string line;
	if (ifs.is_open()){
		//std::getline(ifs, line);
		//std::cout << line << std::endl;
		while(std::getline(ifs, line)){
			if(line[line.size()-1] == 13){
				line.pop_back();
			}
			if(line[0] == '[' and line[line.size()-1] == ']'){
				std::string header = line.substr(1);
				header.pop_back();
				if(header == "General"){
					std::string subLine;
					while(std::getline(ifs, subLine)){
						if(subLine[subLine.size()-1] == 13){
							subLine.pop_back();
						}

						if(subLine.size() == 0){
							break;
						}

						std::string key;
						std::string value;
						bool hasSpaceBefore;
						bool hasSpaceAfter;
						hasSpaceBefore = false;
						hasSpaceAfter = true;
						int spliceLocation;
						for(spliceLocation = 0; spliceLocation < subLine.size(); spliceLocation++){
							if(subLine[spliceLocation] == ':')
								break;
						}
						key = subLine.substr(0, spliceLocation - hasSpaceBefore);
						value = subLine.substr(spliceLocation + hasSpaceAfter + 1, subLine.size() - 1 - spliceLocation - hasSpaceAfter);
						std::cout << key << std::endl;
						std::cout << value << std::endl;
						configGeneral[key] = value;
					}
				}
			}
		}
	}	
}

bool Parser::stringCompare(std::string firstString, std::string secondString){
	bool ans = true;
	for(int i = 0; i < std::min(firstString.size(), secondString.size()); i++){
		if(firstString[i] != secondString[i]){
			ans = false;
			break;
		}
	}
	return ans;
}
