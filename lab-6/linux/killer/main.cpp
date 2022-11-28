#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>

int system(const std::string& str) {
  return system(str.c_str());
}

void KillByName(const char* _name) {
  system("killall " + std::string(_name));
}

void KillById(int pid) {
  system("kill " + std::to_string(pid));
}

int main(int argc, char* args[]) {
  bool kill_by_name = true;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(args[i], "--name") == 0) {
      kill_by_name = true;
    } else if (strcmp(args[i], "--id") == 0) {
      kill_by_name = false;
    } else {
      try {
        if (kill_by_name) {
          KillByName(args[i]);
        } else {
          KillById(std::stoi(args[i]));
        }
      }
      catch (...) {
        std::cerr << "Wrong argument at " << i << ": " << args[i] << std::endl;
      }
    }
  }
  char buff[100];
  if (getenv("PROC_TO_KILL") == nullptr)
    return 0;

  strcpy(buff, getenv("PROC_TO_KILL"));
  std::stringstream ss(buff);
  while (ss.getline(buff, sizeof(buff), ',')) {
    KillByName(buff);
  }
  return 0;
}