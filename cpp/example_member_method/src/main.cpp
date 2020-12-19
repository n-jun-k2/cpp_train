#include <iostream>

class [[nodiscard]]  Test {

private:
  int i = 9;

public:
	Test& operator=(const Test&) & {
		std::cout << "lvalue" << std::endl;
		return *this;
	}

	Test& operator=(const Test&) && = delete;
	//{
	//	std::cout << "rvalue" << std::endl;
	//	return *this;
	//}

  operator int() & {
		std::cout << "lvalue" << std::endl;
		return i;
	}

	operator int()&& {
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

	return 0;
}