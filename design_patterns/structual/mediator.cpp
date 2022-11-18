#include <iostream>

class Mediator;

class Component {
 protected:
  Mediator* mediator_;

 public:
  Component(): mediator_(nullptr) {}

  void SetMediator(Mediator* mediator) {
    mediator_ = mediator;
  }
};

class Mediator {
 public:
  virtual ~Mediator() {}
  virtual void Notify(Component*, char i) const = 0;
};

class ConcreteComponentA: public Component {
 public:
  virtual ~ConcreteComponentA() {}

  void OperatorA() {
    mediator_->Notify(this, 'A');
  }

  void OperatorB() {
    mediator_->Notify(this, 'B');
  }
};

class ConcreteComponentB: public Component {
 public:
  virtual ~ConcreteComponentB() {}

  void OperatorC() {
    mediator_->Notify(this, 'C');
  }

  void OperatorD() {
    mediator_->Notify(this, 'D');
  }
};

class ConcreteMediator: public Mediator {
 private:
  ConcreteComponentA* ca_;
  ConcreteComponentB* cb_;

 public:
  ConcreteMediator(ConcreteComponentA* ca, ConcreteComponentB* cb)
      : ca_(ca), cb_(cb) {}
  virtual ~ConcreteMediator() {}

  virtual void Notify(Component* c, char i) const {
    std::cout << "Notify '" << i << "' from " << c << std::endl;

    if (i == 'A') {
      ca_->OperatorB();
    }

    if (i == 'B') {
      cb_->OperatorC();
    }

    if (i == 'C') {
      cb_->OperatorD();
    }

    if (i == 'D') {
      // doing something
    }
  }
};

int main() {
  ConcreteComponentA ca;
  ConcreteComponentB cb;
  ConcreteMediator mediator(&ca, &cb);
  ca.SetMediator(&mediator);
  cb.SetMediator(&mediator);
  ca.OperatorA();
  return 0;
}
