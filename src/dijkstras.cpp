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
    // If there's no path to destination (distance is INF), return empty path
    if (distances[destination] == INF) {
        return vector<int>();
    }
    
    // Create vector to store the path
    vector<int> path;
    int current = destination;
    
    // Keep going backwards from destination until we hit source (where previous[current] == -1)
    while (current != -1) {
        path.push_back(current);  // Add current vertex to path
        current = previous[current];  // Move to the previous vertex
    }
    
    // Since we built the path backwards (from destination to source),
    // we need to reverse it to get source to destination
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
        // Print the path first
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i < path.size() - 1) {
                cout << " ";
            }
        }
        cout << " \n";  // Space before newline to match expected output
    } else {
        // If there's no path printed but a valid cost exists, add a newline before "Total cost"
        cout << "\n";
    }

    // Print total cost
    cout << "Total cost is " << total << "\n";
}
