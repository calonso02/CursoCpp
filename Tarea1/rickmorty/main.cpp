#include <iostream>
#include <string>
#include <vector>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Character
{
    std::string name;
    std::string origin;
    std::string species;
    std::string status;

    std::vector<std::string> episode_url;
};

struct Episode
{
    std::string name;
    std::string episode;
    std::string air_date;
};

std::vector<Character> parse_characters(const std::string &body)
{
  std::vector<Character> characters;

  json j = json::parse(body);

  if (!j.contains("results") || !j["results"].is_array())
  {
    return characters;
  }

  for (const auto &item : j["results"])
  {
    Character c;

    c.name = item.value("name", "Desconocido");
    c.status = item.value("status", "Desconocido");
    c.species = item.value("species", "Desconocida");

    if (item.contains("origin") && item["origin"].is_object())
    {
      c.origin = item["origin"].value("name", "Desconocido");
    }
    else
    {
      c.origin = "Desconocido";
    }
    // Nuevo
    if (item.contains("episode")) {
        for (const auto &episode_id : item["episode"]) {
            c.episode_url.push_back(episode_id);
        }
    }
    characters.push_back(std::move(c));
  }

  return characters;
}

Episode parse_episode(const std::string &body) {

    json j = json::parse(body);

    Episode e;

    e.name = j.value("name", "Desconocido");
    e.episode = j.value("episode", "Desconocido");
    e.air_date = j.value("air_date", "Desconocido");

    return e;
}

std::vector<Episode> buscar_episodios (const std::vector<std::string>& episode_urls) {
    std::vector<Episode> episodios{};

    for (const auto &url : episode_urls) {
        auto response = cpr::Get(cpr::Url{url});

        if (response.error)
        {
            std::cerr << "Error en la petición HTTP: " << response.error.message << "\n";
        }

        if (response.status_code != 200)
        {
            std::cerr << "La API devolvió código HTTP "
                      << response.status_code << "\nCuerpo:\n"
                      << response.text << "\n";
        }

        auto episode_info = parse_episode(response.text);
        episodios.push_back(episode_info);
    }

    return episodios;

}

int main()
{
    try
    {
        bool continuar = true;
        while (continuar) {
            std::cout << "Introduce el nombre (o parte del nombre) del personaje de Rick & Morty: ";
            std::string search{};
            std::getline(std::cin, search);

            if (search.empty())
            {
                std::cerr << "Cadena de búsqueda vacía. Terminando.\n";
                return 1;
            }

            // Petición GET con cpr, usando parámetros (hace URL encoding por ti)
            auto response = cpr::Get(
                cpr::Url{"https://rickandmortyapi.com/api/character/"},
                cpr::Parameters{{"name", search}});

            if (response.error)
            {
                std::cerr << "Error en la petición HTTP: " << response.error.message << "\n";
                return 1;
            }

            if (response.status_code != 200)
            {
                std::cerr << "La API devolvió código HTTP "
                          << response.status_code << "\nCuerpo:\n"
                          << response.text << "\n";
                return 1;
            }

            //std::cout << response.text << std::endl;

            auto characters = parse_characters(response.text);
            if (characters.empty())
            {
                std::cout << "No se encontraron resultados para: " << search << "\n";
                return 0;
            }

            // Mostrar listado de resultados
            std::cout << "\nResultados encontrados:\n";
            for (std::size_t i = 0; i < characters.size(); ++i)
            {
                std::cout << i << ") " << characters[i].name << '\n';
            }

            // Seleccionar uno
            std::cout << "\nSelecciona el índice del personaje que te interesa: ";
            std::size_t index = 0;
            if (!(std::cin >> index))
            {
                std::cerr << "Entrada no válida.\n";
                return 1;
            }

            if (index >= characters.size())
            {
                std::cerr << "Índice fuera de rango.\n";
                return 1;
            }

            const auto &c = characters[index];

            const auto episodios = buscar_episodios(c.episode_url);

            std::cout << "\n--- Detalles del personaje ---\n";
            std::cout << "Nombre : " << c.name << '\n';
            std::cout << "Planeta (origen): " << c.origin << '\n';
            std::cout << "Especie: " << c.species << '\n';
            std::cout << "Status : " << c.status << '\n';
            std::cout << "Episodios: " << std::endl;
            for (const auto& ep : episodios) {
                std::cout << ep.name << " - " << ep.episode << std::endl;
            }

            std::cout << "Deseas consultar otro personaje? ('S' para continuar) " << std::endl;
            char cont{}; std::cin >> cont;
            continuar = (cont == 'S');
            //std::cin.clear();
            std::cin.ignore();
        }
    }
    catch (const std::exception &ex)
    {
      std::cerr << "Se produjo una excepción: " << ex.what() << '\n';
      return 1;
    }

    return 0;
}
