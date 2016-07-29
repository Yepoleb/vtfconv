#pragma once
#include <vector>
#include <string>

#include "VTFLib13/VTFFormat.h"

extern const std::vector<const std::string> FORMATNAMES;
VTFImageFormat parse_format(std::string formatstr);

