#include "dijkstras.h"

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
    vector<int> path;
    
    if (distances[destination] == INF) {
        return path;  // No path exists
    }
    
    for (int v = destination; v != -1; v = previous[v]) {
        path.push_back(v);
    }
    
    reverse(path.begin(), path.end());
    return path;
}

void print_path(const vector<int>& path, int total) {
    if (path.empty()) {
        cout << "No path exists" << endl;
        return;
    }
    
    cout << "Path: ";
    for (size_t i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1) {
            cout << " ";  
        }
    }
    cout << "\nTotal cost: " << total << endl;
}
