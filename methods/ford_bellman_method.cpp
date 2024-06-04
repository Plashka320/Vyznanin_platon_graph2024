#include <nlohmann/json.hpp>
#include <weighted_oriented_graph.hpp>
#include <ford_bellman.hpp>
using graph::WeightedOrientedGraph;

namespace graph {
  template<typename GraphType>
  int FordBellmanMethodHelper(const nlohmann::json& input,
  nlohmann::json* output);
  int FordBellmanMethod(const nlohmann::json& input,
  nlohmann::json* output) {
  std::string graphType = input.at("graph_type");
  if (graphType != "WeightedOrientedGraph") {
    return -1;
  }

  std::string weightType = input.at("weight_type");
  if (weightType == "int") {
    return FordBellmanMethodHelper<int>(input, output);
  } else if (weightType == "double") {
    return FordBellmanMethodHelper<double>(input, output);
  } else {
    return -1;
    }
  }

  template<typename WeightType>
  int FordBellmanMethodHelper(const nlohmann::json& input,
  nlohmann::json* output) {
    graph::WeightedOrientedGraph<WeightType> graph;
    std::map<size_t, WeightType> res;

    for (auto& vertex : input.at("vertices")) {
      graph.AddVertex(vertex);
    }

    for (auto& edge : input.at("edges")) {
      graph.AddEdge(edge.at("from"), edge.at("to"), edge.at("weight"));
    }

    FordBellman<WeightType>(graph, &res);

    for (size_t i = 0; i < res.size(); ++i) {
      (*output)["result"].push_back(std::make_pair(i, res[i]));
    }

    return 0;
  }
}  // namespace graph
