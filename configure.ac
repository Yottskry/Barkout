# Autoconf file to generate ./configure script

# Init autoconf and specify package name, version, email, gnu name and URL
AC_INIT([barkout], [1.0], [fathorsegames@retrojunkies.co.uk], [barkout], [https://www.retrojunkies.co.uk])
AM_INIT_AUTOMAKE(foreign)
# Specify the compiler CXX for C++, CC for C
AC_PROG_CC
PKG_CHECK_MODULES([SDL2], [sdl2 >= 2.0.14])
PKG_CHECK_MODULES([SDL2_image], [SDL2_image]) 
PKG_CHECK_MODULES([SDL2_mixer], [SDL2_mixer])
# 2.0.18 needed for wrapped text
PKG_CHECK_MODULES([SDL2_ttf], [SDL2_ttf >= 2.0.18])
# Init automake to create a Makefile
AC_CONFIG_FILES([Makefile])
# Work out which -l flags we need by searching for functions within libraries
AC_SEARCH_LIBS([asinl],[m])
AC_SEARCH_LIBS([SDL_Init],[SDL2])
AC_SEARCH_LIBS([IMG_Init],[SDL2_image])
AC_SEARCH_LIBS([Mix_Init],[SDL2_mixer])
AC_SEARCH_LIBS([TTF_Init],[SDL2_ttf])
# Output the file
AC_OUTPUT
