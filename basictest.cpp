#include "uniqueptr.hpp"

#include <utility>
#include <iostream>
#include <string>
#include <cassert>

class Base {
    private:
        int *array;
    public:
        Base() : array{new int[10]} { }
        ~Base() {
            delete [] this->array;
        }

};

class Derived : public Base {
    private:
        int *another_array;
    public:
        Derived() : another_array{new int[20]} { }
        ~Derived() {
            delete [] this->another_array;
        }
};

class Person {
    private:
        std::string name;
        int age;
    public:
        Person(std::string n, int a)
            : name{n},
            age{a}
        { }

        void say_hi() const {
            std::cout << "Hello my name is " << this->name
                      << " and I am " << this->age << " years old\n";
        }
};


// instantiate all member functions
template class cs540::UniquePtr<Base>;

int main() {
    cs540::UniquePtr<Base> uptr{new Derived};
    cs540::UniquePtr<Base> uptr2{std::move(uptr)};
    assert(uptr2 != nullptr);
    uptr = std::move(uptr2);


    // shouldn't compile:
    // cs540::UniquePtr<Base> badptr1{uptr}; // copy construct
    //
    // shouldn't compile:
    // cs540::UniquePtr<Base> badptr2;
    // badptr2 = uptr; // copy assign

    cs540::UniquePtr<Base> uptr3{new Derived};
    uptr3.reset(new Base);
    uptr3.reset(new Derived);
    uptr3.reset(nullptr);
    assert(uptr3 == nullptr);


    // * and ->
    cs540::UniquePtr<Person> personp{new Person{"John", 30}};
    (*personp).say_hi();
    personp->say_hi();

    cs540::UniquePtr<int> ip{new int{}};
    *ip = 99;
    std::cout << *ip << '\n'; // should print 99

    return 0;
}
