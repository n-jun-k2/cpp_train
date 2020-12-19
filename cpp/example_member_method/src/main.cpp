#include "../include/sum.h"
#include <iostream>

class [[nodiscard]]  Test {
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

	Test& func()& {
		std::cout << "lvalue" << std::endl;
		return *this;
	}

	Test& func()&& {
		std::cout << "rvalue" << std::endl;
		return *this;
	}
};

int main() {

	Test x;

	x = Test(); // lvalue
	// Test() = x;  error

	x.func(); //lvalue

	Test()
		.func();// rvalue


	return 0;
}