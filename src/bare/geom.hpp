#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>
#include <type_traits>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

#define _USE_MATH_DEFINES

#ifndef __STLB_EPS
#define __STLB_EPS 1e-9
#endif


namespace stlb
{

    template<typename type>
    type absol(const type& val) noexcept {
        if (std::is_integral<type>::value) {
            return abs(val);
        }
        else {
            return fabsl(val);
        }
    }

    template<typename type>
    bool is_zero(const type& val) noexcept {
        return absol(val) <= static_cast<type>(__STLB_EPS);
    }

    template<typename type>
    bool greater_zero(const type& val) noexcept {
        return val > static_cast<type>(__STLB_EPS);
    }

    template<typename type>
    bool equal_greater_zero(const type& val) noexcept {
        return val >= -static_cast<type>(__STLB_EPS);
    }

    template<typename type>
    bool less_zero(const type& val) noexcept {
        return val < -static_cast<type>(__STLB_EPS);
    }

    template<typename type>
    bool equal_less_zero(const type& val) noexcept {
        return val <= static_cast<type>(__STLB_EPS);
    }

    long double rad_to_deg(const long double& val) noexcept {
        return val * 180.0l / M_PIl;
    }

    long double deg_to_rad(const long double& val) noexcept {
        return val * M_PIl / 180.0l;
    }

    template<typename type>
    int sign(const type& val) noexcept {
        return (static_cast<type>(0) < val) - (val < static_cast<type>(0));
    }

    template<typename store_type> struct vector2;
    template<typename store_type> struct point;
    template<typename store_type> struct segment;
    template<typename store_type> struct ray;
    template<typename store_type> struct line;
    template<typename store_type> struct polygon;

    template<typename store_type>
    struct vector2
    {

        store_type x;
        store_type y;

        constexpr vector2() noexcept : x(0), y(0) {}

        constexpr vector2(const store_type x, const store_type y) noexcept : x(x), y(y) {}

        template<typename other_store_type>
        constexpr vector2(const vector2<other_store_type>& other) : x(other.x), y(other.y) {}

        template<typename other_store_type>
        explicit constexpr vector2(const point<other_store_type>& other) : x(other.x), y(other.y) {}

        template<typename other_store_type>
        vector2<store_type>& operator= (const vector2<other_store_type>& other) {
            x = other.x;
            y = other.y;
            return *this;
        }

        template<typename other_store_type>
        vector2<store_type>& operator= (const point<other_store_type>& other) {
            x = other.x;
            y = other.y;
            return *this;
        }

        vector2<store_type> operator- () const noexcept {
            return vector2<store_type>(-x, -y);
        }

