//
// Created by bordplate on 3/24/2025.
//

#ifndef RAC1_MULTIPLAYER_PROFILER_H
#define RAC1_MULTIPLAYER_PROFILER_H


#include "../lib/string.h"
#include <sys/sys_time.h>

class Profiler {
public:
    explicit Profiler(const String& name) : name(name), average_time(0), _start_time(0), frame_time(0) {
        Logger::info("Profiler created: %s", name.c_str());

        if (Profiler::_s_profilers == nullptr) {
            Profiler::_s_profilers = new Vector<Profiler*>();
        }

        Profiler::_s_profilers->push_back(this);
    }

    String name;

    system_time_t average_time;
    system_time_t frame_time;

    void start() {
        _start_time = sys_time_get_system_time();
    }

    void stop() {
        system_time_t end_time = sys_time_get_system_time();

        average_time = (average_time + (end_time - _start_time)) / 2;
        frame_time += end_time - _start_time;
    }

    void reset() {
        frame_time = 0;
    }

    static Profiler* get(const String& name) {
        for (int i = 0; i < Profiler::_s_profilers->size(); i++) {
            if (Profiler::_s_profilers->at(i)->name == name) {
                return Profiler::_s_profilers->at(i);
            }
        }

        Profiler* profiler = new Profiler(name);

        return profiler;
    }

    static String get_profile_data() {
        String data = "Profiling:\1";

        for (int i = 0; i < Profiler::_s_profilers->size(); i++) {
            Profiler* profiler = Profiler::_s_profilers->at(i);
            data += String::format("%s: %dus\1", profiler->name.c_str(), profiler->average_time);
        }

        return data;
    }

    static void reset_all() {
        for (int i = 0; i < Profiler::_s_profilers->size(); i++) {
            Profiler::_s_profilers->at(i)->reset();
        }
    }

    class Scope {
    public:
        explicit Scope(Profiler* profiler) {
            this->profiler = profiler;
            profiler->start();
        }

        ~Scope() {
            profiler->stop();
        }

        Profiler* profiler;
    };

    static Vector<Profiler*>* _s_profilers;
private:
    system_time_t _start_time;
};

#endif //RAC1_MULTIPLAYER_PROFILER_H
