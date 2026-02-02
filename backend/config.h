#pragma once
#include <string>
#include <glib.h>

enum class LgConfigCategory {
    Mouse,
    RGB
};

class LogitechConfig {
public:
    static void init();
    static void save(LgConfigCategory cat);

    static void set_int(LgConfigCategory cat, const std::string& device_name, const std::string& key, int val);
    static int get_int(LgConfigCategory cat, const std::string& device_name, const std::string& key, int def);

private:
    static GKeyFile* get_file(LgConfigCategory cat);
    static std::string get_path(LgConfigCategory cat);
    static void ensure_dir(const std::string& path);
};
