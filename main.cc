#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>
#include <vector>
#include <experimental/filesystem>


namespace  fs = std::experimental::filesystem;


std::string extract_section(const fs::path& path, const std::string& section_start, const std::string& section_end);

int main(int argc, char* argv[])
{	
    if (argc != 4)
    {
        std::cout << "Usage: textex <file_path> <section_start> <section_end>\n" << std::endl;
        return -1;
    }
    
    fs::path file_path{argv[1]};
    std::string section_start{argv[2]};
    std::string section_end{argv[3]};

    try {
      auto section = extract_section(file_path, section_start, section_end);
      std::cout << std::endl << section << std::endl;
    } catch (const std::invalid_argument& e) {
      std::cerr << e.what()  << std::endl;
      return -1;
    } 
    
    return 0;
}


std::string extract_section(const fs::path& path, const std::string& section_start, const std::string& section_end)
{
  if (!fs::exists(path) || !fs::is_regular_file(path))
    throw std::invalid_argument("ERROR::Specified file is not valid");

  std::ifstream file{path};
  std::string line{};
  std::vector<std::string> section_lines{};

  int section_start_line_num{};
  int section_end_line_num{};

  if(!file.is_open())
    throw std::invalid_argument("ERROR::Failed to open the specified file");
  else {
    int counter{0};
    while(file.good()) {
      std::getline(file, line);
      if (line.find(section_start) != std::string::npos)
	section_start_line_num = counter;
      if (line.find(section_end) != std::string::npos)
	section_end_line_num = counter;
      
      section_lines.push_back(line);
      counter++;
    }
  }

  std::string fulltext{};
  for (auto i = section_start_line_num + 1; i < section_end_line_num; i++) {
    fulltext.append(section_lines[i]);
  }
  
  return fulltext;
}
