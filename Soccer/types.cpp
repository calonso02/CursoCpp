#include "types.h"
#include "msg_parser.h"
#include "trilaterate.h"

#include <regex>
#include <iostream>
#include <sstream>
#include <cmath>

const std::vector<std::string> Player::posiciones_iniciales = {
    "",        // vacio
    "-48 0",   // Portero
    "-38 -22", // Lateral Derecho
    "-38 22",  // Lateral Izquierdo
    "-42 -8",  // Central Derecho
    "-42 8",   // Central Izquierdo
    "-32 0",   // Pivote (MCD
    "-10 -32", // Extremo Derecho
    "-25 -20", // Interior Derecho
    "-5 0",    // Delantero Centro
    "-25 20",  // Interior Izquierdo
    "-10 32"   // Extremo Izquierdo
};

const std::vector<std::string> Player::target_flag_l = {
    "",        // vacio
    "",   // Portero
    "f p l t", // Lateral Derecho
    "f p l b",  // Lateral Izquierdo
    "f l t 10",  // Central Derecho
    "f l b 10",   // Central Izquierdo
    "f c",   // Pivote (MCD
    "f p r t", // Extremo Derecho
    "f c t", // Interior Derecho
    "f p r c",    // Delantero Centro
    "f c b",  // Interior Izquierdo
    "f p r b"   // Extremo Izquierdo
};

const std::vector<std::string> Player::target_flag_r = {
    "",        // vacio
    "",   // Portero
    "f p r t", // Lateral Derecho
    "f p r b",  // Lateral Izquierdo
    "f r t 10",  // Central Derecho
    "f r b 10",   // Central Izquierdo
    "f c",   // Pivote (MCD
    "f p l t", // Extremo Derecho
    "f c t", // Interior Derecho
    "f p l c",    // Delantero Centro
    "f c b",  // Interior Izquierdo
    "f p l b"   // Extremo Izquierdo
};

Player::Player(const std::string &team, const std::string &init_mgs) : team_name{team}
{
    parse_init(init_mgs, *this);
    auto lado = (this->side == 'l') ? "r" : "l";
    porteria_objetivo = std::string("g ") + lado;
}
/*
std::ostream& operator<< (std::ostream& os, const Player& player)
{
    os << "Jugador: " << player.get_unum() << std::endl;
    os << "Equipo: " << player.get_team_name() << std::endl;
    os << "Lado: " << player.get_side() << std::endl;
    os << "Playmode: " << player.get_playmode() << std::endl;
    return os;
}*/

bool operator== (const FieldObject& obj1, const std::string& s)
{
    return obj1.get_name() == s;
}

void HearMessage::parseHear(const std::string &msg)
{
    std::string aux = msg;

    // Reemplazar paréntesis por espacios
    std::replace(aux.begin(), aux.end(), '(', ' ');
    std::replace(aux.begin(), aux.end(), ')', ' ');

    std::stringstream ss(aux);

    std::string hearKeyword;

    ss >> hearKeyword;
    ss >> time;
    ss >> sender;
    ss >> message;
}