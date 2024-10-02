#pragma once
#include "Dictionary.hpp"
#include <memory>
#include <map>

namespace wordly {
    class Game {
    private:
        std::shared_ptr<Dictionary> dictionary_;
        std::string word_;
        std::map<char, size_t> char_count_;
        size_t attempts_;

    public:
        explicit Game(const std::shared_ptr<Dictionary>& dictionary);

        void reset();

        std::string guess(const std::string& guess);

    };

}