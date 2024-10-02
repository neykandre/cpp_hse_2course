#include "../include/Dictionary.hpp"
#include "../include/Parser.hpp"

namespace wordly {
    Dictionary::Dictionary(const std::string& path) noexcept {
        Parser parser{path};
        words_ = parser.get_tokens();
    }

    Dictionary::Dictionary(const std::vector<std::string>& words) noexcept
            : words_{words} {
    };

    std::string Dictionary::get_word() {
        std::random_device rd;
        std::mt19937 g(rd());
        std::uniform_int_distribution<size_t> dist(0, words_.size() - 1);
        return words_[dist(g)];
    }

    std::shared_ptr<Dictionary> Dictionary::create_from_file(const std::string& path) {
        return std::shared_ptr<Dictionary>{new Dictionary{path}};
    }

    std::shared_ptr<Dictionary> Dictionary::create_from_vector(const std::vector<std::string>& words) {
        return std::shared_ptr<Dictionary>{new Dictionary{words}};
    }

}