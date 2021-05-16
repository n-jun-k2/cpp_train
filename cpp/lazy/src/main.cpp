#include <iostream>
#include <functional>
#include <stdexcept>

struct Runtime {
  Runtime() {
    std::cout << "constructor" << std::endl;
  }
  ~Runtime() {
    std::cout << "destroy" << std::endl;
  }

  Runtime(const Runtime&) {
    std::cout << "copy constructor" << std::endl;
  }

  Runtime(Runtime&&) {
    std::cout << "move constructor" << std::endl;
  }

  Runtime& operator=(const Runtime&){
    std::cout << "copy operator" << std::endl;
    return *this;
  }

  Runtime& operator=(Runtime&&){
    std::cout << "move operator" << std::endl;
    return *this;
  }

  Runtime& Then(std::function<void()> action) {
    return *this;
  }
  Runtime& Catch(std::function<void()> action) {
    return *this;
  }

  Runtime& Triger(){

  }
};

struct Lazy {
  std::function<void()> pFunc = []{};
  void operator+=(std::function<void()> action) {
    pFunc = [prev = pFunc, next = action] { prev(); next(); };
  }
  void operator()() {
    try {
      pFunc();
    }catch (std::exception) {

    }
  }
};

Runtime& createRuntime() {
  return Runtime().Then([]{}).Catch([]{});
}

int main() {

  // Lazy functor;

  // functor += []{
  //   std::cout << "Hello function 1" << std::endl;
  // };
  // functor += []{
  //   std::cout << "Hello function 2" << std::endl;
  // };

  // functor();

  // auto _ = Runtime().Then([]{}).Catch([]{});
  std::cout << "<<<<< part 1 <<<<<" << std::endl;
  createRuntime();

  std::cout << "<<<<< part 2 <<<<<" << std::endl;
  Runtime().Then([]{}).Catch([]{});

  std::cout << "<<<<< part 3 <<<<<" << std::endl;
  auto _ = Runtime().Then([]{}).Catch([]{});

  return 0;
}