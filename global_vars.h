#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include <QString>

#define APP_NAME "GPShield Server Application"

enum HandleProcessStatus {
    HPS_PEER_CONNECTED = 0,
    HPS_PEER_CLOSED,
};

enum RequestHeader {
    RH_SET_NAME = 0,
};

#endif // GLOBAL_VARS_H
