#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <type_traits>
#include <memory>
#include <tuple>
#include <functional>

void F1Malloc(float** f, int i, double d) {
	*f = new float(10);
	std::cout << "F1 Malloc !! " << **f << ":" << i << ":" << d << std::endl;
}

void F1Free(float* f) {

	std::cout << "F1 Free! " << std::endl;

	delete f;
}

bool F2Malloc(int** i) {
	*i = new int(20);
	std::cout << "F2 Malloc !! " << **i << std::endl;
	return true;
}

bool F2Free(int* i) {
	std::cout << "F2 Free! " << std::endl;
	delete i;
	return true;
}

int F3Malloc(double** d, int i, float f) {
	*d = new double(0.01);
	std::cout << "F3 Malloc !! " << **d << std::endl;
	return 0;
}

int F3Free(double* d) {
	delete d;
	std::cout << "F3 Free!" << std::endl;
	return 0;
}

void F4Malloc(void** ptr, int size) {
	*ptr = new int(size);
	std::cout << "F4 Malloc !! " << **reinterpret_cast<int**>(ptr) << std::endl;
}

void F4Free(void* ptr) {
	delete ptr;
	std::cout << "F4 Free !! " << std::endl;
}

template<class T, class _T = T>
struct RAII {

	template<class mr, class... margs>
	struct malloc {
		template<class fr, class... fargs>
		struct free {

			template<mr(*MALLOC)(T**, margs...), fr(*FREE)(T*, fargs...)>
			struct factory {
				static std::function<std::shared_ptr<_T>(fargs...)> make_shared(margs... __m_args) {
					return [&](fargs&&... __f_args) -> std::shared_ptr<_T> {

						struct __deleter__ {
							using pointer = _T*;
							std::tuple<fargs...> delete_args;
							explicit __deleter__(fargs... __args) : delete_args(std::make_tuple(std::forward<fargs>(__args)...)) {}
							void operator()(pointer p) const {
								auto args = std::tuple_cat(std::tuple<T*>(reinterpret_cast<T*>(p)), delete_args);
								std::apply(FREE, args);
							}
						};

						[[maybe_unused]] _T* obj;
						MALLOC(reinterpret_cast<T**>(&obj), std::forward<margs>(__m_args)...);
						return std::shared_ptr<_T>(obj, __deleter__(__f_args...));
					};
				}
			};

		};
	};

};

using F1Make = typename RAII<float>::malloc<void, int, double>::free<void>::factory<F1Malloc, F1Free>;
using F2Make = typename RAII<int>::malloc<bool>::free<bool>::factory<F2Malloc, F2Free>;
using F3Make = typename RAII<double>::malloc<int, int, float>::free<int>::factory<F3Malloc, F3Free>;
using F4Make = typename RAII<void, int>::malloc<void, int>::free<void>::factory<F4Malloc, F4Free>;


int main() {
	auto shared_value_1 = F1Make::make_shared(1, 0.24)();
	auto shared_value_2 = F2Make::make_shared()();
	auto shared_value_3 = F3Make::make_shared(3, 0.2)();
	auto shared_value_4 = F4Make::make_shared(4)();

	std::cout << "shared_value_1 = " << *shared_value_1 << std::endl;
	std::cout << "shared_value_2 = " << *shared_value_2 << std::endl;
	std::cout << "shared_value_3 = " << *shared_value_3 << std::endl;
	std::cout << "shared_value_4 = " << *shared_value_4 << std::endl;

  return 0;
}