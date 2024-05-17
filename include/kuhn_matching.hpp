/**
 * @file include/kuhn_matching.hpp
 * @author Samarokov Aleksei
 *
 * �������� ���� ���������� ����������� ������������� � ���������� �����.
 */


#ifndef KUHN_MATCHING_HPP
#define KUHN_MATCHING_HPP
#include <iostream>
#include "graph.hpp"
#include <vector>
#include <unordered_map>
#include <set>

namespace graph {

int n, k;
std::unordered_map<size_t, std::unordered_set<size_t>> g;
std::vector<size_t> mt;
std::vector<char> used;

bool HasCommonElement(const std::unordered_set<size_t>& set1,
  const std::unordered_set<size_t>& set2) {
  for (int element : set1) {
    if (set2.count(element) > 0) {
      return true;
    }
  }
  return false;
}

bool try_kuhn(size_t v) {
  if (used[v])  return false;
  used[v] = true;
  for (const auto& value : g[v]) {
    size_t to = value;
    if (mt[to] == -1 || try_kuhn(mt[to])) {
      mt[to] = v;
      return true;
    }
  }
  return false;
}

template<typename GraphType>
size_t GraphMaxId(GraphType& graph) {
  size_t maxVertex = *(graph.Vertices().begin());
  for (size_t id : graph.Vertices()) {
    if (maxVertex < id)
      maxVertex = id;
  }
  return maxVertex;
}

template<typename GraphType>
void KuhnMatching(GraphType& graph,
std::vector<std::pair<size_t, size_t>>* resEdges) {
  std::unordered_set<size_t> minVert = {};
  std::vector<std::pair<size_t, size_t>> edgesHelp;
  std::unordered_map<size_t, std::unordered_set<size_t>> firstPart;
  std::unordered_map<size_t, std::unordered_set<size_t>> secondPart;
  std::vector<size_t> helper;
  std::vector<char> used1(GraphMaxId(graph));

  for (size_t id : graph.Vertices()) {
    if (graph.Edges(id).empty() == true
      && graph.IncomingEdges(id).empty() == true) {
      helper.push_back(id);
    }
  }

  for (size_t id : helper) {
    graph.RemoveVertex(id);
  }

  for (size_t id : graph.Vertices()) {
    if (HasCommonElement(minVert, graph.Edges(id)) == false) {
      firstPart.insert(std::pair<size_t,
      std::unordered_set<size_t>>(id, graph.Edges(id)));
      minVert.insert(id);
    }
    else {
      secondPart.insert(std::pair<size_t,
      std::unordered_set<size_t>>(id, graph.Edges(id)));
    }
  }

  n = firstPart.size();
  k = secondPart.size();
  for (const auto& [i, value] : firstPart) {
    g.insert(std::pair<size_t,
    std::unordered_set<size_t>>(i, value));
  }

  mt.assign(GraphMaxId(graph), -1);
  for (const auto& [i, value] : firstPart)
    for (const auto& value : firstPart[i])
      if (mt[value] == -1) {
        mt[value] = i;
        used1[i] = true;
        break;
      }

  for (const auto& [i, value] : firstPart) {
    if (used1[i])  continue;
    used.assign(GraphMaxId(graph), false);
    try_kuhn(i);
  }

  for (const auto& [i, value] : secondPart)
    if (mt[i] != -1) {
      edgesHelp.push_back({ mt[i],i });
    }

  *resEdges = edgesHelp;

  }

}
#endif // KUHN_MATCHING_HPP