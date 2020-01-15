// Eulerian path/circuit in an undirected graph.
// TODO: Does this handle self-edges?
// Reduced complexity to O(n)
// Edges in adjl are destroyed once used
struct EulerianPath {
  struct edge {
    int u, v;
    bool active;
  };
  int n;
  vector<vector<int>> adjl;
  vector<edge> edges;
  vector<int> circuit;

  // Call this to construct the graph.
  EulerianPath(int n) : n(n), adjl(n) {}

  // Edges are zero-based and undirected (only add each edge once!)
  void add_edge(int x, int y) {
    adjl[x].push_back(edges.size());
    adjl[y].push_back(edges.size());
    edges.push_back({x, y, true});
  }

  void find_path(int node){
    for(int i = 0; i < adjl[node].size(); ++i) {
      int e = adjl[node][i];
      swap(adjl[node][i], adjl[node].back());
      adjl[node].pop_back();
      --i;
      if(edges[e].active) {
        edges[e].active = false;
        int adj;
        if(edges[e].u == node) adj = edges[e].v;
        else adj = edges[e].u;
        find_path(adj);
      }
    }
    circuit.push_back(node);
  }
  
  // IMPORTANT: This assumes eulerian path/circuit exists
  // Preemptively count the number of odd degree nodes to
  // determine if a solution exists (0 -> circuit, 2 -> path)
  // Call this to find the path/circuit (autodetects)
  // Returns the path/circuit itself in "circuit" variable
  // Initial node is repeated at end if it's a circuit.
  void eul_path(){
    int start = -1;
    for(int i = 0; i < n; ++i)
      if(adjl[i].size() & 1)
        start = i;
    if(start == -1) start = 0;
    find_path(start);
  }
};