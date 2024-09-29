#include "resources.h"

void loadResources(ResourceFactory* f, SDL_Renderer* renderer)
{
  // Load some sprites
  af_loadanimation(f, renderer, "red.png", "red", 44, 29);
  af_loadanimation(f, renderer, "blue.png", "blue", 44, 29);
  af_loadanimation(f, renderer, "green.png", "green", 44, 29);
  af_loadanimation(f, renderer, "darkgrey.png", "darkgrey", 44, 29);
  af_loadanimation(f, renderer, "orange.png", "orange", 44, 29);
  af_loadanimation(f, renderer, "purple.png", "purple", 44, 29);
  af_loadanimation(f, renderer, "grey.png", "grey", 44, 29);
  af_loadanimation(f, renderer, "yellow.png", "yellow", 44, 29);
  af_loadanimation(f, renderer, "white.png", "white", 44, 29);
  af_loadanimation(f, renderer, "switch.png", "switch", 44, 29);
  af_loadanimation(f, renderer, "bg1.png", "bg1", 600, 600);
  af_loadanimation(f, renderer, "bg1_mg.png", "bg1-mg", 600, 600);
  af_loadanimation(f, renderer, "bg1_fg.png", "bg1-fg", 660, 600);
  af_loadanimation(f, renderer, "bg2.png", "bg2", 600, 600);
  af_loadanimation(f, renderer, "bg2_mg.png", "bg2-mg", 600, 600);
  af_loadanimation(f, renderer, "bg2_fg.png", "bg2-fg", 660, 600);
  af_loadanimation(f, renderer, "bg3.png", "bg3", 600, 600);
  af_loadanimation(f, renderer, "bg3_mg.png", "bg3-mg", 600, 600);
  af_loadanimation(f, renderer, "bg3_fg.png", "bg3-fg", 660, 600);
  af_loadanimation(f, renderer, "bg4.png", "bg4", 600, 600);
  af_loadanimation(f, renderer, "scores.png", "scores", 200, 600);
  af_loadanimation(f, renderer, "bat.png", "bat", 82, 29);
  af_loadanimation(f, renderer, "ball.png", "ball", 17, 17);
  af_loadanimation(f, renderer, "bat_shrink.png", "bat-shrink", 82, 29);
  af_loadanimation(f, renderer, "bat_laser.png", "bat-laser", 82, 29);
  af_loadanimation(f, renderer, "bat_laserify.png", "bat-laserify", 82, 29);
  af_loadanimation(f, renderer, "bat_grow.png", "bat-grow", 122, 29);
  af_loadanimation(f, renderer, "bonus.png", "bonus-d", 43, 25);
  af_loadanimation(f, renderer, "bonus-s.png", "bonus-s", 43, 25);
  af_loadanimation(f, renderer, "bonus-e.png", "bonus-e", 43, 25);
  af_loadanimation(f, renderer, "bonus-c.png", "bonus-c", 43, 25);
  af_loadanimation(f, renderer, "bonus-p.png", "bonus-p", 43, 25);
  af_loadanimation(f, renderer, "bonus-l.png", "bonus-l", 43, 25);
  af_loadanimation(f, renderer, "bonus-w.png", "bonus-w", 43, 25);
  af_loadanimation(f, renderer, "bat_small.png", "bat-s", 51, 27);
  af_loadanimation(f, renderer, "bat_long.png", "bat-l", 122, 27);
  af_loadanimation(f, renderer, "ball-deadly.png", "ball-deadly", 17, 17);
  af_loadanimation(f, renderer, "barkout-intro.png", "intro", 400, 75);
  af_loadanimation(f, renderer, "life.png", "life", 38, 16);
  af_loadanimation(f, renderer, "warp.png", "warp", 40, 80);
  af_loadanimation(f, renderer, "border.png", "border", 600, 600);
  af_loadanimation(f, renderer, "cat.png", "cat", 40, 40);
  af_loadanimation(f, renderer, "cat-die.png", "cat-die", 40, 40);
  af_loadanimation(f, renderer, "cat-spawn.png", "cat-spawn", 40, 40);
  af_loadanimation(f, renderer, "wormhole2.png", "wormhole", 80, 50);
  af_loadanimation(f, renderer, "white_top.png", "white-top", 44, 29);
  af_loadanimation(f, renderer, "green_bottom.png", "green-bottom", 44, 29);
  af_loadanimation(f, renderer, "grey_broken.png", "grey-broken", 44, 29);
  af_loadanimation(f, renderer, "grey_repair.png", "grey-repair", 44, 29);
  af_loadanimation(f, renderer, "boss.png", "boss", 124, 104);
  af_loadanimation(f, renderer, "FatHorseGames.png", "logo", 100, 100);


  // And some sound
  af_loadsample(f, "barkanoid-getready.wav", "getready");
  af_loadsample(f, "barkanoid-brick.wav", "brick");
  af_loadsample(f, "barkanoid-brick-high.wav", "brick-high");
  af_loadsample(f, "barkanoid-brick-laser.wav", "brick-laser");
  af_loadsample(f, "barkanoid-bat.wav", "bat");
  af_loadsample(f, "barkanoid-dead.wav", "dead");
  af_loadsample(f, "barkanoid-1up.wav", "1up");
  af_loadsample(f, "barkanoid-warp.wav", "warp");
  af_loadsample(f, "barkanoid-hitcat.wav", "cat-hit");
  af_loadsample(f, "barkanoid-laser.wav", "laser");
  af_loadsample(f, "barkanoid-wormhole-in.wav", "wormhole-in");
  af_loadsample(f, "barkanoid-wormhole-out.wav", "wormhole-out");
  af_loadsample(f, "barkanoid-victory.wav", "victory");
  af_loadsample(f, "barkanoid-explosion.wav", "explosion");
  af_loadsample(f, "barkanoid-meow.wav", "meow");
  af_loadsample(f, "barkanoid-ow.wav", "ow");
}
