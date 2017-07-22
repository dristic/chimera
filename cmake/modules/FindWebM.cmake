find_path(WEBM_INCLUDE_DIR vpx/vpx_decoder.h vpx/vp8dx.h)
find_library(WEBM_LIBRARY libvpx.a)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    WebM
    DEFAULT_MSG
    WEBM_LIBRARY
    WEBM_INCLUDE_DIR
)

mark_as_advanced(WEBM_INCLUDE_DIR WEBM_LIBRARY)

set(WEBM_LIBRARIES ${WEBM_LIBRARY} )
set(WEBM_INCLUDE_DIRS ${WEBM_INCLUDE_DIR} )
