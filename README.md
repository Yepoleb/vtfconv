# vtfconv

## Usage

### VTF to Image

    $ vtfconv background01.vtf background01.png

### Image to VTF

    $ vtfconv -f DXT1 background01.png background01.vtf

### Info

    $ vtfconv background01.vtf 
    Name: background01.vtf
    Format: DXT1
    Dimensions: 1024x1024
    Alpha: false

## Dependencies

* [VTFLib]
* [Magick++]
* [TCLAP]

## Building

Run `make` and pray.

## License

GPLv3



[VTFLib]: https://github.com/panzi/VTFLib
[Magick++]: http://www.graphicsmagick.org/index.html
[TCLAP]: http://tclap.sourceforge.net

