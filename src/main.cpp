#include "Parser.h"

//Debug
void print_map(std::string comment, const std::map<std::string, std::string > &m)
{
    std::cout << comment << std::endl;
    for (const auto& [key, value] : m) {
        std::cout << key << " = " << value << "; " <<  std::endl;
    }
    std::cout << "\n";
}

void print_events(std::vector<Event> events)
{
    std::cout << "Events -> " << std::endl;
    for(int i = 0; i < events.size(); i++){
        std::cout << events[i].eventType << " " 
        << events[i].startTime << " " 
        << events[i].filename << " " 
        << events[i].xOffset << " " 
        << events[i].yOffset << " " 
        << events[i].endTime;
        std::cout << "\n";
    }
    std::cout << "\n";
}

int main(){
	Parser parser = Parser("BeautifulWorld.osu");
	parser.parse();
    print_map("General -> ", parser.configGeneral);
    print_map("Editor -> ", parser.configEditor);
    print_map("Metadata -> ", parser.configMetadata);
    print_map("Difficulty -> ", parser.configDifficulty);
    print_map("Colours -> ", parser.configColours);

    print_events(parser.events);
    return 0;
}