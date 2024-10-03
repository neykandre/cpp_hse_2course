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
    return stmt;
}