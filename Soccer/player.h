#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "vector2D.h"


class Player
{
public:
    Player() : posicionGlobal{0.0, 0.0} {}

    const std::string &getUnum() const { return unum; };
    const std::string &getPlaymode() const { return playmode; };
    const std::string &getSide() const { return side; }
    const std::string &getCommand() const { return command; }
    const Vector2D<double> &getPosicionGlobal() const { return posicionGlobal; }

    void setUnum(const std::string &s) { unum = s; }
    void setPlaymode(const std::string &s) { playmode = s; }
    void setSide(const std::string &s) { side = s; }
    void setCommand(const std::string &s) { command = s; }

    std::string irA(Vector2D<double> objetivo);

    void actualizarEstadoVisual(const std::string &serverMsg);

    void parseInit(const std::string &msg);

    void posicionInicial() { command = posicionesIniciales.at(std::stoi(unum)); }

    void decision();

private:
    std::string unum, playmode, side, command;
    Vector2D<double> posicionGlobal;
    bool posicionEsValida = false;

    static std::vector<std::string> posicionesIniciales;
};

#endif