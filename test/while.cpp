#include <iostream>

int main()
{
	int timeout = 1000;
	int itrue = 1;
	while((itrue && timeout > 0) )
	{
		timeout--;
		std::cout << timeout << std::endl;
	}
}
