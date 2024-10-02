#include <iostream>
#include "include/wordly.hpp"

using namespace wordly;

int main() {
    auto dict = Dictionary::create_from_file("Wordly/test_dict.txt");
    Game g{dict};

    std::cout << g.guess("hello") << std::endl;
}