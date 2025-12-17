#include "msg_parser.h"

#include <algorithm>
#include <regex>
#include <iostream>
#include <sstream>
#include <memory>

void parse_init(const std::string &msg, Player &player)
{
    std::string clean_msg = msg;
    std::replace(clean_msg.begin(), clean_msg.end(), '(', ' ');
    std::replace(clean_msg.begin(), clean_msg.end(), ')', ' ');

    std::istringstream iss(clean_msg);

    std::string initWord, side, unum, playmode;
    iss >> initWord >> side >> unum >> playmode;

    // Validaciones mínimas
    if (initWord != "init")
    {
        std::cerr << "Invalid init message: " << msg << std::endl;
        return;
    }

    if (!side.empty())
        player.setSide(side[0]);

    try
    {
        player.setUnum(std::stoi(unum));
    }
    catch (...)
    {
        std::cerr << "Invalid unum: " << unum << std::endl;
        return;
    }

    player.setPlaymode(playmode);
}

void parseSeeMessage(const std::string &msg, Player &player)
{
    player.get_balon().set_visible(false);
    player.get_flags_vistas().clear();
    player.get_jugadores_vistos().clear();
    player.setPorteriaPtr(nullptr);

    std::regex re(R"(\(\(([^)]+)\)\s+([-\d\.]+)\s+([-\d\.]+))");
    std::smatch match;
    std::string::const_iterator searchStart(msg.cbegin());
    while (std::regex_search(searchStart, msg.cend(), match, re))
    {
        std::string name = match[1].str();
        double dist = std::stod(match[2].str());
        double dir = std::stod(match[3].str());
        if (name == "b")
        {
            player.get_balon().set_visible(true);
            player.get_balon().set_name(name);
            player.get_balon().set_angulo(dir);
            player.get_balon().set_distancia(dist);
        }
        else
        {
            std::regex rePlayer(R"(p\s+\"([^"]+)\"(?:\s+(\d+))?(?:\s+goalie)?)");
            std::smatch match;
            if (std::regex_match(name, match, rePlayer))
            {
                std::string teamName = match[1];
                player.get_jugadores_vistos().push_back(PlayerObject{name, dir, dist, teamName});
            }
            else
            {
                FieldObject nuevo{name, dir, dist};
                player.get_flags_vistas().push_back(std::move(nuevo));
                if (name == player.get_porteria_objetivo())
                    player.setPorteriaPtr(std::make_shared<FieldObject>(player.get_flags_vistas().back()));
            }
        }
        searchStart = match.suffix().first;
    }
}