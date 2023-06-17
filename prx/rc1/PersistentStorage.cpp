//
// Created by bordplate on 6/17/2023.
//

#include "PersistentStorage.h"

void PersistentStorage::persist() {
    int err = cellFsOpen(String::format("/dev_hdd0/game/NPEA00385/USRDIR/%s", filename.c_str()).c_str(), CELL_FS_O_RDWR|CELL_FS_O_TRUNC|CELL_FS_O_CREAT, &file_descriptor_, nullptr, 0);

    Logger::debug("Persiting %d keys", keys_.size());

    for (int i = 0; i < keys_.size(); i++) {
        String key = keys_[i];
        String value = values_[i];

        Logger::debug("Key: %s", key.c_str());

        String line = String::format("%s=%s\n", key.c_str(), value.c_str());

        uint64_t num_write;
        cellFsWrite(file_descriptor_, line.c_str(), line.length(), &num_write);
    }

    Logger::debug("That's persitaance");

    cellFsClose(file_descriptor_);
}

void PersistentStorage::load() {
    int err = cellFsOpen(String::format("/dev_hdd0/game/NPEA00385/USRDIR/%s", filename.c_str()).c_str(), CELL_FS_O_RDONLY|CELL_FS_O_CREAT, &file_descriptor_, nullptr, 0);

    Logger::debug("oh wow we're loading wowow");

    char buffer[4096];
    uint64_t num_read;
    cellFsRead(file_descriptor_, (void*)buffer, sizeof(buffer), &num_read);

    Logger::debug("Read that shit");

    Vector<String> parts = String(buffer).split("\n");
    for (int i = 0; i < parts.size(); i++) {
        Vector<String> key_value = parts[i].split("=");
        if (key_value.size() < 2) {
            continue;
        }

        Logger::debug("Oh we in here");

        String key = key_value[0];
        String value = key_value[1];

        if (value[0] == '"') {
            // This is a string value
            set(key, value.slice(1, (int)value.length()-1));
        } else if (value == "true" || value == "false") {
            set(key, value == "true");
        } else {
            // Treat as integer
            set(key, __atoi(value.c_str()));
        }
    }

    Logger::debug("we out");

    cellFsClose(file_descriptor_);
}