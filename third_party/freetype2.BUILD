genrule(
    name = "freetype2-config",
    srcs = glob(["**"]),
    outs = [
        "include/config/ftconfig.h",
        "include/config/ftheader.h",
        "include/config/ftmodule.h",
        "include/config/ftoption.h",
        "include/config/ftstdlib.h",
    ],
    cmd = "cd $$(dirname $(location configure)) && ./configure --without-harfbuzz"
)

cc_library(
    name = "freetype2",
    srcs = [
        "src/base/ftsystem.c",
        "src/base/ftinit.c",
        "src/base/ftdebug.c",

        "src/base/ftbase.c",

        "src/base/ftbbox.c",
        "src/base/ftglyph.c",

        "src/base/ftbdf.c",
        "src/base/ftbitmap.c",
        "src/base/ftcid.c",
        "src/base/ftfntfmt.c",
        "src/base/ftfstype.c",
        "src/base/ftgasp.c",
        "src/base/ftgxval.c",
        "src/base/ftlcdfil.c",
        "src/base/ftmm.c",
        "src/base/ftotval.c",
        # "src/base/ftpatent.c",
        # "src/base/ftpfr.c",
        "src/base/ftstroke.c",
        "src/base/ftsynth.c",
        "src/base/fttype1.c",
        "src/base/ftwinfnt.c",

        "src/base/ftmac.c",

        # Driver
        "src/bdf/bdf.c",
        "src/cff/cff.c",
        "src/cid/type1cid.c",
        "src/pcf/pcf.c",
        "src/pfr/pfr.c",
        "src/sfnt/sfnt.c",
        "src/truetype/truetype.c",
        "src/type1/type1.c",
        "src/type42/type42.c",
        "src/winfonts/winfnt.c",

        "src/raster/raster.c",
        "src/smooth/smooth.c",

        "src/autofit/autofit.c",
        "src/cache/ftcache.c",
        "src/gzip/ftgzip.c",
        "src/lzw/ftlzw.c",
        "src/bzip2/ftbzip2.c",
        "src/gxvalid/gxvalid.c",
        "src/otvalid/otvalid.c",
        "src/psaux/psaux.c",
        "src/pshinter/pshinter.c",
        "src/psnames/psnames.c",
    ],
    hdrs = glob([
        "include/*.h",
        "include/internal/*.h"
    ]) + [":freetype2-config"],
    defines = [
        "FT2_BUILD_LIBRARY",
    ],
    textual_hdrs = glob([
        "src/**/*.c",
        "src/**/*.h"
    ]),
    strip_include_prefix = "include",
    visibility = ["//visibility:public"],
)