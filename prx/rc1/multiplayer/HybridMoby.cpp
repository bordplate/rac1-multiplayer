#include "HybridMoby.h"

#include <rc1/Game.h>

HybridMoby::HybridMoby(Moby* moby, u16 _uid) {
    Logger::trace("Creating HybridMoby for %d", _uid);

    if (moby == nullptr) {
        Logger::error("HybridMoby %d created with null-moby", _uid);
    }

    moby_ = moby;
    uid = _uid;
}

HybridMoby::~HybridMoby() {
    Logger::trace("Destroying HybridMoby for %d", uid);

    for (size_t i = 0; i < monitored_attributes_.size(); i++) {
        delete monitored_attributes_[i];
    }

    for (size_t i = 0; i < monitored_pvars_.size(); i++) {
        delete monitored_pvars_[i];
    }

    monitored_attributes_.resize(0);
    monitored_pvars_.resize(0);
}

void HybridMoby::monitor_attribute(u32 offset, u32 size) {
    Logger::debug("Monitoring attribute %d with size %d", offset, size);
    for (size_t i = 0; i < monitored_attributes_.size(); i++) {
        MonitoredValue* value = monitored_attributes_[i];
        if (value->offset == offset) {
            return;
        }
    }

    MonitoredValue* value = new MonitoredValue();
    value->offset = offset;
    value->size = size;
    value->old_value = 0;

    if (size == 1) {
        memcpy(&value->old_value, (unsigned char*)moby_ + offset, 1);
    } else if (size == 2) {
        memcpy(&value->old_value, (unsigned char*)moby_ + offset, 2);
    } else {
        memcpy(&value->old_value, (unsigned char*)moby_ + offset, 4);
    }

    // Send initial value to the client, should be easily ignorable as both old and new values are the same
    if (Game::shared().client()) {
        Client* client = Game::shared().client();

        // NOTE: We could bundle all the changes into one packet
        client->send_ack(Packet::make_monitored_value_changed_packet(
                uid,
                value->offset,
                value->size,
                MP_MONITORED_VALUE_TYPE_ATTR,
                value->old_value,
                value->old_value
        ));
    }

    monitored_attributes_.push_back(value);
}

void HybridMoby::monitor_pvar(u32 offset, u32 size) {
    Logger::debug("Monitoring pvar %d with size %d", offset, size);
    for (size_t i = 0; i < monitored_pvars_.size(); i++) {
        MonitoredValue* value = monitored_pvars_[i];
        if (value->offset == offset) {
            return;
        }
    }

    MonitoredValue* value = new MonitoredValue();
    value->offset = offset;
    value->size = size;

    if (size == 1) {
        memcpy(&value->old_value, (unsigned char*)moby_->vars + offset, 1);
    } else if (size == 2) {
        memcpy(&value->old_value, (unsigned char*)moby_->vars + offset, 2);
    } else {
        memcpy(&value->old_value, (unsigned char*)moby_->vars + offset, 4);
    }

    // Send initial value to the client, should be easily ignorable as both old and new values are the same
    if (Game::shared().client()) {
        Client* client = Game::shared().client();

        // NOTE: We could bundle all the changes into one packet
        client->send_ack(Packet::make_monitored_value_changed_packet(
                uid,
                value->offset,
                value->size,
                MP_MONITORED_VALUE_TYPE_PVAR,
                value->old_value,
                value->old_value
        ));
    }


    monitored_pvars_.push_back(value);
}

void HybridMoby::on_tick() {
    if (game_state != PlayerControl || moby_ == nullptr) {
        return;
    }

    for (size_t i = 0; i < monitored_attributes_.size(); i++) {
        MonitoredValue* value = monitored_attributes_[i];
        u32 new_value;

        if (value->size == 1) {
            u8 temp_value;
            memcpy(&temp_value, (unsigned char*)moby_ + value->offset, 1);
            new_value = temp_value;
        } else if (value->size == 2) {
            u16 temp_value;
            memcpy(&temp_value, (unsigned char*)moby_ + value->offset, 2);
            new_value = temp_value;
        } else {
            memcpy(&new_value, (unsigned char*)moby_ + value->offset, 4);
        }

        if (new_value != value->old_value) {
            Logger::trace("Attribute %d changed from %d to %d", value->offset, value->old_value, new_value);
            if (Game::shared().client()) {
                Client* client = Game::shared().client();

                // NOTE: We could bundle all the changes into one packet
                client->send_ack(Packet::make_monitored_value_changed_packet(
                        uid,
                        value->offset,
                        value->size,
                        MP_MONITORED_VALUE_TYPE_ATTR,
                        value->old_value,
                        new_value
                ));
            }

            value->old_value = new_value;
        }
    }

    for (size_t i = 0; i < monitored_pvars_.size(); i++) {
        MonitoredValue* value = monitored_pvars_[i];
        u32 new_value;

        if (value->size == 1) {
            u8 temp_value;
            memcpy(&temp_value, (unsigned char*)moby_->vars + value->offset, 1);
            new_value = temp_value;
        } else if (value->size == 2) {
            u16 temp_value;
            memcpy(&temp_value, (unsigned char*)moby_->vars + value->offset, 2);
            new_value = temp_value;
        } else {
            memcpy(&new_value, (unsigned char*)moby_->vars + value->offset, 4);
        }

        if (new_value != value->old_value) {
            Logger::debug("Pvar %d changed from %d to %d", value->offset, value->old_value, new_value);
            if (Game::shared().client()) {
                Client* client = Game::shared().client();

                // NOTE: We could bundle all the changes into one packet
                client->send_ack(Packet::make_monitored_value_changed_packet(
                        uid,
                        value->offset,
                        value->size,
                        MP_MONITORED_VALUE_TYPE_PVAR,
                        value->old_value,
                        new_value
                ));
            }

            value->old_value = new_value;
        }
    }
}

void HybridMoby::refresh_old_values_without_notifying_server() {
    Logger::trace("Refreshing attributes for %d without notifying server for size %d", uid, monitored_attributes_.size());
    for (size_t i = 0; i < monitored_attributes_.size(); i++) {
        MonitoredValue* value = monitored_attributes_[i];
        u32 new_value;

        if (value->size == 1) {
            u8 temp_value;
            memcpy(&temp_value, (unsigned char*)moby_ + value->offset, 1);
            new_value = temp_value;
        } else if (value->size == 2) {
            u16 temp_value;
            memcpy(&temp_value, (unsigned char*)moby_ + value->offset, 2);
            new_value = temp_value;
        } else {
            memcpy(&new_value, (unsigned char*)moby_ + value->offset, 4);
        }

        value->old_value = new_value;
    }

    Logger::trace("Refreshing pvars for %d without notifying server for size: %d", uid, monitored_pvars_.size());
    for (size_t i = 0; i < monitored_pvars_.size(); i++) {

        MonitoredValue* value = monitored_pvars_[i];
        u32 new_value;

        if (value->size == 1) {
            u8 temp_value;
            memcpy(&temp_value, (unsigned char*)moby_->vars + value->offset, 1);
            new_value = temp_value;
        } else if (value->size == 2) {
            u16 temp_value;
            memcpy(&temp_value, (unsigned char*)moby_->vars + value->offset, 2);
            new_value = temp_value;
        } else {
            memcpy(&new_value, (unsigned char*)moby_->vars + value->offset, 4);
        }

        value->old_value = new_value;
    }

    Logger::trace("Done refreshing values for %d without notifying server", uid);
}