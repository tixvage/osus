#include "Parser.h"

#include "stdio.h"
#include <string.h>
#include <fstream>
#include <iostream>


std::vector<CircleData> parse(){
    std::vector<CircleData> circledata;
    std::ifstream ifs("../beatmaps/aaa.osu");
	std::string line;
	if (ifs.is_open()){
		std::getline(ifs, line);
		if (strcmp(line.c_str() , "[HitObjects]")){
			while(std::getline(ifs, line)){
				float x,y,time,type,hit_sound;
				
				std::vector<double> v;
				std::vector<int> d;
				std::string filename;

				int last_loc = 0;
				int pos = 0;

				for (int i = 0; i < line.size(); i++){
					if (line[i] == ','){
						v.push_back(std::stod(line.substr(last_loc, i-last_loc)));
						last_loc = i+1;
						if (v.size() > 3){

						}
					}
					if(line[i] == ':'){
						d.push_back(std::stoi(line.substr(last_loc, i-last_loc)));
						last_loc = i+1;
					}
				}
				if (last_loc != line.size() -1){
					filename = line.substr(last_loc, line.size() - last_loc);
				}

				//debugging stuff
				//for(auto& a : v)
				//	std::cout << a << std::endl;
				//std::cout <<  std::endl;
				//for(auto& a : d)
				//	std::cout << a << std::endl;
				//std::cout <<  std::endl;
				//std::cout << filename << std::endl;
				
				//circledata.push_back({new Circle({v[0]*2, v[1]*2}), v[2]});
			}
		}
	}
	return circledata;
}