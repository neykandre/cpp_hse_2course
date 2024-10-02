#include <cassert>
#include "../include/Game.hpp"
#include "../include/Symbols.hpp"

namespace wordly {
    Game::Game(const std::shared_ptr<Dictionary>& dictionary) {
        dictionary_ = dictionary;
        reset();
    }

    void Game::reset() {
        word_ = dictionary_->get_word();
        attempts_ = 0;
        char_count_.clear();
        for(auto c: word_)
            char_count_[c]++;
    }

    std::string Game::guess(const std::string& guess) {
        assert(!word_.empty());
        ++attempts_;

        std::string result;
        result.resize(guess.size());
        std::map<char, size_t> char_count = char_count_;

        for (size_t i = 0; i < guess.size(); ++i) {
            if (guess[i] == word_[i]) {
                result[i] = static_cast<char>(Symbol::match);
                char_count[guess[i]]--;
            }
        }
    }
}