#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sys/wait.h>
#include <vector>
#include <algorithm>

#define CHILD_PROCESS 0
#define NOT_RUNNING (-1)
#define BUFF_SIZE 255

void StartProcess(const std::string& path) {
  char buff[BUFF_SIZE];
  strcpy(buff, path.c_str());
  execl(buff, nullptr);
}

int system(const std::string& command) {
  char buff[BUFF_SIZE];
  strcpy(buff, command.c_str());
  return system(buff);
}

struct Process {
  std::string path;
  int process_id = NOT_RUNNING;
  int stdIn = dup(STDIN_FILENO);
  int stdOut = dup(STDOUT_FILENO);
  explicit Process(std::string path) : path(std::move(path)) {}
  bool TryStart() {
    int pid = fork();
    if (pid == -1) {
      return false;
    }
    if (pid != CHILD_PROCESS) {
      process_id = pid;
      return true;
    }
    if (stdIn != STDIN_FILENO) {
      dup2(stdIn, 0);
      close(stdIn);
    }
    if (stdOut != STDOUT_FILENO) {
      dup2(stdOut, 1);
      close(stdOut);
    }
    StartProcess(path);
    std::exit(0);
  }
  void Wait() {
    int status = 0;
    if (process_id != NOT_RUNNING)
      waitpid(process_id, &status, 0);
    process_id = NOT_RUNNING;
  }
  void Terminate() {
    if (process_id != NOT_RUNNING) {
      system("kill " + std::to_string(process_id));
    }
    process_id = NOT_RUNNING;
    CloseDescriptors();
  }
  ~Process() {
    Wait();
    CloseDescriptors();
  }
  Process& PipeTo(Process& other) {
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
  Process& operator|(Process& other) {
    return PipeTo(other);
  }
 private:
  void CloseDescriptors() const {
    if (stdIn != STDIN_FILENO) {
      close(stdIn);
    }
    if (stdOut != STDOUT_FILENO) {
      close(stdOut);
    }
  }
};

int main() {
  Process m("m"), a("a"), p("p"), s("s");
  m | a | p | s;
  close(STDIN_FILENO);
  if (!m.TryStart()) { return -1; }
  if (!a.TryStart()) {
    m.Terminate();
    return -1;
  }
  if (!p.TryStart()) {
    m.Terminate();
    a.Terminate();
    return -1;
  }
  if (!s.TryStart()) {
    m.Terminate();
    a.Terminate();
    p.Terminate();
    return -1;
  }
  return 0;
}