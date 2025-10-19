#include "function.h"
#include <SDL3/SDL_main.h>

int main(int argc,char *argv[]){
    //格式化游戏日志
    FILE* file = fopen("Log.txt","w");
    fprintf(file,"");
    fclose;
    LogOutput("Start initializing");
    //初始化SDL视频模块
    if(!SDL_Init(SDL_INIT_VIDEO)){
        LogOutput(SDL_GetError());
        return 1;
    }
    
    //初始化SDL字体模块
    if(!TTF_Init()){
        SDL_Quit();
        LogOutput(SDL_GetError());
        return 1;
    }
    //创建窗口
    char title_text[CHARMAX];
    LangRead("PlaneFiles",title_text);
    SDL_Window* window = SDL_CreateWindow(title_text, 1280 , 720 ,SDL_WINDOW_RESIZABLE);
    //获取窗口大小
    int winW,winH;
    SDL_GetWindowSize(window,&winW,&winH);
    if(!window){
        LogOutput(SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }else {
        LogOutput("SDL_Window has been successfully created");
    }
    //创建渲染器
    SDL_Renderer* render = SDL_CreateRenderer(window,NULL);
    if(!render){
        LogOutput(SDL_GetError());
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }else {
        LogOutput("SDL_Render has been successfully created");
    }
    //加载控件公用材质
    SDL_Texture* control_public[2] = {IMG_LoadTexture(render, "assets/Control/1.png"),IMG_LoadTexture(render, "assets/Control/2.png")};
    //加载背景材质
    SDL_Texture* background = IMG_LoadTexture(render,"assets/background.png");
    //创建链表
    LinkList* head = CreateLinkList(10);
    if(!head){
        LogOutput("Error:The head variable of the LinkList type is set to NULL in memory!");
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(control_public[0]);
        SDL_DestroyTexture(control_public[1]);
        SDL_DestroyRenderer(render);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }else {
        LogOutput("Memory allocation for the head variable of type LinkList is successful.");
    }
    head->data = control_public;
    //创建按钮1
    CreateControl(1001,"Start",render,&head);
    SetControlFontSize(150,230,200,100,1001,&head);
    //创建按钮2
    CreateControl(1002,"Exit",render,&head);
    SetControlFontSize(150,430,200,100,1002,&head);
    //创建渲染
    int running = 0;
    //定义当前图层变量
    int layer = 1;
    SDL_Event event;
    while(!running){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_EVENT_QUIT){
                running = 1;
            }else if(event.type == SDL_EVENT_MOUSE_MOTION){
                ButtonDecision(&head,1001);
            }else if(event.type == SDL_EVENT_WINDOW_RESIZED){
                ControlReSize(&head,1001,window,&winW,&winH);
            }else if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
                if(layer == 1 && GetButtonType(&head,1002)){
                    running = 1;
                }
                if(layer == 1 && GetButtonType(&head,1001)){
                    layer = 2;
                }
            }
        }
        //绘制背景初始颜色
        SDL_SetRenderDrawColor(render,100, 100, 220, 255);
        //清理屏幕
        SDL_RenderClear(render);
        //渲染背景图
        SDL_RenderTexture(render,background,NULL,NULL);
        //渲染控件
        if(GetLayer(&head, 1001) == layer) ControlRender(&(head),1001,render);
        //更新屏幕
        SDL_RenderPresent(render);
        //显示帧率60fps
        SDL_Delay(16);
    }
    //内存释放
    DestroyControl(&head,1001);
    FreeLinkList(&head);
    free(head);
    head = NULL;
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(control_public[0]);
    SDL_DestroyTexture(control_public[1]);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}