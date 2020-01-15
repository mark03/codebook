/*
Searches for the string p in the string t.
Updated to find all occurences of p.
*/

void buildTable(string &p, vector<int> &b) {
  b.resize(p.size());
  int i = 0, j = -1;
  b[0] = -1;

  while (i < p.size()) {
    while(j >= 0 && p[i] != p[j]) j = b[j];
    ++i; ++j;
    b[i] = j;
  }
}

void KMP(string &t, string &p) {
  vector<int> b;
  buildTable(p, b);
  int i = 0, j = 0;
  while(i < t.size()) {
    while(j >=0 && t[i] != p[j]) j = b[j];
    ++i; ++j;
    if(j == p.size()) {
      // handle match here: p found in t starting at i - j
      j = b[j];
    }
  }
}
