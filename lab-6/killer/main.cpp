#include <iostream>
#include <utility>
#include <vector>
#include <windows.h>
#include <tlhelp32.h>
#include <sstream>

const std::string ENV_VAR_NAME = "PROC_TO_KILL";

const std::string ID_PARAM = "--id";
const std::string NAME_PARAM = "--name";

enum ProcessIdentificationType {
  kID = 0,
  kNAME = 1
};

std::vector<std::string> ParseArgs(int argc, char* args[]) {
  std::vector<std::string> result;
  for (int i = 1; i < argc; ++i) {
    result.emplace_back(args[i]);
  }
  return result;
}

struct Process {
  std::string name;
  DWORD id;
  Process(std::string name, DWORD id) : name(std::move(name)), id(id) {}
};

std::vector<Process> GetProcesses() {
  std::vector<Process> result;
  HANDLE process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (process_snap == INVALID_HANDLE_VALUE) {
    return result;
  }
  PROCESSENTRY32 pe32;
  pe32.dwSize = sizeof(PROCESSENTRY32);
  bool proc = Process32First(process_snap, &pe32);
  while (proc) {
    result.emplace_back(std::string(pe32.szExeFile), pe32.th32ProcessID);
    proc = Process32Next(process_snap, &pe32);
  }
  CloseHandle(process_snap);
  return result;
}

bool Kill(const Process& to_kill, int exit_code = 1) {
  std::cout << "Killing" << to_kill.name << " " << to_kill.id << std::endl;
  HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, to_kill.id);
  if (hProc == nullptr) {
    return false;
  }
  bool result = TerminateProcess(hProc, exit_code);
  CloseHandle(hProc);
  return result;
}

ProcessIdentificationType GetPIType(const std::string& str) {
  if (str == ID_PARAM) {
    return ProcessIdentificationType::kID;
  }
  return ProcessIdentificationType::kNAME;
}

void InsertEnvironmentVariableData(std::vector<std::string>& args) {
  const DWORD max_size = 256;
  char buff[max_size];
  bool var = GetEnvironmentVariable(ENV_VAR_NAME.c_str(), buff, max_size);
  if (!var)
    return;

  args.emplace_back("--name");
  std::stringstream ss(buff);
  while (ss.getline(buff, max_size, ',')) {
    args.emplace_back(buff);
  }
}

std::vector<Process> ToProcessVector(const std::vector<std::string>& vector) {
  std::vector<Process> result;
  auto p = GetProcesses();
  ProcessIdentificationType type = kNAME;
  for (auto& s : vector) {
    if (s == ID_PARAM || s == NAME_PARAM) {
      type = GetPIType(s);
      continue;
    }

    auto it = std::find_if(p.begin(), p.end(), [&](const Process& p) -> bool {
      if (type == kID) {
        return p.id == std::stoul(s);
      }
      return p.name == s;
    });
    if (it != p.end())
      result.push_back(*it);
  }
  return result;
}

int main(int argc, char* args[]) {
  std::vector<std::string> args_array = ParseArgs(argc, args);
  InsertEnvironmentVariableData(args_array);
  std::vector<Process> processes = ToProcessVector(args_array);
  for (auto& proc: processes) {
    Kill(proc);
  }
  return 0;
}