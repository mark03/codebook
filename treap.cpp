/* Implicit treap implementation
 * Dynamic array that supports following operations:
 * - Inserting an element in a given position
 * - Deleting an element in a given position
 * - Reversing elements in a given interval
 * - Range queries and updates
 * - Splitting array in a given position in two subarrays
 * - Merging two arrays into a single array
 * All operations are (randomized) O(log n)
 * Warning: high costant factor, use other data structures,
 * e.g. segment tree, if treap is not strictly required
 */

using ll = long long;

const int MAXN = 1e6; // max number of nodes to be allocated
const int SEED = 42;

struct Treap { 
  struct node {
    ll val, q, lazy = 0LL;
    int pr, sz = 1;
    bool rev = false;
    node *l = nullptr, *r = nullptr;
  };

  node *root;
  mt19937 rng;
  node stock[MAXN];
  int si = 0;

  node *newnode(ll v) {
    stock[si].val = stock[si].q = v;
    stock[si].pr = rng();
    return &(stock[si++]);
  }

  Treap(int seed = SEED): root(nullptr), rng(seed) {}

  Treap(vector<ll> &v, int seed = SEED): rng(seed) {
    root = build(v, 0, v.size() - 1);
  }

  node *build(vector<ll> &v, int l, int r) {
    if(l > r) return nullptr;
    int m = (l + r) / 2;
    node *n = newnode(v[m]);
    n->l = build(v, l, m - 1);
    n->r = build(v, m + 1, r);
    fixPr(n);
    updateNode(n);
    return n;
  }

  void fixPr(node *n) {
    if(n) {
      node *t = n;
      if(n->l && n->l->pr > t->pr) t = n->l;
      if(n->r && n->r->pr > t->pr) t = n->r;
      if(n != t) {
        swap(n->pr, t->pr);
        fixPr(t);
      }
    }
  }

  inline int getSize(node *n) {
    if(n) return n->sz;
    else return 0;
  }

  inline ll query(node *n) { // adjust as needed
    if(n) return n->q + n->lazy * n->sz;
    else return 0;
  }

  inline void addLazy(node *n, ll add) {
    if(n) n->lazy += add; // adjust as needed
  }

  inline void toggleRev(node *n) {
    if(n) n->rev = !n->rev;
  }

  void pushLazy(node *n) {
    if(n) {
      if(n->lazy) {
        n->val += n->lazy;
        addLazy(n->l, n->lazy);
        addLazy(n->r, n->lazy);
        n->lazy = 0LL;
      }
      if(n->rev) {
        swap(n->l, n->r);
        toggleRev(n->l);
        toggleRev(n->r);
        n->rev = false;
      }
    }
  }

  void updateNode(node *n) {
    if(n) {
      n->sz = 1 + getSize(n->l) + getSize(n->r);
      n->q = n->val + query(n->l) + query(n->r); // adjust as needed
    }
  }

  void merge(node *&d, node *sl, node *sr) {
    if(!sl || !sr) {
      if(sl) d = sl;
      else d = sr;
    } else if(sl->pr > sr->pr) {
      pushLazy(sl);
      merge(sl->r, sl->r, sr);
      d = sl;
    } else {
      pushLazy(sr);
      merge(sr->l, sl, sr->l);
      d = sr;
    }
    updateNode(d);
  }

  // pos is the first index of the right-side tree
  void split(node *s, node *&dl, node *&dr, int pos, int add = 0) {
    pushLazy(s);
    if(!s) {
      dl = dr = nullptr;
      return;
    }
    int curPos = add + getSize(s->l);
    if(pos <= curPos) {
      split(s->l, dl, s->l, pos, add);
      dr = s;
      updateNode(dr);
    } else {
      split(s->r, s->r, dr, pos, curPos + 1);
      dl = s;
      updateNode(dl);
    }
  }

  node *insert(ll val, int pos) { // indeces are 0-based
    node *n = newnode(val);
    node *tl, *tr;
    split(root, tl, tr, pos);
    merge(root, tl, n);
    merge(root, root, tr);
    return n;
  }

  node *erase(int pos) {
    node *tl, *tr, *n;
    split(root, tl, tr, pos);
    split(tr, n, tr, 1);
    merge(root, tl, tr);
    return n;
  }

  ll find(node *n, int pos, int add = 0) {
    pushLazy(n);
    int curPos = add + getSize(n->l);
    ll ans;
    if(pos == curPos) ans = n->val;
    else if(pos < curPos) ans = find(n->l, pos, add);
    else ans = find(n->r, pos, curPos + 1);
    updateNode(n);
    return ans;
  }

  ll find(int pos) {
    return find(root, pos);
  }

  void add(int l, int r, ll add) {
    node *tl, *tr, *t;
    split(root, tl, tr, l);
    split(tr, t, tr, r - l + 1);
    addLazy(t, add);
    merge(tl, tl, t);
    merge(root, tl, tr);
  }

  void reverse(int l, int r) {
    node *tl, *tr, *t;
    split(root, tl, tr, l);
    split(tr, t, tr, r - l + 1);
    toggleRev(t);
    merge(tl, tl, t);
    merge(root, tl, tr);
  }

  ll query(int l, int r) {
    node *tl, *tr, *t;
    split(root, tl, tr, l);
    split(tr, t, tr, r - l + 1);
    ll ans = query(t);
    merge(tl, tl, t);
    merge(root, tl, tr);
    return ans;
  }
};