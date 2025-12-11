#ifndef SEEN_OBJECT_H
#define SEEN_OBJECT_H

#include <string>

class SeenObject
{
private:
    std::string name;
    double dist;
    double angle;

public:
    SeenObject(const std::string &n) : name(n) {}
    SeenObject(const std::string &n, double d, double a) : name(n), dist(d), angle(a) {}
    

    bool parse_message(const std::string &s);

    const std::string& get_name() const { return name; }
    double get_dist() const { return dist; }
    double get_angle() const { return angle; }
};

#endif