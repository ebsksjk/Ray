///////////////////////////////////////////////////////////////////////////////////////////////////
// hier kommt all der Müll rein
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <SDL2/SDL.h>
#include <math.h>

#define mapWidth 8
#define mapHeight 8

// colors:
/*
1: (0, 81, 194)
2: (0, 1, 127)
3: (68, 1, 103)
4: (153, 1, 102)
5: (76, 0, 64)
*/

int worldMap[mapWidth][mapHeight] = {
    {1, 1, 1, 1, 1, 1, 1, 1}, {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 2, 0, 1, 0, 0, 1}, {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 1, 1, 0, 1}, {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1}, {1, 1, 2, 3, 4, 5, 1, 1}};

double posX = 1.0, posY = 1.0;   // x and y start position
double dirX = -1.0, dirY = 0.0;  // initial direction vector
double planeX = 0, planeY = 0.7; // the 2d raycaster version of camera plane

double rotSpeed = 0.3;
double moveSpeed = 0.3;

///////////////////////////////////////////////////////////////////////////////////////////////////

void rotate(int direction) {
  double oldDirX = dirX;
  dirX = dirX * cos(rotSpeed * direction) - dirY * sin(rotSpeed * direction);
  dirY = oldDirX * sin(rotSpeed * direction) + dirY * cos(rotSpeed * direction);
  double oldPlaneX = planeX;
  planeX =
      planeX * cos(rotSpeed * direction) - planeY * sin(rotSpeed * direction);
  planeY = oldPlaneX * sin(rotSpeed * direction) +
           planeY * cos(rotSpeed * direction);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void forwards(void) {
  if (worldMap[(int)(posX + dirX * moveSpeed)][(int)(posY)] == 0)
    posX += dirX * moveSpeed;
  if (worldMap[(int)(posX)][(int)(posY + dirY * moveSpeed)] == 0)
    posY += dirY * moveSpeed;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void backwards(void) {
  if (worldMap[(int)(posX - dirX * moveSpeed)][(int)(posY)] == 0)
    posX -= dirX * moveSpeed;
  if (worldMap[(int)(posX)][(int)(posY - dirY * moveSpeed)] == 0)
    posY -= dirY * moveSpeed;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void raycast(SDL_Renderer *renderer) {
  for (int x = 0; x < WIDTH; x++) {
    // calculate ray position and direction
    double cameraX = 2 * x / (double)WIDTH - 1; // x-coordinate in camera space
    double rayDirX = dirX + planeX * cameraX;
    double rayDirY = dirY + planeY * cameraX;
    // which box of the map we're in
    int mapX = (int)posX;
    int mapY = (int)posY;

    // length of ray from current position to next x or y-side
    double sideDistX;
    double sideDistY;


    double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
    double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);

    double perpWallDist;

    // what direction to step in x or y-direction (either +1 or -1)
    int stepX;
    int stepY;

    int hit = 0; // was there a wall hit?
    int side;    // was a NS or a EW wall hit?
    // calculate step and initial sideDist
    if (rayDirX < 0) {
      stepX = -1;
      sideDistX = (posX - mapX) * deltaDistX;
    } else {
      stepX = 1;
      sideDistX = (mapX + 1.0 - posX) * deltaDistX;
    }
    if (rayDirY < 0) {
      stepY = -1;
      sideDistY = (posY - mapY) * deltaDistY;
    } else {
      stepY = 1;
      sideDistY = (mapY + 1.0 - posY) * deltaDistY;
    }
    // perform DDA
    while (hit == 0) {
      // jump to next map square, either in x-direction, or in y-direction
      if (sideDistX < sideDistY) {
        sideDistX += deltaDistX;
        mapX += stepX;
        side = 0;
      } else {
        sideDistY += deltaDistY;
        mapY += stepY;
        side = 1;
      }
      // Check if ray has hit a wall
      if (worldMap[mapX][mapY] > 0)
        hit = 1;
    }
    // Calculate distance projected on camera direction. This is the shortest
    // distance from the point where the wall is hit to the camera plane.
    // Euclidean to center camera point would give fisheye effect! This can be
    // computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or
    // same formula with Y for size == 1, but can be simplified to the code
    // below thanks to how sideDist and deltaDist are computed: because they
    // were left scaled to |rayDir|. sideDist is the entire length of the ray
    // above after the multiple steps, but we subtract deltaDist once because
    // one step more into the wall was taken above.
    if (side == 0)
      perpWallDist = (sideDistX - deltaDistX);
    else
      perpWallDist = (sideDistY - deltaDistY);

    // Calculate height of line to draw on screen
    int lineHeight = (int)(HEIGHT / perpWallDist);

    // calculate lowest and highest pixel to fill in current stripe
    int drawStart = -lineHeight / 2 + HEIGHT / 2;
    if (drawStart < 0)
      drawStart = 0;
    int drawEnd = lineHeight / 2 + HEIGHT / 2;
    if (drawEnd >= HEIGHT)
      drawEnd = HEIGHT - 1;

    // colors:
    /*
    1: (0, 81, 194)
    2: (0, 1, 127)
    3: (68, 1, 103)
    4: (153, 1, 102)
    5: (76, 0, 64)
    */

    switch (worldMap[mapX][mapY]) {
    case 1:
      SDL_SetRenderDrawColor(renderer, 0, 81, 194, 255);
      break;

    case 2:
      SDL_SetRenderDrawColor(renderer, 0, 1, 127, 255);
      break;

    case 3:
      SDL_SetRenderDrawColor(renderer, 68, 1, 103, 255);
      break;

    case 4:
      SDL_SetRenderDrawColor(renderer, 153, 1, 102, 255);
      break;

    case 5:
      SDL_SetRenderDrawColor(renderer, 76, 0, 64, 255);
      break;

    default:
      break;
    }

    SDL_RenderDrawLine(renderer, x, drawStart, x, drawEnd);
  }
}