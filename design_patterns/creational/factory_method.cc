#include "product.h"

class Creator {
 public:
   virtual ~Creator() {}

   virtual ProductA* FactoryMethod() = 0;
};

class ConcreteCreator1: public Creator {
 public:
   ~ConcreteCreator1() {}

   ProductA* FactoryMethod() {
     // business logic in here
     return new ConcreteProductA1;
   }
};

class ConcreteCreator2: public Creator {
 public:
   ~ConcreteCreator2() {}

   ProductA* FactoryMethod() {
     // business logic in here
     return new ConcreteProductA2;
   }
};

void ClientCode(Creator* creator) {
  ProductA* product = creator->FactoryMethod();
  product->OperateA();
}

int main() {
  Creator* creator = new ConcreteCreator1;
  ClientCode(creator);
  delete creator;

  creator = new ConcreteCreator2;
  ClientCode(creator);
  delete creator;
  return 0;
}
