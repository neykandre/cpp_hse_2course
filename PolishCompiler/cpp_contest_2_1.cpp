#include <cassert>
#include <iostream>
#include <list>
#include <memory>
#include <regex>
#include <string_view>
#include <vector>

class Statement {
public:
    virtual std::vector<int> apply(std::vector<int> in) const = 0;

    Statement(unsigned arguments, unsigned results, bool pure): arguments(arguments), results(results), pure(pure) {}

    virtual ~Statement() = default;

    bool is_pure() const {
        return pure;
    }

    unsigned get_arguments_count() const {
        return arguments;
    }

    unsigned get_results_count() const {
        return results;
    }
protected:
    unsigned arguments;
    unsigned results;
    bool pure;
};

std::shared_ptr<Statement> operator|(std::shared_ptr<Statement> lhs, std::shared_ptr<Statement> rhs);
std::shared_ptr<Statement> compile(std::string_view str);
std::shared_ptr<Statement> optimize(std::shared_ptr<Statement> stmt);

class ConstOp : public Statement {
    int v;
public:
    explicit ConstOp(int v);

    [[nodiscard]] std::vector<int> apply(std::vector<int> in) const override;
};

class InputOp : public Statement {
public:
    InputOp();

    [[nodiscard]] std::vector<int> apply(std::vector<int> in) const override;
};

class DupOp : public Statement {
public:
    DupOp();

    [[nodiscard]] std::vector<int> apply(std::vector<int> in) const override;
};

class AbsOp : public Statement {
public:
    AbsOp();

    [[nodiscard]] std::vector<int> apply(std::vector<int> in) const override;
};

template<auto func>
class BinaryOp : public Statement {
public:
    BinaryOp()
            : Statement(2, 1, true) {};

    [[nodiscard]] std::vector<int> apply(std::vector<int> in) const override {
//        if (in.size() < 2) {
//            std::cout << "BinaryOp Error: not enough arguments" << std::endl;
//            exit(1);
//        }
        int result = func(in[in.size() - 2], in.back());
        in.pop_back();
        in.pop_back();
        in.push_back(result);
        return in;
    }
};

class Combine : public Statement {
    std::list<std::shared_ptr<Statement>> conveyor;

public:
    using iterator = std::list<std::shared_ptr<Statement>>::iterator;

    Combine();

    void add(std::shared_ptr<Statement> statement);

    [[nodiscard]] std::vector<int> apply(std::vector<int> in) const override;

    iterator begin();

    iterator end();

};

#ifdef TEST
int main() {
    std::vector<int> stack = {1, 2, 3};
    auto test1 = compile("1 2 3 + -111 - * 10 %");
    test1 = optimize(test1);
    auto sixs = test1 | test1 | compile("6");
    sixs = sixs | compile("dup");
    for (auto i: sixs->apply(stack)) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    assert(sixs->apply(stack) == std::vector<int>({1, 2, 3, 6, 6, 6, 6}));

    auto test2 = compile("-");
    assert(test2->apply(stack) == std::vector<int>({1, -1}));

    return 0;
}
#endif

ConstOp::ConstOp(int v)
        : Statement(0, 1, true), v{v} {};

std::vector<int> ConstOp::apply(std::vector<int> in) const {
    in.push_back(v);
    return in;
}

InputOp::InputOp()
        : Statement(0, 1, false) {};

std::vector<int> InputOp::apply(std::vector<int> in) const {
    int result;
    std::cin >> result;
//    if (std::cin.fail()) {                                    // I don't know why this doesn't work properly in the testing system
//        std::cout << "Error: invalid input" << std::endl;
//        exit(1);
//    }
    in.push_back(result);
    return in;
}

DupOp::DupOp()
        : Statement(1, 2, true) {};

std::vector<int> DupOp::apply(std::vector<int> in) const {
//    if (in.empty()) {
//        std::cout << "Duplicate Error: not enough arguments" << std::endl;
//        exit(1);
//    }
    in.push_back(in.back());
    return in;
}

AbsOp::AbsOp()
        : Statement(1, 1, true) {};

std::vector<int> AbsOp::apply(std::vector<int> in) const {
//    if (in.empty()) {
//        std::cout << "Absolute Error: not enough arguments" << std::endl;
//        exit(1);
//    }
    in.back() = std::abs(in.back());
    return in;
}

Combine::Combine()
        : Statement(0, 0, true), conveyor{} {};

void Combine::add(std::shared_ptr<Statement> statement) {
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

std::vector<int> Combine::apply(std::vector<int> in) const {
    for (auto& i: conveyor) {
        in = i->apply(std::move(in));
    }
    return in;
}

Combine::iterator Combine::begin() {
    return conveyor.begin();
}

Combine::iterator Combine::end() {
    return conveyor.end();
}

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
            exit(1);
        }
    }
    return combine;
}

std::shared_ptr<Statement> operator|(std::shared_ptr<Statement> lhs, std::shared_ptr<Statement> rhs) {
    auto result_combine = std::make_shared<Combine>();
    result_combine->add(lhs);
    result_combine->add(rhs);

    return result_combine;
}

void expand(std::shared_ptr<Statement> stmt, std::shared_ptr<Combine> result) {
    auto stmt_combine = dynamic_pointer_cast<Combine>(stmt);
    if (!stmt_combine) {
        result->add(stmt);
        return;
    }

    for (auto& i: *stmt_combine) {
        expand(i, result);
    }   
}

std::shared_ptr<Statement> optimize(std::shared_ptr<Statement> stmt) {
    auto stmt_combine = std::make_shared<Combine>();

    expand(stmt, stmt_combine);

    std::vector<int> my_stack;
    auto result_combine = std::make_shared<Combine>();

    for (auto& i: *stmt_combine) {
        if (i->is_pure() && i->get_arguments_count() <= my_stack.size()) {
            my_stack = i->apply(std::move(my_stack));
        }
        else {
            for (auto j: my_stack) {
                result_combine->add(std::make_shared<ConstOp>(j));
            }
            my_stack.clear();
            result_combine->add(i);
        }
    }

    for (auto j: my_stack) {
        result_combine->add(std::make_shared<ConstOp>(j));
    }

    return result_combine;
}