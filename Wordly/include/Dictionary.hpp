#pragma once

#include <string>
#include <vector>
#include <random>
#include <memory>

namespace wordly {
    class Dictionary {
    public:
        std::string get_word();

        static std::shared_ptr<Dictionary> create_from_file(const std::string&);

        static std::shared_ptr<Dictionary> create_from_vector(const std::vector<std::string>&);

    private:
        explicit Dictionary(const std::string&) noexcept;

        explicit Dictionary(const std::vector<std::string>&) noexcept;

        std::vector<std::string> words_;
    };
}