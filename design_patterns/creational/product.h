#include <iostream>

#ifndef PRODUCT_H_
#define PRODUCT_H_

class ProductA {
 public:
   virtual ~ProductA() {}

   virtual void OperateA() const = 0;
};

class ConcreteProductA1: public ProductA {
 public:

   virtual void OperateA() const {
     std::cout << "ConcreteProductA1::OperateA" << std::endl;
   }
};

class ConcreteProductA2: public ProductA {
 public:
   virtual ~ConcreteProductA2() {}

   virtual void OperateA() const {
     std::cout << "ConcreteProductA2::OperateA" << std::endl;
   }
};

class ProductB {
 public:
   virtual ~ProductB() {}

   virtual void OperateB() const = 0;
};

class ConcreteProductB1: public ProductB {
 public:
   virtual ~ConcreteProductB1() {}

   virtual void OperateB() const {
     std::cout << "ConcreteProductB1::Operate" << std::endl;
   }
};

class ConcreteProductB2: public ProductB {
 public:
   ConcreteProductB2(): ProductB() {}
   virtual ~ConcreteProductB2() {}

   virtual void OperateB() const {
     std::cout << "ConcreteProductB2::OperateB" << std::endl;
   }
};
#endif
