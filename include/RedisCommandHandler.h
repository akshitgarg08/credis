#ifndef REDIS_COMMAND_HANDLER_H
#define REDIS_COMMAND_HANDLER_H

#include<vector>
#include <string>
std::vector<std::string> parseRespCommand(const std::string &input);
class RedisCommandHandler {
public:
    RedisCommandHandler();
    std::string processCommand(const std::string& commandLine);

};



#endif