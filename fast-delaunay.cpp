// Fast Delaunay triangulation (Bowyer-Watson algorithm) O(N^2)
// Not properly tested
// Edges is inserted twice in the result
// if shared by two triangles

using ld = long double;
constexpr ld eps = 1e-4;

struct Point {
  ld x, y;

  Point(): x(0.0), y(0.0) {}
  Point(ld _x, ld _y): x(_x), y(_y) {}

  bool operator==(const Point& other) const {
      return x == other.x && y == other.y;
  }
};

struct Edge {
  Point p0, p1;

  Edge(const Point& _p0, const Point& _p1): p0(_p0), p1(_p1) {}

  bool operator==(const Edge& other) const {
    return ((other.p0 == p0 && other.p1 == p1) ||
            (other.p0 == p1 && other.p1 == p0));
  }
};

struct Circle {
  ld x, y, radius;
  Circle() = default;
};

struct Triangle {
  Point p0, p1, p2;
  Edge e0, e1, e2;
  Circle circle;

  Triangle(const Point& _p0, const Point& _p1, const Point& _p2)
          :p0(_p0), p1(_p1), p2(_p2),
           e0(_p0, _p1), e1(_p1, _p2), e2(_p0, _p2),
           circle() {
    ld ax = p1.x - p0.x, ay = p1.y - p0.y;
    ld bx = p2.x - p0.x, by = p2.y - p0.y;
    ld m = p1.x * p1.x - p0.x * p0.x
         + p1.y * p1.y - p0.y * p0.y;
    ld u = p2.x * p2.x - p0.x * p0.x
         + p2.y * p2.y - p0.y * p0.y;
    ld s = 1. / (2. * (ax * by - ay * bx));
    circle.x = ((p2.y - p0.y) * m + (p0.y - p1.y) * u) * s;
    circle.y = ((p0.x - p2.x) * m + (p1.x - p0.x) * u) * s;
    ld dx = p0.x - circle.x;
    ld dy = p0.y - circle.y;
    circle.radius = dx * dx + dy * dy;
  }
};

struct Delaunay {
  vector<Triangle> triangles;
  vector<Edge> edges;
};

Delaunay triangulate(const vector<Point>& points) {
  if (points.size() < 3) {
    return Delaunay();
  }
  ld xmin = points[0].x, ymin = points[0].y;
  ld xmax = xmin, ymax = ymin;
  for (auto& pt : points) {
    xmin = min(xmin, pt.x);
    xmax = max(xmax, pt.x);
    ymin = min(ymin, pt.y);
    ymax = max(ymax, pt.y);
  }

  ld dx = xmax - xmin, dy = ymax - ymin;
  ld dmax = max(dx, dy);
  ld midx = (xmin + xmax) / 2.0;
  ld midy = (ymin + ymax) / 2.0;

  // Init Delaunay triangulation.
  Delaunay d = Delaunay();

  Point p0 = Point{midx - 20 * dmax, midy - dmax};
  Point p1 = Point{midx, midy + 20 * dmax};
  Point p2 = Point{midx + 20 * dmax, midy - dmax};
  d.triangles.emplace_back(Triangle(p0, p1, p2));

  for (auto& pt : points) {
    vector<Edge> edges;
    vector<Triangle> tmps;
    for (auto& tri : d.triangles) {
      // Check if the point is inside the triangle circumcircle.
      ld dist = (tri.circle.x - pt.x) * (tri.circle.x - pt.x)
              + (tri.circle.y - pt.y) * (tri.circle.y - pt.y);
      if ((dist - tri.circle.radius) <= eps) {
        edges.push_back(tri.e0);
        edges.push_back(tri.e1);
        edges.push_back(tri.e2);
      } else {
        tmps.push_back(tri);
      }
    }

    // Delete duplicate edges.
    vector<bool> remove(edges.size(), false);
    for(int i = 0; i < edges.size(); ++i) {
        for(int j = i + 1; j < edges.size(); ++j) {
            if(edges[i] == edges[j]) {
                remove[i] = remove[j] = true;
            }
        }
    }

    edges.erase(remove_if(edges.begin(), edges.end(),
      [&](auto& e) {
        return remove[&e - &edges[0]];
      }), edges.end());

    // Update triangulation.
    for (auto& e : edges) {
      tmps.push_back(Triangle(e.p0, e.p1, Point(pt.x, pt.y)));
    }
    d.triangles = tmps;
  }

  // Remove original super triangle.
  d.triangles.erase(
    remove_if(d.triangles.begin(), d.triangles.end(),
    [&](auto& tri) {
      return ((tri.p0 == p0 || tri.p1 == p0 || tri.p2 == p0) ||
              (tri.p0 == p1 || tri.p1 == p1 || tri.p2 == p1) ||
              (tri.p0 == p2 || tri.p1 == p2 || tri.p2 == p2));
    }), d.triangles.end());

  // Add edges.
  for (auto& tri : d.triangles) {
    d.edges.push_back(tri.e0);
    d.edges.push_back(tri.e1);
    d.edges.push_back(tri.e2);
  }
  return d;
}