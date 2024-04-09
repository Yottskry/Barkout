#include "levels.h"

void levels_compile(const char* folder, const char* ofolder)
{
  //loop through specified folder and find level files
  #ifndef _WIN32
  int count = 0;
  DIR* d;
  struct dirent* dir;
  d = opendir(folder);

  assert(d);

  if(d)
  {
    while((dir = readdir(d))!=NULL)
    {
      if(dir->d_type != DT_REG)
        continue;
      int len = strlen(dir->d_name);
      char* cpy = calloc(sizeof(char), len+1);

      strcpy(cpy, dir->d_name);
      if(strcmp(cpy+(len-4), ".lvl")==0)
        count++;
      free(cpy);
    }
    closedir(d);
  }

  printf("Found %d levels. Compiling...\n", count);

  char ofilename[255] = "";
  strcpy(ofilename, ofolder);
  strcat(ofilename, "/levels.dat");

  printf("Creating output file %s\n", ofilename);

  FILE* outfile = fopen(ofilename, "wb");

  fwrite(&count, sizeof(int), 1, outfile);

  for(int i = 1; i <= count; i++)
  {
    // Open the file and read the data, flip the bits, and concatenate to dat file
    char fname[20] = "";
    char apath[255] = "";
    strcpy(apath, folder);
    sprintf(fname, "/level%d.lvl", i);
    strcat(apath, fname);

    printf("Including %s\n", apath);

    FILE* levelfile = fopen(apath, "rb");
    // File if the file could not be opened
    assert(levelfile != NULL);

    int levelsize = 0;
    fwrite(&levelsize, sizeof(int), 1, outfile);

    char buffer[100];
    size_t totalbytes = 0;
    size_t readbytes = fread(buffer, sizeof(char), 100, levelfile);
    while((readbytes > 0))
    {
      totalbytes += readbytes;

      for(size_t j = 0; j < readbytes; j++)
      {
        buffer[j] = buffer[j] ^ 0xFF;
      }
      fwrite(buffer, sizeof(char), readbytes, outfile);
      readbytes = fread(buffer, sizeof(char), 100, levelfile);
    }
    fseek(outfile, -1 * (totalbytes + sizeof(int)), SEEK_CUR);
    fwrite(&totalbytes, sizeof(int), 1, outfile);
    fseek(outfile, 0, SEEK_END);
    fclose(levelfile);
  }
  fclose(outfile);
  #else
  printf("This function is not supported in Windows\n");
  #endif // _WIN32
}

void levels_processrow(ResourceFactory* factory, Level* level, Bounds* bounds, const char* rowdata, int row, int* brickno)
{
  // Read each character. This is the column.
  for(size_t col = 0; col < strlen(rowdata); col++)
  {
    if(rowdata[col] == '.')
      continue;

    if(rowdata[col] == '@')
    {
      level->spawnx = bounds->left + (col * BRICKW);
      level->spawny = bounds->top + (row * BRICKH);
      continue;
    }

    Brick* brick;

    level->bricks = realloc(level->bricks, sizeof(Brick*) * ++level->brickcount);
    level->bricks[*brickno] = malloc(sizeof(Brick));

    brick = level->bricks[*brickno];
    brick->isdead = false;
    brick->left = bounds->left + (col * BRICKW);
    brick->right = bounds->left + (col * BRICKW) + BRICKW;
    brick->top = bounds->top + (row * BRICKH);
    brick->bottom = bounds->top + (row * BRICKH) + BRICKH;
    brick->starthitcount = 1;
    brick->sprite = malloc(sizeof(Sprite));
    brick->type = btNormal;
    brick->solidedges = eNone;
    brick->counter = 0;
    brick->sprite->onanimfinished = NULL;
    brick->sprite->data = NULL;
    brick->sprite->sender = NULL;
    Animation* brickanim;
    char c = rowdata[col];
    switch(c){
      case 'r':
        brickanim = af_getanimation(factory, "red");
      break;
      case 'b':
        brickanim = af_getanimation(factory, "blue");
      break;
      case 'g':
        brickanim = af_getanimation(factory, "green");
      break;
      case 'p':
        brickanim = af_getanimation(factory, "purple");
      break;
      case 'w':
        brickanim = af_getanimation(factory, "yellow");
      break;
      case 'y':
        brickanim = af_getanimation(factory, "grey");
      break;
      case 't':
        brickanim = af_getanimation(factory, "white");
      break;
      case 'v':
        brick->solidedges = eLeft | eRight | eBottom;
        brickanim = af_getanimation(factory, "white-top");
      break;
      case '^':
        brick->solidedges = eLeft | eRight | eTop;
        brickanim = af_getanimation(factory, "green-bottom");
      break;
      case '#':
        // Resurrecting bricks
        brick->starthitcount = -1;
        brickanim = af_getanimation(factory, "grey-broken");
        brick->type = btResurrecting;
        brick->sprite->sender = (void*)(brick);
        brick->sprite->data = (void*)(factory);
        brick->sprite->onanimfinished = arena_brickfinished;
      break;
      case 'G':
        brick->starthitcount = 2;
        brick->type = btHard;
        brickanim = af_getanimation(factory, "darkgrey");
      break;
      case 'O':
        brick->starthitcount = -1;
        brick->type = btIndestructible;
        // Not really necessary as we never test for this type of brick
        brick->solidedges = eLeft | eRight | eTop | eBottom;
        brickanim = af_getanimation(factory, "orange");
      break;
      case '*':
        brick->starthitcount = -1;
        brick->type = btWormhole;
        brick->left = bounds->left + (col * BRICKW) - (BRICKW / 2);
        brick->right = bounds->left + (col * BRICKW) + (BRICKW * 2);
        brick->top = bounds->top + (row * BRICKH) - (BRICKH / 2);
        brick->bottom = bounds->top + (row * BRICKH) + (BRICKH * 2);
        brickanim = af_getanimation(factory, "wormhole");
      break;
      case '%':
        // The boss brick!
        brick->starthitcount = 32;
        brick->right = bounds->left + (col * BRICKW) + (BRICKW * 3);
        brick->bottom = bounds->top + (row * BRICKH) + (BRICKH * 4);
        brickanim = af_getanimation(factory, "boss");
      break;
    }
    brick->hitcount = brick->starthitcount;
    brick->sprite->anim = brickanim;
    brick->sprite->state = brick->type == btWormhole ? asLooping : asStatic;
    brick->sprite->currentframe = 0;
    brick->sprite->loop = brick->type == btWormhole ? 1 : 0;
    brick->sprite->lastticks = 0;

    (*brickno)++;
  }
}

void arena_brickfinished(void* sender, void* data)
{
  Brick* brick = (Brick*)sender;
  brick->counter = RESURRECTTIMER;
  brick->sprite->currentframe = 0;
  ResourceFactory* factory = (ResourceFactory*)data;
  af_setanimation(factory, brick->sprite, "grey-repair", 0, arena_brickrepaired, (void*)brick, (void*)factory);
  // Set the animation to the reappearing animation here
  // as the frame doesn't increase unless the sprite as
  // drawn, and it won't draw while the counter > 0
}

void arena_brickrepaired(void* sender, void* data)
{
  // Repair animation has finished. Set it back to standard brick animation.
  Brick* brick = (Brick*)sender;
  brick->sprite->currentframe = 0;
  ResourceFactory* factory = (ResourceFactory*)data;
  af_setanimation(factory, brick->sprite, "grey-broken", 0, arena_brickfinished, (void*)brick, (void*)factory);
  // We need to set this to prevent these two routines just calling each other forever
  brick->sprite->state = asStatic;
}
