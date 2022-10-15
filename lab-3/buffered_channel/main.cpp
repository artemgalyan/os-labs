#include <iostream>
#include "buffered_channel.h"

int main() {
  const int CHANNEL_SIZE = 10;
  BufferedChannel<int> channel(CHANNEL_SIZE);
  for (int i = 0; i < CHANNEL_SIZE; ++i)
    channel.Send(i);
  std::thread thread([&channel](){
    std::this_thread::sleep_for(std::chrono::seconds(2));
    channel.Send(100);
    std::cout << "Value is sent" << std::endl;
    channel.Close();
  });
  for (int i = 0; i < 12; ++i) {
    std::pair<int, bool> value = channel.Receive();
    std::cout << value.first << " " << (int)value.second << std::endl;
  }
  thread.join();

  return 0;
}
