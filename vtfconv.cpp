#include <iostream>
#include <string>
#include <utility>

#include <VTFLib13/VTFLib.h>
#include <VTFLib13/VTFFile.h>
#include <Magick++.h>
#include <tclap/CmdLine.h>

#include "formatnames.hpp"
#include "common.hpp"

using VTFLib::CVTFFile;

Magick::Image load_bitmap(const std::string& path)
{
    // Load source file
    Magick::Image image;
    try {
        image.read(path);
    } catch(Magick::ErrorMissingDelegate) {
        std::cerr << "Unsupported input image format" << std::endl;
        exit(1);
    } catch(Magick::ErrorCorruptImage) {
        std::cerr << "Failed to decode input image" << std::endl;
        exit(1);
    }
    return std::move(image);
}

CVTFFile load_vtf(const std::string& path)
{
    CVTFFile vtf;

    if (!vtf.Load(path.c_str())) {
        std::cerr << "Failed to load .vtf file." << std::endl;
        exit(1);
    }
    return std::move(vtf);
}

CVTFFile convert_bitmap(Magick::Image& image)
{
    // Get size
    Magick::Geometry imgsize = image.size();
    // Needs testing
    int width = image.columns();
    int height = image.rows();

    // Create baseimage
    CVTFFile baseimage;
    baseimage.Create(width, height);
    image.write(0, 0, width, height, "RGBA", Magick::CharPixel,
        baseimage.GetData(0, 0, 0, 0));

    return std::move(baseimage);
}

CVTFFile convert_vtf(CVTFFile& vtf)
{
    int width = vtf.GetWidth();
    int height = vtf.GetHeight();

    // Create baseimage
    CVTFFile baseimage;
    baseimage.Create(width, height);
    vtf.Convert(
        vtf.GetData(0,0,0,0), baseimage.GetData(0,0,0,0), width, height,
        vtf.GetFormat(), baseimage.GetFormat()
    );

    return std::move(baseimage);
}

void save_vtf(
    const CVTFFile& baseimage,
    VTFImageFormat format,
    const std::string& path)
{
    int width = baseimage.GetWidth();
    int height = baseimage.GetHeight();

    CVTFFile vtf;
    vtf.Create(width, height, 1, 1, 1, format);
    CVTFFile::Convert(
        baseimage.GetData(0,0,0,0), vtf.GetData(0,0,0,0), width, height,
        IMAGE_FORMAT_RGBA8888, format
    );

    // Write output
    vtf.Save(path.c_str());
}

void save_bitmap(
    const CVTFFile& baseimage,
    Magick::ImageType format,
    const std::string& path)
{
    Magick::Image image;
    image.type(format);
    image.modifyImage();
    image.read(
        baseimage.GetWidth(), baseimage.GetHeight(), "RGBA",
        Magick::CharPixel, baseimage.GetData(0,0,0,0)
    );

    image.write(path);
}

bool has_alpha(const Magick::Image& image)
{
    return image.matte();
}

bool has_alpha(const CVTFFile& vtf)
{
    VTFImageFormat format = vtf.GetFormat();
    SVTFImageFormatInfo format_info = CVTFFile::GetImageFormatInfo(format);
    return format_info.uiAlphaBitsPerPixel > 0;
}

VTFImageFormat get_vtf_format(bool alpha)
{
    if (alpha) {
        return IMAGE_FORMAT_ARGB8888;
    } else {
        return IMAGE_FORMAT_RGB888;
    }
}

Magick::ImageType get_bitmap_format(bool alpha)
{
    if (alpha) {
        return Magick::TrueColorMatteType;
    } else {
        return Magick::TrueColorType;
    }
}

enum class ProgramMode
{
    BMP2VTF,
    VTF2BMP,
    INFO
};

int main(int argc, char* argv[])
{
    Magick::InitializeMagick(argv[0]);
    TCLAP::CmdLine cmdline(
        "Convert between vtf textures and various image formats", ' ', "0.1");
    TCLAP::UnlabeledValueArg<std::string> input_arg(
        "input", "Input file path", true, "", "input");
    TCLAP::UnlabeledValueArg<std::string> output_arg(
        "output", "Output file path", false, "", "output");
    TCLAP::ValueArg<std::string> format_arg(
        "f", "format", "Format of the output texture", false, "AUTO",
        "format");
    cmdline.add(input_arg);
    cmdline.add(output_arg);
    cmdline.add(format_arg);
    cmdline.parse(argc, argv);

    std::string inpath = input_arg.getValue();
    std::string outpath = output_arg.getValue();
    std::string formatstr = format_arg.getValue();

    ProgramMode mode;
    if (outpath.empty()) {
        mode = ProgramMode::INFO;
    } else if (endswith(lower(inpath), ".vtf")) {
        mode = ProgramMode::VTF2BMP;
    } else {
        mode = ProgramMode::BMP2VTF;
    }

    if (mode == ProgramMode::BMP2VTF) {
        // Parse output format
        VTFImageFormat outformat = IMAGE_FORMAT_NONE;
        if (formatstr != "AUTO") {
            outformat = parse_format(formatstr);
            if (outformat == IMAGE_FORMAT_NONE) {
                std::cerr << "Unknown texture format: " << formatstr << std::endl;
                return 1;
            }
        }

        Magick::Image image = load_bitmap(inpath);
        CVTFFile baseimage = convert_bitmap(image);

        if (outformat == IMAGE_FORMAT_NONE) {
            outformat = get_vtf_format(has_alpha(image));
        }
        save_vtf(baseimage, outformat, outpath);
    } else if (mode == ProgramMode::VTF2BMP) {
        CVTFFile vtf = load_vtf(inpath);
        CVTFFile baseimage = convert_vtf(vtf);
        Magick::ImageType outformat = get_bitmap_format(has_alpha(vtf));
        save_bitmap(baseimage, outformat, outpath);
    } else if (mode == ProgramMode::INFO) {
        CVTFFile vtf = load_vtf(inpath);
        VTFImageFormat format = vtf.GetFormat();
        SVTFImageFormatInfo format_info = CVTFFile::GetImageFormatInfo(format);

        std::cout << "Name: " << inpath << std::endl;
        std::cout << "Format: " << format_info.lpName << std::endl;
        std::cout << "Dimensions: "
            << vtf.GetWidth() << 'x' << vtf.GetHeight() << std::endl;
        std::cout << "Alpha: " << std::boolalpha << has_alpha(vtf) << std::endl;
    }

    return 0;
}
