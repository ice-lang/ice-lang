#ifndef __ANOLE_COMPILER_AST_HPP__
#define __ANOLE_COMPILER_AST_HPP__

#include "token.hpp"

#include <map>
#include <list>
#include <vector>
#include <utility>
#include <optional>

namespace anole
{
class Code;

using StmtList
    = std::list<Ptr<struct Stmt>>
;
using ExprList
    = std::list<Ptr<struct Expr>>
;
using DeclList
    = std::list<Ptr<struct DeclarationStmt>>
;
using ArgumentList
    = std::list<std::pair<Ptr<struct Expr>, bool>>
; // boolean stands for whether it is unpacked
using ParameterList
    = std::list<std::pair<Ptr<struct VariableDeclarationStmt>, bool>>
; // boolean stands for whether it is packed

/**
 * as an attribute
 *  because not every node needs locations
*/
struct with_location
{
    Location location;
};
struct with_locations
{
    std::vector<Location> locations;
};

struct AST
{
    virtual ~AST() = 0;
    virtual void codegen(Code &) = 0;

    virtual bool is_integer_expr();
    virtual bool is_expr_stmt();
};

struct Stmt : AST
{
    virtual ~Stmt() = 0;
    virtual void codegen(Code &) = 0;
};

struct Expr : AST
{
    virtual ~Expr() = 0;
    virtual void codegen(Code &) = 0;
};

struct BlockExpr : Expr
{
    StmtList statements;

    void codegen(Code &) override;
};

struct NoneExpr : Expr
{
    void codegen(Code &) override;
};

struct IntegerExpr : Expr
{
    int64_t value;

    IntegerExpr(int64_t value) : value(value) {}

    void codegen(Code &) override;
    bool is_integer_expr() override;
};

struct FloatExpr : Expr
{
    double value;

    FloatExpr(double value) : value(value) {}

    void codegen(Code &) override;
};

struct BoolExpr : Expr
{
    bool value;

    BoolExpr(bool value) : value(value) {}

    void codegen(Code &) override;
};

struct StringExpr : Expr
{
    String value;

