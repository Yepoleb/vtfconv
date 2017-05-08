#include <vector>
#include <string>
#include <stddef.h>

#include "VTFLib13/VTFFormat.h"

#include "common.hpp"

const char* FORMATNAMES[] = {
    "RGBA8888",
    "ABGR8888",
    "RGB888",
    "BGR888",
    "RGB565",
    "I8",
    "IA88",
    "P8",
    "A8",
    "RGB888_BLUESCREEN",
    "BGR888_BLUESCREEN",
    "ARGB8888",
    "BGRA8888",
    "DXT1",
    "DXT3",
    "DXT5",
    "BGRX8888",
    "BGR565",
    "BGRX5551",
    "BGRA4444",
    "DXT1_ONEBITALPHA",
    "BGRA5551",
    "UV88",
    "UVWQ8888",
    "RGBA16161616F",
    "RGBA16161616",
    "UVLX8888",
    "R32F",
    "RGB323232F",
    "RGBA32323232F",
    "NV_DST16",
    "NV_DST24",
    "NV_INTZ",
    "NV_RAWZ",
    "ATI_DST16",
    "ATI_DST24",
    "NV_NULL",
    "ATI2N",
    "ATI1N"
};

const size_t FORMATNAMES_SIZE = ARRAY_SIZE(FORMATNAMES);

VTFImageFormat parse_format(std::string formatstr)
{
    formatstr = upper(formatstr);

    for (size_t i = 0; i < FORMATNAMES_SIZE; i++) {
        if (formatstr == FORMATNAMES[i]) {
            return static_cast<VTFImageFormat>(i);
        }
    }

    return IMAGE_FORMAT_NONE;
}
