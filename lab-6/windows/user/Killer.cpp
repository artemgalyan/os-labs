
#include "Killer.h"

const std::string Killer::PATH_TO_KILLER = "../../killer/cmake-build-debug/killer.exe";
Killer::Killer(const std::vector<std::string>& names,
               const std::vector<DWORD>& ids,
               const std::vector<std::string>& namesToVar) {
  if (!names.empty()) {
    cmd_string_ += "--name ";
  }
  for (auto& name: names) {
    cmd_string_ += name + " ";
  }
  if (!ids.empty()) {
    cmd_string_ += "--id ";
  }
  for (DWORD id: ids) {
    cmd_string_ += std::to_string(id) + " ";
  }
  for (int i = 0; i < namesToVar.size(); ++i) {
    var_string_ += namesToVar[i];
    if (i != namesToVar.size() - 1) {
      var_string_ += ',';
    }
  }
  processes_to_kill_ = Match(names, namesToVar, ids);
}

const Killer& Killer::PrintAllInfo() const {
  for (auto& process : processes_to_kill_) {
    std::cout << "Process [" << process.id << "] " << process.name << " is " << (IsRunning(process.id)
                                                                                 ? "running"
                                                                                 : "stopped");
    std::cout << std::endl;
  }
  std::cout << std::endl;
  return *this;
}

const Killer& Killer::Kill() const {
  SetEnvironmentVariable("PROC_TO_KILL", var_string_.c_str());
  STARTUPINFO si = {sizeof(si)};
  PROCESS_INFORMATION process_information;
  const int BUFF_SIZE = 500;
  char buff[BUFF_SIZE];
  std::string run_str = PATH_TO_KILLER + " " + cmd_string_;
  strcpy_s(buff, BUFF_SIZE, run_str.c_str());
  if (CreateProcess(nullptr, buff, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &process_information)) {
    CloseHandle(process_information.hThread);
    WaitForSingleObject(process_information.hProcess, INFINITE);
    CloseHandle(process_information.hProcess);
    Sleep(500);
  } else {
    std::cerr << "Failed to start killer process." << std::endl;
  }
  SetEnvironmentVariable("PROC_TO_KILL", nullptr);
  return *this;
}

bool Killer::IsRunning(DWORD id) {
  auto processes = GetActiveProcesses();
  return std::any_of(processes.begin(), processes.end(), [&](const Process& p) {
    return p.id == id;
  });
}

std::vector<Killer::Process> Killer::Match(const std::vector<std::string>& names,
                                   const std::vector<std::string>& names2,
                                   const std::vector<DWORD>& ids) {
  std::vector<Process> result;
  auto active = GetActiveProcesses();
  for (auto& p: active) {
    if (Contains(names, p.name) || Contains(names2, p.name) || Contains(ids, p.id)) {
      result.push_back(p);
    }
  }
  return result;
}

std::vector<Killer::Process> Killer::GetActiveProcesses() {
  std::vector<Process> result;
  HANDLE process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (process_snap == INVALID_HANDLE_VALUE) {
    return result;
  }

  PROCESSENTRY32 pe32;
  pe32.dwSize = sizeof(PROCESSENTRY32);
  bool proc = Process32First(process_snap, &pe32);
  while (proc) {
    result.emplace_back(pe32.th32ProcessID, std::string(pe32.szExeFile));
    proc = Process32Next(process_snap, &pe32);
  }
  CloseHandle(process_snap);
  return result;
}

Killer::Process::Process(DWORD handle, std::string name) : id(handle), name(std::move(name)) {}

bool Killer::Process::operator==(const Killer::Process& rhs) const {
  return name == rhs.name;
}

bool Killer::Process::operator!=(const Killer::Process& rhs) const {
  return !(rhs == *this);
}