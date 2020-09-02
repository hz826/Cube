#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#define SIZE 9
#define APP "com.houzhe.cube"

// 窗口
SDL_Window *Window = NULL;
SDL_Renderer *R = NULL;
SDL_Texture *T = NULL;
SDL_Event E;

double W,H;
int screen_w,screen_h;
int map[SIZE][SIZE], item_num[5], itemid[5];
int item = 0, level = 0, max_level = 0, init_level;
char path[100];	//关卡文件1.txt储存位置
bool quit = false;

#include "maps.h"
#include "init.h"
#include "cube.h"

Cube cube;
/*
	0->空
	1->白色方块
	2-5->起点（4个方向）
	6->终点
	7-10->转向（4个方向）
	11-14->输送带（4个方向）
	15->传送门
	16->黑色方块
	17-18->开关（关/开）
	19-20->桥（关/开）
	21->旋转
	22->紫色方块
*/

void show_hint(int) ;
void copy_map() {//调取指定关卡数据
	item = 0;
	memcpy(map,maps[level],sizeof(map));
	memcpy(item_num,item_nums[level],sizeof(item_num));
	memcpy(itemid,itemids[level],sizeof(itemid));
	show_hint(level);
}

int getdirection(int x,int y,
				float tx,float ty) {//通过点击获取方块放置方向
	cube.draw_map(false,false );
	copy(300,240,
		200,200,
		240+40*x-40*y,50+30*x+30*y,
		220,220);
	SDL_RenderPresent(R);
	
	while (1) {
		if
(gettouch(tx-110,ty-110,tx+110,ty+110)) {
			float Tx = E.tfinger.x*720;
			float Ty = E.tfinger.y*1280;
			
			if (Tx-tx>0 && Ty-ty<0) return 0;
			if (Tx-tx<0 && Ty-ty>0) return 1;
			if (Tx-tx<0 && Ty-ty<0) return 2;
			return 3;
		}
	}
}
#define BACKGROUND_COL 50,50,50,180
void show_text(char *str,int _x,int _y,int _w,int _h) {
	for (int i=0;i<=screen_h/16;i+=screen_h/160) {
		cube.draw_map(false,false);
		SDL_SetRenderDrawColor(R,BACKGROUND_COL);
		SDL_Rect Box = {0,screen_h*6.0/16.0-i,screen_w,i*2};
		SDL_RenderFillRect(R,&Box);
		SDL_RenderPresent(R);
		SDL_Delay(15);
	}
	
	int len = strlen(str), start = 0;
	for (int i=2;i<len;i+= str[i+1] > 127 ? 3 : 1) {
		char text[100];  memset(text,0,sizeof(text));
		for (int j=0;j<=i;j++) text[j] = str[j];
		cube.draw_map(false,false);
		SDL_SetRenderDrawColor(R,BACKGROUND_COL);
		SDL_Rect Box = {0,screen_h*5.0/16.0,screen_w,screen_h/8};
		SDL_RenderFillRect(R,&Box);
		RenderText(&Box,_x,_y,_w,_h,text);
		SDL_RenderPresent(R);
		SDL_Delay(80);
	}
	
	while (!gettouch(0,0,720,1280)) SDL_Delay(15);
	for (int i=screen_h/16;i>=0;i-=screen_h/160) {
		cube.draw_map(false,false);
		SDL_SetRenderDrawColor(R,BACKGROUND_COL);
		SDL_Rect Box = {0,screen_h*6.0/16.0-i,screen_w,i*2};
		SDL_RenderFillRect(R,&Box);
		SDL_RenderPresent(R);
		SDL_Delay(15);
	}
	cube.draw_map(false,true);
}

