#pragma once

#include "types.h"
#include <string>
#include <optional>

namespace ucchar {

// Load swara definitions from JSON.
bool swara_table_load(const std::string& path);

// Load akshara definitions from JSON.
bool akshara_table_load(const std::string& path);

// Look up a swara by name. Returns nullopt if not found.
std::optional<Swara> swara_lookup(const std::string& name);

// Look up an akshara by its Devanagari character.
std::optional<Akshara> akshara_lookup(const std::string& devanagari);

// Get the swara that corresponds to an akshara.
std::optional<Swara> swara_for_akshara(const std::string& devanagari);

} // namespace ucchar