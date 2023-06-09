//
// Created by Vetle Hjelle on 20/12/2022.
//

#ifndef PS3_CKIT_MPMOBY_H
#define PS3_CKIT_MPMOBY_H

#include <rc1/moby.h>

struct MPMoby : public Moby {
public:
    void update();

    static MPMoby* spawn(uint16_t uuid, unsigned short o_class, unsigned short flags, uint16_t modeBits);
};


#endif //PS3_CKIT_MPMOBY_H
