#include <array>
#include <vector>
#include <functional>
#include <map>
#include <memory>
#include <ranges>
#include <optional>

#include "node.h"
#include "printing.hpp"
#include "optimizer.h"
#include "utility.h"

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

Node build_node(const Solution& solution){
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
        else{
            if(nd->is_commutative()){
                nd->left.swap(nd->right);
                if (*nd == *pattern) matches.emplace_back(nd);
                else nd->left.swap(nd->right);
            }

        }
        return Iteration::carry_on;
    });

    for(auto& [matched, captures]: matches){
        sync_traverse(pattern, matched, [&captures](SNode& pattern, SNode& matched){
            if(pattern->type == Node::glob_var || pattern->type == Node::glob_num)
                captures.insert({pattern->value, matched});
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
                default:;
            }
            if(!nd->left or !nd->right) throw "Trying to evalute node with nullptr branches\n";
            switch (nd->type) {
                case Node::add:  nd->value = nd->left->value + nd->right->value; break;
                case Node::sub:  nd->value = nd->left->value - nd->right->value; break;
                case Node::mul:  nd->value = nd->left->value * nd->right->value; break;
                case Node::div:  nd->value = nd->left->value / nd->right->value; break;
                default:;
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

void winnow_solutions(std::vector<Solution>& solutions){
    Node a = Node('a');
    Node b = Node('b');
    Node _1 = Node(-1);
    Node lit = Node(Node::glob_num);

    Transformation all_transformations[] = {
         a - b        >> a + ( _1 * b),
        _1 * (a * b)      >> (_1 * a) * b,
        _1 * (a / b)      >> (_1 * a) / b,
        _1 * (a + b)      >> (_1 * a) + (_1 * b),
            _1 * lit      >> -lit,
            1 * a         >> a
    };
    for(auto& sol: solutions){
        SNode node = std::make_shared<Node>(build_node(sol));

        while(true){
            auto transformed = 0;
            for(const auto& transformation: all_transformations){
                sort_nodes(node);
                transformed += transform(node, transformation);
            }
            if(!transformed) break;
        }
        evaluate_node(node);
        sort_nodes(node);
        sol.infix = node->to_string();
    }
    std::ranges::sort(solutions, {}, &Solution::infix);
    const auto [first, last] = std::ranges::unique(solutions, {}, &Solution::infix);
    solutions.erase(first, last);
    for(auto& sol: solutions)
        sol.infix = build_node(sol).to_string();
}
