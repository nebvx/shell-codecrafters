#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>
#include <unistd.h>
#include <sys/wait.h>
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

std::string searchPath(std::string command) {
  const char* env_path = std::getenv("PATH");
  if (env_path == nullptr) return "";

  std::stringstream path_stream(env_path);
  std::string dir;
  while (std::getline(path_stream, dir, ':')) {
    std::string path_with_command = dir + "/" + command;
    if (std::filesystem::exists(path_with_command)) {
      if (isExecutable(path_with_command)) {
        return path_with_command;
      }
    }
  }

  return "";
} 

void handleType(std::vector<std::string> input) {
  std::string command = input.at(1);
  if (command == "echo" || command == "type" || command == "exit") {
    std::cout << command << " is a shell builtin\n";
  } else {
    std::string path = searchPath(command);
    if (!path.empty()) {
      std::cout << command << " is " << path << "\n";
    } else {
      std::cout << command << ": not found\n";
    }
  }
}

bool handleRunProgram(std::vector<std::string> args) {
  if (searchPath(args.at(0)).empty()) return false;

  std::vector<char*> c_args;
  for (auto& arg : args) {
    c_args.push_back(arg.data());
  }
  c_args.push_back(nullptr);

  pid_t pid = fork();
  if (pid == 0) {
    execvp(c_args[0], c_args.data());
    _exit(1);
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
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
  } else if (!handleRunProgram(input)) {
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
