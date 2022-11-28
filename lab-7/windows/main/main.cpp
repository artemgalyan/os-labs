#include <iostream>
#include <windows.h>
#include <string>

std::string GetExecutablePath(const std::string& name) {
  return "../" + name + "/" + name + ".exe";
}

bool CreateProcess(const std::string& path, STARTUPINFO* si, PROCESS_INFORMATION* pi, bool inherit_handles = true) {
  char buff[1000];
  strcpy_s(buff, sizeof(buff), path.c_str());
  return CreateProcess(nullptr, buff, nullptr, nullptr, inherit_handles, 0, nullptr, nullptr, si, pi);
}

bool CreatePipe(HANDLE& in, HANDLE& out, SECURITY_ATTRIBUTES* attr) {
  return CreatePipe(&in, &out, attr, 0);
}

struct ProcessInfo {
  std::string path;
  STARTUPINFO si{};
  PROCESS_INFORMATION pi{};
  HANDLE in = nullptr;
  HANDLE out = nullptr;
  explicit ProcessInfo(const std::string& path) : path(GetExecutablePath(path)) {
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
  }
  bool CreateProc() {
    return ::CreateProcess(path, &si, &pi);
  }
  void WaitAndClose() const {
    CloseHandles();
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
  }
  ~ProcessInfo() {
    WaitAndClose();
  }
  void CloseHandles() const {
    if (in != nullptr) {
      CloseHandle(in);
    }
  }
  ProcessInfo& PipeTo(ProcessInfo& other) {
    SECURITY_ATTRIBUTES saPipe = {sizeof(SECURITY_ATTRIBUTES), nullptr, true};
    CreatePipe(other.in, out, &saPipe);
    si.hStdOutput = out;
    other.si.hStdInput = other.in;
    si.dwFlags = STARTF_USESTDHANDLES;
    other.si.dwFlags = STARTF_USESTDHANDLES;
    return other;
  }
  ProcessInfo& operator|(ProcessInfo& other) {
    return PipeTo(other);
  }
};
int main() {
  ProcessInfo m("M"), a("A"), p("P"), s("S");
  SECURITY_ATTRIBUTES saFile = {sizeof(SECURITY_ATTRIBUTES), nullptr, TRUE};
  char fileName[] = "../../file.txt";
  HANDLE hFileIn = CreateFile(fileName, GENERIC_READ, 0, &saFile, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  s.si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  m.si.hStdInput = hFileIn;
  m | a | p | s;
  if (!m.CreateProc()) { return -1; }
  if (!a.CreateProc()) { return -1; }
  if (!p.CreateProc()) { return -1; }
  if (!s.CreateProc()) { return -1; }
  CloseHandle(hFileIn);
  return 0;
}