/* This version allows insertion of a line with any slope
 * at any moment at the extra cost of O(log n) per operation.
 * Implementation answers minimum queries. Changing the line
 * comparison operator should be enough to handle maximium
 * queries instead (or just insert the lines mirrored with
 * respect to the x-axis).
 */

using ll = long long;
using ld = long double;
 
const ll INF = (ll)1e18 + 10;

struct CHT{
  struct line{
    ll m, q;
    ld rx;
    bool query;

    line(ll m, ll q, ld x, bool query){
      this->m = m; this->q = q;
      this->rx = x;
      this->query = query;
    }

    bool operator<(const line &l) const {
      if(l.query) return rx < l.rx;
      else return m == l.m ? q < l.q : m > l.m;
    }

    ld intersect(const line &l) const {
      if(m == l.m) return (ld)INF;
      else return (ld)(l.q - q) / (m - l.m);
    }
  };

  using hullit = set<line>::iterator;

  set<line> hull;

  bool hasPrev(hullit it){
    return it != hull.begin();
  }

  bool hasNext(hullit it){
    return it != hull.end() && next(it) != hull.end();
  }

  bool canErase(hullit it){
    if(!hasPrev(it) || !hasNext(it)) return false;
    hullit pr = prev(it), nx = next(it);
    return pr->intersect(*nx) <= pr->intersect(*it);
  }

  void updateRx(hullit it){
    ld x;
    if(!hasNext(it)) x = (ld)INF;
    else x = it->intersect(*next(it));
    line l = *it;
    l.rx = x;
    hull.erase(it);
    hull.insert(l);
    return;
  }

  // add line y = m * x + q
  void update(ll m, ll q){
    line l(m, q, 0, false);
    hullit it = hull.lower_bound(l);
    it = hull.insert(l).first;
    if(canErase(it)){
      hull.erase(it);
      return;
    }
    if(hasPrev(it)){
      hullit pr = prev(it);
      while(canErase(pr)){
        hull.erase(pr);
        pr = prev(it);
      }
    }
    if(hasNext(it)){
      hullit nx = next(it);
      while(canErase(nx)){
        hull.erase(nx);
        nx = next(it);
      }
    }
    if(hasPrev(it)) updateRx(prev(it));
    updateRx(it);
    return;
  }

  ll query(ll x){
    line l(0LL, 0LL, (ld)x, true);
    hullit it = hull.lower_bound(l);
    return it->m * x + it->q;
  }
};