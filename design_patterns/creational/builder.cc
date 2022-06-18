#include "product.h"

class Builder {
public:
  virtual ~Builder() {}

  virtual void ProducePartA() = 0;
  virtual void ProducePartB() = 0;
};

class ConcreteBuilder final: public Builder {
public:
  ConcreteBuilder(): producta_(new ConcreteProductA1) {}
  virtual ~ConcreteBuilder() {}

  virtual void ProducePartA () override {}
  virtual void ProducePartB () override {}

  ProductA* GetProductA() { return producta_; }
private:
  ProductA* producta_;
};

class Director {
public:
  Director(): builder_(nullptr) {}

  void SetBuilder(Builder* builder) {
    builder_ = builder;
  }

  void BuildProduct() {
    builder_->ProducePartA();
    builder_->ProducePartB();
  }

private:
  Builder* builder_;
};

int main() {
  ConcreteBuilder builder;
  Director director;
  director.SetBuilder(&builder);
  director.BuildProduct();
  ProductA* product = builder.GetProductA();
  product->OperateA();
  return 0;
}
