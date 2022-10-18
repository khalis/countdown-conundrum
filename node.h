#pragma once

#include <memory>
#include <string>
#include <format>
#include <functional>

constexpr char alphabet[] = "abcdefghijklmnopqrstuvwxyz";

struct Node {
    enum Type {num, add, sub, mul, div, glob_var, glob_num};

    Type type;
    int value;

    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    Node(int value);
    Node(char name);
    Node(Type type);
    Node(Type type, const Node& left,const Node& right);
    Node(Type type, Node&& left, Node&& right);
    
    std::string to_string() const;
    bool is_operator() const;
    bool operator==(const Node& rhs) const;
    Node operator-();

};

Node operator+(const Node& a, const Node& b);
Node operator-(const Node& a, const Node& b);
Node operator*(const Node& a, const Node& b);
Node operator/(const Node& a, const Node& b);


using SNode = std::shared_ptr<Node>;

template<>
struct std::formatter<Node> : std::formatter<std::string> {
    template<class FormatContext>
    auto format( Node node, FormatContext& fc) const {
        return std::formatter<std::string>::format(node.to_string(), fc);
    }
};

enum class Iteration { carry_on, finish };
enum class Traversal { pre, in, post };

void traverse(SNode& root, const std::function<Iteration(SNode&)>& fn, Traversal order = Traversal::post);
void sync_traverse(SNode& one, SNode& two, std::function<void(SNode&, SNode&)> fn);
void deep_copy(SNode& from, SNode& to);






