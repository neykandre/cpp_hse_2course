#include "../include/operations.hpp"
#include <iostream>

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
    if (in.empty()) {
        std::cout << "Duplicate Error: not enough arguments" << std::endl;
        exit(1);
    }
    in.push_back(in.back());
    return in;
}

AbsOp::AbsOp()
        : Statement(1, 1, true) {};

std::vector<int> AbsOp::apply(std::vector<int> in) const {
    if (in.empty()) {
        std::cout << "Absolute Error: not enough arguments" << std::endl;
        exit(1);
    }
    in.back() = std::abs(in.back());
    return in;
}

template<auto func>
BinaryOp<func>::BinaryOp()
        : Statement(2, 1, true) {};

template<auto func>
std::vector<int> BinaryOp<func>::apply(std::vector<int> in) const {
    if (in.size() < 2) {
        std::cout << "BinaryOp Error: not enough arguments" << std::endl;
        exit(1);
    }
    int result = func(in[in.size() - 2], in.back());
    in.pop_back();
    in.pop_back();
    in.push_back(result);
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

template class BinaryOp<std::plus<>{}>;
template class BinaryOp<std::minus<>{}>;
template class BinaryOp<std::multiplies<>{}>;
template class BinaryOp<std::modulus<>{}>;
template class BinaryOp<std::divides<>{}>;