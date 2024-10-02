#include "../include/Parser.hpp"

wordly::Parser::Parser(const std::string& path) {
    std::ifstream file{path};

    if (!file) {
        throw NoSuchFileException{path};
    }

    std::ostringstream stream;
    stream << file.rdbuf();
    std::string text = stream.str();

    std::regex word_regex{R"(\w+)"};
    auto words_begin = std::sregex_iterator(text.begin(), text.end(), word_regex);
    auto words_end = std::sregex_iterator();

    for (auto it = words_begin; it != words_end; ++it) {
        tokens_.push_back(it->str());
    }

}

std::vector<std::string> wordly::Parser::get_tokens() const {
    return tokens_;
}

