
#ifndef USER_USER_KILLER_H_
#define USER_USER_KILLER_H_

#include <windows.h>
#include <string>
#include <vector>
#include <tlhelp32.h>
#include <iostream>

class Killer {
 public:
  const static std::string PATH_TO_KILLER;
  struct Process {
    DWORD id;
    std::string name;
    Process(DWORD handle, std::string name);
    bool operator==(const Process& rhs) const;
    bool operator!=(const Process& rhs) const;
  };
  explicit Killer(const std::vector<std::string>& names, const std::vector<DWORD>& ids, const std::vector<std::string>& namesToVar);
  const Killer& PrintAllInfo() const;
  const Killer& Kill() const;
 private:
  static bool IsRunning(DWORD id);
  static std::vector<Process> Match(const std::vector<std::string>& names,
                                    const std::vector<std::string>& names2,
                                    const std::vector<DWORD>& ids);

  template<typename T>
  static bool Contains(const std::vector<T>& v, T value) {
    return std::any_of(v.begin(), v.end(), [&value](const T& t) { return t == value; });
  }

  static std::vector<Process> GetActiveProcesses();
  std::string cmd_string_;
  std::string var_string_;
  std::vector<Process> processes_to_kill_;
};

#endif //USER_USER_KILLER_H_
