# Automakes makefile template - this is processed by ./configure to create a Makefile
# This uses the same structure as a regular Makefile

# Automake can pull in files from subdirectories but build in the main directory.
AUTOMAKE_OPTIONS = subdir-objects
# Add the -D option to the INSTALL (/usr/bin/install) command to prevent it omitting directories
INSTALL = @INSTALL@ -D
# The build target (the exe name)
bin_PROGRAMS = barkout
# Source files must be listed manually. No wildcards.
barkout_SOURCES = src/barkanoid.c src/app.h src/brick.h \
		src/arena.c src/arena.h \
		src/ball.c src/ball.h \
		src/bat.c src/bat.h \
		src/bonus.c src/bonus.h \
		src/bounds.c src/bounds.h \
		src/cat.c src/cat.h \
		src/config.c src/config.h \
		src/intro.c src/intro.h \
		src/levels.c src.levels.h \
		src/menu.c src/menu.h \
		src/resourcefactory.c src/resourcefactory.h \
		src/scores.c src/scores.h \
		src/text.c src/text.h \
		src/vector.c src/vector.h \
		src/resources.c src/resources.h
# Additional platform-independent data files
FONTS = 	Fonts/Nordine.ttf \
		Fonts/Vectroid.otf \
		Fonts/Oxanium-Light.ttf
#LEVELS = 	Levels/level1.lvl \
#		Levels/level2.lvl \
#		Levels/level3.lvl \
#		Levels/level4.lvl \
#		Levels/level5.lvl \
#		Levels/level6.lvl \
#		Levels/level7.lvl \
#		Levels/level8.lvl \
#		Levels/level9.lvl \
#		Levels/level10.lvl \
#		Levels/level11.lvl \
#		Levels/level12.lvl \
#		Levels/level13.lvl \
#		Levels/level14.lvl \
#		Levels/level15.lvl \
#		Levels/level16.lvl \
#		Levels/level17.lvl \
#		Levels/level18.lvl \
#		Levels/level19.lvl \
#		Levels/level20.lvl \
#		Levels/level21.lvl \
#		Levels/level22.lvl \
#		Levels/level23.lvl \
#		Levels/level24.lvl \
#		Levels/level25.lvl
SOUNDS = 	Sounds/barkanoid-1up.wav \
		Sounds/barkanoid-bat.wav \
		Sounds/barkanoid-brick.wav \
		Sounds/barkanoid-brick-high.wav \
		Sounds/barkanoid-brick-laser.wav \
		Sounds/barkanoid-dead.wav \
		Sounds/barkanoid-explosion.wav \
		Sounds/barkanoid-getready.wav \
		Sounds/barkanoid-hitcat.wav \
		Sounds/barkanoidiii.mp3 \
		Sounds/barkanoid-laser.wav \
		Sounds/barkanoid-victory.wav \
		Sounds/barkanoid-warp.wav \
		Sounds/barkanoid-wormhole-in.wav \
		Sounds/barkanoid-wormhole-out.wav \
		Sounds/barkanoid-meow.wav \
		Sounds/barkanoid-ow.wav
SPRITES = 	Sprites/ball-deadly.png \
		Sprites/ball.png \
		Sprites/barkout-intro.png \
		Sprites/bat_grow.png \
		Sprites/bat_laserify.png \
		Sprites/bat_laser.png \
		Sprites/bat_long.png \
		Sprites/bat.png \
		Sprites/bat_shrink.png \
		Sprites/bat_small.png \
		Sprites/bg1_fg.png \
		Sprites/bg1_mg.png \
		Sprites/bg1.png \
		Sprites/bg2_fg.png \
		Sprites/bg2_mg.png \
		Sprites/bg2.png \
		Sprites/bg3_fg.png \
		Sprites/bg3_mg.png \
		Sprites/bg3.png \
		Sprites/bg4.png \
		Sprites/blue.png \
		Sprites/bonus-c.png \
		Sprites/bonus-e.png \
		Sprites/bonus-l.png \
		Sprites/bonus.png \
		Sprites/bonus-p.png \
		Sprites/bonus-s.png \
		Sprites/bonus-w.png \
		Sprites/border.png \
		Sprites/boss.png \
		Sprites/cat-die.png \
		Sprites/cat.png \
		Sprites/cat-spawn.png \
		Sprites/darkgrey.png \
		Sprites/green_bottom.png \
		Sprites/green.png \
		Sprites/grey_broken.png \
		Sprites/grey.png \
		Sprites/grey_repair.png \
		Sprites/life.png \
		Sprites/orange.png \
		Sprites/purple.png \
		Sprites/red.png \
		Sprites/scores.png \
		Sprites/warp.png \
		Sprites/white.png \
		Sprites/white_top.png \
		Sprites/wormhole2.png \
		Sprites/wormhole.png \
		Sprites/yellow.png \
		Sprites/FatHorseGames.png \
		Sprites/switch.png

# nobase tells it to include the prefixing directory names in the destination (i.e. not just dump the files into the single data folder)
nobase_dist_pkgdata_DATA = $(FONTS) $(SOUNDS) $(SPRITES) Levels/levels.dat

# Flags passed to the C Preprocessor; in this case the install directory
# The game tests if an installed file exists and then sets a variable (see config.c)
AM_CPPFLAGS = -DINSTALLDIR=\"$(datadir)/@PACKAGE@\"
