#include <iostream>
#include <string>

// 用一个中介者来封装一系列的对象的交互（通讯协议)
// 使各个对象不直接互相引用，使其松耦合，并且可以独立的改变它们的交互

class BaseComponent;

class Mediator {
public:
  virtual ~Mediator() {}

  virtual void Notify(BaseComponent* sender, const std::string& event) = 0;
};

class BaseComponent {
public:
  BaseComponent(): mediator_(nullptr) {}
  BaseComponent(Mediator* mediator): mediator_(mediator) {}

  void SetMediator(Mediator* mediator) { mediator_ = mediator; }

protected:
  Mediator* mediator_;
};

class ComponentA: public BaseComponent {
public:
  void OperationA() {
    std::cout << "CompoentA::OperationA" << std::endl;
    mediator_->Notify(this, "ComponentA::OperationA");
  }

  void OperationC() {
    std::cout << "CompoentA::OperationC" << std::endl;
  }
};

class ComponentB: public BaseComponent {
public:
  virtual ~ComponentB() {}

  void OperationB() {
    std::cout << "ComponentB::OperationB" << std::endl;
    mediator_->Notify(this, "ComponentB::OperationB");
  }
};

class ConcreteMediator: public Mediator {
public:
  ConcreteMediator(ComponentA* ca, ComponentB* cb): ca_(ca), cb_(cb) {
    ca_->SetMediator(this);
    cb_->SetMediator(this);
  }
  virtual ~ConcreteMediator() {}

  virtual void Notify(BaseComponent*, const std::string& event) {
    if (event == "ComponentA::OperationA") {
      cb_->OperationB();
    }

    if (event == "ComponentB::OperationB") {
      ca_->OperationC();
    }
  }

  ComponentA* ca_;
  ComponentB* cb_;
};

int main() {
  ComponentA ca;
  ComponentB cb;
  ConcreteMediator mediator(&ca, &cb);
  ca.OperationA();
  return 0;
}
