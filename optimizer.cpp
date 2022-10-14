#include <array>
#include <vector>
#include <functional>
#include <map>
#include <memory>
#include <ranges>
#include <optional>

#include "node.h"
#include "../cpppad/printing.hpp"


struct Solution{
    size_t ind;
    size_t expression;
    std::array<int, 6> numbers;
    std::array<size_t, 5> operations;
    std::string infix;
};

std::vector<bool> expr_to_vec(size_t expr){
    std::vector<bool> result;
    size_t ones{}, zeroes{};
    for(size_t i = 0; expr >> i ; ++i){
        if(expr & (1ull << i)) ones++;
        else zeroes++;
        result.push_back(expr & (1ull << i));
    }
    while(ones > zeroes++ + 1) result.push_back(false);
    return result;
}

template<typename T>
auto double_pop(T &vec) -> std::pair<typename T::value_type, typename T::value_type> {
    auto a = vec.back(); vec.pop_back();
    auto b = vec.back(); vec.pop_back();
    return std::make_pair(a, b);
}
Node build_node(Solution& solution){
    std::vector<Node> stack;
    auto number = begin(solution.numbers);
    auto operation = begin(solution.operations);    
    for(auto el: expr_to_vec(solution.expression)){
        if(el) { stack.emplace_back(*number++); continue; }

        auto [b, a] = double_pop(stack);
        switch (*operation++) {
            case 0: stack.push_back( a + b ); break; // Node::add
            case 1: stack.push_back( a - b ); break; // Node::sub
            case 2: stack.push_back( a * b ); break; // Node::mul
            case 3: stack.push_back( a / b ); break; // Node::div
        }
    }
    return stack.back();
}

enum class Iteration { carry_on, finish };

enum class Traversal { pre, in, post };



void traverse(SNode& root, const std::function<Iteration(SNode&)>& fn, Traversal order = Traversal::post){
    if(!root) return;

    if(order == Traversal::pre && fn(root) == Iteration::finish) return;
    traverse(root->left, fn, order);
    if(order == Traversal::in && fn(root) == Iteration::finish) return;
    traverse(root->right, fn, order);
    if(order == Traversal::post && fn(root) == Iteration::finish) return;
}

