#include <iostream>
class example1 {
	int x1;
	public:
		void func1() {
			std::cout<<"Hello";
		}
}

static void example2() {
	char newArray[] = { 'a', 'b', 'c' };
}

int main(int argc, char* argv[]) {
	if(true) 
	{
		std::cout << "Now in main";
	}
	else
	{
		std::cout << "Else";
		return;
	}
}
