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

void Player::decision()
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

// Mueve al jugador hacia un objetivo usando vectores
std::string Player::irA(Vector2D<double> objetivo) {
    // Calculamos el vector error
    Vector2D<double> error = objetivo - posicionGlobal;
    double distancia = sqrt(error.x*error.x + error.y*error.y);
    double angulo = error.getAngle() * 180.0 / M_PI; // Convertir a grados
    
    // Controlador Proporcional simple (P-Controller)
    if (abs(angulo) > 10.0) {
        // Si no miro al objetivo, giro
        return "(turn " + std::to_string(angulo) + ")";
    } else {
        return "(dash 100 )";
    }
}

/*
std::string Player::controlPortero(const std::map<std::string, std::pair<double,double>>& positions)
{   
    SeenObject balon("b");
    // Obtener posiciones del map

    double posX = getposicionglobal().x;
    double posY = getposicionglobal().y;

    // Línea del portero
    double targetX = -52.0;
    double targetY = ballY * 0.4;


    // Si está cerca → intenta atrapar balón
    if (balon.get_dist() < 1.5)
        return "(catch 0)";

    // Si no, ajustarse a targetY
    double dyTarget = targetY - posY;

    if (std::abs(dyTarget) > 1.0)
    {
        // girar hacia la posición del balón
        double ang = std::atan2(dyTarget, targetX - posX) * 180 / M_PI;
        return "(turn " + std::to_string(ang) + ")";
    }

    return "(dash 60)";
}


// Lógica de persecución del balón
void Player::perseguirBalon(SeenObject balon) {
    // Asumiendo que tienes un miembro 'balon' actualizado
    if (balon.dist < 0.7) {
        // Si está en rango de disparo (kickable margin), patear a portería rival
        cout << "(kick 100 0)" << endl; 
    } else {
        // Interceptar. (Nota: Idealmente predecirías la posición futura, 
        // pero ir a la posición actual es un buen primer paso).
        irA(posicionBalon); 
    }
}

// Lógica de posicionamiento defensivo (Muelle virtual)
void mantenerPosicionDefensiva(Vector2D<double> casa) {
    // El jugador se mueve a su posición base 'casa', pero se desplaza 
    // ligeramente hacia el balón para cerrar espacios.
    Vector2D<double> vectorBalon = posicionBalon - casa;
    Vector2D<double> objetivo = casa + Vector2D<double>(vectorBalon.x * 0.2, vectorBalon.y * 0.2);
    
    // Histéresis: Solo moverse si el error es grande para no vibrar
    double dist = sqrt(pow(objetivo.x - this->posicionGlobal.x, 2) + pow(objetivo.y - this->posicionGlobal.y, 2));
    if (dist > 2.0) {
        irA(objetivo);
    } else {
        // Escanear el campo si estoy en posición
        cout << "(turn 30)" << endl; 
    }
}
*/