#ifndef MSG_PARSER_H
#define MSG_PARSER_H

#include "types.h"
#include <string>
#include <vector>

void parse_init(const std::string &msg, Player &player);
HearMessage parseHearMessage(const std::string &msg);
void parseSeeMessage(const std::string &msg, Player& player);

inline bool isSeeCommand(const std::string &msg) { return msg.find("(see") != std::string::npos; }
inline bool isHearCommand(const std::string &msg) { return msg.find("(hear") != std::string::npos; }

#endif