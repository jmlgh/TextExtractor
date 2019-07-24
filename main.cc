#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>
#include <vector>
#include <experimental/filesystem>


namespace  fs = std::experimental::filesystem;

enum class UserExtractChoice
{
   SAME_LINE,
   SPECIFIED_LINE,
   END_OF_FILE,
   INVALID_CHOICE
};


// signatures
std::string extract_section(const fs::path& path, const std::string& section_start, const std::string& section_end);
UserExtractChoice assign_extraction_choice(const std::string& end_line);
std::ostream& operator<<(std::ostream& os, const UserExtractChoice& choice);


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
      std::cout << section << std::endl;
    } catch (const std::invalid_argument& e) {
      std::cerr << e.what()  << std::endl;
      return -1;
    } 
    
    return 0;
}


UserExtractChoice assign_extraction_choice(const std::string& end_line)
{
  if (end_line.length() == 1) {
    if (end_line == "s" || end_line == "S")
      return UserExtractChoice::SAME_LINE;
    else if (end_line == "e" || end_line == "E")
      return UserExtractChoice::END_OF_FILE;
    else
      return UserExtractChoice::INVALID_CHOICE;
  } else {
    return UserExtractChoice::SPECIFIED_LINE;
  }
}


std::string extract_section(const fs::path& path, const std::string& section_start, const std::string& section_end)
{
  if (!fs::exists(path) || !fs::is_regular_file(path))
    throw std::invalid_argument("ERROR::Specified file is not valid");

  std::ifstream file{path};
  std::string line{};
  std::vector<std::string> section_lines{};

  auto user_extraction_option{assign_extraction_choice(section_end)};
  if (user_extraction_option == UserExtractChoice::INVALID_CHOICE) throw std::invalid_argument("ERROR:Specified extraction strategy is not valid");
  int section_start_line_num{};
  int section_end_line_num{};
  
  if(!file.is_open())
    throw std::invalid_argument("ERROR::Failed to open the specified file");
  else {
    int counter{0};
      
    while(file.good()) {
      std::getline(file, line);

      if (line.find(section_start) != std::string::npos) {
	section_start_line_num = counter;
	if (user_extraction_option == UserExtractChoice::SAME_LINE) {
	  section_end_line_num = section_start_line_num;
	  section_lines.push_back(line);
	  break;
	}
      }

      if (line.find(section_end) != std::string::npos) {
	section_end_line_num = counter;
      }
      
      section_lines.push_back(line);
      counter++;
    }
    
    if (user_extraction_option == UserExtractChoice::END_OF_FILE)
      section_end_line_num = counter--;
  }

  std::cout << "start " << section_start_line_num << std::endl;
  std::cout << "end " << section_end_line_num << std::endl;
  std::string fulltext{};
  if (user_extraction_option == UserExtractChoice::SPECIFIED_LINE || user_extraction_option == UserExtractChoice::END_OF_FILE) {
    for (auto i = section_start_line_num + 1; i < section_end_line_num; i++) {
      fulltext.append(section_lines[i]);
    }
  } else {
    fulltext.append(section_lines[section_start_line_num]);
  }
  
  return fulltext;
}


std::ostream& operator<<(std::ostream& os, const UserExtractChoice& choice)
{
  switch(choice)
  {
  case UserExtractChoice::SAME_LINE:
    os << "SAME_LINE";
    break;
  case UserExtractChoice::SPECIFIED_LINE:
    os << "SPECIFIED_LINE";
    break;
  case UserExtractChoice::END_OF_FILE:
    os << "END_OF_FILE";
    break;
  case UserExtractChoice::INVALID_CHOICE:
    os << "INVALID_CHOICE";
    break;
  }
  return os;
}
