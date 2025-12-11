#include "player.h"
#include <regex>
#include "vector2D.h"
#include "trilaterate_utils.h"
#include "seen_object.h"
#include <vector>

void Player::actualizarEstadoVisual(const std::string &serverMsg)
{
    std::vector<SeenObject> visualData = MessageParser::parseSeeMessage(serverMsg);
    std::vector<SeenObject> usefulFlags;
    for (const auto &obj : visualData)
    {
        if (obj.get_name()[0] == 'f' || obj.get_name()[0] == 'g')
        {
            usefulFlags.push_back(obj);
        }
    }
    if (usefulFlags.size() < 2)
    {
        this->posicionEsValida = false;
        return;
    }
    SeenObject obs1 = usefulFlags[0];
    SeenObject obs2 = usefulFlags[1];
    Vector2D<double> p1 = FieldMap::getFlagPos(obs1.get_name());
    Vector2D<double> p2 = FieldMap::getFlagPos(obs2.get_name());
    auto solutions = LocalizationUtils::trilaterate(p1, obs1.get_dist(), p2, obs2.get_dist());
    if (!solutions.empty())
    {
        posicionGlobal = solutions[0];
        this->posicionEsValida = true;
    }
    else
    {
        this->posicionEsValida = false;
    }
}

std::vector<std::string> Player::posicionesIniciales = {
    "",               // vacio
    "(move -53 0)",   // Portero
    "(move -42 -22)", // Lateral Derecho
    "(move -42 22)",  // Lateral Izquierdo
    "(move -46 -8)",  // Central Derecho
    "(move -46 8)",   // Central Izquierdo
    "(move -35 0)",   // Pivote (MCD)
    "(move -10 -28)", // Extremo Derecho
    "(move -25 -10)", // Interior Derecho
    "(move -5 0)",    // Delantero Centro
    "(move -25 10)",  // Interior Izquierdo
    "(move -10 28)"   // Extremo Izquierdo
};

void Player::parseInit(const std::string &msg)
{
    std::istringstream iss(msg);
    std::string initWord;
    iss >> initWord >> side >> unum >> playmode;
}

std::ostream &operator<<(std::ostream &os, const Player &player)
{
    os << "Jugador pos " << player.getUnum() << " Lado " << player.getSide() << " Play Mode " << player.getPlaymode() << std::endl;
    return os;
}

void Player::decision(const std::string &msg)
{
    if (playmode == "before_kick_off" || playmode == "goal_r_?" || playmode == "goal_l_?") 
    {
        posicionInicial();
    }
    if (playmode == "kick_off_L" && side == "l" ||
        playmode == "kick_off_R" && side == "r")
    {
        // Logica para el saque
    }
    else if (playmode == "play_on")
    {
        switch (std::stoi(unum))
        {
        case 1: // Portero

            break;

        case 2:
        case 3:
        case 4:
        case 5: // Defensas
            
            break;

        case 6:
        case 7:
        case 8: // Centrocampistas

            break;

        case 9:
        case 10:
        case 11: // Delanteros

            break;

        default:
            break;
        }
    }
}