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
    //初始化随机种子
    srand(time(0));
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
    //加载动态背景图
    SDL_Texture* background_gif = IMG_LoadTexture(render,"assets/cloud.png");
    //创建链表
    LinkList* head = CreateLinkList(10);
    //创建标题
    SDL_Texture* title_texture = IMG_LoadTexture(render,"assets/title1.png");
    SDL_FRect title_frect = {.h= 100, .w = 400, .x = 40, .y =40};
    if (!strcmp(title_text,"PlaneFiles"))
    {
        title_texture = IMG_LoadTexture(render,"assets/title2.png");
    }
    //创建链表
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
    CreateControl(1002,"Setting",render,&head);
    SetControlFontSize(150,400,200,100,1002,&head);
    //创建按钮3
    CreateControl(1003,"Exit",render,&head);
    SetControlFontSize(150,570,200,100,1003,&head);
    //初始化共享数据
    SharedDate shared;
    shared.mutex = SDL_CreateMutex();
    shared.running = 0;
    shared.exit = 1;
    shared.num = 10;
    shared.WinW = winW;
    SDL_FRect* frectyun = MallocSDLFRectArray(shared.num);
    shared.data = frectyun;
    initSubstancePosition(frectyun,shared.num,winW,winH);
    if(!shared.mutex){
        LogOutput("Create Mutex Error!");
    }
    //创建线程
    SDL_Thread* RenderGIF = SDL_CreateThread(RenderGIFTexture,"RenderGIFTexture",&shared);
    if(!RenderGIF){
        LogOutput("Create RenderGIF Error!");
    }
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
                FollowZoom(window,&title_frect,&winW,&winH);
                ControlReSize(&head,1001,window,&winW,&winH);
                SDL_LockMutex(shared.mutex);
                FRectResize(window,frectyun,shared.num,&winW,&winH);
                SDL_GetWindowSize(window,&winW,&winH);
                shared.WinW = winW;
                SDL_UnlockMutex(shared.mutex);
            }else if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
                if(layer == 1 && GetButtonType(&head,1003)){
                    running = 1;
                }
                if(layer == 1 && GetButtonType(&head,1001)){
                    SDL_LockMutex(shared.mutex);
                    shared.running = 1;
                    SDL_UnlockMutex(shared.mutex);
                    layer = 2;
                }
            }
        }
        //绘制背景初始颜色
        SDL_SetRenderDrawColor(render,87, 163, 249, 255);
        //清理屏幕
        SDL_RenderClear(render);
        //渲染背景图
        if(layer == 1) SDL_RenderTexture(render,background,NULL,NULL);
        SDL_LockMutex(shared.mutex);
        if(layer == 2) {
            for(int i = 0;i < shared.num;i++){
                SDL_RenderTexture(render,background_gif,NULL,&frectyun[i]);
            }
        }
        SDL_UnlockMutex(shared.mutex);
        //渲染控件
        if (layer == 1) SDL_RenderTexture(render,title_texture,NULL,&title_frect);
        
        if(GetLayer(&head, 1001) == layer) ControlRender(&(head),1001,render);
        //更新屏幕
        SDL_RenderPresent(render);
        //显示帧率60fps
        SDL_Delay(16);
    }
    SDL_LockMutex(shared.mutex);
    shared.running = 0;
    shared.exit = 0;
    SDL_UnlockMutex(shared.mutex);
    //等待线程结束
    LogOutput("wait thread(RenderGIF) end...");
    SDL_WaitThread(RenderGIF,NULL);
    LogOutput("wait thread(RenderGIF) end!");
    free(frectyun);
    LogOutput("The SDL_FRect array has been successfully dynamically released.");
    //内存释放
    DestroyControl(&head,1001);
    FreeLinkList(&head);
    free(head);
    head = NULL;
    SDL_DestroyMutex(shared.mutex);
    SDL_DestroyTexture(background_gif);
    SDL_DestroyTexture(title_texture);
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(control_public[0]);
    SDL_DestroyTexture(control_public[1]);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

