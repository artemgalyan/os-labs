#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <csignal>
#include <sys/wait.h>

std::string GetIdRegex(int id) {
  return R"(^\\s+)" + std::to_string(id) + R"(\\s\(\\?\|[\\w\|\\/\|\\d]+\)\\s*\\d{2}:\\d{2}:\\d{2}\\s*\\w+)";
}

std::string GetNameRegex(const std::string& name) {
  return R"(^\\s+\\d+\\s\(\\?\|[\\w\|\\/\|\\d]+\)\\s*\\d{2}:\\d{2}:\\d{2}\\s*)" + name + "$";
}

char** GetArgs(const std::vector<std::string>& toKillByName, const std::vector<int>& toKillById) {
  int argc = toKillByName.size() + toKillById.size() + 2;
  char **args = new char *[argc + 1];

  args[0] = new char[sizeof("--name")];
  strcpy(args[0], "--name");
  int i = 1;
  for (auto& name: toKillByName) {
    args[i] = new char[name.length() + 1];
    strcpy(args[i], name.c_str());
    ++i;
  }
  args[i] = new char[sizeof("--id")];
  strcpy(args[i], "--id");
  i++;
  for (auto id: toKillById) {
    std::string str = std::to_string(id);
    args[i] = new char[str.length() + 1];
    strcpy(args[i], str.c_str());
    ++i;
  }
  return args;
}

const std::string KILLER_PATH = "/home/tyoma/Programming/cpp/os-labs/lab-6/linux/killer/cmake-build-debug/killer";

std::string Join(const std::vector<std::string>& vector, const std::string& delimiter) {
  std::string result;
  for (auto i = 0; i < vector.size(); ++i) {
    result += vector[i];
    if (i != vector.size() - 1) {
      result += delimiter;
    }
  }
  return result;
}

std::string Join(const std::vector<int>& vector, const std::string& delimiter) {
  std::string result;
  for (auto i = 0; i < vector.size(); ++i) {
    result += std::to_string(vector[i]);
    if (i != vector.size() - 1) {
      result += delimiter;
    }
  }
  return result;
}

void PrintActiveProcesses(const std::vector<std::string>& toKillByName,
                          const std::vector<int>& toKillById,
                          const std::vector<std::string>& toKillByEnvVar) {
  std::vector<std::string> regexes;
  std::transform(toKillByName.begin(),
                 toKillByName.end(),
                 std::back_inserter(regexes),
                 [](const std::string& s) { return GetNameRegex(s); });
  std::transform(toKillByEnvVar.begin(),
                 toKillByEnvVar.end(),
                 std::back_inserter(regexes),
                 [](const std::string& s) { return GetNameRegex(s); });
  std::transform(toKillById.begin(),
                 toKillById.end(),
                 std::back_inserter(regexes),
                 [](int n) { return GetIdRegex(n); });
  std::string command = "ps -e | grep -P " + Join(regexes, "\\|");
  std::cout << "Active processes: " << std::endl;
  system(command.c_str());
}

void KillAll(const std::vector<std::string>& toKillByName,
             const std::vector<int>& toKillById,
             const std::vector<std::string>& toKillByEnvVar) {
  std::string env_var = Join(toKillByEnvVar, ",");
  char buff[1000];
  strcpy(buff, env_var.c_str());
  setenv("PROC_TO_KILL", buff, 1);
  int pid = fork();

  if (pid == 0) {
    int argc = toKillByName.size() + toKillById.size() + 2;
    char** args = GetArgs(toKillByName, toKillById);
    execv(KILLER_PATH.c_str(), args);
    for (int i = 0; i < argc; ++i) {
      delete[] args[i];
    }
    delete[] args;
    exit(0);
  }
  else if (pid == -1) {
    std::cerr << "Unable to start child process" << std::endl;
    unsetenv("PROC_TO_KILL");
    exit(pid);
  }
  int status;
  waitpid(pid, &status, 0);
  unsetenv("PROC_TO_KILL");
}

int main() {
  std::vector<std::string> toKillByName = {"telegram-deskto"};
  std::vector<int> toKillById = {18308, 234};
  std::vector<std::string> toKillByEnvVar = {"gnome-mines", "gnome-calendar"};
  PrintActiveProcesses(toKillByName, toKillById, toKillByEnvVar);
  std::cout << "Killing..." << std::endl;
  KillAll(toKillByName, toKillById, toKillByEnvVar);
  sleep(1);
  PrintActiveProcesses(toKillByName, toKillById, toKillByEnvVar);
  return 0;
}