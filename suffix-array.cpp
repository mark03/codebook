// Suffix array implementation in O(nlogn).
// Includes implementation of lcp array in O(n).
struct SuffixArray {
  int n;
  string T;
  vector<int> SA, tempSA, RA, tempRA, L, lcp;

  SuffixArray(const string &s): n(s.size() + 1), T(s),
      SA(n), tempSA(n), RA(n), tempRA(n), lcp(n) {
    T.push_back('$'); // text must end with $
  }
  
  inline int getRA(int i) {
    return (i < n) ? RA[i] : 0;
  }

  void radix_sort(int k) {
    int mx = max(n, 256);
    L.assign(mx, 0);
    for(int i = 0; i < n; ++i) L[getRA(i + k)]++;
    for(int i = 0, s = 0; i < mx; ++i) {
      int x = L[i];
      L[i] = s;
      s += x;
    }
    for(int i = 0; i < n; ++i) {
      int& x = L[getRA(SA[i] + k)];
      tempSA[x++] = SA[i];
    }
    for(int i = 0; i < n; ++i) SA[i] = tempSA[i];
  }

  // call this to build suffix array, it will be stored in SA
  // first position in SA is always occupied by string "$"
  void buildSA() {
    for(int i = 0; i < n; ++i) {
      SA[i] = i;
      RA[i] = T[i];
    }
    for(int k = 1; k < n; k <<= 1) {
      radix_sort(k);
      radix_sort(0);
      tempRA[SA[0]] = 0;
      for(int i = 1, r = 0; i < n; ++i) {
        if(getRA(SA[i - 1]) != getRA(SA[i])) r++;
        else if(getRA(SA[i - 1] + k) != getRA(SA[i] + k)) r++;
        tempRA[SA[i]] = r;
      }
      for(int i = 0; i < n; ++i) RA[i] = tempRA[i];
      if(RA[SA[n - 1]] == n - 1) break;
    }
  }

  // call this to build lcp array, it will be stored in lcp
  // make sure suffix array is already built
  void buildLCP() { 
    for(int i = 0, k = 0; i < n; ++i) { 
      if(RA[i] == n - 1) { 
        k = 0; 
        continue; 
      }

      int j = SA[RA[i] + 1]; 
      while(i + k < n && j + k < n && T[i + k] == T[j + k]) ++k; 
      lcp[RA[i]] = k; 
      if(k > 0) --k; 
    }
  }  
};