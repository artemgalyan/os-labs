#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sys/wait.h>

#define CHILD_PROCESS 0
#define NOT_RUNNING (-5)

void StartProcess(const std::string& path) {
  char buff[200];
  strcpy(buff, path.c_str());
  execl(buff, nullptr);
}

struct ProcessInfo {
  std::string path;
  int process_id = NOT_RUNNING;
  int stdIn = dup(STDIN_FILENO);
  int stdOut = dup(STDOUT_FILENO);
  explicit ProcessInfo(std::string path) : path(std::move(path)) {}
  bool CreateProc() {
    int pid = fork();
    if (pid == -1) {
      return false;
    }
    if (pid != CHILD_PROCESS) {
      process_id = pid;
      return true;
    }
    if (stdIn != STDIN_FILENO) {
      if (dup2(stdIn, 0) == -1) {
        std::cerr << "Error1";
      }
      close(stdIn);
    }
    if (stdOut != STDOUT_FILENO) {
      if (dup2(stdOut, 1) == -1) {
        std::cerr << "Error2";
      }
      close(stdOut);
    }
    StartProcess(path);
    std::exit(10);
  }
  void WaitAndClose() const {
    int status = 0;
    if (process_id != NOT_RUNNING)
      waitpid(process_id, &status, 0);
    ClosePipes();
  }
  ~ProcessInfo() {
    WaitAndClose();
  }
  void ClosePipes() const {
    if (stdIn != STDIN_FILENO) {
//      printf("in %i\n", stdIn);
      close(stdIn);
    }
    if (stdOut != STDOUT_FILENO) {
//      printf("out %i\n", stdOut);
      close(stdOut);
    }
  }
  ProcessInfo& PipeTo(ProcessInfo& other) {
    int pipes[2];
    if (pipe(pipes) == -1) {
      std::cerr << "Failed to create pipe" << std::endl;
    }
    close(stdOut);
    close(other.stdIn);
    stdOut = pipes[1];
    other.stdIn = pipes[0];
    return other;
  }
  ProcessInfo& operator|(ProcessInfo& other) {
    return PipeTo(other);
  }
};

int main() {
  ProcessInfo m("m"), a("a"), p("p"), s("s");
  m | a | p | s;
  close(STDIN_FILENO);
  if (!m.CreateProc()) { return -1; }
  if (!a.CreateProc()) { return -1; }
  if (!p.CreateProc()) { return -1; }
  if (!s.CreateProc()) { return -1; }
  return 0;
}