#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <tuple>
#include <queue>
#include "vertex.h"
#include "graphics.h"

class Graph {
private:
    std::vector<std::vector<int>> graph_;
    vertex source;
    vertex destination;
    std::vector<vertex> walls;
    Graphics graphics_;
    inline static std::vector<std::vector<int>> directions{{0, -1}, {0, 1}, {-1, 0}, {1, 0}}; //"left", "right", "up", "down"

    void randomize();
    void reset_graph();
    void draw_path(const std::vector<std::vector<int>>& path);
    bool is_move_valid(const vertex& current, const std::vector<int>& direction);
    bool dfs_recursion(std::vector<std::vector<int>>& path, const vertex& current, bool& found_route);

public:
    Graph();
    Graph(Graphics);
    std::vector<std::vector<int>> graph() const;
    void new_graph();
    void clear_graph();
    std::tuple<int, std::vector<std::vector<int>>> dfs();
    std::tuple<int, std::vector<std::vector<int>>> bfs();
    static constexpr int GRAPH_EDGE{50};
    enum class cell_values{unvisited = 0, wall = 1, source = 2, destination = 3, visited = 4, path = 5};
};

#endif // GRAPH_H
