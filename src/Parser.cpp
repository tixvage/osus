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

						if(subLine[0] == '/' and subLine[1] == '/'){
							continue;
						}

						std::pair<std::string, std::string> keyValue = parseKeyValue(subLine, false, true);
						
						configGeneral[keyValue.first] = keyValue.second;
					}
				}else if(header == "Editor"){
					std::string subLine;
					while(std::getline(ifs, subLine)){
						if(subLine[subLine.size()-1] == 13){
							subLine.pop_back();
						}

						if(subLine.size() == 0){
							break;
						}

						if(subLine[0] == '/' and subLine[1] == '/'){
							continue;
						}

						std::pair<std::string, std::string> keyValue = parseKeyValue(subLine, false, true);
						
						configEditor[keyValue.first] = keyValue.second;
					}
				}else if(header == "Metadata"){
					std::string subLine;
					while(std::getline(ifs, subLine)){
						if(subLine[subLine.size()-1] == 13){
							subLine.pop_back();
						}

						if(subLine.size() == 0){
							break;
						}

						if(subLine[0] == '/' and subLine[1] == '/'){
							continue;
						}

						std::pair<std::string, std::string> keyValue = parseKeyValue(subLine, false, false);
						
						configMetadata[keyValue.first] = keyValue.second;
					}
				}else if(header == "Difficulty"){
					std::string subLine;
					while(std::getline(ifs, subLine)){
						if(subLine[subLine.size()-1] == 13){
							subLine.pop_back();
						}

						if(subLine.size() == 0){
							break;
						}

						if(subLine[0] == '/' and subLine[1] == '/'){
							continue;
						}

						std::pair<std::string, std::string> keyValue = parseKeyValue(subLine, false, false);
						
						configDifficulty[keyValue.first] = keyValue.second;
					}
					
				}else if(header == "Colours"){
					std::string subLine;
					while(std::getline(ifs, subLine)){
						if(subLine[subLine.size()-1] == 13){
							subLine.pop_back();
						}

						if(subLine.size() == 0){
							break;
						}

						if(subLine[0] == '/' and subLine[1] == '/'){
							continue;
						}

						std::pair<std::string, std::string> keyValue = parseKeyValue(subLine, false, false);
						
						configColours[keyValue.first] = keyValue.second;
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

std::pair<std::string, std::string> Parser::parseKeyValue(std::string line, bool hasSpaceBefore, bool hasSpaceAfter)
{
	int spliceLocation;
	for(spliceLocation = 0; spliceLocation < line.size(); spliceLocation++){
		if(line[spliceLocation] == ':')
			break;
	}
	return make_pair(line.substr(0, spliceLocation - hasSpaceBefore), 
	line.substr(spliceLocation + hasSpaceAfter + 1, line.size() - 1 - spliceLocation - hasSpaceAfter));
}
