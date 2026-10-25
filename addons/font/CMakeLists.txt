set(FONT_SOURCES font.c fontbmp.c stdfont.c text.c)
set(FONT_DEFINES "-DALLEGRO_FONT_SRC")
if(WANT_FONT_BMFONT)
    list(APPEND FONT_SOURCES bmfont.c xml.c)
else()
    string(APPEND FONT_DEFINES " -DALLEGRO_CFG_NO_FONT_BMFONT")
endif()

set(FONT_INCLUDE_FILES allegro5/allegro_font.h)

set_our_header_properties(${FONT_INCLUDE_FILES})

add_our_addon_library(allegro_font
    AllegroFont-${ALLEGRO_SOVERSION}
    "${FONT_SOURCES};${FONT_INCLUDE_FILES}"
    "${FONT_DEFINES}"
    "${ALLEGRO_LINK_WITH}"
    )

install_our_headers(${FONT_INCLUDE_FILES})

add_addon(font)

#-----------------------------------------------------------------------------#
# vim: set ts=8 sts=4 sw=4 et:
