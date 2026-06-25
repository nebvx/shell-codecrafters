#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  //user input
  std::string user_input = "";
  while(true) {
    std::cout << "$ ";
    std::cin >> user_input;
    if (user_input == "exit") break;
    std::cout << user_input << ": command not found \n";
    user_input = "";
  }
}
