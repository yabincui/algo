#ifndef _GEOMETRY_H
#define _GEOMETRY_H

#include <float.h>
#include <math.h>

#include <algorithm>
#include <vector>

#include "logging.h"

using std::max;
using std::min;
using std::vector;

namespace algo_lib {

static inline int compare(double a, double b) {
	double diff = a - b;
	double abs_diff = diff < 0 ? -diff : diff;
	double abs_epsilon = 1e-9;
	double relative_epsilon = DBL_EPSILON;
	if (abs_diff < abs_epsilon || abs_diff < relative_epsilon * max(a, b)) {
		return 0;
	}
	return diff > 0 ? 1 : -1;
}

struct Point {
	double x, y;

	explicit Point(double x = 0, double y = 0) :
			x(x), y(y) {
	}

	double distTo(const Point& p) const {
		double dx = x - p.x;
		double dy = y - p.y;
		return sqrt(dx * dx + dy * dy);
	}

	bool equalTo(const Point& p) const {
		return compare(x, p.x) == 0 && compare(y, p.y) == 0;
	}

	// return the reflect point of p, by using current point as the center.
	Point reflect(const Point& p) const {
		return Point(2 * x - p.x, 2 * y - p.y);
	}

	Point shift(double x, double y) const {
		return Point(this->x + x, this->y + y);
	}

	// Rotate point p around current point counterclockwise angle degrees.
	Point rotate(const Point& p, double angle) {
		double radian = M_PI * angle / 180;
		Point shift_p = p.shift(-x, -y);
		double rx = shift_p.x * cos(radian) - shift_p.y * sin(radian);
		double ry = shift_p.x * sin(radian) + shift_p.y * cos(radian);
		return Point(rx, ry).shift(x, y);
	}
};

struct Vector {
	double x, y;

	explicit Vector(double x = 0, double y = 0) :
			x(x), y(y) {
	}
	explicit Vector(Point from, Point to) :
			x(to.x - from.x), y(to.y - from.y) {
	}

	double dotProduct(const Vector& v) const {
		return x * v.x + y * v.y;
	}

	double crossProduct(const Vector& v) const {
		return x * v.y - v.x * y;
	}
};

struct Line {
	// ax + by = c
	double a, b, c;

	explicit Line(double a, double b, double c) :
			a(a), b(b), c(c) {
	}

	explicit Line(Point p1, Point p2) {
		// ax1 + by1 = c
		// ax2 + by2 = c
		// a(x1-x2) + b(y1-y2) = 0
		a = p1.y - p2.y;
		b = p2.x - p1.x;
		c = a * p1.x + b * p1.y;
	}

	Line(Point p, Vector v) : Line(p, Point(p.x + v.x, p.y + v.y)) {
	}

	Vector getVector() const {
		return Vector(-b, a);
	}

	Vector getVerticalVector() const {
		return Vector(a, b);
	}

	double distFromPoint(const Point& p) const {
		Vector rv = getVerticalVector();
		Line v_line(p, rv);
		Point q;
		CHECK(findIntersect(v_line, &q));
		return p.distTo(q);
	}

	bool findIntersect(const Line& other, Point* p) const {
		// a1 * x + b1 * y = c1
		// a2 * x + b2 * y = c2
		double d = a * other.b - b * other.a;
		if (compare(d, 0) == 0) {
			return false;
		}
		// (a1b2 - a2b1) x = c1b2 - c2b1
		// (a1b2 - a2b1) y = c2a1 - c1a2
		p->x = (c * other.b - other.c * b) / d;
		p->y = (other.c * a - c * other.a) / d;
		return true;
	}

	Point reflect(const Point& p) const {
		Line line(p, getVerticalVector());
		Point center;
		CHECK(findIntersect(line, &center));
		return center.reflect(p);
	}
};

struct LineSegment: Line {
	Point p1, p2;

	explicit LineSegment(Point p1, Point p2) :
			Line(p1, p2), p1(p1), p2(p2) {
	}

	Point getCenter() const {
		return Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
	}

