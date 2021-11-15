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

						std::string data[] = subLine.s
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

void Parser::removeWhitespaces(std::string &str){
	for(int i = str.size()-1;i>=0;i--){
		if(str[i] == ' ') str.pop_back();
		else break;
	}
}