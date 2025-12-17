#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "types.h"
#include <string>
#include <memory>

std::string controlSaque(const Player &player);
std::string controlPortero(const Player &player);
std::string controlDefensa(Player &player);
std::string controlCentrocampista(Player &player);
std::string controlDelantero(Player &player);

std::string controlBalonParado(const Player &player);
bool esNuestroBalon(const Player &player);

std::shared_ptr<const PlayerObject> buscar_mejor_companero(const std::string& team, const std::vector<PlayerObject>& jugadores_vistos);
bool contrarios_delante(const std::string &team, const std::vector<PlayerObject> &jugadores_vistos);
#endif