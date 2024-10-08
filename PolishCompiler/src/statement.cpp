#include "../include/operations.hpp"
#include <regex>
#include <iostream>

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