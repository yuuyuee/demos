
void LoadConfig();
void CreateShareMemory();
void AssignShareMemory();
void CreateWorkers();
void WaitWrokersReady();
void EventLoop();

#define PRIMARY_THREAD 1
#define SECONDARY_THREAD 2

int GetThreadFlags();

// can not creating multiple processes with boot many process, because
// the number of processes is configured in configuration.

int main(int argc, char* argv[]) {

  int flag = GetThreadFlags();

  LoadConfig();
  if (flag == PRIMARY_THREAD) {
    CreateShareMemory();
    CreateWorkers();
    WaitWrokersReady();
    EventLoop();
  } else {
    AssignShareMemory();
  }


  return 0;
}
