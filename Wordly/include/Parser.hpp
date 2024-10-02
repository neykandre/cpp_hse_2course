#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <regex>

namespace wordly {
    class NoSuchFileException : public std::runtime_error {
    public:
        explicit NoSuchFileException(const std::string& path)
                : std::runtime_error{"No such file: " + path} {};
    };

    class Parser {
    public:
        explicit Parser(const std::string& path);

        [[nodiscard]] std::vector<std::string> get_tokens() const;

    private:
        std::vector<std::string> tokens_;
    };
}