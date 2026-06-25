#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> getVectorOfInput(std::string user_input) {
  //std::cout << "getvector called \n";

  std::vector<std::string> input; //the whole input fromm the user sperated by spaces
  std::string command = ""; //can be echo or the words after

  for (size_t i {0}; i < user_input.size(); ++i) {
    if (user_input.at(i) == ' ') {
      input.push_back(command);
      std::cout << "command: " << command << "\n"; //debugging
      command = "";
    } else {
      command += user_input[i];
    }
  }

  return input;
}

void handleEcho(std::vector<std::string> input) {
 // std::cout << "handleecho called \n";
  
  for (auto word : input) {
    if (word != "echo"){
      std::cout << word;
    }
  }

}


bool handleInput(std::string user_input) {
  //std::cout << "handleinputcalled \n";
  std::vector<std::string> input = getVectorOfInput(user_input);
  if (input.at(0) == "echo") {
    handleEcho(input);
  } else if (input.at(0) == "exit") {
    return false;
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
    std::cin >> user_input;
    if (!handleInput(user_input)) {
      std::cout << "exit called \n"; //debug
      break;
    }

    user_input = "";
  }
}
