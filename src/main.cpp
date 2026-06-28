#include <iostream>
#include <string>
#include <vector>
#include <filesystem> // file system operations
namespace fs = std::filesystem;

std::vector<std::string> getVectorOfInput(std::string user_input) {
  std::vector<std::string> input; //the whole input fromm the user sperated by spaces
  std::string command = ""; //can be echo or the words after

  for (size_t i {0}; i < user_input.size(); ++i) {
    if (user_input.at(i) == ' ') {
      input.push_back(command);
      command = "";
    } else {
      command += user_input[i];
    }
  }
  
  input.push_back(command);
  return input;
}

void handleEcho(std::vector<std::string> input) {
  for (auto word : input) {
    if (word != "echo"){
      std::cout << word << " ";
    }
  }
  std::cout << "\n";

}

bool isExecutable(const fs::path& p) {
  auto perm = fs::status(p).permissions();
  return (perm & fs::perms::owner_exec) != fs::perms::none;
}

bool searchPath(std::string command) {
  const char* env_path = std::getenv("PATH");
  if (env_path == nullptr) return false;

  std::stringstream path_stream(env_path);
  std::string dir;
  while (std::getline(path_stream, dir, ':')) {
    std::string path_with_command = dir + "/" + command;
    if (std::filesystem::exists(path_with_command)) {
      if (isExecutable(path_with_command)) {
        std::cout << command << " is " << path_with_command << "\n";
        return true;
      }
    }
  }

  return false;
} 

void handleType(std::vector<std::string> input) {
  bool builtin {false};
  std::string command = input.at(1);
  if (command == "echo" ||command == "type" || command == "exit") {
    std::cout << command << " is a shell builtin\n";
  } else if (!searchPath(command)) {
    std::cout << command << ": not found\n";
  }
}

bool handleRunProgram(std::string input) {
  std::string user_input = input;
  const char* env_path = user_input.c_str();
  if (isExecutable(user_input)) {
    std::system(env_path);
    return true;
  }
  return false;
}

bool handleInput(std::string user_input) {
  std::vector<std::string> input = getVectorOfInput(user_input);
  if (input.at(0) == "echo") {
    handleEcho(input);
  } else if (input.at(0) == "exit") {
    return false;
  } else if (input.at(0) == "type") {
    handleType(input);
  } else if (!handleRunProgram(user_input)) {
    std::cout << input.at(0) << ": command not found\n";
  }
  return true;
}


int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  //user input
  std::string user_input = "";
  while(true) {
    std::cout << "$ ";
    //user input
    std::getline(std::cin, user_input);
    //std::cout << "user_input: " << user_input << "\n";
    if (!user_input.empty()) {
      if (!handleInput(user_input)) {
        //std::cout << "exit called \n"; //debug
        break;
      }
    }

    user_input = "";
  }
}