void openmenu() {//打开关卡选择目录
	SDL_Delay(30);
	for (int i=0;i<=300;i+=40) {
		cube.draw_map(false,false );
		SDL_Rect Box = {0,0,i*W,screen_h};
		SDL_SetRenderDrawColor(R,50,50,50,150);
		SDL_RenderFillRect(R,&Box);
		SDL_RenderPresent(R);
		SDL_Delay(8);
	}
	
	for (int i=1;i<=max_level;i++) {
		cube.draw_map(false,false );
		SDL_SetRenderDrawColor(R,50,50,50,150);
		SDL_Rect Box = {0,0,screen_w*5/12,screen_h};
		SDL_RenderFillRect(R,&Box);
		copy(510,160,290,80,0,50,300,80);
		
		SDL_SetRenderDrawColor(R,150,150,150,150);
		for (int j=1;j<=i;j++) {
			Box = {5*W,(105+j*55)*H,290*W,53*H};
			SDL_RenderFillRect(R,&Box);
			RenderText(20,120+j*55,name[j]);
		}
		SDL_RenderPresent(R);
		SDL_Delay(15);
	}
	
	while (!gettouch(0,0,720,1280));
	if (E.tfinger.x * 720 > 300 ||
		E.tfinger.y * 1280 < 180) {
		cube.draw_map(false,true );
		return;
	}
	
	int t = (E.tfinger.y*1280 - 180) / 55;
	if (t < max_level) {
		level = t + 1;
		copy_map();
		for (int i=300;i > 0;i-=40) {
			cube.draw_map(false,false );
			SDL_Rect Box = {0,0,i*W,screen_h};
			SDL_SetRenderDrawColor(R,50,50,50,150);
			SDL_RenderFillRect(R,&Box);
			SDL_RenderPresent(R);
			SDL_Delay(8);
		}
	} cube.draw_map(false,true );
}

bool run() {//运行
	const int u[4] = {-1,1,0,0};
	const int v[4] = {0,0,-1,1};
	int x,y,d;
	int x1 = -1,x2 = -1,y1 = -1,y2 = -1;
	for (int i=0;i<SIZE;i++) {
		for (int j=0;j<SIZE;j++) {
			if (map[i][j]>1 && map[i][j]<6) {
				x=i;  y=j;  d=map[i][j]-2;
			}
			if (map[i][j] == 15) {
				if (x1 == -1) {
					x1 = i;  y1 = j;
				} else {
					x2 = i;  y2 = j;
				}
			}
		}
	}
	
	cube.appear(y,x);
	while (x>=0 && x<SIZE 
		&& y>=0 && y<SIZE 
		&& map[x][y] != 0 
		&& map[x][y] != 6 
		&& map[x][y] != 19) {

		bool f = false;
		if (map[x][y] < 0) {
			map[x][y] = -map[x][y];
			f = true;
		}
		if (map[x][y] > 6 && 
		map[x][y] < 11) {

			d = map[x][y] - 7;
			if (f) map[x][y] = 22;
			if (!cube.roll(y,x,d)) return false;
			x+=u[d];  y+=v[d];
		}
		else if (map[x][y] > 10 &&
				 map[x][y] < 15) {

			int k = map[x][y];
			if (f) map[x][y] = 22;
			if (!cube.move(y,x,k-11)) return false;
			k-=11;
			x+=u[k];
			y+=v[k];
		}
		else if (map[x][y] == 15) {
			cube.shrink(y,x);
			if (x == x1 && y == y1) {
				x = x2;
				y = y2;
			} else {
				x = x1;
				y = y1;
			}
			cube.magnify(y,x);
			if (f) map[x][y] = 22;
			if (!cube.roll(y,x,d)) return false;
			x+=u[d];  y+=v[d];
		}
		else if (map[x][y] == 17) {
			bool flag = true;
			map[x][y]++;
			for (int i=0;i<SIZE;i++)
			for (int j=0;j<SIZE;j++) {
				if (map[i][j] == 17)
					flag = false;
			}
			if (flag) {
				for (int i=0;i<SIZE;i++)
				for (int j=0;j<SIZE;j++) {
					if (map[i][j] == 19)
						map[i][j]++;
				}
			}
		}
		else if (map[x][y] == 21) {
			const int w[4] = {3,2,0,1};
			d = w[d];
			if (f) map[x][y] = 22;
			if (!cube.turn(y,x)) return false;
			if (!cube.roll(y,x,d)) return false;
			x+=u[d];  y+=v[d];
		}
		else if (map[x][y] == 22) {
			map[x][y] = 0;
			if (f) map[x][y] = 22;
			if (!cube.roll(y,x,d)) return false;
			x+=u[d];  y+=v[d];
		}
		else {
			if (!cube.roll(y,x,d)) return false;
			x+=u[d];  y+=v[d];
		}
	}
	if (map[x][y] == 6) {
		cube.disappear(y,x);
		return true;
	}
	else return false;
}

