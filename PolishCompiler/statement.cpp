#include "statement.h"
#include <iostream>
#include <utility>
#include <functional>
#include <regex>
#include <string_view>
#include <cassert>

class ConstOp : public Statement {
    int v;
public:
    explicit ConstOp(int v)
            : Statement(0, 1, true), v{v} {};

    [[nodiscard]] std::vector<int> apply(std::vector<int> in) const override {
        in.push_back(v);
        return in;
    }
};

class InputOp : public Statement {
public:
    InputOp()
            : Statement(0, 1, false) {};

    [[nodiscard]] std::vector<int> apply(std::vector<int> in) const override {
        int result;
        std::cin >> result;
        if (!std::cin) {
            std::cout << "Error: invalid input" << std::endl;
            exit(0);
        }
        in.push_back(result);
        return in;
    }
};

class DupOp : public Statement {
public:
    DupOp()
            : Statement(1, 2, true) {};

    [[nodiscard]] std::vector<int> apply(std::vector<int> in) const override {
        if (in.empty()) {
            std::cout << "Duplicate Error: not enough arguments" << std::endl;
            exit(0);
        }
        in.push_back(in.back());
        return in;
    }
};

class AbsOp : public Statement {
public:
    AbsOp()
            : Statement(1, 1, true) {};

    [[nodiscard]] std::vector<int> apply(std::vector<int> in) const override {
        if (in.empty()) {
            std::cout << "Absolute Error: not enough arguments" << std::endl;
            exit(0);
        }
        in.back() = std::abs(in.back());
        return in;
    }
};

template<auto func>
class BinaryOp : public Statement {
public:
    BinaryOp()
            : Statement(2, 1, true) {};

    [[nodiscard]] std::vector<int> apply(std::vector<int> in) const override {
        if (in.size() < 2) {
            std::cout << "BinaryOp Error: not enough arguments" << std::endl;
            exit(0);
        }
        int result = func(in[in.size() - 2], in.back());
        in.pop_back();
        in.pop_back();
        in.push_back(result);
        return in;
    }
};

class Combine : public Statement {
    std::vector<std::shared_ptr<Statement>> conveyor;
public:
    Combine()
            : Statement(0, 0, true), conveyor{} {}

    void add(std::shared_ptr<Statement> statement) {
        if (statement->get_arguments_count() > results) {
            arguments += statement->get_arguments_count() - results;
            results = statement->get_results_count();
        } else {
            results = statement->get_results_count() + results - statement->get_arguments_count();
        }

        if (!statement->is_pure()) {
            pure = false;
        }

        conveyor.emplace_back(std::move(statement));
    }

    friend std::shared_ptr<Statement> operator|(std::shared_ptr<Statement> lhs, std::shared_ptr<Statement> rhs);

    [[nodiscard]] std::vector<int> apply(std::vector<int> in) const override {
        for (auto& i: conveyor) {
            in = i->apply(std::move(in));
        }
        return in;
    }

};

std::shared_ptr<Statement> compile(std::string_view str) {
    std::regex token_regex(R"(\S+)");
    std::regex number_regex(R"([-+]?\d+)");

    auto tokens_begin = std::regex_iterator<std::string_view::iterator>{str.begin(), str.end(), token_regex};
    auto tokens_end = std::regex_iterator<std::string_view::iterator>{};

    auto combine = std::make_shared<Combine>();
    for (auto it = tokens_begin; it != tokens_end; ++it) {
        if (it->str() == "input") {
            combine->add(std::make_shared<InputOp>());
        } else if (it->str() == "dup") {
            combine->add(std::make_shared<DupOp>());
        } else if (it->str() == "+") {
            combine->add(std::make_shared<BinaryOp<std::plus<>{}>>());
        } else if (it->str() == "-") {
            combine->add(std::make_shared<BinaryOp<std::minus<>{}>>());
        } else if (it->str() == "*") {
            combine->add(std::make_shared<BinaryOp<std::multiplies<>{}>>());
        } else if (it->str() == "%") {
            combine->add(std::make_shared<BinaryOp<std::modulus<>{}>>());
        } else if (it->str() == "/") {
            combine->add(std::make_shared<BinaryOp<std::divides<>{}>>());
        } else if (it->str() == "abs") {
            combine->add(std::make_shared<AbsOp>());
        } else if (std::regex_match(it->str(), number_regex)) {
            combine->add(std::make_shared<ConstOp>(std::stoi(it->str())));
        } else {
            std::cout << "Compile error: " << it->str() << "unknown token\n" << std::endl;
            exit(0);
        }
    }
    return combine;
}

std::shared_ptr<Statement> operator|(std::shared_ptr<Statement> lhs, std::shared_ptr<Statement> rhs) {
    auto result_combine = std::make_shared<Combine>();
    auto lhs_combine = dynamic_cast<Combine*>(lhs.get());
    auto rhs_combine = dynamic_cast<Combine*>(rhs.get());

    if (lhs_combine == nullptr) {
        result_combine->add(lhs);
    } else {
        for (auto& i: lhs_combine->conveyor) {
            result_combine->add(i);
        }
    }

    if (rhs_combine == nullptr) {
        result_combine->add(rhs);
    } else {
        for (auto& i: rhs_combine->conveyor) {
            result_combine->add(i);
        }
    }

    return result_combine;
}

std::shared_ptr<Statement> optimize(std::shared_ptr<Statement> stmt) {
    
}

int main() {
    std::vector<int> stack = {1, 2, 3};
    auto test1 = compile("1 2 3 + -111 - * 10 %");
    auto sixs = test1 | test1 | compile("6");
    sixs = sixs | compile("dup");
    for (auto i: sixs->apply(stack)) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    assert(sixs->apply(stack) == std::vector<int>({1, 2, 3, 6, 6, 6, 6}));

    auto test2 = compile("-");
    assert(test2->apply(stack) == std::vector<int>({1, -1}));
}