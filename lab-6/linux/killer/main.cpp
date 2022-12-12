#include <cstdlib>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <csignal>

void KillById(int pid, int signal = SIGTERM) {
  kill(pid, signal);
}

void KillByName(const std::string& name) {
  std::string command = "pidof " + name;
  char buff[512];
  strcpy(buff, command.c_str());
  FILE* cmd = popen(command.c_str(), "r");
  fgets(buff, sizeof(buff), cmd);
  fclose(cmd);
  std::string result = buff;
  std::stringstream ss(result);
  int pid;
  while (ss >> pid) {
    KillById(pid);
  }
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
  if (getenv("PROC_TO_KILL") == nullptr)
    return 0;

  char buff[100];
  strcpy(buff, getenv("PROC_TO_KILL"));
  std::stringstream ss(buff);
  while (ss.getline(buff, sizeof(buff), ',')) {
    KillByName(buff);
  }
  return 0;
}