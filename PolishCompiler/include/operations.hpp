#pragma once

#include "statement.h"
#include <iostream>
#include <list>

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