void sync_traverse(SNode& one, SNode& two, std::function<Iteration(SNode&, SNode&)> fn, Traversal order = Traversal::post){
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

struct Transformation {
    SNode pattern;
    SNode replace;
};

Transformation operator>>(const Node& pattern, const Node& replace){
    return {std::make_shared<Node>(pattern), std::make_shared<Node>(replace)};
}

bool transform(SNode& node, const Transformation& transform){
    auto [pattern, replace] = transform;
    struct Match {
        SNode matched_node;
        std::map<int, SNode> captures;
    };
    std::vector<Match> matches;
    traverse(node, [&pattern, &matches](SNode& nd){
        if(*nd == *pattern) matches.emplace_back(nd);
        return Iteration::carry_on;
    });

    for(auto& [matched, captures]: matches){
        sync_traverse(pattern, matched, [&captures](SNode& pattern, SNode& matched){
            if(pattern->type == Node::glob_var || pattern->type == Node::glob_num)
                captures.insert({pattern->value, matched});
            return Iteration::carry_on;
        });

        auto new_node = std::make_shared<Node>(*replace);
        deep_copy(replace, new_node);

        traverse(new_node, [&captures](SNode& nd){
            if(nd->type == Node::glob_var || nd->type == Node::glob_num){
                if(captures.contains(nd->value)) nd = captures.at(nd->value);
                else if(captures.contains(-nd->value)) {
                    nd = captures.at(-nd->value);
                    nd->value = -nd->value;
                }
            }
            return Iteration::carry_on;
        });
        *matched = *new_node;
    }
    return !matches.empty();
}

std::optional<int> evaluate_node(SNode& node){
    int result;
    try{
        traverse(node, [&result](SNode& nd){
            switch (nd->type) {
                case Node::num:      return Iteration::carry_on;
                case Node::glob_var: throw "Trying to evaluate [glob_var] node\n";
                case Node::glob_num: throw "Trying to evaluate [glob_num] node\n";
            }
            if(!nd->left or !nd->right) throw "Trying to evalute node with nullptr branches\n";
            switch (nd->type) {
                case Node::add:  nd->value = nd->left->value + nd->right->value; break;
                case Node::sub:  nd->value = nd->left->value - nd->right->value; break;
                case Node::mul:  nd->value = nd->left->value * nd->right->value; break;
                case Node::div:  nd->value = nd->left->value / nd->right->value; break;
            }
            result = nd->value;
            return Iteration::carry_on;
        });
    } catch (...) {
        print("(evaluate function): Caught an exception");
        return std::nullopt;
    }
    return result;
}

void sort_nodes(SNode& root){
    using std::prev, std::next, std::advance, std::ranges::find;
    Node::Type expression_types[] = {Node::add, Node::mul};
    for(auto expression_type: expression_types){
        traverse(root, [&expression_type](SNode& node){
            std::vector<SNode> nodes;
            traverse(node, [&nodes, &expression_type](SNode& nd){
                if(nd->type != expression_type) return Iteration::finish;
                // children nodes substitute parent node in place
                // otherwise vector <-> tree equivalence will be broken
                if(auto it = find(nodes, nd); it != end(nodes)){
                    *it = nd->left;
                    nodes.insert(next(it), nd->right);
                } else {
                    nodes.push_back(nd->left);
                    nodes.push_back(nd->right);
                }
                return Iteration::carry_on;
            }, Traversal::pre);

            auto cmp = [](const SNode& n1, const SNode& n2){ return n1->value < n2->value; };

            for(auto nd = begin(nodes); nd != end(nodes); ++nd)
                for(auto curr = nd; 
                    curr != begin(nodes) and cmp(*prev(curr), *curr);
                    std::iter_swap(*prev(curr), *curr), advance(curr, -1)
                );
            return Iteration::carry_on;
        });
    }
}

int main(){
    auto solutions = std::to_array<Solution>({
        {68,  0b1001100111, {10, 100, 8, 5, 5, 75}, {1, 2, 3, 1, 1}, ""}, // "(((100-8)*10)-(5/5))-75"
        {40,  0b110100111,  {10, 100, 8, 75, 5, 5}, {1, 2, 1, 3, 1}, ""}, // "(((100-8)*10)-75)-(5/5)"
        {56,  0b11100111,   {10, 100, 8, 75, 5, 5}, {1, 2, 3, 0, 1}, ""}, // "((100-8)*10)-(75+(5/5))"
        {80,  0b1010001111, {8, 7, 6, 100, 50, 6},  {0, 2, 2, 1, 3}, ""},
        {81,  0b1010001111, {7, 8, 6, 100, 50, 6},  {0, 2, 2, 1, 3}, ""},
        {112, 0b1010011011, {6, 100, 7, 8, 50, 6},  {0, 2, 2, 1, 3}, ""}
    });

    Node a = Node('a');
    Node b = Node('b');
    Node _1 = Node(-1);
    Node lit = Node(Node::glob_num);

    Transformation all_transformations[] = {
         a - b        >> a + ( _1 * b),
        _1 * ( a * b) >> (_1 * a) * b,
        _1 * ( a / b) >> (_1 * a) / b,
        _1 * ( a + b) >> (_1 * a) + (_1 * b),
        _1 * lit      >> -lit
    };

    using std::ranges::take_view;
    // for(auto sol: take_view{solutions, 1}){
    for(auto sol: solutions){
        SNode node = std::make_shared<Node>(build_node(sol));
        auto node_before = node->to_string();

        while(true){
            auto transformed = 0;
            for(const auto& transformation: all_transformations)
                transformed += transform(node, transformation);
            if(!transformed) break;
        }
        evaluate_node(node);
        sort_nodes(node);
        print("{: <30} -> {: <30}", node_before, *node);
    }
    return 0;
}