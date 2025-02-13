#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H

#include <QString>

#define APP_NAME "GPShield Server Application"

enum HandleProcessStatus {
    HPS_PEER_CONNECTED = 0,
    HPS_PEER_CLOSED,
    HPS_PEER_SET_INFO,
    HPS_PEER_SEND,
    HPS_PEER_RECEIVED,
};

enum RequestHeader {
    RH_SET_NAME = 0,
    RH_SEND_ALERT,
    RH_RECEIVED_ALERT
};

#endif // GLOBAL_VARS_H