    StringExpr(String value)
      : value(std::move(value))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct IdentifierExpr : Expr, with_location
{
    String name;

    IdentifierExpr(String name)
      : name(std::move(name))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct ParenOperatorExpr : Expr, with_location
{
    Ptr<Expr> expr;
    ArgumentList args;

    ParenOperatorExpr(Ptr<Expr> &&expr,
        ArgumentList &&args)
      : expr(std::move(expr))
      , args(std::move(args))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct UnaryOperatorExpr : Expr, with_location
{
    Token op;
    Ptr<Expr> expr;

    UnaryOperatorExpr(Token op,
        Ptr<Expr> &&expr)
      : op(op), expr(std::move(expr))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct BinaryOperatorExpr : Expr, with_location
{
    Token op;
    Ptr<Expr> lhs, rhs;

    BinaryOperatorExpr(Ptr<Expr> &&lhs,
        Token op, Ptr<Expr> &&rhs)
      : op(op), lhs(std::move(lhs))
      , rhs(std::move(rhs))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct LambdaExpr : Expr
{
    ParameterList parameters;
    Ptr<BlockExpr> block;

    LambdaExpr(ParameterList &&parameters,
        Ptr<BlockExpr> &&block)
      : parameters(std::move(parameters))
      , block(std::move(block))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct DotExpr : Expr, with_location
{
    Ptr<Expr> left;
    std::string name;

    DotExpr(Ptr<Expr> &&left, std::string name)
      : left(std::move(left))
      , name(std::move(name))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct EnumExpr : Expr
{
    std::list<struct VariableDeclarationStmt> decls;

    EnumExpr() = default;

    void codegen(Code &) override;
};

struct MatchExpr : Expr, with_locations
{
    Ptr<Expr> expr;
    std::vector<ExprList> keylists;
    std::vector<Ptr<Expr>> values;
    Ptr<Expr> else_expr;

    MatchExpr() noexcept = default;

    void codegen(Code &) override;
};

struct ListExpr : Expr, with_location
{
    ExprList exprs;

    ListExpr() noexcept = default;

    void codegen(Code &) override;
};

struct IndexExpr : Expr, with_location
{
    Ptr<Expr> expr, index;

    IndexExpr(Ptr<Expr> &&expr,
        Ptr<Expr> &&index)
      : expr(std::move(expr))
      , index(std::move(index))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct DictExpr : Expr
{
    ExprList keys, values;

    DictExpr() noexcept = default;

    void codegen(Code &) override;
};

struct ClassExpr : Expr
{
    String name;
    ArgumentList bases;
    DeclList members;

    ClassExpr(String name,
        ArgumentList &&bases,
        DeclList &&members)
      : name(std::move(name))
      , bases(std::move(bases))
      , members(std::move(members))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct DelayExpr : Expr
{
    Ptr<Expr> expr;

    DelayExpr(Ptr<Expr> &&expr)
      : expr(std::move(expr))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct QuesExpr : Expr, with_location
{
    Ptr<Expr> cond, true_expr, false_expr;

    QuesExpr(Ptr<Expr> &&cond,
        Ptr<Expr> &&true_expr,
        Ptr<Expr> &&false_expr)
      : cond(std::move(cond))
      , true_expr(std::move(true_expr))
      , false_expr(std::move(false_expr))
    {
        // ...
    }

    void codegen(Code &code) override;
};

struct UseStmt : Stmt
{
    struct Module
    {
        enum class Type
        {
            Name,
            Path,
            Null
        };
        String mod;
        Type type;
    };

    // second String is the alias
    using Alias = std::pair<Module, String>;
    using Aliases = std::list<Alias>;

    // aliases are empty means `use *`
    Aliases aliases;
    // from may be a name or a path
    Module from;

    UseStmt(Aliases &&aliases, Module from)
      : aliases(std::move(aliases))
      , from(std::move(from))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct ExprStmt : Stmt
{
    Ptr<Expr> expr;

    ExprStmt(Ptr<Expr> &&expr)
      : expr(std::move(expr))
    {
        // ...
    }

    void codegen(Code &) override;
    bool is_expr_stmt() override;
};

struct DeclarationStmt : Stmt
{
    virtual ~DeclarationStmt() = 0;
    virtual void codegen(Code &) = 0;
};

struct VariableDeclarationStmt : DeclarationStmt
{
    String name;
    Ptr<Expr> expr;
    bool is_ref;

    VariableDeclarationStmt(String name,
        Ptr<Expr> &&expr, bool is_ref = false)
      : name(std::move(name))
      , expr(std::move(expr))
      , is_ref(is_ref)
    {
        // ...
    }

    void codegen(Code &) override;
};

struct MultiVarsDeclarationStmt : DeclarationStmt
{
    // just reuse VariableDeclarationStmt
    std::list<VariableDeclarationStmt> decls;
    ExprList exprs;

    MultiVarsDeclarationStmt(
        std::list<VariableDeclarationStmt> &&decls,
        ExprList &&exprs)
      : decls(std::move(decls))
      , exprs(std::move(exprs))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct PrefixopDeclarationStmt : Stmt
{
    std::string op;

    PrefixopDeclarationStmt(std::string op)
      : op(std::move(op))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct InfixopDeclarationStmt : Stmt
{
    Size precedence;
    std::string op;

    InfixopDeclarationStmt(std::string op, Size precedence)
      : precedence(precedence), op(std::move(op))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct BreakStmt : Stmt
{
    void codegen(Code &) override;
};

struct ContinueStmt : Stmt
{
    void codegen(Code &) override;
};

struct ReturnStmt : Stmt
{
    ExprList exprs;

    ReturnStmt(ExprList &&exprs)
      : exprs(std::move(exprs))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct IfElseStmt : Stmt, with_location
{
    Ptr<Expr> cond;
    Ptr<BlockExpr> true_block;
    Ptr<AST> false_branch;

    IfElseStmt(Ptr<Expr> &&cond,
        Ptr<BlockExpr> &&true_block,
        Ptr<AST> &&false_branch)
      : cond(std::move(cond))
      , true_block(std::move(true_block))
      , false_branch(std::move(false_branch))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct WhileStmt : Stmt, with_location
{
    Ptr<Expr> cond;
    Ptr<BlockExpr> block;

    WhileStmt(Ptr<Expr> &&cond,
        Ptr<BlockExpr> &&block)
      : cond(std::move(cond))
      , block(std::move(block))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct DoWhileStmt : Stmt, with_location
{
    Ptr<Expr> cond;
    Ptr<BlockExpr> block;

    DoWhileStmt(Ptr<Expr> &&cond,
        Ptr<BlockExpr> &&block)
      : cond(std::move(cond))
      , block(std::move(block))
    {
        // ...
    }

    void codegen(Code &) override;
};

struct ForeachStmt : Stmt
{
    Ptr<Expr> expr;
    std::string varname;
    Ptr<BlockExpr> block;

    ForeachStmt(Ptr<Expr> &&expr,
        std::string varname,
        Ptr<BlockExpr> &&block)
      : expr(std::move(expr))
      , varname(std::move(varname))
      , block(std::move(block))
    {
        // ...
    }

    void codegen(Code &) override;
};
}

#endif