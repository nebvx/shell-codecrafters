#include <iostream>
#include <string>
#include <vector>

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

void handleType(std::vector<std::string> input) {
  bool builtin {false};
  std::string command = input.at(1);
  if (command == "echo" ||command == "type" || command == "exit") {
    builtin = true;
  }
  
  std::string message = "";
  message = builtin ? " is a shell builtin" : ": not found";
  std::cout << input.at(1) << message << "\n";
}

bool handleInput(std::string user_input) {
  std::vector<std::string> input = getVectorOfInput(user_input);
  if (input.at(0) == "echo") {
    handleEcho(input);
  } else if (input.at(0) == "exit") {
    return false;
  } else if (input.at(0) == "type") {
    handleType(input);
  } else {
    std::cout << user_input << ": command not found \n";
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
    if (!user_input.empty()) {
      if (!handleInput(user_input)) {
        //std::cout << "exit called \n"; //debug
        break;
      }
    }
  
    user_input = "";
  }
}
