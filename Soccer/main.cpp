#include <iostream>
#include <sstream>
#include <string>
#include <MinimalSocket/udp/UdpSocket.h>
#include <vector>
#include <regex>
#include "seen_object.cpp"

using namespace std;

struct Player
{

    string unum, playmode, side;

    void ParseInit(string msg)
    {
        istringstream iss(msg);
        std::string initWord;
        iss >> initWord >> side >> unum >> playmode;
    }

    string PosicionInicial()
    {

        string command{};

        if (side == "r")
        {
            switch (std::stoi(unum))
            {
            case 1:
                command = "(move 53 0)";
                break; // Portero
            case 2:
                command = "(move 42 -22)";
                break; // Lateral Derecho
            case 3:
                command = "(move 42 22)";
                break; // Lateral Izquierdo
            case 4:
                command = "(move 46 -8)";
                break; // Central Derecho
            case 5:
                command = "(move 46 8)";
                break; // Central Izquierdo
            case 6:
                command = "(move 35 0)";
                break; // Pivote (MCD)
            case 7:
                command = "(move 10 -28)";
                break; // Extremo Derecho
            case 8:
                command = "(move 25 -10)";
                break; // Interior Derecho
            case 9:
                command = "(move 5 0)";
                break; // Delantero Centro
            case 10:
                command = "(move 25 10)";
                break; // Interior Izquierdo
            case 11:
                command = "(move 10 28)";
                break; // Extremo Izquierdo
            default:
                break;
            }
        }
        else
        {
            switch (std::stoi(unum))
            {
            case 1:
                command = "(move -53 0)";
                break; // Portero
            case 2:
                command = "(move -42 -22)";
                break; // Lateral Derecho
            case 3:
                command = "(move -42 22)";
                break; // Lateral Izquierdo
            case 4:
                command = "(move -46 -8)";
                break; // Central Derecho
            case 5:
                command = "(move -46 8)";
                break; // Central Izquierdo
            case 6:
                command = "(move -35 0)";
                break; // Pivote (MCD)
            case 7:
                command = "(move -10 -28)";
                break; // Extremo Derecho
            case 8:
                command = "(move -25 -10)";
                break; // Interior Derecho
            case 9:
                command = "(move -5 0)";
                break; // Delantero Centro
            case 10:
                command = "(move -25 10)";
                break; // Interior Izquierdo
            case 11:
                command = "(move -10 28)";
                break; // Extremo Izquierdo
            default:
                break;
            }
        }
        cout << command << endl;
        return command;
    }
};

ostream &operator<<(ostream &os, const Player &player)
{
    os << "Jugador pos " << player.unum << " Lado " << player.side << " Play Mode " << player.playmode << endl;
    return os;
}

bool isSeeComand(const string &s)
{
    std::regex seeRegex("^\\(see\\s");
    return std::regex_search(s, seeRegex);
}

/*bool isFacingBall(const string &s)
{
    std::regex ballRegex(
        R"(\(\(b\)\s+([-\d\.]+)\s+([-\d\.]+)(?:\s+([-\d\.]+)\s+([-\d\.]+))?)");
    std::smatch match;

    if (std::regex_search(s, match, ballRegex))
    {
        auto distance = std::stod(match[1]);
        auto direction = std::stod(match[2]);

        return direction <= 15 && direction >= -15;
    }
    else
    {
        return false;
    }
}

string getGoalDir(const string &s, const Player &p)
{
    std::regex goalRegexR(
        R"(\(\(g\sr\)\s+([-\d\.]+)\s+([-\d\.]+)(?:\s+([-\d\.]+)\s+([-\d\.]+))?)");

    std::regex goalRegexL(
        R"(\(\(g\sl\)\s+([-\d\.]+)\s+([-\d\.]+)(?:\s+([-\d\.]+)\s+([-\d\.]+))?)");

    std::smatch match;
    if (p.side == "l")
    {
        if (std::regex_search(s, match, goalRegexR))
        {
            auto distance = std::stod(match[1]);
            auto direction = match[2];

            return direction;
        }
    }
}*/

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

    std::cout << received_message_content << std::endl;

    Player player;
    player.ParseInit(received_message_content);
    // cout << player;

    // update upd port to provided by the other udp
    MinimalSocket::Address other_sender_udp = received_message->sender;
    MinimalSocket::Address server_udp = MinimalSocket::Address{"127.0.0.1", other_sender_udp.getPort()};

    udp_socket.sendTo(player.PosicionInicial(), server_udp);
    SeenObject balon("b");
    SeenObject porteriaDer("g r");
    SeenObject porteriaIzq("g l");

    while (true)
    {
        auto received_message = udp_socket.receive(message_max_size);
        std::string received_message_content = received_message->received_message;
        if (isSeeComand(received_message_content))
        {
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
                        double angle_to_goal = (player.side == "l") ? porteriaDer.get_angle() : porteriaIzq.get_angle();
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
    }
    return 0;
}