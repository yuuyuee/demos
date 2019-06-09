#include <iostream>

class AbstractProductA {
public:
    AbstractProductA() {}
    virtual ~AbstractProductA() {}

    virtual void Help() const = 0;
};

class ProductA1: public AbstractProductA {
public:
    ProductA1(): AbstractProductA() {}
    ~ProductA1() {}

    void Help() const {
        std::cout << "class ProductA1: help function" << std::endl;
    }
};

class ProductA2: public AbstractProductA {
public:
    ProductA2(): AbstractProductA() {}
    ~ProductA2() {}

    void Help() const {
        std::cout << "class ProductA2: help function" << std::endl;
    }
};

class AbstractProductB {
public:
    AbstractProductB() {}
    virtual ~AbstractProductB() {}

    virtual void Help() const = 0;
};

class ProductB1: public AbstractProductB {
public:
    ProductB1(): AbstractProductB() {}
    ~ProductB1() {}

    void Help() const {
        std::cout << "class ProductB1: help function" << std::endl;
    }
};

class ProductB2: public AbstractProductB {
public:
    ProductB2(): AbstractProductB() {}
    ~ProductB2() {}

    void Help() const {
        std::cout << "class ProductB2: help function" << std::endl;
    }
};

class AbstractFactory {
public:
    AbstractFactory() {}
    virtual ~AbstractFactory() {}

    virtual AbstractProductA* CreateProductA() = 0;
    virtual AbstractProductB* CreateProductB() = 0;
};

class ConcreteFactory1: public AbstractFactory {
public:
    ~ConcreteFactory1() {}

    AbstractProductA* CreateProductA() {
        return new ProductA1;
    }

    AbstractProductB* CreateProductB() {
        return new ProductB1;
    }

    static ConcreteFactory1* Instance() {
        if (!instance_)
            instance_ = new ConcreteFactory1;
        return instance_;
    }

private:
    ConcreteFactory1() {}
    static ConcreteFactory1* instance_;
};

ConcreteFactory1* ConcreteFactory1::instance_ = 0;

class ConcreteFactory2: public AbstractFactory {
public:
    ~ConcreteFactory2() {}

    AbstractProductA* CreateProductA() {
        return new ProductA2;
    }

    AbstractProductB* CreateProductB() {
        return new ProductB2;
    }

    static ConcreteFactory2* Instance() {
        if (!instance_)
            instance_ = new ConcreteFactory2;
        return instance_;
    }

private:
    ConcreteFactory2() {}
    static ConcreteFactory2* instance_;
};

ConcreteFactory2* ConcreteFactory2::instance_ = 0;

int main(void) {
    do {
        AbstractFactory* fac1 = ConcreteFactory1::Instance();
        AbstractProductA* pa = fac1->CreateProductA();
        AbstractProductB* pb = fac1->CreateProductB();
        pa->Help();
        pb->Help();
    } while (0);

    do {
        AbstractFactory* fac2 = ConcreteFactory2::Instance();
        AbstractProductA* pa = fac2->CreateProductA();
        AbstractProductB* pb = fac2->CreateProductB();
        pa->Help();
        pb->Help();
    } while (0);

    return 0;
}
