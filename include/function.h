#pragma once
#include "StructClass.h"
LinkList* CreateLinkList(int type);
void AddLinkList(LinkList** head, int type);
void TraversalLink(LinkList** list);
void FreeLinkList(LinkList** list);
LinkList* LinkSearchAndModify(LinkList** list ,int type);
void LangRead(char* vkey,char* text);
SDL_Texture* FontTextureFromSurface(char* text,SDL_Color color,SDL_Renderer* render);
void CreateControl(int ID,char* text,SDL_Renderer* render,LinkList** list);
void SetControlFontSize(int x ,int y ,int w,int h,int ID, LinkList** list);
SDL_FRect SetControlSize(SDL_FRect fre);
void ControlRender(LinkList** list,int ID,SDL_Renderer* render);
void DestroyControl(LinkList** list,int ID);
void ButtonDecision(LinkList** list,int ID);
void LogOutput(const char* text);
void FollowZoom(SDL_Window* window,SDL_FRect* rect,int* winW,int* winH);
void ControlReSize(LinkList** list,int ID,SDL_Window* window,int* winW,int* winH);