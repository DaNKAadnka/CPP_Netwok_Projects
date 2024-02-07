#include "quoter.h"
#include <fstream>
#include <iostream>
#include <random>

Quoter::Quoter(std::string path) {

	std::ifstream in;

	in.open(path);

	std::string buffer;
	std::vector <char> words;
	while (getline(in, buffer)) {
		for (char c : buffer) {
			words.push_back(c);
		}
	}
	in.close();

	for (int i = 0; i < words.size(); i++) {
		if (words[i] == '{') {
			i++;
			std::string current_quote = "";
			while (words[i] != '}') {
				current_quote.push_back(words[i]);
				i++;
			}
			quotes.push_back(current_quote);
		}
	}

	std::cout << "Quotes generated successfully\n";

	/*for (std::string quote : quotes) {
		std::cout << quote << "\n\n\n";
	}*/
}

std::string Quoter::GetRandom()
{
	// Generate some random number and return quotes[rand_num]

	std::mt19937 mt(45218965);

	int rand_quote_pos = mt() % quotes.size();

	return quotes[rand_quote_pos];
}