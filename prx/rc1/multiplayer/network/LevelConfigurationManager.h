//
// Created by bordplate on 3/22/2026.
//

#ifndef RAC1_MULTIPLAYER_LEVELCONFIGURATION_H
#define RAC1_MULTIPLAYER_LEVELCONFIGURATION_H

#include "../../../lib/vector.h"

enum LevelCongigurationType {
    LEVEL_CONFIGURATION_TYPE_BLOCK_MOBY_UID = 1,
    LEVEL_CONFIGURATION_TYPE_BLOCK_MOBY_O_CLASS = 2,
};

struct LevelConfigurationOption {
    LevelCongigurationType type;
    u32 value;
};

struct LevelConfiguration {
    Vector<LevelConfigurationOption*> options;
};

class LevelConfigurationManager {
public:
    static LevelConfiguration level_configurations[20];
};


#endif //RAC1_MULTIPLAYER_LEVELCONFIGURATION_H