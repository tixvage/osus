#include "Parser.h"

Parser::Parser(){}


//this here is just a dumb parser, nothing to document here tbh
GameFile Parser::parse(std::string filename){
	GameFile gameFile;
	gameFile.configDifficulty["SliderMultiplier"] = "1";
	gameFile.configDifficulty["HPDrainRate"] = "5";
	gameFile.configDifficulty["CircleSize"] = "5";
	gameFile.configDifficulty["OverallDifficulty"] = "5";
	gameFile.configDifficulty["ApproachRate"] = "5";
	gameFile.configDifficulty["SliderTickRate"] = "1";
    std::ifstream ifs("resources/beatmaps/" + filename);
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
						
						gameFile.configGeneral[keyValue.first] = keyValue.second;
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
						
						gameFile.configEditor[keyValue.first] = keyValue.second;
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
						
						gameFile.configMetadata[keyValue.first] = keyValue.second;
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
						//std::cout << keyValue.first << " - " << keyValue.second << std::endl;
						gameFile.configDifficulty[keyValue.first] = keyValue.second;
					}
					
				}else if(header == "Events"){
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

						if(subLine[0] == ' '){
							continue;
						}

						Event tempEvent;
						std::vector<std::string> tempVector;
						tempVector = parseSeperatedLists(subLine, ',');

						if(tempVector[0] == "0"){
							tempEvent.eventType = 0;
						}else if(tempVector[0] == "1" or tempVector[0] == "Video"){
							tempEvent.eventType = 1;
						}else if(tempVector[0] == "2" or tempVector[0] == "Break"){
							tempEvent.eventType = 2;
						}else{
							continue;
						}

						tempEvent.startTime = std::stoi(tempVector[1]);
						//std::cout<<tempEvent.eventType<<std::endl;
						if(tempEvent.eventType == 0){
							tempEvent.filename = tempVector[2].substr(1, tempVector[2].size()-2);
							if(tempVector.size()>3)
							{
								tempEvent.xOffset = std::stoi(tempVector[3]);
								tempEvent.yOffset = std::stoi(tempVector[4]);
							}else{
								tempEvent.xOffset = 0;
								tempEvent.yOffset = 0;
							}
							
						}else if(tempEvent.eventType == 1){
							tempEvent.filename = tempVector[2].substr(1, tempVector[2].size()-2);
							if(tempVector.size()>3)
							{
								tempEvent.xOffset = std::stoi(tempVector[3]);
								tempEvent.yOffset = std::stoi(tempVector[4]);
							}else{
								tempEvent.xOffset = 0;
								tempEvent.yOffset = 0;
							}
						}else if(tempEvent.eventType == 2){
							tempEvent.endTime = std::stoi(tempVector[2]);
						}

						gameFile.events.push_back(tempEvent);
					}
				}else if(header == "TimingPoints"){
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

						TimingPoint tempTimingPoint;
						std::vector<std::string> tempVector;
						tempVector = parseSeperatedLists(subLine, ',');
						tempTimingPoint.time = std::stoi(tempVector[0]);
						tempTimingPoint.beatLength = std::stof(tempVector[1]);
						tempTimingPoint.meter = std::stoi(tempVector[2]);
						tempTimingPoint.sampleSet = std::stoi(tempVector[3]);
						tempTimingPoint.sampleIndex = std::stoi(tempVector[4]);
						tempTimingPoint.volume = std::stoi(tempVector[5]);
						tempTimingPoint.uninherited = bool(std::stoi(tempVector[6]));
						tempTimingPoint.effects = std::stoi(tempVector[7]);
						gameFile.timingPoints.push_back(tempTimingPoint);
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
						
						if(subLine[0] == 'C'){
							std::vector<int> tempColour (3);
							std::vector<std::string> tempVectorColours;
							tempVectorColours = parseSeperatedLists(keyValue.second, ',');
							for(int i = 0; i < 3; i++){
								tempColour[i] = std::stoi(tempVectorColours[i]);
							}
							gameFile.comboColours.push_back(tempColour);
						}

						gameFile.configColours[keyValue.first] = keyValue.second;
					}
				}else if(header == "HitObjects"){
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

						std::vector<std::string> tempVector;
						tempVector = parseSeperatedLists(subLine, ',');

						/*for(int i = 0; i < tempVector.size(); i++){
							std::cout << tempVector[i] << " <-> ";
						}*/

						HitObjectData tempHitObject;

						tempHitObject.x = std::stoi(tempVector[0]) + 64;
						tempHitObject.y = std::stoi(tempVector[1]) + 48;
						tempHitObject.time = std::stoi(tempVector[2]);
						tempHitObject.type = std::stoi(tempVector[3]);

						std::string typeInBinary;
						typeInBinary = DecimalToBinaryString(tempHitObject.type);

						/*std::cout << typeInBinary;
						std::cout << std::endl;*/
						if(typeInBinary[0] == '1'){
							tempHitObject.type = 1;
							tempHitObject.hitSound = std::stoi(tempVector[4]);
							tempHitObject.startingACombo = typeInBinary[2] == '1';
							tempHitObject.skipComboColours = int(typeInBinary[4] == '1') + int(typeInBinary[5] == '1') * 2 + int(typeInBinary[6] == '1') * 4;

							if(tempVector.size() > 5){
								std::vector<std::string> tempVectorHitSample;
								tempVectorHitSample = parseSeperatedLists(tempVector[5], ':');

								tempHitObject.normalSet = std::stoi(tempVectorHitSample[0]);
								tempHitObject.additionSet = std::stoi(tempVectorHitSample[1]);
								tempHitObject.index = std::stoi(tempVectorHitSample[2]);
								tempHitObject.volume = std::stoi(tempVectorHitSample[3]);
								if(tempVectorHitSample.size() > 4){
									tempHitObject.filename = tempVectorHitSample[4];
									tempHitObject.useDefaultHitSound = false;
								}
								else tempHitObject.useDefaultHitSound = true;
							}
							else {
								tempHitObject.normalSet = 0;
								tempHitObject.additionSet = 0;
								tempHitObject.index = 0;
								tempHitObject.volume = 0;
								tempHitObject.useDefaultHitSound = true;
							}

						}
						else if(typeInBinary[1] == '1'){
							tempHitObject.type = 2;
							tempHitObject.hitSound = std::stoi(tempVector[4]);
							tempHitObject.startingACombo = typeInBinary[2] == '1';
							tempHitObject.skipComboColours = int(typeInBinary[4] == '1') + int(typeInBinary[5] == '1') * 2 + int(typeInBinary[6] == '1') * 4;

							std::vector<std::string> tempVectorCurvePoints;
							tempVectorCurvePoints = parseSeperatedLists(tempVector[5], '|');

							tempHitObject.curveType = tempVectorCurvePoints[0][0];

							for(int i = 1; i < tempVectorCurvePoints.size(); i++){
								std::vector<std::string> tempVectorCurvePointCords;
								tempVectorCurvePointCords = parseSeperatedLists(tempVectorCurvePoints[i], ':');
								tempHitObject.curvePoints.push_back(std::make_pair(std::stoi(tempVectorCurvePointCords[0])+64, std::stoi(tempVectorCurvePointCords[1])+48));
							}

							tempHitObject.slides = std::stoi(tempVector[6]);
							tempHitObject.length = std::stoi(tempVector[7]);

							if(tempVector.size() > 8){
								std::vector<std::string> tempVectorEdgeSounds;
								tempVectorEdgeSounds = parseSeperatedLists(tempVector[8], '|');
								for(int i = 0; i < tempVectorEdgeSounds.size(); i++){
									tempHitObject.edgeSounds.push_back(std::stoi(tempVectorEdgeSounds[i]));
								}
							}
							if(tempVector.size() > 9){
								std::vector<std::string> tempVectorEdgeSets;
								tempVectorEdgeSets = parseSeperatedLists(tempVector[9], '|');

								for(int i = 1; i < tempVectorEdgeSets.size(); i++){
									std::vector<std::string> tempVectorEdgeSetsCords;
									tempVectorEdgeSetsCords = parseSeperatedLists(tempVectorEdgeSets[i], ':');
									tempHitObject.edgeSets.push_back(std::make_pair(std::stoi(tempVectorEdgeSetsCords[0]), std::stoi(tempVectorEdgeSetsCords[1])));
								}
							}
							if(tempVector.size() > 10){
								std::vector<std::string> tempVectorHitSample;
								tempVectorHitSample = parseSeperatedLists(tempVector[10], ':');

								tempHitObject.normalSet = std::stoi(tempVectorHitSample[0]);
								tempHitObject.additionSet = std::stoi(tempVectorHitSample[1]);
								tempHitObject.index = std::stoi(tempVectorHitSample[2]);
								tempHitObject.volume = std::stoi(tempVectorHitSample[3]);
								if(tempVectorHitSample.size() > 4){
									tempHitObject.filename = tempVectorHitSample[4];
									tempHitObject.useDefaultHitSound = false;
								}
								else tempHitObject.useDefaultHitSound = true;
							}
							else {
								tempHitObject.normalSet = 0;
								tempHitObject.additionSet = 0;
								tempHitObject.index = 0;
								tempHitObject.volume = 0;
								tempHitObject.useDefaultHitSound = true;
							}
						}
						else if(typeInBinary[3] == '1'){
							tempHitObject.type = 3;
							tempHitObject.hitSound = std::stoi(tempVector[4]);
							tempHitObject.startingACombo = typeInBinary[2] == '1';
							tempHitObject.skipComboColours = int(typeInBinary[4] == '1') + int(typeInBinary[5] == '1') * 2 + int(typeInBinary[6] == '1') * 4;

							tempHitObject.endTime = std::stoi(tempVector[5]);

							if(tempVector.size() > 6){
								std::vector<std::string> tempVectorHitSample;
								tempVectorHitSample = parseSeperatedLists(tempVector[6], ':');

								tempHitObject.normalSet = std::stoi(tempVectorHitSample[0]);
								tempHitObject.additionSet = std::stoi(tempVectorHitSample[1]);
								tempHitObject.index = std::stoi(tempVectorHitSample[2]);
								tempHitObject.volume = std::stoi(tempVectorHitSample[3]);
								if(tempVectorHitSample.size() > 4){
									tempHitObject.filename = tempVectorHitSample[4];
									tempHitObject.useDefaultHitSound = false;
								}
								else tempHitObject.useDefaultHitSound = true;
							}
							else {
								tempHitObject.normalSet = 0;
								tempHitObject.additionSet = 0;
								tempHitObject.index = 0;
								tempHitObject.volume = 0;
								tempHitObject.useDefaultHitSound = true;
							}
						}
						gameFile.hitObjects.push_back(tempHitObject);
					}
				}
			}
		}
	}

	return gameFile;	
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

std::vector<std::string> Parser::parseSeperatedLists(std::string list, char seperator){
	std::vector<std::string> ans;
	int lastCommaLocation = - 1;
	for(int i = 0; i < list.size(); i++){
		if(list[i] == seperator or i == list.size()-1){
			ans.push_back(list.substr(lastCommaLocation + 1, i - lastCommaLocation - 1 + int(i == list.size()-1)));
			lastCommaLocation = i;
		}
	}
	return ans;
}

std::string Parser::DecimalToBinaryString(int a)
{
    std::string binary = "";
    int mask = 1;
    for(int i = 0; i < 7; i++){
        if((mask & a) >= 1)
            binary = binary + "1";
        else
            binary = binary + "0";
        mask <<= 1;
    }
    //std::cout << binary << std::endl;
    return binary;
}
