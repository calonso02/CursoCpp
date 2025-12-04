#include <string>
#include <regex>
#include <iostream>

class SeenObject
{
private:
    std::string name; // Nombre (ej: "f c")
    double dist;      // Distancia (metros)
    double angle;     // Ángulo (grados)
public:
    SeenObject(const std::string &n) : name(n) {}
    void parse_message(const std::string &s);
    // Devuelve el angulo relativo al jugador
    double get_dist() const;
    // Devuelve la distancia relativa al jugador
    double get_angle() const;
};

inline double SeenObject::get_dist() const
{
    return dist;
}

inline double SeenObject::get_angle() const
{
    return angle;
}

void SeenObject::parse_message(const std::string &s)
{
    std::string pattern_str = R"(\(\()" + name + R"(\)\s+([-+]?\d*\.?\d+)\s+([-+]?\d*\.?\d+)(?:\s+([-+]?\d*\.?\d+)\s+([-+]?\d*\.?\d+))?\))";

    std::regex regex(pattern_str);
    std::smatch match;

    if (std::regex_search(s, match, regex))
    {
        if (match[1].matched && match[2].matched)
        {
            std::string dist_str = match[1].str();
            std::string angle_str = match[2].str();

            dist = std::stod(dist_str);
            angle = std::stod(angle_str);
        }
    }
}