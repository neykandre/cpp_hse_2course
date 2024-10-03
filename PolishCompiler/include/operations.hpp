#pragma once

#include "statement.h"

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
    BinaryOp();

    [[nodiscard]] std::vector<int> apply(std::vector<int> in) const override;
};

class Combine : public Statement {
    std::vector<std::shared_ptr<Statement>> conveyor;
public:
    Combine();

    void add(std::shared_ptr<Statement> statement);

    friend std::shared_ptr<Statement> operator|(std::shared_ptr<Statement> lhs, std::shared_ptr<Statement> rhs);

    [[nodiscard]] std::vector<int> apply(std::vector<int> in) const override;

};
