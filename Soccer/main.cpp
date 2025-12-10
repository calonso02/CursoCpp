#include <iostream>
#include <sstream>
#include <string>
#include <MinimalSocket/udp/UdpSocket.h>
#include <vector>
#include <regex>
#include "seen_object.h"
#include "player.h"

using namespace std;

bool isSeeComand(const string &s)
{
    std::regex seeRegex("^\\(see\\s");
    return std::regex_search(s, seeRegex);
}

bool isHearComand(const string &s)
{
    std::regex seeRegex("^\\(hear\\s");
    return std::regex_search(s, seeRegex);
}

// main with two args
int main(int argc, char *argv[])
{
    // check if the number of arguments is correct
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <team-name> <this-port>" << endl;
        return 1;
    }

    // get the team name and the port
    string team_name = argv[1];
    MinimalSocket::Port this_socket_port = std::stoi(argv[2]);

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
    cout << "(init " + team_name + "(version 19))";

    udp_socket.sendTo("(init " + team_name + "(version 19))", other_recipient_udp);
    cout << "Init Message sent" << endl;

    std::size_t message_max_size = 1000;
    cout << "Waiting for a message" << endl;
    auto received_message = udp_socket.receive(message_max_size);
    std::string received_message_content = received_message->received_message;

    Player player;
    player.parseInit(received_message_content);

    MinimalSocket::Address other_sender_udp = received_message->sender;
    MinimalSocket::Address server_udp = MinimalSocket::Address{"127.0.0.1", other_sender_udp.getPort()};

    player.posicionInicial();
    udp_socket.sendTo(player.getCommand(), server_udp);

    SeenObject balon("b");
    SeenObject porteriaDer("g r");
    SeenObject porteriaIzq("g l");

    while (true)
    {
        auto received_message = udp_socket.receive(message_max_size);
        std::string received_message_content = received_message->received_message;

        if (isSeeComand(received_message_content))
        {
            //player.decision(received_message_content);
            porteriaDer.parse_message(received_message_content);
            porteriaIzq.parse_message(received_message_content);
            if (balon.parse_message(received_message_content))
            {

                if (balon.get_angle() >= -10.0 && balon.get_angle() <= 10.0)
                {
                    if (std::abs(balon.get_dist()) > 1)
                    {
                        std::string turn_command = "(dash 75)";

                        udp_socket.sendTo(turn_command, server_udp);
                    }
                    else
                    {
                        double angle_to_goal = (player.getSide() == "l") ? porteriaDer.get_angle() : porteriaIzq.get_angle();
                        std::string kick_cmd = "(kick 100 " + std::to_string(angle_to_goal) + ")";
                        udp_socket.sendTo(kick_cmd, server_udp);
                    }
                }
                else
                {
                    udp_socket.sendTo("(turn 15)", server_udp);
                }
            }
            else
            {
                std::string turn_command = "(turn 25)";
                udp_socket.sendTo(turn_command, server_udp);
            }
        }
        else if (isHearComand(received_message_content))
        {
            // update play_mode
        }
    }
    return 0;
}