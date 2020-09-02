/*关于SDL和文件系统的一些基本操作
  函数列表：
	void Init() 初始化SDL和文件系统
	
	void Destroy() 清理使用内存和退出SDL
	
	void RenderText(int x,int y,char *text)
					在(x,y)处显示字符串text
	
	bool gettouch(int x1,int y1,
				int x2,int y2) 获取屏幕点击事件
	
	void copy(int x1,int x2,int x3,int x4,
				int x5,int x6,int x7,int x8)
			从图片中截取 {x1,x2,x3,x4}
			并粘贴到屏幕 {x5,x6,x7,x8} 位置上
	
	void save_data(int n) 更新并保存关卡到1.txt
*/
#define TTF_path "/data/data/com.houzhe.cube/files/yh_mono.ttf"
TTF_Font *font26, *font40;

void Init() {
	SDL_Init(SDL_INIT_VIDEO);
	Window = SDL_CreateWindow("SDL_Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 240, 400, SDL_WINDOW_SHOWN);
	
	R = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(R,SDL_BLENDMODE_BLEND);
	T = IMG_LoadTexture(R,"texture.png");
	SDL_GetWindowSize(Window,
		&screen_w, &screen_h);
	W = (double )screen_w / 720;
	H = (double )screen_h / 1280;
	
	FILE* fp = NULL;
#ifdef APP
	sprintf(path, "/data/data/%s/files/1.txt", APP);
#else
	fp = fopen("/data/data/com.n0n3m4.droidc/files/file.stdin","r");

	char *c = path;
	for (*c=fgetc(fp);*c!='\"';*c=fgetc(fp));
	for (*c=fgetc(fp);*c!='\"';*c=fgetc(fp)) c++;
	strcpy(c,"/1.txt");
	fclose(fp);
#endif
	if (fp = fopen(path,"r")) {
		fscanf(fp,"%d",&level);  init_level = level;
		fclose(fp);
	} else {
		char p[100] = "true > ";
		strcpy(p+7,path);
		system(p);
	}
	TTF_Init();
	font26 = TTF_OpenFont(TTF_path, 26);
	font40 = TTF_OpenFont(TTF_path, 40);
}

void Destroy() {
	TTF_CloseFont(font26);
	TTF_CloseFont(font40);
	TTF_Quit();
	SDL_DestroyTexture(T);
	SDL_DestroyRenderer(R);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void copy(int x1,int x2,int x3,int x4,
			int x5,int x6,int x7,int x8) {
	SDL_Rect Cut = {x1,x2,x3,x4};
	x5 = x5*W;
	x6 = x6*H;
	x7 = x7*W;
	x8 = x8*H;
	SDL_Rect Box = {x5,x6,x7,x8};
	SDL_RenderCopy(R,T,&Cut,&Box);
}

void RenderText(int x,int y,char *text) {
	x *= W;
	y *= H;
	SDL_Texture *T1 = NULL;
	SDL_Surface *S = NULL;
	
	SDL_Color color = {230,230,230,0};
	
	S = TTF_RenderUTF8_Blended(font26, text, color);
	int w = S->w * W, h = S->h * H;
	T1 = SDL_CreateTextureFromSurface(R,S);
	
	SDL_Rect Box = {x,y,w,h};
	SDL_RenderCopy(R,T1,NULL,&Box);
	SDL_DestroyTexture(T1);
	SDL_FreeSurface(S);
}


void RenderText(SDL_Rect *Box,int _x,int _y,int _w,int _h,char *text) {
	SDL_Texture *T1 = NULL;
	SDL_Surface *S = NULL;
	SDL_Color color = {200,250,70,255};
	
	S = TTF_RenderUTF8_Blended(font40, text, color);
	int w = S->w * W, h = S->h * H;
	T1 = SDL_CreateTextureFromSurface(R,S);
	
	copy(_x,_y,_w,_h,20,410 + _h / 2,80,80);
	SDL_Rect Box2 = {100 + Box->x + (Box->w - w) / 2,Box->y + (Box->h - h) / 2,w,h};
	SDL_RenderCopy(R,T1,NULL,&Box2);
	
	SDL_DestroyTexture(T1);
	SDL_FreeSurface(S);
}

bool gettouch(int x1,int y1,int x2,int y2) {
	if (SDL_PollEvent(&E))
	if (E.type == SDL_FINGERDOWN) {
		float Tx = E.tfinger.x*720;
		float Ty = E.tfinger.y*1280;
		if (Ty < 120 && 
			Tx > 480 && Tx < 600) exit(0);
		if (Tx > x1 && Tx < x2 &&
			Ty > y1 && Ty < y2) return true;
	} return false;
}

void save_data(int n) {
	if (n > MAX) {
		level--;  return;
	}
	FILE *fp = fopen(path,"w");
	if (fp) {
		fprintf(fp,"%d\n",n);
		fclose(fp);
	} if (max_level <= n) max_level = n;
}
