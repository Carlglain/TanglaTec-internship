#include <iostream>

int main(){
	int a = 5;
	int b =9;
	int temp;
	temp=a;
	a=b;
	b=temp;
	std::cout << a;
	std::cout<< "\n";
	std::cout<< b;
	return 0;
}
