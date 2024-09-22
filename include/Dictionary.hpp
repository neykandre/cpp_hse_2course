#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <random>

class NoSuchWordException : public std::runtime_error {
public:
    explicit NoSuchWordException(const std::string& path) : std::runtime_error{"No such file: " + path} {};
};

class Dictionary {
public:
    explicit Dictionary(const std::string& path)
    {
        std::ifstream file{path};
        if (!file) {
            throw NoSuchWordException{path};
        }

        std::ostringstream stream;
        stream << file.rdbuf();

        std::regex word_regex{R"(\w+)"};

        std::string word;
        auto words_begin = std::sregex_iterator(stream.str().begin(), stream.str().end(), word_regex);
        auto words_end = std::sregex_iterator();

        for (auto it = words_begin; it != words_end; ++it) {
            std::smatch match = *it;
            words_.push_back(match.str());
        }
    }

    std::string get_random_word() const {
        std::random_device rd;
        std::default_random_engine gen{rd()};
        std::uniform_int_distribution<size_t> dist{0, words_.size() - 1};
        return words_[dist(gen)];
    }

private:
    std::vector<std::string> words_;
};