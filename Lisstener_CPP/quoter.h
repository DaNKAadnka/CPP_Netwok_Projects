#pragma once
#include <vector>
#include <string>

class Quoter
{
public:

	Quoter(std::string path);

	std::string GetRandom();

private:

	std::vector < std::string > quotes;
};