#include <iostream>

class Receiver {
 public:
  void Operation(int a, int b, int c) {
    // The Receiver classes contain some important business logic.
    // They know how to perform all kinds of operations, associated
    // with carrying out a request.
    std::cout << "Receiver.Operation("
              << a << ", " << b << ", " << c << ")" << std::endl;
  }
};

// The Command interface declares a method for executing a command.
class Command {
 public:
  virtual ~Command() {}

  virtual void Execute() = 0;
};

class ConcreteCommand: public Command {
 public:
  ConcreteCommand(Receiver* receiver, int a, int b, int c)
    : receiver_(receiver), a_(a), b_(b), c_(c) {}
  virtual ~ConcreteCommand() {}

  // Delegate to methods of a receiver.
  virtual void Execute() {
    receiver_->Operation(a_, b_, c_);
  }

 private:
  Receiver* receiver_;
  int a_, b_, c_;
};

class Invoker {
 public:
  Invoker(): command_(nullptr) {}
  ~Invoker() {
    if (command_)
      delete command_;
  }

  void SetCommand(Command* command) {
    command_ = command;
  }

  void ExecuteCommand() {
    if (command_)
      command_->Execute();
  }

 private:
  Command* command_;
};

int main() {
  Receiver receiver;
  Invoker invoker;
  invoker.SetCommand(new ConcreteCommand(&receiver, 1, 2, 3));
  invoker.ExecuteCommand();
  return 0;
}
