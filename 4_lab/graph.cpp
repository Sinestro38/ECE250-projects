#include "graph.h"
#include <sstream>
#include <fstream>
#include <queue>
#include <algorithm>

bool Graph::edge_exists(int a, int b) {
    bool ab_exists = g.find(a) != g.end() && g[a].find(b) != g[a].end();
    bool ba_exists = g.find(b) != g.end() && g[b].find(a) != g[b].end();
    return ab_exists && ba_exists;
}

bool Graph::node_exists(int a) {
    return g.find(a) != g.end();
}

void Graph::insert(int a, int b, double d, double s, bool silence){
    g[a][b].d = d;
    g[a][b].s = s;

    g[b][a].d = d;
    g[b][a].s = s;

    if (!silence)
        cout << "success" << endl;
}

void Graph::delete_n(int a) {
    if (this->node_exists(a)) {
        // delete the vertex a and any edges containing a
        for (auto i : g[a]) {
            g[i.first].erase(a);
        }
        g.erase(a);

        cout << "success" << endl;
        return;
    }
    cout << "failure" << endl;
}

void Graph::traffic(int a, int b, double aprime, bool silence) {
    if (!this->edge_exists(a, b)) {
        if (!silence)
            cout << "failure" << endl;
        return;
    }

    g[a][b].a = aprime;
    g[b][a].a = aprime;
    if (!silence)
        cout << "success" << endl;
}

void Graph::print(int a) {
    if (!this->node_exists(a)) {
        cout << "failure" << endl;
        return;
    }

    for (auto i: g[a]) {
        cout << i.first << " ";
    }

    cout << endl;
}

void Graph::update(const string& filename) {
    // update traffic data between all certices in the file
    // Traffice data will be given in the same format as the traffice command.
    // This operation is successful only if at least one edge is updated in
    // the graph
    ifstream file{filename};
    string line;
    bool success = false;
    while (getline(file, line)) {
        istringstream iss(line);
        int a, b;
        double aprime;
        if (iss >> a >> b >> aprime) {
            if (this->edge_exists(a, b)) {
                this->traffic(a, b, aprime, true);
                success = true;
            }
        }
    }

    if (success) {
        cout << "success" << endl;
    } else {
        cout << "failure" << endl;
    }

}


void Graph::load(const string& filename) {
    ifstream file{filename};
    string line;
    // call insert
    while (getline(file, line)){
        istringstream iss(line);
        int from, to;
        double weight1, weight2;
        if (iss >> from >> to >> weight1 >> weight2) {
            this->insert(from, to, weight1, weight2, true);
        }
    }
    cout << "success" << endl;
}

void Graph::dijkstra(int source, int dest, unordered_map<int, double>& dist, unordered_map<int, int>& prev) {
    /**
     * Implements Dijkstra's algorithm to find the shortest path from a source node to all other nodes in the graph.
     * 
     * This function updates two maps: `dist` which holds the minimum distance from the source node to every other node,
     * and `prev` which stores the previous node in the optimal path from the source to a given node.
     * 
     * @param source The source node from which distances are calculated.
     * @param dist A reference to a map where the key is the node and the value is the minimum distance from the source node.
     * @param prev A reference to a map where the key is the node and the value is the previous node in the optimal path from the source.
     */
    // Initialize a priority queue to store nodes in ascending order of their distance from the source.
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    // Set the initial distance of all nodes to infinity and their previous node to -1.
    for (auto& node : g) {
        dist[node.first] = numeric_limits<double>::infinity();
        prev[node.first] = -1;
    }
    // Set the distance of the source node to itself as 0 and push it into the priority queue.
    dist[source] = 0;
    pq.push({0, source});

    // Process nodes in the priority queue until it's empty.
    while (!pq.empty()) {
        // Extract the node with the smallest distance from the priority queue.
        double current_dist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // skip if not less than the shortest path
        if (current_dist > dist[u]) {
            continue;
        }

        // stop the algorithm if the destination node is reached
        if (u == dest) {
            break;
        }

        // Iterate through all edges of the current node.
        for (const auto& edge : g[u]) {
            int v = edge.first; // The adjacent node
            // Calculate the weight of the edge using the given formula.
            double weight = edge.second.d / (edge.second.s * edge.second.a);
            // If a shorter path to the adjacent node is found, update its distance and previous node.
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                // Push the updated distance of the adjacent node into the priority queue.
                pq.push({dist[v], v});
            }
        }
    }
}

void Graph::path(int a, int b) {
    if (!this->node_exists(a) || !this->node_exists(b)) {
        cout << "failure" << endl;
        return;
    }

    if (a == b) {
        cout << "failure" << endl;
        return;
    }

    unordered_map<int, double> dist;
    unordered_map<int, int> prev;
    this->dijkstra(a, b, dist, prev);

    if (dist[b] == numeric_limits<double>::infinity()) {
        cout << "failure" << endl;
        return;
    }

    vector<int> path;
    for (int at = b; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());

    if (path.size() == 1 && path[0] == a) {
        cout << "failure" << endl;
        return;
    }

    for (int i : path) {
        cout << i << " ";
    }
    cout << endl;
}

void Graph::lowest(int a, int b) {
    if (!this->node_exists(a) || !this->node_exists(b)) {
        cout << "failure" << endl;
        return;
    }

    unordered_map<int, double> dist;
    unordered_map<int, int> prev;
    this->dijkstra(a, b, dist, prev);

    if (dist[b] == numeric_limits<double>::infinity()) {
        cout << "failure" << endl;
    } else {
    //after getting the lowest_cost  you can use the following code to round-off the lowest_cost before printing .
        std::cout << std::fixed << std::showpoint;
        std::cout << std::setprecision(3);
        cout << dist[b] << endl;
    }
}
