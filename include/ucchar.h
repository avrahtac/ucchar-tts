#pragma once

#include "types.h"
#include <string>

namespace ucchar {

    
bool synthesise(const SynthesisRequest& request,
                const std::string& output_path);

bool load_swara_table(const std::string& swaras_json_path);


bool load_akshara_table(const std::string& aksharas_json_path);

} // namespace ucchar