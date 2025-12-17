#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>
#include <iostream>
#include <memory>

struct Punto
{
    Punto(double x, double y) : x{x}, y{y} {}

    double x, y;
};

struct HearMessage
{
    int time;
    std::string sender;
    std::string message;

    void parseHear(const std::string &msg);
};

class FieldObject
{
public:
    FieldObject() : name{}, ang{}, dist{} {}
    FieldObject(const std::string &s, double a, double d) : name{s}, ang{a}, dist{d} {}

    void set_name(const std::string &n) { name = n; }
    void set_angulo(double a) { ang = a; }
    void set_distancia(double d) { dist = d; }

    double get_angulo() const { return ang; }
    double get_dist() const { return dist; }
    const std::string &get_name() const { return name; }

private:
    std::string name;
    double ang, dist;
};

bool operator==(const FieldObject &obj1, const std::string &s);

class BallObject : public FieldObject
{
public:
    bool es_visible() const { return visible; }
    void set_visible(bool b) { visible = b; }


private:
    bool visible{false};
};

class PlayerObject : public FieldObject
{
public:
    PlayerObject(const std::string &s, double a, double d, const std::string &t) : FieldObject{s, a, d}, team{t} {}

    const std::string get_team() const { return team; }

private:
    std::string team;
};

class Player
{
public:
    Player(const std::string &team, const std::string &init_mgs);

    const std::string &get_team_name() const { return team_name; }
    int get_unum() const { return unum; }
    char get_side() const { return side; }
    const std::string &get_playmode() const { return playmode; }
    std::string get_posicion_inicial() const { return "(move " + posiciones_iniciales[unum] + ")"; }
    const std::vector<FieldObject> &get_flags_vistas() const { return flags_vistas; }
    const std::vector<PlayerObject> &get_jugadores_vistos() const { return jugadores_vistos; };
    const BallObject &get_balon() const { return balon; }
    const std::string &get_porteria_objetivo() const { return porteria_objetivo; }
    std::shared_ptr<const FieldObject> get_porteria_ptr() const { return porteria_objetivo_ptr; }
    bool estoy_posicionado() const { return posicionado; }
    const std::string &get_target_flag() const
    {
        if (side == 'l')
            return target_flag_l.at(unum);
        else
            return target_flag_r.at(unum);
    }

    std::vector<FieldObject> &get_flags_vistas() { return flags_vistas; }
    std::vector<PlayerObject> &get_jugadores_vistos() { return jugadores_vistos; };
    BallObject &get_balon() { return balon; }

    void setUnum(int num) { unum = num; }
    void setSide(char s) { side = s; }
    void setPlaymode(const std::string &s) { playmode = s; }
    void setPorteriaPtr(std::shared_ptr<FieldObject> ptr) { porteria_objetivo_ptr = ptr; }
    void setPosicionado(bool f) { posicionado = f; }

private:
    static const std::vector<std::string> posiciones_iniciales;
    static const std::vector<std::string> target_flag_l;
    static const std::vector<std::string> target_flag_r;

    std::string team_name, playmode;
    int unum;
    char side;
    bool posicionado = false;

    std::string porteria_objetivo;
    std::shared_ptr<FieldObject> porteria_objetivo_ptr;

    std::vector<FieldObject> flags_vistas{};
    std::vector<PlayerObject> jugadores_vistos{};
    BallObject balon{};
};

// ostream& operator<< (ostream& os, const Player& player);
#endif