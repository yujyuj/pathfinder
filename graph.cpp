#include "graph.h"
#include <QRandomGenerator>
#include <QTime>

/**
 * Default constructor
*/
Graph::Graph() {}


/**
 * Parameterized constructor
*/
Graph::Graph(Graphics graphics) : graphics_{graphics}{
    graph_ = std::vector<std::vector<int>>(GRAPH_EDGE, std::vector<int>(GRAPH_EDGE, static_cast<int>(cell_values::unvisited)));
    randomize();
}


/**
 * Generate random x and y values for source, destination, walls, and the number of walls.
 * x and y range [0, 50), number of walls range [0, 199)
*/
void Graph::randomize() {
    walls.clear();

    // generate source, destination and number of walls
    QVector<quint32> vector;
    vector.resize(5);
    QRandomGenerator::global()->fillRange(vector.data(), vector.size());
    int num_walls = static_cast<int>(vector[0]%200);
    source = vertex{static_cast<int>(vector[1]%50), static_cast<int>(vector[2]%50)};
    destination = vertex{static_cast<int>(vector[3]%50), static_cast<int>(vector[4]%50)};

    // fills walls
    vector.resize(num_walls*2);
    QRandomGenerator::global()->fillRange(vector.data(), vector.size());
    for (int i=0; i<num_walls*2-1; i++) {
        int x = vector[i]%50;
        int y = vector[i+1]%50;
        if ( (x != source.x || y != source.y) && (x != destination.x || y != destination.y)) // add the wall when cell is not source and not destination
            walls.push_back(vertex{x, y});
    }
}


/**
 * Reset the graph using the same source, destination and walls
*/
void Graph::reset_graph() {
    graph_ = std::vector<std::vector<int>>(GRAPH_EDGE, std::vector<int>(GRAPH_EDGE, static_cast<int>(cell_values::unvisited)));
    graph_[source.x][source.y] = static_cast<int>(cell_values::source);
    graph_[destination.x][destination.y] = static_cast<int>(cell_values::destination);
    for (const auto& wall:walls) {
        graph_[wall.x][wall.y] = static_cast<int>(cell_values::wall);
    }
}


/**
 * Generate a new graph with new source, destination and walls
*/
void Graph::new_graph() {
    randomize();
    reset_graph();
    graphics_.update();
}


/**
 * Generate a new graph with new source, destination and walls
*/
void Graph::clear_graph() {
    reset_graph();
    graphics_.update();
}


/**
 * Getter
*/
std::vector<std::vector<int>> Graph::graph() const {
    return graph_;
}


/**
 * A valid move: unvisited && not a wall && not out of bound
*/
bool Graph::is_move_valid(const vertex& current, const std::vector<int>& direction) {
    int row = current.x + direction[0];
    int col = current.y + direction[1];
    return ( row >= 0 && row < GRAPH_EDGE && col >= 0 && col < GRAPH_EDGE && (graph_[row][col] == static_cast<int>(cell_values::unvisited) || graph_[row][col] == static_cast<int>(cell_values::destination)) );
}


/**
 * Recursively traverse the graph. Add vertex to the path only when true is returned.
*/
bool Graph::dfs_recursion(std::vector<std::vector<int>>& path, const vertex& current, bool& found_route) {
    if (current.x == destination.x && current.y == destination.y) {
        found_route = true;
        return true;
    }

    for (const auto& direction : directions) {
        if (!found_route && is_move_valid(current, direction)) {
            vertex new_vertex{current.x + direction[0], current.y + direction[1]};
            if (new_vertex.x != destination.x || new_vertex.y != destination.y) {
                graph_[new_vertex.x][new_vertex.y] = static_cast<int>(cell_values::visited);
                graphics_.update();
            }
            if (dfs_recursion(path, new_vertex, found_route)) {
                path.push_back({new_vertex.x, new_vertex.y});
            }
        }
    }
    return found_route;
}


/**
 * Call dfs_recursion to perform depth first search
*/
std::tuple<int, std::vector<std::vector<int>>> Graph::dfs() {
    clear_graph();

    bool found_route{false};
    std::vector<std::vector<int>> path;
    dfs_recursion(path, source, found_route);

    // Generate distance and path
    int distance;
    if (!found_route) {
        distance = -1;
        path.push_back({-1, -1});
    } else {
        distance = path.size(); // path includes destination, not includes source
    }

    draw_path(path); // path includes destination, not includes source
    return {distance, path};
}


/**
 * Check each directin to see if it's a valid move
 * If valid and it's destination --> break
 * If valid and not destination --> push it to queue
*/
std::tuple<int, std::vector<std::vector<int>>> Graph::bfs() {
    clear_graph();

    bool found_route{false};
    std::vector<std::vector<vertex>> parents(GRAPH_EDGE, std::vector<vertex>(GRAPH_EDGE));
    std::queue<vertex> q;
    q.push({source.x, source.y, 0});
    parents[source.x][source.y] = source;
    vertex current_vertex;

    while (!found_route && !q.empty()) {
        current_vertex = q.front(); q.pop();
        for (const auto& direction : directions) {
            if (is_move_valid(current_vertex, direction)) {
                vertex new_vertex{current_vertex.x + direction[0], current_vertex.y + direction[1], current_vertex.distance + 1};
                parents[new_vertex.x][new_vertex.y] = current_vertex;
                if (new_vertex.x == destination.x && new_vertex.y == destination.y) { // check if the valid move is the destination
                    found_route = true;
                    break;
                } else {
                    q.push(new_vertex);
                    graph_[new_vertex.x][new_vertex.y] = static_cast<int>(cell_values::visited);
                }
                graphics_.update();
            }
        }
    }

    // Generate distance and path
    int distance;
    std::vector<std::vector<int>> path;
    if (!found_route) {
        distance = -1;
        path.push_back({-1, -1});
    } else {
        int current_row = destination.x;
        int current_col = destination.y;

        path.push_back({destination.x, destination.y});
        while (current_row != source.x || current_col != source.y) {
            vertex parent = parents[current_row][current_col];
            path.push_back({parent.x, parent.y});
            current_row = parent.x;
            current_col = parent.y;
        }
        distance = path.size() - 1; // path includes both destination and source
    }

    draw_path(path); // path includes both destination and source
    return {distance, path};
}


/**
 * Change graph cell values to path.
*/
void Graph::draw_path(const std::vector<std::vector<int>>& path) {
    if (path[0][1] == -1)
        return;

    for (unsigned long long i = 0; i < path.size(); i++) {
        graph_[path[i][0]][path[i][1]] = static_cast<int>(cell_values::path);
    }

    // change source and destination back to their own color
    graph_[destination.x][destination.y] = static_cast<int>(cell_values::destination);
    graph_[source.x][source.y] = static_cast<int>(cell_values::source);
    graphics_.update();
}
