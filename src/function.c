#include "function.h"

//创建链表元素
LinkList* CreateLinkList(int type){
    LinkList* link = (LinkList*)malloc(sizeof(LinkList));
    if(!link){
        LogOutput("Error:LinkList dynamic memory allocation failed!");
        return NULL;
    }else {
        LogOutput("LinkList dynamic memory allocation succeeded.");
    }
    link->data = NULL;
    link->next = NULL;
    link->pre = NULL;
    link->type = type;
    return link;
}

//在链表尾部添加元素
void AddLinkList(LinkList** list, int type){
    LinkList* temp = *list;
    while(temp){
        if(!temp->next){
            temp->next = CreateLinkList(type);
            temp->next->pre = temp;
            LogOutput("An empty element has been added to the head variable.");
            return;
        }
        temp = temp->next;
    }
   
}

//遍历链表的内容
void TraversalLink(LinkList** list){
    LinkList* temp = *list;
    while(temp){
        printf("%d\n",temp->type);
        temp = temp->next;
    }
}

//释放链表(无法释放链表头，需手动释放)
void FreeLinkList(LinkList** list){
    LinkList* temp = *list;
    //先让指针指到链表尾部
    while(temp->next){
        temp = temp->next;
    }
    //在从下到上依次释放
    while(temp){
        char log_t[CHARMAX];
        sprintf(log_t,"Free Link:%d",temp->type);
        LogOutput(log_t);
        temp = temp->pre;
        if(temp){
            free(temp->next);
            temp->next = NULL;
        }
    }
}

