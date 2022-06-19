#include <iostream>

// 表示一个作用于对象中各元素的操作
// 可以在不改变各元素的类的前提下定义作用于这些元素的新操作
// 将一系列固定的类的可变操作抽象为单独的Visitor，方便添加新的操作
// 与迭代器不同，Visitor可以对不同类集合进行遍历

class ConcreteElementA;
class ConcreteElementB;

class Visitor {
public:
  virtual ~Visitor() {}

  virtual void VisitConcreteElementA(ConcreteElementA* elem) = 0;
  virtual void VisitConcreteElementB(ConcreteElementB* elem) = 0;
};

class Element {
public:
  virtual ~Element() {}

  virtual void Accept(Visitor* visitor) = 0;
};

class ConcreteElementA: public Element {
public:
  virtual ~ConcreteElementA() {}

  void OperationA() { std::cout << "ConcreteElementA::OperationA" << std::endl; }
  virtual void Accept(Visitor* visitor) {
    visitor->VisitConcreteElementA(this);
  }
};

class ConcreteElementB: public Element {
public:
  virtual ~ConcreteElementB() {}

  void OperationB() { std::cout << "ConcreteElementA::OperationB" << std::endl; }
  virtual void Accept(Visitor* visitor) {
    visitor->VisitConcreteElementB(this);
  }
};

class ConcreteVisitor: public Visitor {
public:
  virtual ~ConcreteVisitor() {}

  virtual void VisitConcreteElementA(ConcreteElementA* elem) {
    elem->OperationA();
  }

  virtual void VisitConcreteElementB(ConcreteElementB* elem) {
    elem->OperationB();
  }
};

int main() {
  Visitor* visitor = new ConcreteVisitor;
  Element* element = new ConcreteElementA;
  element->Accept(visitor);
  delete element;
  element = new ConcreteElementB;
  element->Accept(visitor);
  delete element;
  return 0;
}
