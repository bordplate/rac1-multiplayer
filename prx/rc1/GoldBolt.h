//
// Created by bordplate on 7/20/2023.
//

#ifndef RAC1_MULTIPLAYER_GOLDBOLT_H
#define RAC1_MULTIPLAYER_GOLDBOLT_H

#include "moby.h"

// All of this Gold bolt stuff is really hacky, and is just a temporary solution
#define blocked_bolts ((char*)0xB00500)
#define collected_bolts ((char*)0xB00600)

struct GoldBoltVars {
    int number;
};

struct GoldBolt : public Moby {
public:
    void update();
};


#endif //RAC1_MULTIPLAYER_GOLDBOLT_H
