#include "dijkstras.h"

struct Node {
    int vertex;
    int weight;
    
    Node(int v, int w) : vertex(v), weight(w) {}
    
    bool operator>(const Node& other) const {
        return weight > other.weight;
    }
};

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous) {
    int n = G.numVertices;
    vector<int> distance(n, INF);
    previous.resize(n, -1);
    vector<bool> visited(n, false);
    
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    
    pq.push(Node(source, 0));
    distance[source] = 0;
    
    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();
        
        int u = current.vertex;
        
        if (visited[u]) {
            continue;
        }
        
        visited[u] = true;
        
        for (const Edge& edge : G[u]) {
            int v = edge.dst;
            int weight = edge.weight;
            
            if (!visited[v] && distance[u] != INF && distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                previous[v] = u;
                pq.push(Node(v, distance[v]));
            }
        }
    }
    
    return distance;
}

vector<int> extract_shortest_path(const vector<int>& distances, const vector<int>& previous, int destination) {
    vector<int> path;
    
    if (distances[destination] == INF) {
        return path;
    }
    
    // 构建从目标到源的路径
    for (int at = destination; at != -1 && distances[at] != 0; at = previous[at]) {
        path.push_back(at);
    }
    
    // 添加源顶点
    for (size_t i = 0; i < distances.size(); i++) {
        if (distances[i] == 0) {
            path.push_back(i);
            break;
        }
    }
    
    reverse(path.begin(), path.end());
    return path;
}

void print_path(const vector<int>& path, int total) {
    if (path.empty()) {
        cout << endl;
        cout << "Total cost is " << total << endl;
        return;
    }
    
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i < path.size() - 1) {
            cout << " ";
        }
    }
    cout << " " << endl;
    cout << "Total cost is " << total << endl;
}