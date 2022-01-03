/*
 * Assignment 5
 * Shir Fintsy 206949075
 * Ohad Marmor 207481524
 */

#include "minCircle.h"
#include <cmath>
#include <vector>

/**
 * this function returns the distance between 2 points
 * @param first point
 * @param second point
 * @return thae distance between them
 */
double dist(const Point& first, const Point& second)
{
    return sqrt(pow(first.x - second.x, 2)
                + pow(first.y - second.y, 2));
}

/**
 *
 * @param c - the circle we want to check
 * @param p the point we want to check
 * @return true if the circle encloses the point and false if doesn't
 */
bool is_inside(const Circle& c, const Point& p)
{
    return dist(c.center, p) <= c.radius;
}

/* The following two functions are used
To find the equation of the circle when
three points are given.

Helper method to get a circle defined by 3 points
*/

// this one returns the center of the circle
Point get_circle_center(double bx, double by,
                        double cx, double cy)
{
    double B = bx * bx + by * by;
    double C = cx * cx + cy * cy;
    double D = bx * cy - by * cx;
    return { static_cast<float>((cy * B - by * C) / (2 * D)), static_cast<float>((bx * C - cx * B) / (2 * D)) };
}

// this function returns a circle that intersects three points

Circle circle_from(const Point& first, const Point& second, const Point& third)
{
    Point center = get_circle_center(second.x - first.x, second.y - first.y, third.x - first.x, third.y - first.y);

    center.x += first.x;
    center.y += first.y;
    return { center, static_cast<float>(dist(center, first)) };
}

/* Function to return the smallest circle
that intersects 2 points
*/
Circle circle_from(const Point& first, const Point& second)
{
    // Set the center to be the midpoint of first and second
    Point C = {static_cast<float>((first.x + second.x) / 2.0), static_cast<float>((first.y + second.y) / 2.0) };
    // Set the radius to be half the distance AB
    return { C, static_cast<float>(dist(first, second) / 2.0) };
}

/* Function to check whether a circle
encloses the given points
 */
bool is_valid_circle(const Circle& c, const vector<Point>& P)
{
    /* Iterating through all the points to check  whether the points lie inside the circle or not */
    for (const Point& p : P)
        if (!is_inside(c, p))
            return false;
    return true;
}

Circle min_circle_trivial(vector<Point>& P) {
    unsigned long size = P.size();

    // faster than if-else:
    switch(size) {
        case 0:
            return {{0, 0}, 0};
        case 1:
            return {P[0], 0};
        case 2:
            return circle_from(P[0], P[1]);
        default:
            break;
    }

    // an attempt to create a circle that encloses all the points:
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {

            Circle c = circle_from(P[i], P[j]);
            if (is_valid_circle(c, P))
                return c;
        }
    }
    return circle_from(P[0], P[1], P[2]);
}

/**
 * this is the algorithm for creating a circle by O(n). uses recursion
 * @param points - the points we want to enclose
 * @param rPoints - empty at first call, and then adds the missing points and create a circle for them
 * @param size - number of points in the array
 * @return - a minimum circle.
 */
Circle alg_welzel(Point **points, vector<Point> rPoints, size_t size) {
    // here we stop the recursion:
    if (size == 0 || rPoints.size() == 3)
        return min_circle_trivial(rPoints);

    //continue the recursion without the last point
    Circle temp = alg_welzel(points, rPoints, size - 1);

    // the last point (faster than taking a random point):
    Point p = *points[size - 1];

    // if the temp circle contains p, return the temp circle
    if (is_inside(temp, p)) {
        return temp;
    }
    // Otherwise, must be on the boundary of the MEC
    rPoints.push_back(p);
    // Return the MEC for P - {p} and R U {p}
    return alg_welzel(points, rPoints, size - 1);
}

// just calling the function that implements the algorithm of welzl:
Circle findMinCircle(Point **points, size_t size) {
    vector<Point> R;
    return alg_welzel(points, R, size);
}