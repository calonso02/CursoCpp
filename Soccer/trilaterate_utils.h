#ifndef TRILATERATE_UTILS_H
#define TRILATERATE_UTILS_H

#include "vector2D.h"
#include "seen_object.h"
#include <sstream>
#include <vector>
#include <cmath>
#include <map>
#include <string>

const double PITCH_HALF_LENGTH = 52.5; // 105.0 / 2.0
const double PITCH_HALF_WIDTH = 34.0;  // 68.0 / 2.0
const double GOAL_WIDTH_HALF = 7.01;   // 14.02 / 2.0
const double PENALTY_X = 36.0;         // 52.5 - 16.5 (Distancia área penal)
const double PENALTY_Y = 20.16;        // 40.32 / 2.0 (Ancho área penal)

struct LocalizationUtils
{
    static std::vector<Vector2D<double>> trilaterate(
        Vector2D<double> p1, double r1,
        Vector2D<double> p2, double r2)
    {
        std::vector<Vector2D<double>> solutions;

        Vector2D<double> dVec = p2 - p1;
        double d = std::sqrt(dVec.x * dVec.x + dVec.y * dVec.y);

        if (d > r1 + r2)
            return solutions;
        if (d < std::abs(r1 - r2))
            return solutions;
        if (d == 0)
            return solutions;

        double a = (r1 * r1 - r2 * r2 + d * d) / (2.0 * d);

        double h = std::sqrt(std::max(0.0, r1 * r1 - a * a));

        double x2 = p1.x + a * (p2.x - p1.x) / d;
        double y2 = p1.y + a * (p2.y - p1.y) / d;

        const double TOLERANCE = 3.0;

        auto isInsideField = [&](const Vector2D<double> &p)
        {
            return (std::abs(p.x) <= PITCH_HALF_LENGTH + TOLERANCE) &&
                   (std::abs(p.y) <= PITCH_HALF_WIDTH + TOLERANCE);
        };

        Vector2D<double> sol1(
            x2 + h * (p2.y - p1.y) / d,
            y2 - h * (p2.x - p1.x) / d);
        Vector2D<double> sol2(
            x2 - h * (p2.y - p1.y) / d,
            y2 + h * (p2.x - p1.x) / d);

        if (isInsideField(sol1))
            solutions.push_back(sol1);

        if (h > 1e-5 && isInsideField(sol2))
            solutions.push_back(sol2);

        return solutions;
    }
};

class FieldMap
{
public:
    // Devuelve la posición absoluta dada la etiqueta del string (ej: "f t l 10")
    static Vector2D<double> getFlagPos(std::string flagName)
    {
        static std::map<std::string, Vector2D<double>> landmarks = {
            // --- Banderas Centrales ---
            {"f c", {0.0, 0.0}},
            {"f c t", {0.0, -PITCH_HALF_WIDTH}},
            {"f c b", {0.0, PITCH_HALF_WIDTH}},

            // --- Porterías (Goals) ---
            {"g l", {-PITCH_HALF_LENGTH, 0.0}},
            {"g r", {PITCH_HALF_LENGTH, 0.0}},

            // --- Esquinas (Corners) ---
            {"f t l", {-PITCH_HALF_LENGTH, -PITCH_HALF_WIDTH}}, // Top Left
            {"f t r", {PITCH_HALF_LENGTH, -PITCH_HALF_WIDTH}},  // Top Right
            {"f b l", {-PITCH_HALF_LENGTH, PITCH_HALF_WIDTH}},  // Bottom Left
            {"f b r", {PITCH_HALF_LENGTH, PITCH_HALF_WIDTH}},   // Bottom Right

            // --- Líneas de Banda (Flags a 10, 20, 30... metros del centro) ---
            // Top Left (t l) -> Y negativo, X negativo
            {"f t l 10", {-10.0, -PITCH_HALF_WIDTH}},
            {"f t l 20", {-20.0, -PITCH_HALF_WIDTH}},
            {"f t l 30", {-30.0, -PITCH_HALF_WIDTH}},
            {"f t l 40", {-40.0, -PITCH_HALF_WIDTH}},
            {"f t l 50", {-50.0, -PITCH_HALF_WIDTH}},

            // Top Right (t r) -> Y negativo, X positivo
            {"f t r 10", {10.0, -PITCH_HALF_WIDTH}},
            {"f t r 20", {20.0, -PITCH_HALF_WIDTH}},
            // ... (puedes llenar el resto siguiendo la lógica)

            // Bottom Left (b l) -> Y positivo, X negativo
            {"f b l 10", {-10.0, PITCH_HALF_WIDTH}},
            // ...

            // Bottom Right (b r) -> Y positivo, X positivo
            {"f b r 10", {10.0, PITCH_HALF_WIDTH}},
            // ...

            // --- Área de Penal (Penalty Area) ---
            {"f p l t", {-36.0, -20.16}}, // Penalty Left Top
            {"f p l c", {-36.0, 0.0}},    // Penalty Left Center
            {"f p l b", {-36.0, 20.16}},  // Penalty Left Bottom

            {"f p r t", {36.0, -20.16}}, // Penalty Right Top
            {"f p r c", {36.0, 0.0}},    // Penalty Right Center
            {"f p r b", {36.0, 20.16}}   // Penalty Right Bottom
        };

        if (landmarks.find(flagName) != landmarks.end())
        {
            return landmarks[flagName];
        }

        return Vector2D<double>(0, 0);
    }
};

