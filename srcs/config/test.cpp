#include <vector>
#include <iostream>


int main(int argc, char **argv)
{
	std::vector<std::string> tokens;
	for (int i = 1; i < argc; ++i)
		tokens.push_back(argv[i]);



	return (0);
}