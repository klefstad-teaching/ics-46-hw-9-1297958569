#include "dijkstras.h"

int main() {
    Graph G;
    try {
        file_to_graph("small.txt", G);
    } catch (const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
    
    vector<int> previous;
    vector<int> distances = dijkstra_shortest_path(G, 0, previous);
    
    // 打印从源（0）到所有顶点的最短路径
    for (int i = 0; i < G.numVertices; i++) {
        cout << "\nShortest path from 0 to " << i << ":" << endl;
        vector<int> path = extract_shortest_path(distances, previous, i);
        print_path(path, distances[i]);
    }
    
    return 0;
}
