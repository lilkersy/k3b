
check_cxx_source_compiles("
#include <FLAC++/metadata.h>
int main()
{
    FLAC::Metadata::VorbisComment* vc;
    vc->get_vendor_string().get_field();
    return 0;
}
" FLAC_LESS_THAN_1_1_1 )

if( NOT FLAC_LESS_THAN_1_1_1)
    set(FLAC_NEWER_THAN_1_1_1 1)
endif()

configure_file(config-flac.h.cmake config-flac.h )

add_library(k3bflacdecoder MODULE k3bflacdecoder.cpp)

target_include_directories(k3bflacdecoder PRIVATE ${FLAC++_INCLUDE_DIR} ${FLAC_INCLUDE_DIR})

target_link_libraries(k3bflacdecoder k3bdevice k3blib KF5::I18n ${FLAC++_LIBRARIES} ${FLAC_LIBRARIES})

if(ENABLE_TAGLIB)
    target_link_libraries(k3bflacdecoder ${TAGLIB_LIBRARIES})
endif()

install(TARGETS k3bflacdecoder DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES  k3bflacdecoder.desktop DESTINATION ${SERVICES_INSTALL_DIR})
