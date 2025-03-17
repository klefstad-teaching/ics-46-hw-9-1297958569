#include "dijkstras.h"
#include <set>
#include <queue>
#include <algorithm>

struct Node {
    int vertex;
    int distance;
    
    Node(int v, int d) : vertex(v), distance(d) {}
    
    bool operator>(const Node& other) const {
        return distance > other.distance;
    }
};

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous) {
    int n = G.numVertices;
    vector<int> distances(n, INF);
    previous.resize(n, -1);
    vector<bool> visited(n, false);
    
    // Priority queue for vertices
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    
    // Initialize source
    distances[source] = 0;
    pq.push(Node(source, 0));
    
    while (!pq.empty()) {
        int u = pq.top().vertex;
        pq.pop();
        
        if (visited[u]) continue;
        
        visited[u] = true;
        
        // Check all adjacent vertices
        for (const Edge& edge : G[u]) {
            int v = edge.dst;
            int weight = edge.weight;
            
            if (!visited[v] && distances[u] != INF && 
                distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                previous[v] = u;
                pq.push(Node(v, distances[v]));
            }
        }
    }
    
    return distances;
}

vector<int> extract_shortest_path(const vector<int>& distances, const vector<int>& previous, int destination) {
    // Check for invalid destination
    if (destination < 0 || destination >= distances.size()) {
        return vector<int>();
    }
    
    // Check for unreachable destination
    if (distances[destination] == INF) {
        return vector<int>();
    }

    vector<int> path;
    // Check for cycles or invalid previous pointers
    set<int> visited;
    
    for (int v = destination; v != -1; v = previous[v]) {
        // Check for cycles
        if (visited.count(v)) {
            return vector<int>();  // Cycle detected
        }
        visited.insert(v);
        
        // Check for invalid previous index
        if (v >= previous.size()) {
            return vector<int>();
        }
        
        path.push_back(v);
    }
    
    reverse(path.begin(), path.end());
    return path;
}

void print_path(const vector<int>& path, int total) {
    if (total == INF) {
        // Only print "No path exists" when no valid cost is found
        cout << "No path exists\n";
        return;
    }

    if (!path.empty()) {
        // Print the path first, ensuring a space before newline
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i] << " ";  // Always add a space after each number
        }
        cout << "\n";  // Ensure newline after path
    }

    // Print total cost on a new line
    cout << "Total cost is " << total << "\n";
}
