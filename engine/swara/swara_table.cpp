#include "engine/swara/swara_table.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <unordered_map>
#include <iostream>

using json = nlohmann::json;

namespace ucchar {

// Internal storage
static std::unordered_map<std::string, Swara>   g_swaras;
static std::unordered_map<std::string, Akshara> g_aksharas;

// Helper: string to Sthana
static Sthana sthana_from_string(const std::string& s) {
    if (s == "URAS")   return Sthana::URAS;
    if (s == "KANTHA") return Sthana::KANTHA;
    if (s == "TALU")   return Sthana::TALU;
    if (s == "MURDHA") return Sthana::MURDHA;
    if (s == "NASA")   return Sthana::NASA;
    if (s == "DANTA")  return Sthana::DANTA;
    if (s == "OSTHA")  return Sthana::OSTHA;
    return Sthana::KANTHA; // default
}

// Helper: string to Matra
static Matra matra_from_string(const std::string& s) {
    if (s == "GURU") return Matra::GURU;
    return Matra::LAGHU;
}

bool swara_table_load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[ucchar] could not open " << path << "\n";
        return false;
    }

    json j;
    try { file >> j; }
    catch (const json::exception& e) {
        std::cerr << "[ucchar] JSON parse error: " << e.what() << "\n";
        return false;
    }

    for (const auto& s : j["swaras"]) {
        Swara sw;
        sw.name       = strdup(s["name"].get<std::string>().c_str());
        sw.devanagari = strdup(s["devanagari"].get<std::string>().c_str());
        sw.f0_hz      = s["f0_hz"].get<float>();
        sw.f1_hz      = s["f1_hz"].get<float>();
        sw.f2_hz      = s["f2_hz"].get<float>();
        sw.f3_hz      = s["f3_hz"].get<float>();
        sw.sthana     = sthana_from_string(s["sthana"].get<std::string>());
        g_swaras[sw.name] = sw;
    }

    std::cout << "[ucchar] loaded " << g_swaras.size() << " swaras\n";
    return true;
}

bool akshara_table_load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[ucchar] could not open " << path << "\n";
        return false;
    }

    json j;
    try { file >> j; }
    catch (const json::exception& e) {
        std::cerr << "[ucchar] JSON parse error: " << e.what() << "\n";
        return false;
    }

    auto load_group = [&](const std::string& group_key) {
        if (!j.contains(group_key)) return;
        for (const auto& a : j[group_key]) {
            Akshara ak;
            ak.devanagari        = strdup(a["devanagari"].get<std::string>().c_str());
            ak.swara_name        = strdup(a["swara"].get<std::string>().c_str());
            ak.default_matra     = matra_from_string(a["matra"].get<std::string>());
            ak.has_consonant     = a["has_consonant"].get<bool>();
            ak.articulation_point = a.contains("articulation")
                ? sthana_from_string(a["articulation"].get<std::string>())
                : Sthana::KANTHA;
            g_aksharas[ak.devanagari] = ak;
        }
    };

    load_group("vowels");
    load_group("ka_varga");
    // add more groups here as you expand aksharas.json

    std::cout << "[ucchar] loaded " << g_aksharas.size() << " aksharas\n";
    return true;
}

std::optional<Swara> swara_lookup(const std::string& name) {
    auto it = g_swaras.find(name);
    if (it == g_swaras.end()) return std::nullopt;
    return it->second;
}

std::optional<Akshara> akshara_lookup(const std::string& devanagari) {
    auto it = g_aksharas.find(devanagari);
    if (it == g_aksharas.end()) return std::nullopt;
    return it->second;
}

std::optional<Swara> swara_for_akshara(const std::string& devanagari) {
    auto ak = akshara_lookup(devanagari);
    if (!ak) return std::nullopt;
    return swara_lookup(ak->swara_name);
}

} // namespace ucchar