void show_hint(int l) {
	if (l == 0) show_text("点击'开始'以释放'立方体'  ",680,240,100,100);
	if (l == 1) show_text("路标能改变'立方体'的行进方向",560,0,80,80);
	if (l == 2) show_text("传送带不改变'立方体'的方向  ",80,80,80,80);
	if (l == 4) show_text("传送节点将'立方体'传送到对应位置",400,80,80,80);
	if (l == 7) show_text("黑色地面上不能放置任何命令方块",480,80,80,80);
	if (l == 8) {
		show_text(" 桥需要被开关激活才能通过  ",0,160,80,80);
		show_text("用'立方体'打开开关以激活桥",560,80,80,80);
	}
	if (l == 10) show_text("旋转路标使立方体行进方向顺时针旋转",80,160,80,80);
	if (l == 13) {
		show_text("  注意紫色地面走过就会塌陷   ",160,160,80,80);
		show_text("紫色地面上的路标在走过后会消失",160,160,80,80);
	}
}

int ceil(float n) {//四舍五入
	int m = floor(n);
	return (n-m>=0.5) ? m+1 : m;
}

int main(int argc,char **argv) {
	Init();
	save_data(level);
	copy_map();
	cube.draw_map(false,true );
	
	while (!quit)
	if (gettouch(0,0,720,1280)) {
		float Tx = E.tfinger.x * 720;
		float Ty = E.tfinger.y * 1280;

		if (Ty<120 && Tx>600) openmenu();

		else if (Ty<240 && Tx>600) {
			int a[SIZE][SIZE];
			memcpy(a,map,sizeof(a));
			if (run()) {
				save_data(++level);
				copy_map();
				cube.draw_map(false,true );
				if (level == 15) {
					show_text(" 游戏结束，感谢支持٩( 'ω' )و ",480,0,80,80);
					show_text("原作：flash游戏'cube_mayhem'",480,0,80,80);
					show_text("         制作： hz          ",480,0,80,80);
				}
			} else {
				memcpy(map,a,sizeof(a));
				cube.draw_map(false,true );
			}
		}

		else if (Ty > 720) {
			int num = 0;
			for (int i=0;i<5;i++)
				if (item_num[i]) num++;
				
			if (Ty < 720 + num*100 &&
				Tx < 200) {

				int click = (Ty-720)/100;
				for (int i=0;i<5;i++)
				if (item_num[i])
				if (!click--) {
					item = i;
					cube.draw_map(false,true );
					break;
				}
			} continue;
		}

		int x = ceil((3*Tx+4*Ty-1480)/240)-1;
		int y = ceil((4*Ty-3*Tx+440)/240);
		if (x>=0 && x<SIZE && 
			y>=0 && y<SIZE) {

			if (item_num[item] &&
			map[y][x] == 1) {

				map[y][x] = itemid[item];
				int id = itemid[item];
				if (id>6 && id<15)
					map[y][x]+=
					getdirection(x,y,Tx,Ty);

				if (!--item_num[item]) {
					for (int i=0;i<5;i++)
					if (item_num[i]) {
						item = i;
						break;
					}
				}
			}
			
			else if (item_num[item] && 
			map[y][x] == 22) {

				map[y][x] = -itemid[item];
				int id = itemid[item];
				if (id>6 && id<15)
					map[y][x] -=
					getdirection(x,y,Tx,Ty);
				
				if (!--item_num[item]) {
					for (int i=0;i<5;i++)
					if (item_num[i]) {
						item = i;
						break;
					}
				}
			} else if (map[y][x] > 6) {
				int k = map[y][x];
				if (k>6 && k<11) k=7;
				if (k>10 && k<15) k=11;
		
				for (int i=0;i<5;i++)
				if (itemid[i] == k) {
					item_num[i]++;
					map[y][x]=1;
				}
			}
			else if (map[y][x] < 0) {
				int k = -map[y][x];
				if (k>6 && k<11) k=7;
				if (k>10 && k<15) k=11;
				
				for (int i=0;i<5;i++)
				if (itemid[i] == k) {
					item_num[i]++;
					map[y][x]=22;
				}
			} cube.draw_map(false,true );
		}
	}
	Destroy();
}
