#include "Parser.h"

int main(){
	Parser parser = Parser("BeautifulWorld.osu");
	parser.parse();
	std::cout << std::endl;
	std::cout << parser.configGeneral["SampleSet"].size() << std::endl;
	std::cout << parser.configGeneral["SampleSet"] << std::endl; 
    return 0;
}