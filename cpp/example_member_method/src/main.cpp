#include <iostream>

class [[nodiscard]]  Test {

private:
  int i = 9;

public:

  Test() = default;

  Test(const Test&) {
    std::cout << "Copy" << std::endl;
  }

	Test& operator=(const Test&) & {
		std::cout << "lvalue" << std::endl;
		return *this;
	}

	Test& operator=(const Test&) && = delete;
	//{
	//	std::cout << "rvalue" << std::endl;
	//	return *this;
	//}

  Test(Test&&) = default;
  Test& operator=(Test&&) = default;

  operator int() const & {
		std::cout << "lvalue" << std::endl;
		return i;
	}

	operator int() const && {
		std::cout << "rvalue" << std::endl;
		return std::move(i);
	}

	Test& func()& {
		std::cout << "lvalue" << std::endl;
		return *this;
	}

	Test& func()&& {
		std::cout << "rvalue" << std::endl;
		return *this;
	}
};


void func(const int& i) {
	std::cout << "lvalue" << i << std::endl;
}

void func(int&& i) {
	int ip = std::move(i);
	std::cout << "rvalue" << ip << std::endl;
	func(ip);
}

template <typename Type>
class ImplicitCast {
  protected:
    Type value;
  public:
    explicit ImplicitCast(Type arg) : value(arg) {}
    ImplicitCast()          = delete;
    virtual ~ImplicitCast() = default;

    operator Type() const & {
      std::cout << "lvalue" << std::endl;
      return value;
    }

    operator Type() const && {
      std::cout << "rvalue" << std::endl;
      return value;
    }
};

class ResorceTest : public ImplicitCast<int> {
  public:
    ResorceTest() = delete;
    ~ResorceTest() = default;
    explicit ResorceTest(int v) : ImplicitCast(v) {}
};

int main() {

	Test x;

	x = Test(); // lvalue
	// Test() = x;  error

	x.func(); //lvalue

	Test()
		.func();// rvalue


	int p = 10;
	func(p); // lvalue
	func(5); // rvalue lvalue

	func(x); // lvalue rvalue lvalue
	func(Test()); // rvalue rvalue lvalue

  ResorceTest test(11);

  func(test); // lvalue rvalue11 lvalue11
  func(ResorceTest(12)); // rvalue rvalue12 lvalue12

	return 0;
}