        vector2<store_type>& operator*= (const store_type scalar) noexcept {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        vector2<store_type>& operator/= (const store_type scalar) noexcept {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        vector2<store_type>& operator+= (const vector2<store_type>& rhs) noexcept {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        vector2<store_type>& operator-= (const vector2<store_type>& rhs) noexcept {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        friend vector2<store_type> operator* (vector2<store_type> lhs, const store_type rhs) noexcept {
            return lhs *= rhs;
        }

        friend vector2<store_type> operator* (const store_type lhs, vector2<store_type> rhs) noexcept {
            return rhs *= lhs;
        }

        friend vector2<store_type> operator/ (vector2<store_type> lhs, const store_type rhs) noexcept {
            return lhs /= rhs;
        }

        friend vector2<store_type> operator/ (const store_type lhs, vector2<store_type> rhs) noexcept {
            return rhs /= lhs;
        }

        friend vector2<store_type> operator+ (
            vector2<store_type> lhs,
            const vector2<store_type>& rhs
        ) noexcept {
            return lhs += rhs;
        }

        friend vector2<store_type> operator- (
            vector2<store_type> lhs,
            const vector2<store_type>& rhs
        ) noexcept {
            return lhs -= rhs;
        }

        friend bool operator== (
            const vector2<store_type>& lhs,
            const vector2<store_type>& rhs
        ) noexcept {
            return is_zero(lhs - rhs);
        }

        friend bool operator!= (
            const vector2<store_type>& lhs,
            const vector2<store_type>& rhs
        ) noexcept {
            return !(lhs == rhs);
        }

    };

    template<typename store_type>
    struct point
    {

        store_type x;
        store_type y;

        constexpr point() noexcept : x(0), y(0) {}

        constexpr point(const store_type x, const store_type y) noexcept : x(x), y(y) {}

        template<typename other_store_type>
        constexpr point(const point<other_store_type>& other) : x(other.x), y(other.y) {}

        template<typename other_store_type>
        constexpr point(const vector2<other_store_type>& other) : x(other.x), y(other.y) {}

        template<typename other_store_type>
        point<store_type>& operator= (const point<other_store_type>& other) {
            x = other.x;
            y = other.y;
            return *this;
        }

        template<typename other_store_type>
        point<store_type>& operator= (const vector2<other_store_type>& other) {
            x = other.x;
            y = other.y;
            return *this;
        }

        point<store_type>& operator+= (const vector2<store_type>& rhs) noexcept {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        point<store_type>& operator-= (const vector2<store_type>& rhs) noexcept {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        friend point<store_type> operator+ (
            point<store_type> lhs,
            const vector2<store_type> rhs
        ) noexcept {
            return lhs += rhs;
        }

        friend point<store_type> operator+ (
            const vector2<store_type> lhs,
            point<store_type> rhs
        ) noexcept {
            return rhs += lhs;
        }

        friend point<store_type> operator- (
            point<store_type> lhs,
            const vector2<store_type> rhs
        ) noexcept {
            return lhs -= rhs;
        }

        friend vector2<store_type> operator- (
            point<store_type> lhs,
            const point<store_type>& rhs
        ) noexcept {
            return vector2<store_type>(lhs) -= vector2<store_type>(rhs);
        }

        friend bool operator== (
            const point<store_type>& lhs,
            const point<store_type>& rhs
        ) noexcept {
            return is_zero(lhs - rhs);
        }

        friend bool operator!= (
            const point<store_type>& lhs,
            const point<store_type>& rhs
        ) noexcept {
            return !(lhs == rhs);
        }

    };

    template<typename store_type>
    struct segment
    {

        point<store_type> point1;
        point<store_type> point2;

        constexpr segment() noexcept = default;

        constexpr segment(
            const store_type& a, const store_type& b,
            const store_type& c, const store_type& d
        ) noexcept : point1(a, b), point2(c, d) {}

        template<typename other_store_type>
        constexpr segment(const segment<other_store_type>& other) noexcept
            : point1(other.point1), point2(other.point2) {}

        template<typename other_store_type>
        constexpr segment(
            const point<other_store_type>& point1,
            const point<other_store_type>& point2
        ) noexcept : point1(point1), point2(point2) {}

        template<typename other_store_type>
        segment<store_type>& operator= (const segment<other_store_type>& rhs) noexcept {
            point1 = rhs.point1;
            point2 = rhs.point2;
            return *this;
        }

        friend bool operator== (
            const segment<store_type>& lhs,
            const segment<store_type>& rhs
        ) noexcept {
            return (lhs.point1 == rhs.point1 && lhs.point2 == rhs.point2) ||
                   (lhs.point2 == rhs.point1 && lhs.point1 == rhs.point2);
        }

        friend bool operator!= (
            const segment<store_type>& lhs,
            const segment<store_type>& rhs
        ) noexcept {
            return !(lhs == rhs);
        }

    };

    template<typename store_type>
    struct ray
    {

        store_type A = 0;
        store_type B = 0;
        store_type C = 0;
        point<store_type> begin;

        constexpr ray() = default;

        template<typename other_store_type>
        constexpr ray(
            const store_type A,
            const store_type B,
            const store_type C,
            const point<other_store_type>& begin
        ) noexcept : A(A), B(B), C(C), begin(begin) {}

        template<typename other_store_type>
        constexpr ray(
            const point<other_store_type>& point1,
            const point<other_store_type>& point2
        ) noexcept {
            this->begin = point1;
            A = point1.y - point2.y;
            B = point2.x - point1.x;
            C = point2.y * point1.x - point2.x * point1.y;
        }

        template<typename other_store_type>
        constexpr ray(const line<other_store_type>& ln) noexcept : A(A), B(B), C(C) {}

        constexpr ray(
            const store_type a,
            const store_type b,
            const store_type c,
            const store_type d
        ) noexcept {
            *this = ray<store_type>(point<store_type>(a, b), point<store_type>(c, d));
        }

        friend bool operator== (
            const ray<store_type>& lhs,
            const ray<store_type>& rhs
        ) noexcept {
            return is_zero(lhs.A * rhs.B - lhs.B * rhs.A) &&
                   is_zero(lhs.B * rhs.C - lhs.C * rhs.B) &&
                   lhs.begin == rhs.begin;
        }

        friend bool operator!= (
            const ray<store_type>& lhs,
            const ray<store_type>& rhs
        ) noexcept {
            return !(lhs == rhs);
        }

    };

    template<typename store_type>
    struct line
    {

        store_type A = 0;
        store_type B = 0;
        store_type C = 0;

        constexpr line() = default;

        constexpr line(
            const store_type& A,
            const store_type& B,
            const store_type& C
        ) noexcept : A(A), B(B), C(C) {}

        template<typename other_store_type>
        constexpr line(
            const point<other_store_type>& point1,
            const point<other_store_type>& point2
        ) noexcept {
            A = point1.y - point2.y;
            B = point2.x - point1.x;
            C = point2.y * point1.x - point2.x * point1.y;
        }

        template<typename other_store_type>
        constexpr line(const line<other_store_type>& ln) noexcept : A(ln.A), B(ln.B), C(ln.C) {}

        template<typename other_store_type>
        constexpr line(const ray<other_store_type>& bm) noexcept
            : A(bm.A), B(bm.B), C(bm.C) {}

        template<typename other_store_type>
        constexpr line(const segment<other_store_type>& seg) noexcept {
            *this = line(seg.point1, seg.point2);
        }

        friend bool operator== (
            const line<store_type>& lhs,
            const line<store_type>& rhs
        ) noexcept {
            return is_zero(lhs.A * rhs.B - lhs.B * rhs.A) && is_zero(lhs.B * rhs.C - lhs.C * rhs.B);
        }

        friend bool operator!= (
            const line<store_type>& lhs,
            const line<store_type>& rhs
        ) noexcept {
            return !(lhs == rhs);
        }

    };

    template<typename store_type>
    struct polygon
    {

        std::vector<point<store_type>> points;

        constexpr polygon() noexcept = default;

        constexpr polygon(const size_t vertex_count) noexcept {
            points.resize(vertex_count);
        }

        constexpr polygon(const std::vector<point<store_type>>& points) noexcept : points(points) {}

        template<typename other_store_type>
        constexpr polygon(const polygon<other_store_type>& pl) noexcept {
            *this = polygon<store_type>(pl);
        }

        size_t size() const noexcept {
            return points.size();
        }

        point<store_type>& operator[] (const size_t pos) noexcept {
            return pos < size() ? points[pos] : points[pos % size()];
        }

        const point<store_type>& operator[] (const size_t pos) const noexcept {
            return pos < size() ? points[pos] : points[pos % size()];
        }

    };

    template<typename store_type>
    struct circle
    {

        point<store_type> center;
        store_type R;

        constexpr circle() noexcept = default;

        constexpr circle(
            const point<store_type>& center,
            const store_type& R
        ) noexcept : center(center), R(R) {}

        friend bool operator== (
            const circle<store_type>& lhs,
            const circle<store_type>& rhs
        ) noexcept {
            return lhs.center == rhs.center && is_zero(lhs.R - rhs.R);
        }

        friend bool operator!= (
            const circle<store_type>& lhs,
            const circle<store_type>& rhs
        ) noexcept {
            return !(lhs == rhs);
        }

    };

    template<typename store_type>
    bool lex_cmp(
        const vector2<store_type>& lhs,
        const vector2<store_type>& rhs
    ) noexcept {
        return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
    }

    template<typename store_type>
    bool lex_cmp(
        const point<store_type>& lhs,
        const point<store_type>& rhs
    ) noexcept {
        return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
    }

    /*template<typename store_type>
    bool polar_cmp(
        const vector2<store_type>& lhs,
        const vector2<store_type>& rhs
    ) noexcept {
        const auto on_div = [&](const vector2<store_type>& vec) {
            return is_zero(cross(vec, vector2<store_type>(1, 0)));
        };
        const auto left_div = [&](const vector2<store_type>& vec) {
            return are_semiplane_same(
                vector2<store_type>(0, 1),
                vector2<store_type>(1, 0),
                vec
            );
        };
        const auto diff = [&](
            const vector2<store_type>& X,
            const vector2<store_type>& Y
        ) {
            return are_semiplane_same()
        }
        if (on_div(lhs)) {
            if (left_div(lhs)) {
                return !on_div(rhs) || !left_div(rhs);
            }
            else {
                if (on_div(rhs)) {
                    return false;
                }
                else {
                    return are_semiplane_same(
                        vector2<store_type>(-1, 0),
                        vector2<store_type>(0, -1),
                        rhs
                    );
                }
            }
        }
        else {
            if (on_div(rhs)) {
                if (left_div(rhs)) {
                    return false;
                }
                else {
                    return are_semiplane_same(
                        vector2<store_type>(-1, 0),
                        vector2<store_type>(0, 1),
                        lhs
                    );
                }
            }
            else {
                if (are_semiplane_same(
                    vector2<store_type>(1, 0),
                    vector2<store_type>(0, 1),
                    lhs
                )) {
                    return are_semiplane_same(rhs, normal(rhs), lhs);
                }
                else {
                    return are_semiplane_same(lhs, -normal(lhs), rhs);
                }
            }
        }
    }*/

    template<typename store_type>
    long double len(const vector2<store_type>& lhs) noexcept {
        return hypotl(lhs.x, lhs.y);
    }

    template<typename store_type>
    store_type len2(const vector2<store_type>& lhs) noexcept {
        return lhs.x * lhs.x + lhs.y * lhs.y;
    }

    template<typename store_type>
    bool is_zero(const vector2<store_type>& lhs) noexcept {
        if (std::is_integral<store_type>::value) {
            return is_zero(lhs.x) && is_zero(lhs.y);
        }
        else {
            return is_zero(len(lhs));
        }
    }

    template<typename store_type>
    vector2<long double> normalized(const vector2<store_type>& lhs) noexcept {
        return vector2<long double>(lhs.x, lhs.y) /= len(lhs);
    }

    template<typename store_type>
    vector2<store_type> normal(const vector2<store_type>& lhs) noexcept {
        return vector2<store_type>(lhs.y, -lhs.x);
    }

    template<typename store_type>
    vector2<store_type> direction(const ray<store_type>& bm) noexcept {
        return vector2<store_type>(bm.B, -bm.A);
    }

    template<typename store_type>
    vector2<store_type> direction(const line<store_type>& ln) noexcept {
        return vector2<store_type>(ln.B, -ln.A);
    }

    template<typename store_type>
    vector2<store_type> direction(const segment<store_type>& seg) noexcept {
        return vector2<store_type>(seg.point1) - vector2<store_type>(seg.point2);
    }

    template<typename store_type>
    store_type dot(
        const vector2<store_type>& lhs,
        const vector2<store_type>& rhs
    ) noexcept {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    template<typename store_type>
    store_type cross(
        const vector2<store_type>& lhs,
        const vector2<store_type>& rhs
    ) noexcept {
        return lhs.x * rhs.y - lhs.y * rhs.x;
    }

    template<typename store_type>
    long double cos(
        const vector2<store_type>& lhs,
        const vector2<store_type>& rhs
    ) noexcept {
        return dot(lhs, rhs) / len(lhs) / len(rhs);
    }

    template<typename store_type>
    long double angle(
        const vector2<store_type>& lhs,
        const vector2<store_type>& rhs
    ) noexcept {
        return acosl(cos(lhs, rhs));
    }

    template<typename store_type>
    vector2<store_type> projection(
        const vector2<store_type>& lhs,
        const vector2<store_type>& rhs
    ) noexcept {
        return dot(lhs, rhs) / len(rhs);
    }

    template<typename store_type>
    store_type area(
        const vector2<store_type>& lhs,
        const vector2<store_type>& rhs
    ) noexcept {
        return absol(cross(lhs, rhs));
    }

    template<typename store_type>
    long double area(const polygon<store_type>& pl) noexcept {
        long double ret = 0;
        for (size_t i = 1; i + 1 < pl.size(); ++i) {
            ret += cross(pl[i + 1] - pl[0], pl[i] - pl[0]);
        }
        return absol(ret) / 2.0l;
    }

    template<typename store_type>
    bool are_on_line(
        const point<store_type> pt1,
        const point<store_type> pt2,
        const point<store_type> pt3
    ) noexcept {
        return is_zero(cross(pt2 - pt1, pt3 - pt1));
    }

    template<typename store_type>
    bool collinear(
        const vector2<store_type>& lhs,
        const vector2<store_type>& rhs
    ) noexcept {
        return is_zero(cross(lhs, rhs));
    }

    template<typename store_type>
    bool collinear(
        const segment<store_type>& lhs,
        const segment<store_type>& rhs
    ) noexcept {
        return collinear(lhs.point1 - lhs.point2, rhs.point1 - rhs.point2);
    }

    template<typename store_type>
    bool collinear(
        const ray<store_type>& lhs,
        const ray<store_type>& rhs
    ) noexcept {
        return collinear(direction(lhs), direction(rhs));
    }

    template<typename store_type>
    bool collinear(
        const line<store_type>& lhs,
        const line<store_type>& rhs
    ) noexcept {
        return collinear(direction(lhs), direction(rhs));
    }

    template<typename store_type>
    bool are_semiplane_same(
        const vector2<store_type>& divider,
        const vector2<store_type>& point1,
        const vector2<store_type>& point2
    ) noexcept {
        return sign(cross(divider, point1)) == sign(cross(divider, point2));
    }

    template<typename store_type>
    bool are_semiplane_same(
        const point<store_type>& line_point1,
        const point<store_type>& line_point2,
        const point<store_type>& point1,
        const point<store_type>& point2
    ) noexcept {
        return are_semiplane_same(
            line_point1 - line_point2,
            vector2<store_type>(point1),
            vector2<store_type>(point2)
        );
    }

    template<typename store_type>
    bool clockwise(
        const vector2<store_type>& side1,
        const vector2<store_type>& side2
    ) noexcept {
        return less_zero(cross(side1, side2));
    }

    template<typename store_type>
    bool clockwise(
        const point<store_type>& center,
        const point<store_type>& point1,
        const point<store_type>& point2
    ) noexcept {
        return clockwise(point1 - center, point2 - center);
    }

    template<typename store_type>
    bool counter_clockwise(
        const vector2<store_type>& side1,
        const vector2<store_type>& side2
    ) noexcept {
        return greater_zero(cross(side1, side2));
    }

    template<typename store_type>
    bool counter_clockwise(
        const point<store_type>& center,
        const point<store_type>& point1,
        const point<store_type>& point2
    ) noexcept {
        return counter_clockwise(point1 - center, point2 - center);
    }

    template<typename store_type>
    polygon<store_type> convex_hull(
        polygon<store_type> pl,
        const bool is_counter_clockwise = true
    ) noexcept {
        if (pl.size() <= 1) {
            return pl;
        }
        std::sort(pl.points.begin(), pl.points.end(), &lex_cmp<store_type>);
        const point<store_type> pt1 = pl.points.front();
        const point<store_type> pt2 = pl.points.back();
        std::vector<point<store_type>> up = { pt1 };
        std::vector<point<store_type>> down = { pt1 };
        for (size_t i = 1; i < pl.size(); ++i) {
            if (i + 1 == pl.size() || clockwise(pt1, pl[i], pt2)) {
                while (up.size() >= 2 && !clockwise(up[up.size() - 2], up[up.size() - 1], pl[i])) {
                    up.pop_back();
                }
                up.push_back(pl[i]);
            }
            if (i + 1 == pl.size() || counter_clockwise(pt1, pl[i], pt2)) {
                while (down.size() >= 2 && !counter_clockwise(down[down.size() - 2], down[down.size() - 1], pl[i])) {
                    down.pop_back();
                }
                down.push_back(pl[i]);
            }
        }
        pl.points = up;
        if (down.size() > 1) {
            std::for_each(down.rbegin() + 1, down.rend() - 1, [&](const point<store_type>& pt) noexcept {
                pl.points.push_back(pt);
            });
        }
        if (is_counter_clockwise) {
            std::reverse(pl.points.begin(), pl.points.end());
        }
        return pl;
    }

    template<typename store_type>
    bool are_intersect(
        const point<store_type>& pt,
        const line<store_type>& ln
    ) noexcept {
        return is_zero(ln.A * pt.x + ln.B * pt.y + ln.C);
    }

    template<typename store_type>
    bool are_intersect(
        const line<store_type>& ln,
        const point<store_type>& pt
    ) noexcept {
        return are_intersect(pt, ln);
    }

    template<typename store_type>
    bool are_intersect(
        const point<store_type>& pt,
        const ray<store_type>& bm
    ) noexcept {
        return pt == bm.begin || (are_intersect(
                pt, line<store_type>(bm)
            ) && greater_zero(dot(pt - bm.begin, direction(bm)))
        );
    }

    template<typename store_type>
    bool are_intersect(
        const ray<store_type>& bm,
        const point<store_type>& pt
    ) noexcept {
        return are_intersect(pt, bm);
    }

    template<typename store_type>
    bool are_intersect(
        const point<store_type>& pt,
        const segment<store_type>& seg
    ) noexcept {
        return is_zero(cross(seg.point1 - pt, seg.point2 - pt)) &&
               equal_greater_zero(dot(seg.point1 - pt, seg.point2 - pt));
    }

    template<typename store_type>
    bool are_intersect(
        const segment<store_type>& seg,
        const point<store_type>& pt
    ) noexcept {
        return are_intersect(pt, seg);
    }

    template<typename store_type>
    bool are_intersect(
        const segment<store_type>& seg1,
        const segment<store_type>& seg2
    ) noexcept {
        if (
            are_on_line(seg1.point1, seg1.point2, seg2.point1) &&
            are_on_line(seg1.point1, seg1.point2, seg2.point2)
        ) {
            const auto intersect_1d = [](store_type a, store_type b, store_type c, store_type d) noexcept {
                if (a > b) {
                    std::swap(a, b);
                }
                if (c > d) {
                    std::swap(c, d);
                }
                return std::max(a, c) <= std::min(b, d);
            };
            return intersect_1d(seg1.point1.x, seg1.point2.x, seg2.point1.x, seg2.point2.x) &&
                   intersect_1d(seg1.point1.y, seg1.point2.y, seg2.point1.y, seg2.point2.y);
        }
        else {
            return !are_semiplane_same(seg1.point2 - seg1.point1, seg2.point1 - seg1.point1, seg2.point2 - seg1.point1) &&
                   !are_semiplane_same(seg2.point2 - seg2.point1, seg1.point1 - seg2.point1, seg1.point2 - seg2.point1);
        }
    }

    template<typename store_type>
    bool are_intersect(
        const point<store_type>& pt,
        const circle<store_type>& cl
    ) noexcept {
        return is_zero(len2(pt - cl.center) - cl.R * cl.R);
    }

    template<typename store_type>
    bool are_intersect(
        const circle<store_type>& cl,
        const point<store_type>& pt
    ) noexcept {
        return are_intersect(pt, cl);
    }

    template<typename store_type>
    bool are_intersect(
        const line<store_type>& ln1,
        const line<store_type>& ln2
    ) noexcept {
        return ln1 == ln2 || !collinear(ln1, ln2);
    }

    /*template<typename store_type>
    bool are_intersect(
        const ray<store_type>& bm1,
        const ray<store_type>& bm2
    ) noexcept {
        if (!are_intersect(line<store_type>(bm1), line<store_type>(bm2))) {
            return false;
        }
        const vector2<store_type> div = bm2.begin - bm1.begin;
        if (collinear(div, direction(bm2)) || collinear(div, direction(bm1))) {
            return are_intersect(bm1.begin, bm2) || are_intersect(bm2.begin, bm1);
        }
        return are_semiplane_same(div, direction(bm1), direction(bm2));
    }*/

    template<typename store_type>
    bool is_point_on_angle(
        const vector2<store_type>& pt,
        const vector2<store_type>& side1,
        const vector2<store_type>& side2
    ) noexcept {
        return are_intersect(point<store_type>(pt), ray<store_type>(point<store_type>(), point<store_type>(side1))) ||
               are_intersect(point<store_type>(pt), ray<store_type>(point<store_type>(), point<store_type>(side2))) ||
               (are_semiplane_same(side1, pt, side2) && are_semiplane_same(side2, pt, side1));
    }

    template<typename store_type>
    bool is_point_on_angle(
        const point<store_type>& pt,
        const point<store_type>& side1,
        const point<store_type>& side2,
        const point<store_type>& center
    ) noexcept {
        return is_point_on_angle(pt - center, side1 - center, side2 - center);
    }

    template<typename store_type>
    long double distance(
        const vector2<store_type>& lhs,
        const vector2<store_type>& rhs
    ) noexcept {
        return len(lhs - rhs);
    }

    template<typename store_type>
    long double distance(
        const point<store_type>& lhs,
        const point<store_type>& rhs
    ) noexcept {
        return len(lhs - rhs);
    }

    template<typename store_type>
    long double distance(
        const point<store_type>& pt,
        const line<store_type>& ln
    ) noexcept {
        return absol(ln.A * pt.x + ln.B * pt.y + ln.C) / sqrtl(ln.A * ln.A + ln.B * ln.B);
    }

    template<typename store_type>
    long double distance(
        const line<store_type>& ln,
        const point<store_type>& pt
    ) noexcept {
        return distance(pt, ln);
    }

    template<typename store_type>
    long double distance(
        const point<store_type>& pt,
        const ray<store_type>& bm
    ) noexcept {
        if (greater_zero(dot(pt - bm.begin, bm.direction()))) {
            return point_distance_to_line(pt, line<store_type>(bm));
        }
        else {
            return distance(pt, bm.begin);
        }
    }

    template<typename store_type>
    long double distance(
        const ray<store_type>& bm,
        const point<store_type>& pt
    ) noexcept {
        return distance(pt, bm);
    }

    template<typename store_type>
    long double distance(
        const point<store_type>& pt,
        const segment<store_type>& seg
    ) noexcept {
        if (
            greater_zero(dot(seg.point2 - seg.point1, pt - seg.point1)) &&
            greater_zero(dot(seg.point1 - seg.point2, pt - seg.point2))
        ) {
            return distance(pt, line<store_type>(seg));
        }
        else {
            return std::min(
                distance(pt, seg.point1),
                distance(pt, seg.point2)
            );
        }
    }

    template<typename store_type>
    long double distance(
        const segment<store_type>& seg,
        const point<store_type>& pt
    ) noexcept {
        return distance(pt, seg);
    }

    template<typename store_type>
    bool is_inside(
        const point<store_type>& pt,
        const circle<store_type>& cl
    ) noexcept {
        return equal_less_zero(len2(pt - cl.center) - cl.R * cl.R);
    }

    template<typename store_type>
    bool is_inside(
        const circle<store_type>& cl,
        const point<store_type>& pt
    ) noexcept {
        return is_inside(pt, cl);
    }

    template<typename store_type>
    bool is_inside(
        const point<store_type>& pt,
        const polygon<store_type>& pl
    ) noexcept {
        const long double area1 = area(pl);
        long double area2 = 0;
        for (size_t i = 0; i < pl.size(); ++i) {
            area2 += area(pl[i + 1] - pt, pl[i] - pt);
        }
        return 2.0l * area1 == area2;
    }

    template<typename store_type>
    bool is_inside(
        const polygon<store_type>& pl,
        const point<store_type>& pt
    ) noexcept {
        return is_inside(pt, pl);
    }

    /*template<typename store_type>
    bool is_inside_convex_polygon(
        const point<store_type>& pt,
        const polygon<store_type>& pl
    ) noexcept {
        size_t l = 1;
        size_t r = pl.size() - 1;
        const point<store_type> center = pl[0];
        while (r - l > 1) {
            size_t mid = (l + r) / 2;
            if (is_point_on_angle(pt, pl[l], pl[mid], center)) {
                r = mid;
            }
            else {
                l = mid;
            }
        }
        return is_inside(pt, polygon<store_type>({ center, pl[l], pl[r] }));
    }

    template<typename store_type>
    bool is_inside_convex_polygon(
        const polygon<store_type>& pl,
        const point<store_type>& pt
    ) noexcept {
        return inside_convex_polygon(pt, pl);
    }*/

    template<typename store_type>
    point<long double> intersect(
        const line<store_type>& ln1,
        const line<store_type>& ln2
    ) noexcept {
        return point<long double>(
            static_cast<long double>(ln2.C * ln1.B - ln1.C * ln2.B) / static_cast<long double>(ln1.A * ln2.B - ln2.A * ln1.B),
            static_cast<long double>(ln2.A * ln1.C - ln1.A * ln2.C) / static_cast<long double>(ln1.A * ln2.B - ln2.A * ln1.B)
        );
    }

    template<typename store_type>
    std::vector<point<long double>> intersect(
        const line<store_type>& ln,
        const circle<store_type>& cl
    ) noexcept {
        const long double dst = distance(cl.center, ln);
        if (greater_zero(dst - cl.R)) {
            return std::vector<point<long double>>();
        }
        std::vector<point<long double>> ret;
        vector2<long double> ptr = normalized(normal(direction(ln))) * dst;
        if (!are_intersect(cl.center + ptr, ln)) {
            ptr = -ptr;
        }
        const vector2<long double> step = normalized(direction(ln)) * sqrtl(cl.R * cl.R - dst * dst);
        if (is_zero(len(step))) {
            ret.push_back(cl.center + ptr);
        }
        else {
            ret.push_back(cl.center + ptr - step);
            ret.push_back(cl.center + ptr + step);
        }
        return ret;
    }

    template<typename store_type>
    std::vector<point<long double>> intersect(
        const circle<store_type>& cl,
        const line<store_type>& ln
    ) noexcept {
        return intersect(ln, cl);
    }

    template<typename store_type>
    std::vector<point<long double>> intersect(
        const segment<store_type>& seg,
        const circle<store_type>& cl
    ) noexcept {
        std::vector<point<long double>> ret;
        const segment<long double> tmp(seg);
        for (point<long double>& i : intersect(line<store_type>(seg), cl)) {
            if (are_intersect(i, tmp)) {
                ret.push_back(i);
            }
        }
        return ret;
    }

    template<typename store_type>
    std::vector<point<long double>> intersect(
        const circle<store_type>& cl,
        const segment<store_type>& seg
    ) noexcept {
        return intersect(seg, cl);
    }

    template<typename store_type>
    polygon<long double> intersect(
        const polygon<store_type>& pl1,
        const polygon<store_type>& pl2
    ) noexcept {
        polygon<long double> ret;
        for (size_t i = 0; i < pl1.size(); ++i) {
            if (is_inside(pl1[i], pl2)) {
                ret.points.push_back(vector2<long double>(pl1[i].x, pl1[i].y));
            }
        }
        for (size_t i = 0; i < pl2.size(); ++i) {
            if (is_inside(pl2[i], pl1)) {
                ret.points.push_back(vector2<long double>(pl2[i].x, pl2[i].y));
            }
        }
        for (size_t i = 0; i < pl1.size(); ++i) {
            for (size_t j = 0; j < pl2.size(); ++j) {
                const segment<store_type> seg1 = segment<store_type>(pl1[i], pl1[i + 1]);
                const segment<store_type> seg2 = segment<store_type>(pl2[j], pl2[j + 1]);
                if (are_intersect(seg1, seg2) && !are_intersect_inf(seg1, seg2)) {
                    ret.points.push_back(
                        intersect(
                            line<store_type>(seg1),
                            line<store_type>(seg2)
                        )
                    );
                }
            }
        }
        return convex_hull(ret);
    }

    template<typename store_type>
    bool is_clockwise(const polygon<store_type>& pl) noexcept {
        const size_t extremum = std::min_element(pl.points.begin(), pl.points.end(), &lex_cmp<store_type>) - pl.points.begin();
        return clockwise(
            pl[extremum],
            pl[extremum + 1 == pl.size() ? 0 : extremum + 1],
            pl[extremum == 0 ? pl.size() - 1 : extremum]
        );
    }

    template<typename store_type>
    bool are_intersect_inf(
        const segment<store_type>& seg1,
        const segment<store_type>& seg2
    ) noexcept {
        if (
            is_zero(cross(seg1.point1 - seg2.point1, seg2.point2 - seg2.point1)) &&
            is_zero(cross(seg1.point2 - seg2.point1, seg2.point2 - seg2.point1))
        ) {
            const auto intersect = [](store_type a, store_type b, store_type c, store_type d) noexcept {
                if (a == b && b == c && c == d) {
                    return true;
                }
                if (a > b) {
                    std::swap(a, b);
                }
                if (c > d) {
                    std::swap(c, d);
                }
                return std::max(a, c) < std::min(b, d);
            };
            return intersect(seg1.point1.x, seg1.point2.x, seg2.point1.x, seg2.point2.x) &&
                   intersect(seg1.point1.y, seg1.point2.y, seg2.point1.y, seg2.point2.y);
        }
        else {
            return false;
        }
    }

    enum location : uint8_t
    {
        inside = 2,
        border = 1,
        outside = 0
    };

    template<typename store_type>
    class __are_inside_polygon
    {

    private:

        enum query_type : uint8_t
        {
            segment_erase = 0,
            segment_insert = 1,
            handle_point = 2
        };

        struct query
        {
            query_type type;
            size_t num;
            point<store_type> sort_by;
            segment<store_type> seg;

            query(
                const query_type type,
                const size_t num,
                const point<store_type>& sort_by,
                const segment<store_type>& seg
            ) noexcept : type(type), num(num), sort_by(sort_by), seg(seg) {}
        };

        struct cmp_tree_
        {
            bool strict_inside(
                const point<store_type>& pt,
                const segment<store_type>& seg
            ) const noexcept {
                const int sgn = sign(cross(-vector2<store_type>(pt), seg.point1 - pt));
                if (sgn == 0) {
                    return false;
                }
                int cur_sgn = sign(cross(seg.point1 - pt, seg.point2 - pt));
                if (cur_sgn != sgn) {
                    return false;
                }
                cur_sgn = sign(cross(seg.point2 - pt, -vector2<store_type>(pt)));
                if (cur_sgn != sgn) {
                    return false;
                }
                return true;
            }

            bool strict_outside(
                const point<store_type>& pt,
                const segment<store_type>& seg
            ) const noexcept {
                return sign(cross(vector2<store_type>(seg.point1), vector2<store_type>(pt))) ==
                       sign(cross(vector2<store_type>(pt), vector2<store_type>(seg.point2))) &&
                       sign(cross(vector2<store_type>(pt), vector2<store_type>(seg.point2))) == 1 &&
                       sign(cross(seg.point1 - pt, seg.point2 - pt)) == -1;
            }

            bool side_segment(
                const point<store_type>& pt1,
                const point<store_type>& pt2
            ) const noexcept {
                return is_zero(cross(-vector2<store_type>(pt1), pt2 - pt1)) &&
                       less_zero(dot(-vector2<store_type>(pt1), pt2 - pt1));
            }

            bool operator() (const segment<store_type>& lhs, const segment<store_type>& rhs) const noexcept {
                return strict_inside(lhs.point1, rhs) ||
                       strict_inside(lhs.point2, rhs) ||
                       strict_outside(rhs.point1, lhs) ||
                       strict_outside(rhs.point2, lhs) ||
                       (lhs.point1 != rhs.point1 && side_segment(lhs.point1, rhs.point1)) ||
                       (lhs.point1 != rhs.point2 && side_segment(lhs.point1, rhs.point2)) ||
                       (lhs.point2 != rhs.point1 && side_segment(lhs.point2, rhs.point1)) ||
                       (lhs.point2 != rhs.point2 && side_segment(lhs.point2, rhs.point2));
            }
        };

        struct lex_cmp_
        {
            bool operator() (const point<store_type>& lhs, const point<store_type>& rhs) const noexcept {
                return lex_cmp(lhs, rhs);
            }
        };

        struct cmp_sort_
        {
            bool operator() (const query& lhs, const query& rhs) const noexcept {
                const store_type crs = cross(
                    vector2<store_type>(lhs.sort_by),
                    vector2<store_type>(rhs.sort_by)
                );
                if (is_zero(crs)) {
                    return lhs.type < rhs.type;
                }
                else {
                    return greater_zero(crs);
                }
            }
        };

        const std::vector<point<store_type>>& _pt;
        const polygon<store_type>& _pl;
        std::vector<stlb::location> ret;
        point<store_type> origin;
        std::vector<query> _qrs;
        __gnu_pbds::tree<
            segment<store_type>, __gnu_pbds::null_type, cmp_tree_,
            __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update
        > segments;
        std::set<
            point<store_type>,
            lex_cmp_
        > _point_st;

        void find_origin_() noexcept {
            origin = _pl[0];
            for (size_t i = 1; i < _pl.size(); ++i) {
                origin.x = std::min(origin.x, _pl[i].x);
                origin.y = std::min(origin.y, _pl[i].y);
            }
            for (size_t i = 0; i < _pt.size(); ++i) {
                origin.x = std::min(origin.x, _pl[i].x);
                origin.y = std::min(origin.y, _pt[i].y);
            }
            origin.x -= 1;
            origin.y -= _pl.size() + 1;
            std::vector<bool> used(_pl.size() + 1, false);
            for (size_t i = 0; i < _pl.size(); ++i) {
                const line<store_type> ln(_pl[i], _pl[i + 1]);
                if (is_zero(ln.B)) {
                    continue;
                }
                const store_type val = -ln.A * origin.x - ln.C;
                const store_type ind = static_cast<ssize_t>(val / ln.B - origin.y);
                if (is_zero(val % ln.B) && 0 <= ind && static_cast<size_t>(ind) < used.size()) {
                    used[ind] = true;
                }
            }
            origin.y += std::find_if_not(used.begin(), used.end(), [](const bool x) {
                return x;
            }) - used.begin();
        }

        void fill_and_sort_query_() noexcept {
            for (size_t i = 0; i < _pl.size(); ++i) {
                _point_st.insert(_pl[i] - origin);
                const point<store_type> pt1(_pl[i] - origin);
                const point<store_type> pt2(_pl[i + 1] - origin);
                if (greater_zero(cross(vector2<store_type>(pt1), vector2<store_type>(pt2)))) {
                    const segment<store_type> seg(pt1, pt2);
                    _qrs.emplace_back(query_type::segment_erase, 0, pt2, seg);
                    _qrs.emplace_back(query_type::segment_insert, 0, pt1, seg);
                }
                else {
                    const segment<store_type> seg(pt2, pt1);
                    _qrs.emplace_back(query_type::segment_erase, 0, pt1, seg);
                    _qrs.emplace_back(query_type::segment_insert, 0, pt2, seg);
                }
            }
            for (size_t i = 0; i < _pt.size(); ++i) {
                const point<store_type> pt(_pt[i] - origin);
                _qrs.emplace_back(query_type::handle_point, i, pt, segment<store_type>(pt, pt));
            }
            std::sort(_qrs.begin(), _qrs.end(), cmp_sort_());
        }

        void handle_queries_() noexcept {
            ret.resize(_pt.size());
            for (const auto& i : _qrs) {
                switch (i.type) {
                    case query_type::segment_erase: {
                        segments.erase(i.seg);
                        break;
                    }
                    case query_type::segment_insert: {
                        segments.insert(i.seg);
                        break;
                    }
                    case query_type::handle_point: {
                        size_t order = segments.order_of_key(i.seg);
                        auto it = segments.find_by_order(order);
                        if ((it != segments.end() && are_intersect(i.seg.point1, *it)) || _point_st.find(i.seg.point1) != _point_st.end()) {
                            ret[i.num] = stlb::location::border;
                        }
                        else {
                            ret[i.num] = (order % 2 == 1 ? stlb::location::inside : stlb::location::outside);
                        }
                        break;
                    }
                }
            }
        }

    public:

        __are_inside_polygon(
            const std::vector<point<store_type>>& pt,
            const polygon<store_type>& pl
        ) noexcept : _pt(pt), _pl(pl) {}

        std::vector<stlb::location> calculate() noexcept {
            find_origin_();
            fill_and_sort_query_();
            handle_queries_();
            return ret;
        }

    };

    template<typename store_type>
    std::vector<stlb::location> are_inside_polygon(
        const std::vector<point<store_type>>& pt,
        const polygon<store_type>& pl
    ) noexcept {
        return __are_inside_polygon<store_type>(pt, pl).calculate();
    }

}
