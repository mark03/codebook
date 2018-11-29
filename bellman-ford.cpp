vector<int> dist(V, INF);
dist[s] = 0;
for (int i = 0; i < V - 1; i++)
  for (int u = 0; u < V; u++)
    for (int j = 0; j < AdjList[u].size(); j++) {
      pair<int, int> v = AdjList[u];
      dist[v.first] = min(dist[v.first], dist[u] + v.second);
    }