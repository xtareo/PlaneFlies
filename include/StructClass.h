#pragma once
#define CHARMAX 50
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct LinkList{
    int type;
    void* data;
    struct LinkList* pre;
    struct LinkList* next;
}LinkList;

typedef struct {
    int type;
    SDL_FRect frect;
    SDL_Texture* ttf_texture;
}Control;

typedef struct{
    int type;
    int layer;
    float velocity;
    SDL_Texture* texture[2];
}Plane;