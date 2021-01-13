

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include <memory>
#include "commands.h"

using namespace std;

class CLI {
public:
    DefaultIO *dio;
    CommandUtil cu;
    Command* commandsArray[6];

    CLI(DefaultIO *dio);

    void start();

    virtual ~CLI();
};

#endif /* CLI_H_ */
