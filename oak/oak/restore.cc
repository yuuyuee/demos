// Copyright 2022 The Oak Authors.

void LoadConfig();
void CreateShareMemory();
void AssignShareMemory();
void CreateWorkers();
void WaitWrokersReady();
void EventLoop();

// can not creating multiple processes with boot many process, because
// the number of processes is configured in configuration.

int main(int argc, char* argv[]) {
  LoadConfig();

  CreateShareMemory();
  CreateWorkers();
  WaitWrokersReady();
  EventLoop();
  AssignShareMemory();



  return 0;
}
