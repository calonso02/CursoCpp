#include "player_controller.h"
#include "trilaterate.h"
#include <iostream>
#include <algorithm>

const int CATCHABLE_AREA_L = 2;
const int CATCHABLE_AREA_W = 1;

std::string controlSaque(const Player &player)
{
    std::string comando{};
    auto balon = player.get_balon();
    if (!balon.es_visible())
    {
        comando = "(turn 15)";
    }
    else
    {
        switch (player.get_unum())
        {
        case 9:
            if (balon.get_dist() <= 0.6)
                comando = "(kick 60 -180)";
            else
                comando = "(dash 50 " + std::to_string(balon.get_angulo()) + ")";
            break;
        case 7:
        case 11:
            comando = "(dash 100 0)";
            break;

        default:
            if (balon.get_angulo() > 30)
                comando = "(turn " + std::to_string(balon.get_angulo()) + ")";

            break;
        }
    }
    return comando;
}

std::string controlPortero(const Player &player)
{
    auto balon = player.get_balon();
    auto distancia_balon = balon.get_dist();
    auto angulo_balon = balon.get_angulo();

    if (distancia_balon <= 0.6)
    {
        auto mejor_compi = buscar_mejor_companero(player.get_team_name(), player.get_jugadores_vistos());
        if (mejor_compi)
            return "(kick 50 " + std::to_string(mejor_compi->get_angulo()) + ")";
        else
        {
            return "(kick 100 0)";
        }
    }
    else if (distancia_balon <= CATCHABLE_AREA_L)
        return "(catch " + std::to_string(angulo_balon) + ")";
    else if (distancia_balon <= 52)
    {
        if (angulo_balon > 0)
            return "(dash 25 90)";
        else if (angulo_balon < 0)
            return "(dash 25 -90)";
        else
            return "(turn 0)";
    }
    else
        return "(turn 0)";
}

std::string controlDefensa(Player &player)
{
    auto balon = player.get_balon();
    auto distancia_balon = balon.get_dist();
    auto angulo_balon = balon.get_angulo();

    if (balon.es_visible())
    {
        if (distancia_balon <= 0.6)
        {
            player.setPosicionado(false);
            if (player.get_porteria_ptr()) // Si estoy viendo la porteria
            {
                auto porteria = player.get_porteria_ptr();
                if (porteria->get_dist() <= 25.0) // Si estoy cerca disparo
                    return "(kick 100 " + std::to_string(porteria->get_angulo()) + ")";
                else
                {

                    auto mejor_compi = buscar_mejor_companero(player.get_team_name(), player.get_jugadores_vistos());
                    if (mejor_compi)
                        return "(kick 50 " + std::to_string(mejor_compi->get_angulo()) + ")";
                    else
                        return "(kick 100 " + std::to_string(porteria->get_angulo()) + ")";
                }
            }
            else
            {
                auto mejor_compi = buscar_mejor_companero(player.get_team_name(), player.get_jugadores_vistos());
                if (mejor_compi)
                    return "(kick 50 " + std::to_string(mejor_compi->get_angulo()) + ")";
                else
                    return "(dash 100 0)";
            }
        }
        else if (distancia_balon <= 5.0)
        {
            player.setPosicionado(false);
            if (angulo_balon > 5.0)
                return "(turn " + std::to_string(angulo_balon) + ")";
            else
                return "(dash 75 " + std::to_string(angulo_balon) + ")";
        }
        else if (distancia_balon <= 30)
        {
            player.setPosicionado(false);
            if (angulo_balon > 5.0)
                return "(turn " + std::to_string(angulo_balon) + ")";
            else
                return "(dash 40 0)";
        }
        else
            return "(turn 0)";
    }
    else
    {
        if (player.estoy_posicionado())
            return "(turn 15)";
        else
        {
            auto flags = player.get_flags_vistas();
            auto it = std::find(flags.begin(), flags.end(), player.get_target_flag());
            if (it == flags.end())
                return "(turn 15)";
            else
            {
                auto dist = it->get_dist();
                auto ang = it->get_angulo();
                if (dist <= 20)
                {
                    player.setPosicionado(true);
                    return "(turn 15)";
                }
                else
                    return "(dash 50 " + std::to_string(ang) + ")";
            }
        }
    }
}

