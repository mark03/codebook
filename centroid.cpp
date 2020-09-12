// General overview of centroid decomposition, not too
// detailed on the implementation of queries handling.

int n;
vector<vector<int>> adjl;

// after the decomposition it will store the
// size of the subtree in the centroid tree
vector<int> stz;
// parent in the centroid tree
vector<int> parent;
vector<bool> taken;

void preProcess(int u, int par) {
  // preprocess here subtree rooted at centroid cd
  for(int v: adjl[u]) {
    if(!taken[v] && v != par) {
      preProcess(v, u);  
    }
  }
}

ll query(int u) {
  int v = u;
  while(v != -1) {
    // handle query here for subtree rooted at centroid v
    v = par[v];
  }
}

int findCentroid(int u, int par, int tot, int &cd) {
  stz[u] = 1;
  int mxc = 0;
  for(int v: adjl[u]) {
    if(!taken[v] && v != par) {
      int ch = findCentroid(v, u, tot, cd);
      mxc = max(mxc, ch);
      stz[u] += ch;
    }
  }
  if(mxc <= tot / 2 && (tot - stz[u]) <= tot / 2) cd = u;
  return stz[u];
}

int decompose(int u, int tot) {
  int cd;
  findCentroid(u, -1, tot, cd);
  taken[cd] = true;
  preProcess(cd, -1);
  for(int v: adjl[cd]) {
    if(!taken[v]) {
      int ch = decompose(v, stz[v] > stz[cd] ?
                            tot - stz[cd] : stz[v]);
      parent[ch] = cd;
    }
  }
  stz[cd] = tot;
  return cd;
}

void init() {
  // nodes are 0-based
  parent[decomposition(0, n)] = -1;
}