find_path(HARFBUZZ_INCLUDE_DIR harfbuzz/hb.h harfbuzz/hb-ft.h)
find_library(HARFBUZZ_LIBRARY harfbuzz)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    HarfBuzz
    DEFAULT_MSG
    HARFBUZZ_LIBRARY
    HARFBUZZ_INCLUDE_DIR
)

mark_as_advanced(HARFBUZZ_INCLUDE_DIR HARFBUZZ_LIBRARY)

set(HARFBUZZ_LIBRARIES ${HARFBUZZ_LIBRARY} )
set(HARFBUZZ_INCLUDE_DIRS ${HARFBUZZ_INCLUDE_DIR} )
