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
        _s_profilers.push_back(this);
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
        for (int i = 0; i < _s_profilers.size(); i++) {
            if (_s_profilers[i]->name == name) {
                return _s_profilers[i];
            }
        }

        Profiler* profiler = new Profiler(name);

        return profiler;
    }

    static String get_profile_data() {
        String data = "Profiling:\1";

        for (int i = 0; i < _s_profilers.size(); i++) {
            Profiler* profiler = _s_profilers[i];
//            data += String::format("%s: %dus\1", profiler->name.c_str(), profiler->frame_time);
            data += profiler->name;
            data += ": ";
//            data += String::format("%dus\1", profiler->frame_time);
            data += "blah\1";
//            data = String::format("%s: %dus\1", data.c_str(), profiler->name.c_str(), profiler->frame_time);
//            data += String::format(": %dus\1", profiler->frame_time);
//            data += profiler->name;
        }

        return data;
    }

    static void reset_all() {
        for (int i = 0; i < _s_profilers.size(); i++) {
            _s_profilers[i]->reset();
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
private:
    static Vector<Profiler*> _s_profilers;

    system_time_t _start_time;
};


#endif //RAC1_MULTIPLAYER_PROFILER_H
