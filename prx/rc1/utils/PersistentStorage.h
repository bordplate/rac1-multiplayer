//
// Created by bordplate on 6/17/2023.
//

#ifndef RAC1_MULTIPLAYER_PERSISTENTSTORAGE_H
#define RAC1_MULTIPLAYER_PERSISTENTSTORAGE_H

#include <lib/string.h>
#include <lib/vector.h>

#include <sys/paths.h>
#include <cell/cell_fs.h>

class PersistentStorage {
public:
    PersistentStorage(const String& filename) : filename(filename) {
        load();
    }

    ~PersistentStorage() {
        persist();
    }

    void set(const String& key, const String& value) {
        uint64_t num_write;

        String value_string = String::format("\"%s\"", value.c_str());

        int index = index_of(key);
        if (index >= 0) {
            keys_[index] = key;
            values_[index] = value_string;
        } else {
            keys_.push_back(key);
            values_.push_back(value_string);
        }
    }

    void set(const String& key, const int& value) {
        uint64_t num_write;
        String value_string = String::format("%d", value);

        int index = index_of(key);
        if (index >= 0) {
            keys_[index] = key;
            values_[index] = value_string;
        } else {
            keys_.push_back(key);
            values_.push_back(value_string);
        }
    }

    void set(const String& key, const bool& value) {
        uint64_t num_write;
        String value_string = value ? "true" : "false";

        int index = index_of(key);
        if (index >= 0) {
            keys_[index] = key;
            values_[index] = value_string;
        } else {
            keys_.push_back(key);
            values_.push_back(value_string);
        }
    }

    String get_string(const String& key) {
        int index = index_of(key);
        if (index < 0) {
            return "";
        }

        String value = values_[index];

        return value.slice(1, value.length()-1);
    }

    int get_int(const String& key) {
        int index = index_of(key);
        if (index < 0) {
            return -1;
        }

        String value = values_[index];
        return __atoi(value.c_str());
    }

    bool get_bool(const String& key) {
        int index = index_of(key);
        if (index < 0) {
            return false;
        }

        return values_[index] == "true";
    }

    int index_of(const String& key) {
        for (int i = 0; i < keys_.size(); i++) {
            if (keys_[i] == key) {
                return i;
            }
        }

        return -1;
    }
private:
    String filename;

    Vector<String> keys_;
    Vector<String> values_;

    int file_descriptor_;

    void persist();
    void load();
};

#endif //RAC1_MULTIPLAYER_PERSISTENTSTORAGE_H
