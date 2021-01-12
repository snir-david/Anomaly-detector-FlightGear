

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include <memory>
#include "commands.h"

using namespace std;

class CLI {
    DefaultIO *dio;
    map<int, unique_ptr<Command>> commandsMap;
public:
    CLI(DefaultIO *dio);

    void start();

    virtual ~CLI();
};

#endif /* CLI_H_ */
