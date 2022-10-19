#pragma once

#include <vector>
#include <utility>
#include <functional>
#include <optional>

#include "node.h"
#include "utility.h"

std::vector<bool> expr_to_vec(size_t expr);

Node build_node(const Solution& solution);


struct Transformation {
    SNode pattern;
    SNode replacement;
};

Transformation operator>>(const Node& pattern, const Node& replacement);


bool transform(SNode& node, const Transformation& transform);

std::optional<int> evaluate_node(SNode& node);

void sort_nodes(SNode& root);

void winnow_solutions(std::vector<Solution>& solutions);