class MessageParser
{
public:
    static std::vector<SeenObject> parseSeeMessage(std::string msg)
    {
        std::vector<SeenObject> objects;

        // 1. Limpieza básica: Saltamos el encabezado "(see TIME"
        size_t currentPos = msg.find("(see");
        if (currentPos == std::string::npos)
            return objects; // No es un mensaje 'see'

        // Avanzamos hasta el primer objeto visual.
        // El patrón es siempre: ((nombre) distancia direccion ...)

        while (true)
        {
            // Buscamos el inicio de un objeto: "(("
            size_t startObj = msg.find("((", currentPos);
            if (startObj == std::string::npos)
                break; // No hay más objetos

            // Buscamos el cierre del nombre: ") "
            size_t endName = msg.find(") ", startObj);
            if (endName == std::string::npos)
                break;

            // --- EXTRACCIÓN DEL NOMBRE ---
            // startObj + 2 salta los dos paréntesis "(("
            // endName - (startObj + 2) es la longitud del nombre
            std::string name = msg.substr(startObj + 2, endName - (startObj + 2));

            // --- EXTRACCIÓN DE DATOS NUMÉRICOS ---
            // Creamos un stream empezando justo después del nombre para leer los doubles
            // La estructura es: ") DIST DIR ..."
            std::stringstream ss(msg.substr(endName + 1));

            double dist, angle;
            ss >> dist;  // Lee el primer número automáticamente
            ss >> angle; // Lee el segundo número automáticamente

            objects.push_back({name, dist, angle});

            // Avanzamos el puntero para buscar el siguiente objeto
            currentPos = endName + 1;
        }

        return objects;
    }
};

void updateAgentState(std::string serverMsg)
{
    // 1. Parsear el mensaje crudo
    std::vector<SeenObject> visualData = MessageParser::parseSeeMessage(serverMsg);

    // 2. Filtrar y buscar referencias estáticas (Banderas)
    // Necesitamos al menos 2 banderas para triangular
    std::vector<SeenObject> usefulFlags;

    for (const auto &obj : visualData)
    {
        // Solo nos interesan objetos que empiecen por 'f' (flags) o 'g' (goals)
        // Ignoramos 'b' (ball) y 'p' (players) para localizarse
        if (obj.get_name()[0] == 'f' || obj.get_name()[0] == 'g')
        {
            usefulFlags.push_back(obj);
        }
    }

    if (usefulFlags.size() < 2)
    {
        return;
    }

    SeenObject obs1 = usefulFlags[0];
    SeenObject obs2 = usefulFlags[1];

    // 4. Obtener coordenadas absolutas del mapa (Memoria del Agente)
    Vector2D<double> pos1 = FieldMap::getFlagPos(obs1.get_name());
    Vector2D<double> pos2 = FieldMap::getFlagPos(obs2.get_name());

    // Nota: El parser nos dio ángulos, pero para trilateración pura
    // solo necesitamos las distancias (radios).
    // Usamos los ángulos después para saber la orientación del cuerpo.

    // 5. Ejecutar Trilateración
    auto positions = LocalizationUtils::trilaterate(pos1, obs1.get_dist(), pos2, obs2.get_dist());

    // 6. Resultado
    /*cout << "--- Localización ---" << endl;
    cout << "Usando referencia 1: " << obs1.name << " (en mapa: " << pos1.x << ", " << pos1.y << ")" << endl;
    cout << "Usando referencia 2: " << obs2.name << " (en mapa: " << pos2.x << ", " << pos2.y << ")" << endl;*/

    /*if (positions.empty()) {
        cout << "Error: Datos inconsistentes, no hay intersección." << endl;
    } else {
        // En tu caso específico (tangente), probablemente salga 1 o 2 muy juntas.
        cout << "Mi posición estimada: (" << positions[0].x << ", " << positions[0].y << ")" << endl;
    }*/
}

#endif