std::string controlCentrocampista(Player &player)
{
    auto balon = player.get_balon();
    auto distancia_balon = balon.get_dist();
    auto angulo_balon = balon.get_angulo();

    if (balon.es_visible())
    {
        if (distancia_balon <= 0.6)
        {
            player.setPosicionado(false);
            if (player.get_porteria_ptr()) // Si estoy viendo la porteria
            {
                auto porteria = player.get_porteria_ptr();
                if (porteria->get_dist() <= 25.0) // Si estoy cerca disparo
                    return "(kick 100 " + std::to_string(porteria->get_angulo()) + ")";
                else
                {

                    auto mejor_compi = buscar_mejor_companero(player.get_team_name(), player.get_jugadores_vistos());
                    if (mejor_compi)
                        return "(kick 50 " + std::to_string(mejor_compi->get_angulo()) + ")";
                    else
                        return "(kick 15 " + std::to_string(porteria->get_angulo()) + ")";
                }
            }
            else
            {
                    return "(turn 25)";
            }
        }
        else if (distancia_balon <= 15.0)
        {
            player.setPosicionado(false);
            if (angulo_balon > 5.0)
                return "(turn " + std::to_string(angulo_balon) + ")";
            else
                return "(dash 75 " + std::to_string(angulo_balon) + ")";
        }
        else if (distancia_balon <= 30)
        {
            player.setPosicionado(false);
            if (angulo_balon > 5.0)
                return "(turn " + std::to_string(angulo_balon) + ")";
            else
                return "(dash 40 0)";
        }
        else
            return "(turn 0)";
    }
    else
    {
        if (player.estoy_posicionado())
            return "(turn 15)";
        else
        {
            auto flags = player.get_flags_vistas();
            auto it = std::find(flags.begin(), flags.end(), player.get_target_flag());
            if (it == flags.end())
                return "(turn 15)";
            else
            {
                auto dist = it->get_dist();
                auto ang = it->get_angulo();
                if (dist <= 20)
                {
                    player.setPosicionado(true);
                    return "(turn 15)";
                }
                else
                    return "(dash 50 " + std::to_string(ang) + ")";
            }
        }
    }
}

std::string controlDelantero(Player &player)
{
    auto balon = player.get_balon();
    auto distancia_balon = balon.get_dist();
    auto angulo_balon = balon.get_angulo();

    if (balon.es_visible())
    {
        if (distancia_balon <= 0.7)
        {
            player.setPosicionado(false);
            if (player.get_porteria_ptr()) // Si estoy viendo la porteria
            {
                auto porteria = player.get_porteria_ptr();
                if (porteria->get_dist() <= 25.0) // Si estoy cerca disparo
                    return "(kick 100 " + std::to_string(porteria->get_angulo()) + ")";
                else
                    return "(kick 25 " + std::to_string(porteria->get_angulo()) + ")";
            }
            else
                return "(dash 100 " + std::to_string(angulo_balon) + ")";
        }
        else if (distancia_balon <= 15.0)
        {
            player.setPosicionado(false);
            if (angulo_balon > 5.0)
                return "(turn " + std::to_string(angulo_balon) + ")";
            else
                return "(dash 100 " + std::to_string(angulo_balon) + ")";
        }
        else if (distancia_balon <= 30)
        {
            player.setPosicionado(false);
            if (angulo_balon > 5.0)
                return "(turn " + std::to_string(angulo_balon) + ")";
            else
                return "(dash 40 0)";
        }
        else
            return "(turn 0)";
    }
    else
    {
        if (player.estoy_posicionado())
            return "(turn 15)";
        else
        {
            auto flags = player.get_flags_vistas();
            auto it = std::find(flags.begin(), flags.end(), player.get_target_flag());
            if (it == flags.end())
                return "(turn 15)";
            else
            {
                auto dist = it->get_dist();
                auto ang = it->get_angulo();
                if (dist <= 20)
                {
                    player.setPosicionado(true);
                    return "(turn 15)";
                }
                else
                    return "(dash 50 " + std::to_string(ang) + ")";
            }
        }
    }
}

