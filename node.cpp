#include <functional>
#include <string>
#include <format>
#include <cassert>

#include "node.h"
// #include "printing.hpp"


Node::Node(int value): 
    type(Node::num),
    value(value),
    left(nullptr),
    right(nullptr)
{
}

Node::Node(char name): 
    type(Node::glob_var),
    value((name - alphabet[0] + 1) % 26),
    left(nullptr),
    right(nullptr)
{
}

Node::Node(Type type): 
    type(type),
    value(type == Node::glob_num? 1 : 0),
    left(nullptr),
    right(nullptr)
{
}

Node::Node(Type type, const Node& left, const Node& right):
    type(type),
    value(0),
    left(std::make_shared<Node>(left)),
    right(std::make_shared<Node>(right))
{
}

Node::Node(Type type, Node&& left, Node&& right):
    type(type),
    value(0),
    left(new Node(left)),
    right(new Node(right))
{
    // print("Node using rvalue constructor");
}


std::string Node::to_string() const {
    using namespace std::literals;
    using std::format;
    assert(this && "Node::to_string(), with a nullptr _this_");
    auto str = std::invoke([&](){
        switch (type) {
            case Node::num : return std::to_string(value);
            case Node::add : return "+"s; 
            case Node::sub : return "-"s; 
            case Node::mul : return "*"s; 
            case Node::div : return "/"s; 
            case Node::glob_num :
                return format("{}#", value < 0 ? "-" : "");
            case Node::glob_var :
                auto temp = value < 0 ? -value : value;
                auto name = alphabet[temp-1];
                return format("{}{}", value < 0 ? "-" : "", name);
        }
        return ""s;
    });
    auto child_with_parenthesis = [&](const Node* nd){
        if(!nd) return ""s;
        if(!nd->is_operator()) return nd->to_string();
        if(nd->type == this->type && (nd->type == Node::add || nd->type == Node::mul)) return nd->to_string();
        else return format("({})", nd->to_string());
    };
    return child_with_parenthesis(left.get()) + str + child_with_parenthesis(right.get());
}

bool Node::is_operator() const {
    assert(this && "Node::is_operator() called with nullptr _this_");
    return type == Node::add
        || type == Node::sub
        || type == Node::mul
        || type == Node::div;
}

bool Node::operator== (const Node& rhs) const {
    assert(this && "Node::operator==() called with nullptr _this_");
    return  rhs.type == Node::glob_var
        || (rhs.type == Node::glob_num && type == Node::num)
        || (type == rhs.type
            && left && rhs.left && *left == *rhs.left
            && right && rhs.right && *right == *rhs.right)
        || (type == Node::num && rhs.type == Node::num && value == rhs.value);
}

Node Node::operator-(){
    assert(this && "Node::operator-() called with nullptr _this_");
    this->value = -value; 
    return *this;
}

void traverse(SNode& root, const std::function<Iteration(SNode&)>& fn, Traversal order){
    if(!root) return;

    if(order == Traversal::pre && fn(root) == Iteration::finish) return;
    traverse(root->left, fn, order);
    if(order == Traversal::in && fn(root) == Iteration::finish) return;
    traverse(root->right, fn, order);
    if(order == Traversal::post && fn(root) == Iteration::finish) return;
}

void sync_traverse(SNode& one, SNode& two, std::function<Iteration(SNode&, SNode&)> fn, Traversal order){
    if(!one || !two) return;

    if(order == Traversal::pre && fn(one, two) == Iteration::finish) return;
    sync_traverse(one->left, two->left, fn, order);
    if(order == Traversal::in && fn(one, two) == Iteration::finish) return;
    sync_traverse(one->right, two->right, fn, order);
    if(order == Traversal::post && fn(one, two) == Iteration::finish) return;
}

void deep_copy(SNode& from, SNode& to){
    if(!from) return;
    to = std::make_shared<Node>(*from);
    deep_copy(from->left, to->left);
    deep_copy(from->right, to->right);
}

Node operator+(const Node& a, const Node& b){ return Node( Node::add, a, b ); }
Node operator-(const Node& a, const Node& b){ return Node( Node::sub, a, b ); }
Node operator*(const Node& a, const Node& b){ return Node( Node::mul, a, b ); }
Node operator/(const Node& a, const Node& b){ return Node( Node::div, a, b ); }
