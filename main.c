/// @file main.c
/// @brief Title of Brief: Programming project 1
///        This is a program which represent a simple version of Pacman game.
///        The program includes different tasks such as drawing a maze with walls and pills,
///        moving the pacman in the correct maze areas and animating the pacman using the sprite sheets.
/// @author of start (beginning) code: Jon Macey
/// @author of adding and final code: Anastasija Belaka
/// @version unknown
/// @date Last Revision 15/01/2017 Cleaning and adding comments
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
// include the map for the maze
#include "map.h"
// include boolean type and values
#include <stdbool.h>
// the size of the block to draw
const int BLOCKSIZE=25;
//the size of pacman's width (sprite sheet)
const int pacmanWidth=22;
//the size of pacman's height (sprite sheet)
const int pacmanHeight=20;
// the width of the screen taking into account the maze and block
#define WIDTH COLS*BLOCKSIZE
// the height of the screen taking into account the maze and block
#define HEIGHT ROWS*BLOCKSIZE
// an enumeration for direction to move
enum DIRECTION{UP,DOWN,LEFT,RIGHT,NONE};
//----------------------------------------------------------------------------------------------------------------------
/// @brief movePacman this function allows pacman to move in the correct maze areas and eat red pills, check the
///        collision with the walls, teleport from left to right (and back) on the middle maze crossing sections
/// @param[in] pacman the pacman and its position (x,y)
/// @param[in] directions the current direction of pacman
/// @param[in] prevDirections the previous direction of pacman
/// @param[in] map the map (array) which contains values (from the file map.h)
/// @param[in] tryUP until there is a possibility to move UP keep trying to move UP (prediction)
/// @param[in] tryDOWN until there is a possibility to move DOWN keep trying to move DOWN (prediction)
/// @param[in] tryLEFT until there is a possibility to move LEFT keep trying to move LEFT (prediction)
/// @param[in] tryRIGHT until there is a possibility to move RIGHT keep trying to move RIGHT (prediction)
//----------------------------------------------------------------------------------------------------------------------
void movePacman(SDL_Rect *pacman, int *directions, int *prevDirections, char map[ROWS][COLS], bool *tryUP, bool *tryDOWN, bool *tryLEFT, bool *tryRIGHT)
{
    //if there is no wall from up than after going up stop trying to move up (and other directions)
    if ( *tryUP == true &&
       ( map[((pacman->y+pacmanHeight-1)/BLOCKSIZE)-1][(pacman->x+pacmanWidth-1)/BLOCKSIZE] != 1 ) &&
       ( map[((pacman->y+pacmanHeight-1)/BLOCKSIZE)-1][pacman->x/BLOCKSIZE] != 1 )
       )
    {
        *directions=UP;
        *tryUP=false;
        *tryDOWN=false;
        *tryLEFT=false;
        *tryRIGHT=false;
        // clear previous direction
        *prevDirections=NONE;
    }
    //if there is no wall from down than after going down stop trying to move down (and other directions)
    if ( *tryDOWN == true &&
       ( map[pacman->y/BLOCKSIZE+1][(pacman->x+pacmanWidth-1)/BLOCKSIZE] != 1 ) &&
       ( map[pacman->y/BLOCKSIZE+1][pacman->x/BLOCKSIZE] != 1 )
       )
    {
        *directions=DOWN;
        *tryUP=false;
        *tryDOWN=false;
        *tryLEFT=false;
        *tryRIGHT=false;
        // clear previous direction
        *prevDirections=NONE;
    }
    //if there is no wall from left than after going left stop trying to move left (and other directions)
    if ( *tryLEFT == true &&
       ( map[pacman->y/BLOCKSIZE][((pacman->x+pacmanWidth-1)/BLOCKSIZE)-1] != 1 ) &&
       ( map[(pacman->y+pacmanHeight-1)/BLOCKSIZE][((pacman->x+pacmanWidth-1)/BLOCKSIZE)-1] != 1 )
       )
    {
        *directions=LEFT;
        *tryUP=false;
        *tryDOWN=false;
        *tryLEFT=false;
        *tryRIGHT=false;
        // clear previous direction
        *prevDirections=NONE;
    }
    //if there is no wall from right than after going right stop trying to move right (and other directions)
    if ( *tryRIGHT == true &&
       ( map[pacman->y/BLOCKSIZE][pacman->x/BLOCKSIZE+1] != 1 ) &&
       ( map[(pacman->y+pacmanHeight-1)/BLOCKSIZE][pacman->x/BLOCKSIZE+1] != 1 )
       )
    {
        *directions=RIGHT;
        *tryUP=false;
        *tryDOWN=false;
        *tryLEFT=false;
        *tryRIGHT=false;
        // clear previous direction
        *prevDirections=NONE;
    }

    // teleport from left to right on the middle maze crossing sections
    if ( (pacman->x+pacmanWidth-1)/BLOCKSIZE == 0 && *directions == LEFT )
    {
        pacman->x=BLOCKSIZE*28;
    }
    // teleport from right to left on the middle maze crossing sections
    if ( pacman->x/BLOCKSIZE == 27 && *directions == RIGHT )
    {
        pacman->x=-BLOCKSIZE;
    }

    if ( *directions == LEFT )
    {
        // check if there is no wall from the left of the pacman
        // need the second line for proper moving because x and y is only the upper left corner of the pacman
        if ( ( map[pacman->y/BLOCKSIZE][((pacman->x+pacmanWidth-1)/BLOCKSIZE)-1] != 1 ) &&
           ( map[(pacman->y+pacmanHeight-1)/BLOCKSIZE][((pacman->x+pacmanWidth-1)/BLOCKSIZE)-1] != 1 )
           )
        {
            // move left
            pacman->x--;
            // replace the red pill from left with empty (black) block when pacman reaches (eats) it
            map[pacman->y/BLOCKSIZE][pacman->x/BLOCKSIZE] = 0;
        }
        else
        {
            // if there is a wall from left do not try to go left
            // while moving in different direction and move as that previous direction
            if( *prevDirections == UP || *prevDirections == DOWN )
            {
                *directions = *prevDirections;
            }
            else
            {
                // otherwise stop completely
                *directions = NONE;
            }
        }
    }

    if ( *directions == RIGHT )
    {
        // check if there is no wall from the right of the pacman
        // need the second line for proper moving because x and y is only the upper left corner of the pacman
        if ( ( map[pacman->y/BLOCKSIZE][pacman->x/BLOCKSIZE+1] != 1 ) &&
           ( map[(pacman->y+pacmanHeight-1)/BLOCKSIZE][pacman->x/BLOCKSIZE+1] != 1 )
           )
        {
            // move right
            pacman->x++;
            // replace the red pill from right with empty (black) block when pacman reaches (eats) it
            map[pacman->y/BLOCKSIZE][(pacman->x+pacmanWidth-1)/BLOCKSIZE] = 0;
        }
        else
        {
            // if there is a wall from right do not try to go right
            // while moving in different direction and move as that previous direction
            if( *prevDirections == UP || *prevDirections == DOWN )
            {
                *directions = *prevDirections;
            }
            else
            {
                // otherwise stop completely
                *directions = NONE;
            }
        }
    }

    if ( *directions == UP )
    {
        // check if there is no wall from the up of the pacman
        // need the first line for proper moving because x and y is only the upper left corner of the pacman
        if ( ( map[((pacman->y+pacmanHeight-1)/BLOCKSIZE)-1][(pacman->x+pacmanWidth-1)/BLOCKSIZE] != 1 ) &&
           ( map[((pacman->y+pacmanHeight-1)/BLOCKSIZE)-1][pacman->x/BLOCKSIZE] != 1 )
           )
        {
            //move up
            pacman->y--;
            // replace the red pill from up with empty (black) block when pacman reaches (eats) it
            map[pacman->y/BLOCKSIZE][pacman->x/BLOCKSIZE] = 0;
        }
        else
        {
            // if there is a wall from up do not try to go up
            // while moving in different direction and move as that previous direction
            if( *prevDirections == LEFT || *prevDirections == RIGHT )
            {
                *directions = *prevDirections;
            }
            else
            {
                // otherwise stop completely
                *directions = NONE;
            }
        }
    }

    if ( *directions == DOWN )
    {
        // check if there is no wall from the down of the pacman
        // need the first line for proper moving because x and y is only the upper left corner of the pacman
        if ( ( map[pacman->y/BLOCKSIZE+1][(pacman->x+pacmanWidth-1)/BLOCKSIZE] != 1 ) &&
           ( map[pacman->y/BLOCKSIZE+1][pacman->x/BLOCKSIZE] != 1 )
           )
        {
            //move down
            pacman->y++;
            // replace the red pill from down with empty (black) block when pacman reaches (eats) it
            map[(pacman->y+pacmanHeight-1)/BLOCKSIZE][pacman->x/BLOCKSIZE] = 0;
        }
        else
        {
            // if there is a wall from down do not try to go down
            // while moving in different direction and move as that previous direction
            if( *prevDirections == LEFT || *prevDirections == RIGHT )
            {
                *directions = *prevDirections;
            }
            else
            {
                // otherwise stop completely
                *directions = NONE;
            }
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------
/// @brief drawMaze this function allows to draw a maze using the map (array)
///        from the file map.h which contains following values:
///             value 0 - representing a black/empty block (a path for the pacman)
///             value 1 - representing a blue block (wall)
///             value 2 - representing a smaller red block (red pill)
/// @param[in] block allows to draw the maze by blocks
/// @param[in] map the map (array) which contains values (from the file map.h) for drawing a maze
/// @param[in] ren the renderer
//----------------------------------------------------------------------------------------------------------------------
void drawMaze (SDL_Rect block, char map[ROWS][COLS], SDL_Renderer *ren)
{
    // start drawing the maze from the position x = 0 and y = 0
    block.x = 0;
    block.y = 0;
    // for each row and for each column - draw the maze by blocks
    for (int a=0; a<ROWS; ++a)
    {
        for (int b=0; b<COLS; ++b)
        {
            block.x=b*BLOCKSIZE;
            block.y=a*BLOCKSIZE;
            // draw the walls
            if (map[a][b] == 1)
            {
                block.w=BLOCKSIZE;
                block.h=BLOCKSIZE;
                // set the colour (blue)
                SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
                SDL_RenderFillRect(ren,&block);
            }
            // draw the path for the pacman
            if (map[a][b] == 0)
            {
                block.w=BLOCKSIZE;
                block.h=BLOCKSIZE;
                // set the colour (black)
                SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
                SDL_RenderFillRect(ren,&block);
            }
            // draw the pills
            if (map[a][b] == 2)
            {
                block.x+=BLOCKSIZE/4;
                block.y+=BLOCKSIZE/4;
                block.w=BLOCKSIZE/2;
                block.h=BLOCKSIZE/2;
                // set the colour (red)
                SDL_SetRenderDrawColor(ren, 255, 0, 0, 0);
                SDL_RenderFillRect(ren,&block);
            }
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------
/// @brief drawPacman this function allows to draw and animate the pacman using sprite sheets from pacsprite.png
/// @param[in] block each block (sprite sheet) of pacman from pacsprite.png (its position and size)
/// @param[in] pacman the pacman (the animation will pass to pacman)
/// @param[in] ren the renderer
/// @param[in] tex the texture we need to render
/// @param[in] directions the current direction need to be matched with sprite sheets
//----------------------------------------------------------------------------------------------------------------------
void drawPacman(SDL_Rect block, SDL_Rect pacman, SDL_Renderer *ren, SDL_Texture *tex, int directions)
{
    // set the width
    block.w=pacmanWidth;
    //set the height
    block.h=pacmanHeight;
    // changing sprite sheet for each direction
    if (directions == LEFT)
    {
        block.x=0*pacmanWidth;
    }
    if (directions == RIGHT)
    {
        block.x=1*pacmanWidth;
    }
    if (directions == UP)
    {
        block.x=2*pacmanWidth;
    }
    if (directions == DOWN)
    {
        block.x=3*pacmanWidth;
    }

    /// The following section is from :-
    /// John Purcell (2014-2016). Game Development with SDL2 Part 4: Animating using Sprite Sheets [online]. [Accessed 2016].
    /// Available from: "https://www.caveofprogramming.com/guest-posts/animating-using-sprite-sheets.html".
    // total number of frames of the animation
    int allFrames = 7;
    // amount of delay in milliseconds for each frame
    int delayTime = 50;
    // SDL_GetTicks() method gives us the time in milliseconds
    // 'normalFrame' will give us the index of frame we want to render
    int normalFrame = (SDL_GetTicks()/delayTime)%allFrames;
    // the index of frame is multiplied by the height of the frame
    // this will give us the appropriate frame dimensions from the sprite sheet
    block.y=normalFrame*pacmanHeight;
    /// end of Citation

    if (directions == NONE)
    {
        block.x=0*pacmanWidth;
        block.y=0*pacmanHeight;
    }

    SDL_RenderCopy(ren, tex, &block, &pacman);
}

int main()
{
  // initialise SDL and check that it worked otherwise exit
  // see here for details http://wiki.libsdl.org/CategoryAPI
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
  {
    printf("%s\n",SDL_GetError());
    return EXIT_FAILURE;
  }
  // we are now going to create an SDL window
  SDL_Window *win = 0;
  win = SDL_CreateWindow("Pacman", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (win == 0)
  {
    printf("%s\n",SDL_GetError());
    return EXIT_FAILURE;
  }
  // the renderer is the core element we need to draw, each call to SDL for drawing will need the
  // renderer pointer
  SDL_Renderer *ren = 0;
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  // check to see if this worked
  if (ren == 0)
  {
    printf("%s\n",SDL_GetError() );
    return EXIT_FAILURE;
  }  
  // SDL image is an abstraction for all images
  SDL_Surface *image;
  // we are going to use the extension SDL_image library to load a png, documentation can be found here
  // http://www.libsdl.org/projects/SDL_image/
  image=IMG_Load("pacsprite.png");
  if(!image)
  {
    printf("IMG_Load: %s\n", IMG_GetError());
    return EXIT_FAILURE;
  }
  // SDL texture converts the image to a texture suitable for SDL rendering  / blitting
  // once we have the texture it will be store in hardware and we don't need the image data anymore
  SDL_Texture *tex = 0;
  tex = SDL_CreateTextureFromSurface(ren, image);
  // free the image
  SDL_FreeSurface(image);

  // define necessary values such as pacman, block and different directions
  SDL_Rect block;
  SDL_Rect pacman;
  int directions;
  int prevDirections;

  bool tryUP=false;
  bool tryDOWN=false;
  bool tryLEFT=false;
  bool tryRIGHT=false;

  // define the position of pacman (on the map) at the start and its size
  pacman.x=15*BLOCKSIZE;
  pacman.y=17*BLOCKSIZE;
  pacman.w=pacmanWidth;
  pacman.h=pacmanHeight;
  // let the pacman to move left from the start
  directions=LEFT;

  // the red pill which pacman stands on at the start should disappear
  map[17][15] = 0;

  // now we clear the screen (will use the clear colour set previously)
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 0);
  SDL_RenderClear(ren);

  // now we are going to loop forever, process the keys then draw
    int quit=0;
    while (quit !=1)
    {
      SDL_Event event;
      // grab the SDL even (this will be keys etc)
      while (SDL_PollEvent(&event))
      {
        // look for the x of the window being clicked and exit
        if (event.type == SDL_QUIT)
          quit = 1;
        // check for a key down
        if (event.type == SDL_KEYDOWN)
        {
          switch (event.key.keysym.sym)
          {
          // if we have an escape quit
          case SDLK_ESCAPE : quit=1; break;
          // key control and the directions for each key
          case SDLK_LEFT :
              prevDirections = directions;
              directions = LEFT;
              tryLEFT = true;
              break;
          case SDLK_RIGHT :
              prevDirections = directions;
              directions = RIGHT;
              tryRIGHT = true;
              break;
          case SDLK_UP :
              prevDirections = directions;
              directions = UP;
              tryUP = true;
              break;
          case SDLK_DOWN :
              prevDirections = directions;
              directions = DOWN;
              tryDOWN = true;
              break;
          }
        }
      }
      // call movePacman function to move the pacman
      movePacman(&pacman, &directions, &prevDirections, map, &tryUP, &tryDOWN, &tryLEFT, &tryRIGHT);
      // call drawMaze function to draw the maze
      drawMaze(block, map, ren);
      // call drawPacman function to draw and animate the pacman
      drawPacman(block, pacman, ren, tex, directions);

  SDL_RenderPresent(ren);

  }

  SDL_Quit();
  return EXIT_SUCCESS;
}