bool contrarios_delante(const std::string &team, const std::vector<PlayerObject> &jugadores_vistos)
{
    bool contrarios_delante{false};

    for (const auto &jugador : jugadores_vistos)
    {
        if (jugador.get_team() != team)
        {
            if (std::abs(jugador.get_angulo()) < 25.0 && jugador.get_dist() <= 20.0)
            {
                contrarios_delante = true;
            }
        }
    }
    return contrarios_delante;
}

std::shared_ptr<const PlayerObject> buscar_mejor_companero(const std::string &team, const std::vector<PlayerObject> &jugadores_vistos)
{
    std::shared_ptr<PlayerObject> mejorCompanero = nullptr;
    double mejorDistancia = 999.0;

    // Recorremos todo lo que vemos
    for (const auto &jugador : jugadores_vistos)
    {
        if (jugador.get_team() == team)
        {
            if (std::abs(jugador.get_angulo()) < 90.0 && jugador.get_dist() > 5.0)
            {
                if (jugador.get_dist() < mejorDistancia)
                {
                    mejorDistancia = jugador.get_dist();
                    mejorCompanero = std::make_shared<PlayerObject>(jugador);
                }
            }
        }
    }
    return mejorCompanero;
}

bool esNuestroBalon(const Player &player)
{
    std::string sufijo = "_" + player.get_side();
    if (player.get_playmode().length() >= sufijo.length())
    {
        return (0 == player.get_playmode().compare(player.get_playmode().length() - sufijo.length(), sufijo.length(), sufijo));
    }
    return false;
}

std::string controlBalonParado(const Player &player)
{
    auto balon = player.get_balon();
    if (!balon.es_visible())
    {
        return "(turn 30)";
    }
    // Ataque
    if (esNuestroBalon(player))
    {
        bool esPenalty = (player.get_playmode().find("penalty_kick") != std::string::npos);
        bool esCorner = (player.get_playmode().find("corner_kick") != std::string::npos);
        bool esSaquePuerta = (player.get_playmode().find("goal_kick") != std::string::npos);
        bool esPase = (player.get_playmode().find("kick_in") != std::string::npos ||
                       player.get_playmode().find("free_kick") != std::string::npos ||
                       player.get_playmode().find("back_pass") != std::string::npos ||
                       player.get_playmode().find("offside") != std::string::npos ||
                       player.get_playmode().find("foul") != std::string::npos);

        bool soyElElegido = false;

        if (esSaquePuerta)
            soyElElegido = (player.get_unum() == 1);
        else if (esPenalty)
            soyElElegido = (player.get_unum() == 9);
        else
            soyElElegido = (balon.get_dist() < 2.0);

        if (soyElElegido)
        {
            if (balon.get_dist() <= 0.6)
            {
                if (esPenalty || esCorner)
                {
                    auto porteria = player.get_porteria_ptr();

                    if (porteria)
                    {
                        if (esPenalty)
                            return "(kick 100 " + std::to_string(porteria->get_angulo()) + ")";
                        else
                            return "(kick 80 " + std::to_string(porteria->get_angulo()) + ")";
                    }
                    else
                    {
                        auto mejor_compi = buscar_mejor_companero(player.get_team_name(), player.get_jugadores_vistos());
                        if (mejor_compi)
                            return "(kick 80 " + std::to_string(mejor_compi->get_angulo()) + ")";
                        return "(turn 20)";
                    }
                }
                else
                {
                    auto mejor_compi = buscar_mejor_companero(player.get_team_name(), player.get_jugadores_vistos());
                    if (mejor_compi)
                        return "(kick 70 " + std::to_string(mejor_compi->get_angulo()) + ")";
                    else
                        return "(kick 60 45)";
                }
            }
            else
                return "(dash 60 " + std::to_string(balon.get_angulo()) + ")";
        }
        else
            return "(turn " + std::to_string(balon.get_angulo()) + ")";
    }
    else
    {
        bool esPenalty = (player.get_playmode().find("penalty_kick") != std::string::npos);
        if (esPenalty && player.get_unum() == 1)
            return "(turn " + std::to_string(balon.get_angulo()) + ")";
        return "(turn " + std::to_string(balon.get_angulo()) + ")";
    }
}
