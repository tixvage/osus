#include "Parser.h"

//Debug
void print_map(std::string_view comment, const std::map<std::string, std::string>& m)
{
    std::cout << comment << std::endl;
    for (const auto& [key, value] : m) {
        std::cout << key << " = " << value << "; " <<  std::endl;
    }
    std::cout << "\n";
}

int main(){
	Parser parser = Parser("BeautifulWorld.osu");
	parser.parse();
    print_map("General : ", parser.configGeneral);
    print_map("Editor : ", parser.configEditor);
    print_map("Metadata : ", parser.configMetadata);
    print_map("Difficulty : ", parser.configDifficulty);
    return 0;
}