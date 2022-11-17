#include <iostream>

class ConcreteElementA;
class ConcreteElementB;

class Visitor {
 public:
  virtual ~Visitor() {}

  virtual void Visit(const ConcreteElementA*) const = 0;
  virtual void Visit(const ConcreteElementB*) const = 0;
};

class ConcreteVisitor: public Visitor {
 public:
  virtual ~ConcreteVisitor() {}

  virtual void Visit(const ConcreteElementA*) const {
    std::cout << "ConcreteVisitor.Visit(ConcreteElementA)" << std::endl;
  }

  virtual void Visit(const ConcreteElementB*) const {
    std::cout << "ConcreteVisitor.Visit(ConcreteElementB)" << std::endl;
  }
};

class Element {
 public:
  virtual ~Element() {}

  virtual void Accept(const Visitor* visitor) const = 0;
};

class ConcreteElementA: public Element {
 public:
  virtual ~ConcreteElementA() {}

  virtual void Accept(const Visitor* visitor) const {
    visitor->Visit(this);
  }
};

class ConcreteElementB: public Element {
 public:
  virtual ~ConcreteElementB() {}

  virtual void Accept(const Visitor* visitor) const {
    visitor->Visit(this);
  }
};

int main() {
  Visitor* visitor = new ConcreteVisitor;
  Element* ea = new ConcreteElementA;
  Element* eb = new ConcreteElementB;

  ea->Accept(visitor);
  eb->Accept(visitor);

  delete visitor;
  delete ea;
  delete eb;
  return 0;
}
