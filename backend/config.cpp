#include "config.h"
#include <filesystem>
#include <iostream>
#include <map>

static std::map<LgConfigCategory, GKeyFile*> g_files;
static std::map<LgConfigCategory, std::string> g_paths;

void LogitechConfig::init() {
}

std::string LogitechConfig::get_path(LgConfigCategory cat) {
    if (g_paths.find(cat) != g_paths.end()) return g_paths[cat];

    const char* config_home = g_get_user_config_dir();
    std::string dir = std::string(config_home) + "/kerneldrive/logitech_ghub";
    
    std::string filename;
    switch(cat) {
        case LgConfigCategory::Mouse: filename = "mouse.ini"; break;
        case LgConfigCategory::RGB: filename = "rgb.ini"; break;
    }
    
    std::string full_path = dir + "/" + filename;
    g_paths[cat] = full_path;
    return full_path;
}

void LogitechConfig::ensure_dir(const std::string& path) {
    std::filesystem::path p(path);
    if (!p.parent_path().empty()) {
        std::filesystem::create_directories(p.parent_path());
    }
}

GKeyFile* LogitechConfig::get_file(LgConfigCategory cat) {
    if (g_files.find(cat) != g_files.end()) return g_files[cat];

    GKeyFile* f = g_key_file_new();
    std::string path = get_path(cat);

    GError* error = nullptr;
    if (!g_key_file_load_from_file(f, path.c_str(), G_KEY_FILE_NONE, &error)) {
        if (!g_error_matches(error, G_FILE_ERROR, G_FILE_ERROR_NOENT)) {
            // std::cerr << "[LogitechConfig] Load failed: " << error->message << std::endl;
        }
        g_error_free(error);
    }
    
    g_files[cat] = f;
    return f;
}

void LogitechConfig::save(LgConfigCategory cat) {
    GKeyFile* f = get_file(cat);
    std::string path = get_path(cat);
    ensure_dir(path);
    
    GError* error = nullptr;
    if (!g_key_file_save_to_file(f, path.c_str(), &error)) {
        std::cerr << "[LogitechConfig] Save failed: " << error->message << std::endl;
        g_error_free(error);
    }
}

void LogitechConfig::set_int(LgConfigCategory cat, const std::string& device_name, const std::string& key, int val) {
    GKeyFile* f = get_file(cat);
    g_key_file_set_integer(f, device_name.c_str(), key.c_str(), val);
    save(cat);
}

int LogitechConfig::get_int(LgConfigCategory cat, const std::string& device_name, const std::string& key, int def) {
    GKeyFile* f = get_file(cat);
    GError* err = nullptr;
    int val = g_key_file_get_integer(f, device_name.c_str(), key.c_str(), &err);
    if (err) { g_error_free(err); return def; }
    return val;
}
