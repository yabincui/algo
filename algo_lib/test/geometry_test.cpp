#include "geometry.h"

#include <stdio.h>

#include <gtest/gtest.h>

using namespace algo_lib;

TEST(point, point) {
	Point p(3, 0);
	Point p2(0, 4);
	ASSERT_DOUBLE_EQ(p.distTo(p2), 5);

	ASSERT_TRUE(p.reflect(p2).equalTo(Point(6, -4)));

	ASSERT_TRUE(p.shift(3, 4).equalTo(Point(6, 4)));
	ASSERT_TRUE(Point(0, 0).rotate(p, 90).equalTo(Point(0, 3)));
}

TEST(vector, vector) {
	Vector a(3, 0);
	Vector b(0, 4);
	ASSERT_DOUBLE_EQ(a.dotProduct(b), 0);
	ASSERT_DOUBLE_EQ(a.crossProduct(b), 12);
	ASSERT_DOUBLE_EQ(b.crossProduct(a), -12);
}

TEST(line, line) {
	Line a(Point(0, 0), Point(3, 0));
	Line b(Point(0, 0), Point(0, 4));
	Point p;
	ASSERT_TRUE(a.findIntersect(b, &p));
	ASSERT_TRUE(p.equalTo(Point(0, 0)));

	ASSERT_DOUBLE_EQ(a.distFromPoint(Point(0, 0)), 0);
	ASSERT_DOUBLE_EQ(a.distFromPoint(Point(0, 4)), 4);

	ASSERT_TRUE(a.reflect(Point(0, 4)).equalTo(Point(0, -4)));
}

TEST(line_segment, line_segment) {
	LineSegment a(Point(0, 0), Point(3, 0));
	ASSERT_DOUBLE_EQ(a.distFromPoint(Point(0, 2)), 2);
	ASSERT_DOUBLE_EQ(a.distFromPoint(Point(6, 4)), 5);
	ASSERT_DOUBLE_EQ(a.distFromPoint(Point(-3, 4)), 5);
	LineSegment b(Point(1, -1), Point(1, 1));
	Point p;
	ASSERT_TRUE(a.findIntersect(b, &p));
	ASSERT_TRUE(p.equalTo(Point(1, 0)));
	LineSegment c(Point(-1, -1), Point(-1, 1));
	ASSERT_FALSE(a.findIntersect(c, &p));
}

TEST(polygon, polygon) {
	Polygon p( { Point(0, 0), Point(0, 4), Point(4, 4), Point(4, 0) });
	ASSERT_DOUBLE_EQ(p.getArea(), 16);

	Polygon p2 =
			Polygon::buildConvexHull(std::vector<Point>( { Point(0, 0),
			Point(0, 4), Point(4, 0),
					Point(4, 4), Point(2, 2),
					Point(0, 2) }),
			true);
	ASSERT_TRUE(
			p2.equalTo(Polygon(
			std::vector<Point>(
					{ Point(0, 4), Point(4, 4),
					Point(4, 0), Point(0, 0), Point(0, 2) }))));

	p2 = Polygon::buildConvexHull(
			std::vector<Point>(
					{ Point(0, 0), Point(0, 4), Point(4, 0), Point(4, 4), Point(
							2, 2), Point(0, 2) }), false);
	ASSERT_TRUE(
			p2.equalTo(Polygon(std::vector<Point>( { Point(0, 4), Point(4, 4),
					Point(4, 0), Point(0, 0) }))));

	p2 = Polygon::buildConvexHull(
			std::vector<Point>( { Point(1, 2), Point(2, 2), Point(4, 2) }),
			true);
	p2.print();
}

TEST(circle, circle) {
	Circle c(Point(-1, 0), Point(0, 1), Point(1, 0));
	ASSERT_TRUE(c.center.equalTo(Point(0, 0)));
	ASSERT_DOUBLE_EQ(c.radius, 1);
}
