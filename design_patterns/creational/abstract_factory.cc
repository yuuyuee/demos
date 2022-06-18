#include <iostream>
#include "product.h"

class AbstractFactory {
 public:
    virtual ~AbstractFactory() {}

    virtual ProductA* CreateProductA() = 0;
    virtual ProductB* CreateProductB() = 0;
};

class ConcreteFactory1: public AbstractFactory {
 public:
    ~ConcreteFactory1() {}

    ProductA* CreateProductA() {
      // business logic in here
      return new ConcreteProductA1;
    }

    ProductB* CreateProductB() {
      // business logic in here
      return new ConcreteProductB1;
    }

    static ConcreteFactory1* Instance() {
        if (!instance_)
            instance_ = new ConcreteFactory1;
        return instance_;
    }

 private:
    static ConcreteFactory1* instance_;
};

ConcreteFactory1* ConcreteFactory1::instance_ = 0;

class ConcreteFactory2: public AbstractFactory {
 public:
    ~ConcreteFactory2() {}

    ProductA* CreateProductA() {
      // business logic in here
      return new ConcreteProductA2;
    }

    ProductB* CreateProductB() {
      // business logic in here
      return new ConcreteProductB2;
    }

    static ConcreteFactory2* Instance() {
        if (!instance_)
            instance_ = new ConcreteFactory2;
        return instance_;
    }

 private:
    static ConcreteFactory2* instance_;
};

ConcreteFactory2* ConcreteFactory2::instance_ = 0;

int main(void) {
    do {
        AbstractFactory* fac1 = ConcreteFactory1::Instance();
        ProductA* pa = fac1->CreateProductA();
        ProductB* pb = fac1->CreateProductB();
        pa->OperateA();
        pb->OperateB();
    } while (0);

    do {
        AbstractFactory* fac2 = ConcreteFactory2::Instance();
        ProductA* pa = fac2->CreateProductA();
        ProductB* pb = fac2->CreateProductB();
        pa->OperateA();
        pb->OperateB();
    } while (0);

    return 0;
}