//元素类型查找链表并返回该链表指针
LinkList* LinkSearchAndModify(LinkList** list ,int type){
    LinkList* temp = *list;
    while(temp){
        if(temp->type == type){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

//语言文件读取
void LangRead(char* vkey,char* text){
    FILE* file = fopen("lang.txt","r");
    if(!file){
        LogOutput("Error:lang.txt failed to open!");
        return;
    }else {
        LogOutput("lang.txt successfully opened!");
    }
    char tkey[CHARMAX];
    strcpy(tkey,vkey);
    char key[CHARMAX];
    char value[CHARMAX];
    char temp[CHARMAX];
    strcpy(temp,"HHHH");
    while(fscanf(file,"%[^=]%*[=]%s%*[\n]",key,value)){
        if(!strcmp(key,tkey)){
            strcpy(text,value);
            LogOutput("Find Value");
            break;
        }
        if(!strcmp(key,temp)){
            LogOutput("Not Find Value\n");
            break;
        }
        strcpy(temp,key);
    }
    fclose(file);
}

//字体转纹理
SDL_Texture* FontTextureFromSurface(char* text,SDL_Color color,SDL_Renderer* render){
    TTF_Font* font = TTF_OpenFont("assets/font/font.ttf",200);
    if(!font){
        LogOutput(SDL_GetError());
        return NULL;
    }else {
        LogOutput("Font opened successfully!");
    }
    char temp_text[CHARMAX];
    LangRead(text,temp_text);
    SDL_Surface* ttf_surface = TTF_RenderText_Blended(font,temp_text,0,color);
    TTF_CloseFont(font);
    if(!ttf_surface){
        LogOutput(SDL_GetError());
        return NULL;
    }
    SDL_Texture* ttf_texture = SDL_CreateTextureFromSurface(render,ttf_surface);
    SDL_DestroySurface(ttf_surface);
    if(!ttf_texture){
         LogOutput(SDL_GetError());
        return NULL;
    }
    return ttf_texture;
}

//创建控件并添加到链表(layer是显示在那个图层,ID是指添加到链表里的ID方便定位,text是按钮文字)
void CreateControl(int ID,char* text,SDL_Renderer* render,LinkList** list){
    Control* control = (Control*)malloc(sizeof(Control));
    if(!control){
        LogOutput("Error:Control dynamic memory allocation failed!");
        return;
    }else {
        LogOutput("Control dynamic memory allocation succeeded.");
    }
    control->type = 0;
    SDL_Color ttf_color = {0,0,0,225};
    control->ttf_texture = FontTextureFromSurface(text,ttf_color,render);
    AddLinkList(list,ID);
    LinkList* temp = LinkSearchAndModify(list,ID);
    temp->data = control;
    LogOutput("The element has been added to the appended element of the head variable.");
}

//设置字体大小长宽
void SetControlFontSize(int x ,int y ,int w,int h,int ID, LinkList** list){
    LinkList* temp = LinkSearchAndModify(list,ID);
    Control* control = (Control*)(temp->data);
    if(!control){
        LogOutput("Error:The value of this control is NULL!");
        return;
    }else {
        LogOutput("The font size for Control has been set.");
    }
    control->frect.x = x;
    control->frect.y = y;
    control->frect.w = w;
    control->frect.h = h;
}

//根据控件字体设置控件大小
SDL_FRect SetControlSize(SDL_FRect fre){
    SDL_FRect frect;
    frect.x = fre.x * 0.65;
    frect.y = fre.y * 0.9;
    frect.w = fre.w * 1.5;
    frect.h = fre.h * 1.5;
    return frect;
}

//多个控件渲染
void ControlRender(LinkList** list,int ID,SDL_Renderer* render){
    LinkList* temp = LinkSearchAndModify(list,ID);
    SDL_Texture** texture = (SDL_Texture**)((*list)->data);
    Control* control;
    SDL_FRect rect;
    if(temp){
        while(temp->type / (ID - 1) == 1){
            control = (Control*)(temp->data);
            if(!control){
                LogOutput("Error:The value of this control is NULL!");
                return;
            }
            rect = SetControlSize(control->frect);
            if(control->type){
                SDL_RenderTexture(render,texture[1],NULL,&rect);
            }else{
                SDL_RenderTexture(render,texture[0],NULL,&rect);
            }
            SDL_RenderTexture(render,control->ttf_texture,NULL,&(control->frect));
            if(!temp->next){
                return;
            }
            temp = temp->next;
        }
    }
}

//控件清理
void DestroyControl(LinkList** list,int ID){
    LinkList* temp = LinkSearchAndModify(list,ID);
    Control* control = NULL;
    if(temp){
        while(temp->type / (ID - 1) == 1){
            control = (Control*)(temp->data);
            if(!control){
                LogOutput("Error:The value of this control is NULL!");
                return;
            }
            if(!control->ttf_texture){
                LogOutput("Error:The value of this ttf_texture is NULL!");
                return;
            }
            SDL_DestroyTexture(control->ttf_texture);
            free(control);
            control = NULL;
            temp->data = NULL;
            if(!temp->next){
                return;
            }
            temp = temp->next;
        }
    }
}

//判断是否在按钮内
void ButtonDecision(LinkList** list,int ID){
    LinkList* temp = LinkSearchAndModify(list,ID);
    Control* control = NULL;
    while(temp->type / (ID - 1) == 1){
        control = (Control*)(temp->data);
        float x, y;
        SDL_GetMouseState(&x,&y);
        if(x > control->frect.x && x < control->frect.x + control->frect.w 
            && y >control->frect.y && y <control->frect.y + control->frect.h
        ){
            control->type = 1;
        }else{
            control->type = 0;
        }
        if(!temp->next){
            return;
        }
        temp = temp->next;
    }
    
}

//日志输出函数
void LogOutput(const char* text){
    char log_text[200];
    FILE* file = fopen("Log.txt","a");
    time_t current_time;
    struct tm *time_info;
    time(&current_time);
    time_info = localtime(&current_time);
    strftime(log_text,sizeof(log_text),"[%Y-%m-%d %H:%M:%S]:",time_info);
    strcat(log_text,text);
    strcat(log_text,"\n");
    fprintf(file,"%s",log_text);
    fclose(file);
}

//跟随窗口缩放
void FollowZoom(SDL_Window* window,SDL_FRect* rect,int* winW,int* winH){
    int w,h;
    SDL_GetWindowSize(window,&w,&h);
    float ratioW = (float)w / (float)(*winW);
    float ratioH = (float)h / (float)(*winH);
    rect->h *= ratioH;
    rect->w *= ratioW;
    rect->x *= ratioW;
    rect->y *= ratioH;
}

//多个控件大跟随缩放
void ControlReSize(LinkList** list,int ID,SDL_Window* window,int* winW,int* winH){
    LinkList* temp = LinkSearchAndModify(list,ID);
    Control* control = NULL;
    int w,h;
    SDL_GetWindowSize(window,&w,&h);
    float ratioW = (float)w / (float)(*winW);
    float ratioH = (float)h / (float)(*winH);
    if(temp){
        while(temp->type / (ID - 1) == 1){
            control = (Control*)(temp->data);
            if(!control){
                LogOutput("Error:The value of this control is NULL!");
                return;
            }
            FollowZoom(window,&(control->frect),winW,winH);
            if(!temp->next){
                *winW = w;
                *winH = h;
                return;
            }
            temp = temp->next;
        }
    }
}