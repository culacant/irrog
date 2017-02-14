#include <iostream>

int main()
{
	for( int y = 0; y < 10;y++)
	{
		for( int x = 0; x < 10;x++)
		{
			std::cout << x+y*10 << "   ";
		}
		std::cout << std::endl;
	}
	return 0;
}
