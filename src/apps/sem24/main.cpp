#include <string>
#include <iostream>
#include <type_traits>
#include <utility>

template<typename T>
void f(T&& x) {
    std::cout << "is ref: " << (std::is_reference<T>::value) << std::endl;
    std::cout << "is const: " << (std::is_const<T>::value) << std::endl;
    std::cout << "is lref: " << (std::is_lvalue_reference<T>::value) << std::endl;
    std::cout << "is rref: " << (std::is_rvalue_reference<T>::value) << std::endl;
    std::cout << typeid(T).name() << std::endl;
    std::forward<T>(x);
    if constexpr (std::is_lvalue_reference<T>::value) {
        std::cout << "value: " << x << std::endl;
    }
}

int main() {
    std::string s = "test";
    f(std::string{});  // like std::move
    f(static_cast<const std::string&>(s));  // just forward reference
    f(s);  // just forward reference
    f(std::move(s));  // like std::move
}
