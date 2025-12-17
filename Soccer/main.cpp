#include "tictoc.h"
#include "types.h"
#include "msg_parser.h"
#include "player_controller.h"
#include <MinimalSocket/udp/UdpSocket.h>
#include <iostream>
#include <string>

using namespace std;

void imprimir_vistas(const std::vector<FieldObject> &flags_vistas,
                     const std::vector<PlayerObject> &jugadores_vistos)
{
    std::cout << "Flags vistas:\n";
    for (const auto &flag : flags_vistas)
    {
        std::cout << flag.get_name() << '\n';
    }

    std::cout << "\nJugadores vistos:\n";
    for (const auto &jugador : jugadores_vistos)
    {
        std::cout << jugador.get_team() << ' ' << jugador.get_dist() << ' ' << jugador.get_angulo() << "\n";
    }
}

int main(int argc, char *argv[])
{
    /******************************************** Conexion con el servidor ****************************************************/
    if (argc != 4)
    {
        cout << "Usage: " << argv[0] << " <team-name> <this-port> <player-num>" << endl;
        return 1;
    }
    string team_name = argv[1];
    MinimalSocket::Port this_socket_port = std::stoi(argv[2]);
    string proc_id = argv[3];
    cout << "Creating a UDP socket" << endl;
    MinimalSocket::udp::Udp<true> udp_socket(this_socket_port, MinimalSocket::AddressFamily::IP_V6);
    cout << "Socket created" << endl;
    bool success = udp_socket.open();
    if (!success)
    {
        cout << "Error opening socket" << endl;
        return 1;
    }
    MinimalSocket::Address other_recipient_udp = MinimalSocket::Address{"127.0.0.1", 6000};
    string init_msg{"(init " + team_name + " (version 19) "};
    if (proc_id == "0")
        init_msg += "(goalie)";
    cout << init_msg + ")" << endl;
    udp_socket.sendTo(init_msg + ")", other_recipient_udp);
    cout << "Init Message sent" << endl;
    std::size_t message_max_size = 1000;
    cout << "Waiting for a message" << endl;
    /******************************************** Recibir mensaje init ********************************************************/
    auto received_message = udp_socket.receive(message_max_size);
    std::string received_message_content = received_message->received_message;
    Player player{team_name, received_message_content};
    cout << "Jugador: " << player.get_unum() << std::endl;
    cout << "Equipo: " << player.get_team_name() << std::endl;
    cout << "Lado: " << player.get_side() << std::endl;
    cout << "Playmode: " << player.get_playmode() << std::endl;
    cout << "Porteria objetivo: " << player.get_porteria_objetivo() << std::endl;
    MinimalSocket::Address other_sender_udp = received_message->sender;
    MinimalSocket::Address server_udp = MinimalSocket::Address{"127.0.0.1", other_sender_udp.getPort()};
    /****************************************** Mandar posicion inicial *******************************************************/
    udp_socket.sendTo(player.get_posicion_inicial(), server_udp);
    TicToc clock;
    clock.tic();
    unsigned long cycle = 0;
    bool posicionados{false};
    while (true)
    {
        auto received_message = udp_socket.receive(message_max_size);
        auto msg_content{received_message->received_message};

        if (isHearCommand(msg_content))
        {
            HearMessage msg;
            msg.parseHear(msg_content);
            if (msg.sender == "referee")
                player.setPlaymode(msg.message);
        }
        else if (isSeeCommand(msg_content))
        {
            parseSeeMessage(msg_content, player);
            imprimir_vistas(player.get_flags_vistas(), player.get_jugadores_vistos());
            std::string comando{};
            if (player.get_playmode().find("kick_off_") != string::npos)
            {
                if ((player.get_playmode() == "kick_off_l" && player.get_side() == 'l') || (player.get_playmode() == "kick_off_r" && player.get_side() == 'r'))
                    comando = std::move(controlSaque(player));
                else
                {
                    auto balon = player.get_balon();
                    if (!balon.es_visible())
                        comando = "(turn 25)";
                    else
                        comando = "(turn " + to_string(balon.get_angulo()) + ")";
                }
            }
            else if (player.get_playmode().find("goal_") != string::npos)
                comando = player.get_posicion_inicial();
            else if (player.get_playmode() == "play_on")
            {

                switch (player.get_unum())
                {
                case 1:
                    comando = controlPortero(player);
                    break;

                case 2:
                case 3:
                case 4:
                case 5:
                    comando = controlDefensa(player);
                    break;

                case 6:
                case 10:
                case 8:
                    comando = controlCentrocampista(player);
                    break;

                case 9:
                case 7:
                case 11:
                    comando = controlDelantero(player);
                    break;

                default:
                    break;
                }
            }
            else if (player.get_playmode().find("kick_in") != std::string::npos ||
                     player.get_playmode().find("corner_kick") != std::string::npos ||
                     player.get_playmode().find("goal_kick") != std::string::npos ||
                     player.get_playmode().find("free_kick") != std::string::npos ||
                     player.get_playmode().find("offside") != std::string::npos ||
                     player.get_playmode().find("foul_") != std::string::npos ||
                     player.get_playmode().find("penalty_kick") != std::string::npos ||
                     player.get_playmode().find("back_pass") != std::string::npos ||
                     player.get_playmode().find("illegal_defense") != std::string::npos)
                comando = controlBalonParado(player);

            udp_socket.sendTo(comando, server_udp);
        }
    }
}