	double distFromPoint(const Point& p) const {
		Vector v12(p1, p2);
		Vector v2p(p2, p);
		if (v12.dotProduct(v2p) > 0) {
			return p2.distTo(p);
		}
		Vector v21(p2, p1);
		Vector v1p(p1, p);
		if (v21.dotProduct(v1p) > 0) {
			return p1.distTo(p);
		}
		double triangleArea = fabs(v12.crossProduct(v1p));
		return triangleArea / p1.distTo(p2);
	}

	bool findIntersect(const LineSegment& segment, Point* p) const {
		if (!Line::findIntersect(segment, p)) {
			return false;
		}
		if (!containsPointOnLine(*p) || !segment.containsPointOnLine(*p)) {
			return false;
		}
		return true;
	}

	bool containsPointOnLine(const Point& p) const {
		double minX = min(p1.x, p2.x);
		double maxX = max(p1.x, p2.x);
		double minY = min(p1.y, p2.y);
		double maxY = max(p1.y, p2.y);
		return compare(minX, p.x) <= 0 && compare(p.x, maxX) <= 0
				&& compare(minY, p.y) <= 0 && compare(p.y, maxY) <= 0;
	}
};

struct Polygon {
	vector<Point> points;

	Polygon(const vector<Point>& points) : points(points) {}

	// if morePoints is true, use all points in the same line.
	// otherwise, only select two edge points in the same line.
	static Polygon buildConvexHull(const vector<Point>& points,
			bool morePoints) {
		if (points.size() < 3) {
			return Polygon(points);
		}
		int start = 0;
		for (int i = 1; i < points.size(); ++i) {
			int tmp = compare(points[i].x, points[start].x);
			if (tmp < 0
					|| (tmp == 0 && compare(points[i].y, points[start].y) > 0)) {
				start = i;
			}
		}
		vector<Point> result;
		result.push_back(points[start]);
		vector<bool> used(points.size(), false);
		int prev = -1;
		int cur = start;
		while (true) {
			int next = -1;
			for (int i = 0; i < points.size(); ++i) {
				if (used[i] || i == cur) {
					continue;
				}
				if (next == -1) {
					next = i;
				} else {
					double tmp = Vector(points[cur], points[i]).crossProduct(
							Vector(points[cur], points[next]));
					if (compare(tmp, 0) < 0) {
						next = i;
					} else if (compare(tmp, 0) == 0) {
						if (prev == start && morePoints) {
							if (next == start) {
								next = i;
								continue;
							} else if (i == start) {
								continue;
							}
						}
						double d1 = points[cur].distTo(points[i]);
						double d2 = points[cur].distTo(points[next]);
						if ((compare(d1, d2) < 0 && morePoints)
								|| (compare(d1, d2) > 0 && !morePoints)) {
							next = i;
						}
					}
				}
			}
			if (next == start) {
				break;
			}
			result.push_back(points[next]);
			used[next] = true;
			prev = cur;
			cur = next;
		}
		return Polygon(result);
	}

	bool equalTo(const Polygon& p) const {
		if (points.size() != p.points.size()) {
			return false;
		}
		for (size_t i = 0; i < points.size(); ++i) {
			if (!points[i].equalTo(p.points[i])) {
				return false;
			}
		}
		return true;
	}

	double getArea() const {
		if (points.size() < 3) return 0;
		vector<Vector> v;
		for (int i = 1; i < points.size(); ++i) {
			v.push_back(Vector(points[0], points[i]));
		}
		double area = 0;
		for (int i = 0; i + 1 < v.size(); ++i) {
			area += v[i].crossProduct(v[i+1]);
		}
		return fabs(area / 2.0);
	}

	void print() const {
		printf("polygon(%zu) {", points.size());
		for (auto& p : points) {
			printf(" p(%f, %f)", p.x, p.y);
		}
		printf("\n");
	}
};

struct Circle {
	Point center;
	double radius;

	Circle(Point center, double radius) :
			center(center), radius(radius) {
	}

	Circle(Point a, Point b, Point c) {
		LineSegment s1(a, b);
		LineSegment s2(b, c);
		Line line1(s1.getCenter(), s1.getVerticalVector());
		Line line2(s2.getCenter(), s2.getVerticalVector());
		CHECK(line1.findIntersect(line2, &center));
		radius = center.distTo(a);
	}
};

}  // namespace algo_lib

#endif  // _GEOMETRY_H
