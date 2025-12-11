#ifndef HEAR_MESSAGE_H
#define HEAR_MESSAGE_H

#include <string>
#include <regex>

struct HearMessage
{
    int time;
    std::string sender;
    std::string message;

    void parseHear(const std::string &msg)
    {
        static const std::regex hearRegex(
            R"(^\(hear\s+(\S+)\s+(\S+)\s+(.+)\)$)");

        std::smatch match;
        if (std::regex_search(msg, match, hearRegex))
        {
            time = std::stod(match[1]); // si time puede ser decimal
            sender = match[2];
            message = match[3];
        }
    }
};

#endif