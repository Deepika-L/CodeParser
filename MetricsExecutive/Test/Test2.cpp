#include <iostream>

namespace Test {

void func ()
    {
        [this] () { cout << "This is a lambda"; } ();
    }

int main() {
	std::cout<<"This is a test";
}
}