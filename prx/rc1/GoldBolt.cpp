//
// Created by bordplate on 7/20/2023.
//

#include "GoldBolt.h"
#include "Game.h"

#include "multiplayer/Packet.h"

#include <lib/shk.h>

extern "C" SHK_HOOK(void, goldBoltUpdate, Moby* moby);
void GoldBolt::update() {
    GoldBoltVars* vars = (GoldBoltVars*)this->pVars;

    if (this->state != 2 && blocked_bolts[(current_planet * 4) + vars->number] != 0) {
        delete_moby(this);
        return;
    }

    if (this->state == 0) {
        collected_bolts[vars->number] = 0;
    }
    if (this->state == 2 && collected_bolts[vars->number] == 0) {
        Logger::debug("Hey it's the gold bolt stuff");
        collected_bolts[vars->number] = 1;

        Client* client = Game::shared().client();
        if (client != nullptr) {
            Packet* packet = Packet::make_collected_gold_bolt_packet(vars->number);
            client->make_ack(packet, nullptr);
            client->send(packet);
        }
    }

    SHK_CALL_HOOK(goldBoltUpdate, this);
}