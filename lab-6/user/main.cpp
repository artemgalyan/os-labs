#include <iostream>
#include <windows.h>
#include <vector>

#include "Killer.h"

int main() {
  std::vector<std::string> cmd_processes_names = {"chrome.exe", "devenv"};
  std::vector<std::string> var_processes_names = {"Telegram.exe"};
  std::vector<DWORD> ids_to_kill = {124, 12154};
  Killer killer(cmd_processes_names, ids_to_kill, var_processes_names);
  killer.PrintAllInfo()
        .Kill()
        .PrintAllInfo();
  return 0;
}
