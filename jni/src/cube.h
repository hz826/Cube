//此游戏的核心代码 一个通过旋转和三角函数实现的（伪）3D算法
#define max(a,b) (a>b ? a : b)
#define min(a,b) (a<b ? a : b)

struct Cube {
	private:
		struct point {double x,y;} p[8];
		const int A=30,B=40,C=50;
		const int u[4] = {40,-40,-40,40};
		const int v[4] = {-30,30,-30,30};
		
		void getline(point *a,point *b,
					double *kk,double *bb,
					double *l,double *r);
				//求一次函数解析式
		void line(point a,point b,bool mode);
				//连接点a，点b
		point rec(int x,int y,int r,int a);
				//直角坐标转极坐标
		
		void fill(int p1,int p2,int p3,int p4,int rr,int gg,int bb,int aa);
		
		void draw_cube();
		int cube_alpha = 220;
	public:
		void draw_map(bool show_stop,
				  	bool present);
		bool roll(int xx,int yy,int d);
		bool move(int xx,int yy,int d);
		bool turn(int xx,int yy);
		bool magnify(int xx,int yy);
		bool shrink(int xx,int yy);
		bool appear(int xx,int yy);
		bool disappear(int xx,int yy);
};

void Cube::getline(point *a,point *b,double *kk,double *bb,double *l,double *r) {
	if (a->x == b->x) {*l=*r=0;  return;}
	*kk = (a->y-b->y) / (a->x-b->x);
	*bb = (b->y*a->x - a->y*b->x) / (a->x-b->x);
	*l = min(a->x,b->x);
	*r = max(a->x,b->x);
}

void Cube::line(point a,point b,bool mode) {
	if (!(a.x && a.y && b.x && b.y)) return;
	if (mode) {//加粗
		a.x = a.x*W;
		a.y = a.y*H;
		b.x = b.x*W;
		b.y = b.y*H;
		SDL_RenderDrawLine(R,
					a.x+1,a.y,b.x+1,b.y);
		SDL_RenderDrawLine(R,
					a.x-1,a.y,b.x-1,b.y);
		SDL_RenderDrawLine(R,
					a.x,a.y+1,b.x,b.y+1);
		SDL_RenderDrawLine(R,
					a.x,a.y-1,b.x,b.y-1);
	}
	SDL_RenderDrawLine(R,a.x,a.y,b.x,b.y);
}

Cube::point Cube::rec(int x,int y,int r,int a) {
	double p = M_PI/180;
	return {r*cos(a*p)+x,r*sin(a*p)+y};
}

void Cube::draw_map(bool show_stop,bool present) {
	SDL_RenderClear(R);
	copy(801,0,135,240,0,0,720,1280);
	//背景图片
	
	for (int i=0;i<SIZE;i++)
	for (int j=0;j<SIZE;j++) if (map[j][i]) {
		int n = abs(map[j][i]);
		copy(80*(n%10),(n/10)*80,
			80,80,
			320+40*i-40*j,130+30*i+30*j,
			80,80);
	}//map方块数组
	
	for (int j=0;j<5;j++) if (item_num[j]) {
		int num = 0;
		for (int i=0;i<5;i++)
		if (item_num[i]) {
			if (item == i) {
				copy(500,240,
					180,100,
					30,720+num*95,
					150,100);
			
				int n = itemid[i];
				copy(80*(n%10),(n/10)*80,
					80,80,
					35,730+num*95,
					80,80);
			
				n = item_num[i];
				copy((n%5)*60,(n/5)*100+240,
					60,100,
					130,770+(num++)*95,
					20,35);
			} else {
				copy(500,340,
					180,100,
					0,720+num*95,
					150,100);

				int n = itemid[i];
				copy(80*(n%10),(n/10)*80,
					80,80,
					15,730+num*95,
					80,80);
			
				n = item_num[i];
				copy((n%5)*60,(n/5)*100+240,
					60,100,
					110,770+(num++)*95,
					20,35);
			}
		} break;
	}//手中可用方块和个数
	
	if (show_stop)
		copy(780,240,100,100,
			620,130,100,100);
	else copy(680,240,100,100,
			620,130,100,100);
	
	copy(680,340,100,100,
		620,10,100,100);
	copy(780,340,100,100,
		520,10,100,100);//右上角三个图标
	
	int n = level / 10;
	copy((n%5)*60,(n/5)*100+240,
		60,100,
		290,20,
		60,100);
	
	n = level % 10;
	copy((n%5)*60,(n/5)*100+240,
		60,100,
		350,20,
		60,100);
	//关卡号
	if (present) SDL_RenderPresent(R);
}

void Cube::fill(int p1,int p2,int p3,int p4,
			int rr,int gg,int bb,int aa) {
	/*
	填充以p[p1],p[p2],p[p3],p[p4]为顶点的四边形
	填充颜色为{r,g,b,a}
	自动适应屏幕
	*/
	SDL_SetRenderDrawColor(R,rr,gg,bb,aa);
	point P[4] = {p[p1],p[p2],p[p3],p[p4]};
	for (int i=0;i<4;i++) {
		P[i].x *= W;  P[i].y *= H;
	}
	double K[6],B[6],L[6],R[6],l,r;
	getline(P+0,P+1,K+0,B+0,L+0,R+0);
	getline(P+0,P+2,K+1,B+1,L+1,R+1);
	getline(P+0,P+3,K+2,B+2,L+2,R+2);
	getline(P+1,P+2,K+3,B+3,L+3,R+3);
	getline(P+1,P+3,K+4,B+4,L+4,R+4);
	getline(P+2,P+3,K+5,B+5,L+5,R+5);
	
	l = screen_w;  r = 0;
	for (int i=0;i<4;i++) {
		l = min(l,P[i].x);
		r = max(r,P[i].x);
	}
	while (++l < r) {
		double u = screen_h,d = 0;
		for (int i=0;i<6;i++) if (l >= L[i] && l <= R[i]) {
			u = min(u,K[i]*l+B[i]);
			d = max(d,K[i]*l+B[i]);
		}
		if (u < d) line({l,u},{l,d},false);
	}
}


#define cube_color 0xFF,0xFF,0xFF,cube_alpha
void Cube::draw_cube() {//填充立方体可见三面
	if (p[0].x == 0 && p[0].y == 0) {
		fill(6,2,7,3,cube_color);
		fill(6,4,7,5,cube_color);
	} else if (p[4].x == 0 && p[4].y == 0) {
		fill(0,2,1,3,cube_color);
		fill(2,6,3,7,cube_color);
	} else if (p[2].x == 0 && p[2].y == 0) {
		fill(4,6,5,7,cube_color);
		fill(0,4,1,5,cube_color);
	} else if (p[6].x == 0 && p[6].y == 0) {
		fill(2,0,3,1,cube_color);
		fill(0,4,1,5,cube_color);
	} 
	fill(1,3,5,7,cube_color);
	
	SDL_SetRenderDrawColor(R,0,0,0,cube_alpha);
	line(p[0],p[2],true);
	line(p[0],p[4],true);
	line(p[2],p[6],true);
	line(p[4],p[6],true);//底层菱形
	
	line(p[1],p[3],true);
	line(p[1],p[5],true);
	line(p[3],p[7],true);
	line(p[5],p[7],true);//顶层菱形
	
	line(p[0],p[1],true);
	line(p[2],p[3],true);
	line(p[4],p[5],true);
	line(p[6],p[7],true);//连接底层，顶层
}

bool Cube::roll(int xx,int yy,int d) {
	const int n = 60,delay = 15,z = 10;
	double x = 360+40*xx-40*yy;
	double y = 190+30*xx+30*yy;
	//玄学的  滚动算法  （虽然说是伪3D）
	//底层四点：
	//p[0] 位于(x,y) 旋转中不移动的点
	//p[2] 同 p[0] 旋转中不变化的点
	//p[4] 位置与 p[2] 对称
	//p[6] 位置可通过 p[0],p[2],p[4]推出
	if (d == 0) {x+=B;  y-=A;}
	if (d == 2) {x-=B;  y-=A;}
	p[0] = {x,y};
	
	if (d == 0) for (int i=-128;i<0;i+=z) {
		draw_map(true,false );
		if (gettouch(600,120,720,240))
			return false;
		p[2] = {x-B,y-A};
		p[4] = rec(x,y,C,317+i*43/128);
		p[6] = 	{p[2].x+p[4].x-p[0].x,p[2].y+p[4].y-p[0].y};
		p[1] = rec(p[0].x,p[0].y,C,270+i);
		p[3] = rec(p[2].x,p[2].y,C,270+i);
		p[5] = rec(p[4].x,p[4].y,C,270+i);
		p[7] = rec(p[6].x,p[6].y,C,270+i);
		if (i>n-128) p[6] = {0,0};
		else p[2] = {0,0};
		
		draw_cube();
		SDL_RenderPresent(R);
		SDL_Delay(delay);
	}
	if (d == 1) for (int i=0;i>-128;i-=z) {
		draw_map(true,false );
		if (gettouch(600,120,720,240))
			return false;
		p[2] = {x-B,y-A};
		p[4] = rec(x,y,C,317+i*43/128);
		p[6] = 	{p[2].x+p[4].x-p[0].x,p[2].y+p[4].y-p[0].y};
		p[1] = rec(p[0].x,p[0].y,C,270+i);
		p[3] = rec(p[2].x,p[2].y,C,270+i);
		p[5] = rec(p[4].x,p[4].y,C,270+i);
		p[7] = rec(p[6].x,p[6].y,C,270+i);
		if (i<-n) p[2] = {0,0};
		else p[6] = {0,0};
		
		draw_cube();
		SDL_RenderPresent(R);
		SDL_Delay(delay);
	}
	if (d == 2) for (int i=128;i>0;i-=z) {
		draw_map(true,false );
		if (gettouch(600,120,720,240))
			return false;
		p[2] = {x+B,y-A};
		p[4] = rec(x,y,C,227+i*43/128);
		p[6] = 	{p[2].x+p[4].x-p[0].x,p[2].y+p[4].y-p[0].y};
		p[1] = rec(p[0].x,p[0].y,C,270+i);
		p[3] = rec(p[2].x,p[2].y,C,270+i);
		p[5] = rec(p[4].x,p[4].y,C,270+i);
		p[7] = rec(p[6].x,p[6].y,C,270+i);
		if (i<128-n) p[6] = {0,0};
		else p[2] = {0,0};
		
		draw_cube();
		SDL_RenderPresent(R);
		SDL_Delay(delay);
	}
	if (d == 3) for (int i=0;i<128;i+=z) {
		draw_map(true,false );
		if (gettouch(600,120,720,240))
			return false;
		p[2] = {x+B,y-A};
		p[4] = rec(x,y,C,227+i*43/128);
		p[6] = 	{p[2].x+p[4].x-p[0].x,p[2].y+p[4].y-p[0].y};
		p[1] = rec(p[0].x,p[0].y,C,270+i);
		p[3] = rec(p[2].x,p[2].y,C,270+i);
		p[5] = rec(p[4].x,p[4].y,C,270+i);
		p[7] = rec(p[6].x,p[6].y,C,270+i);
		if (i>n) p[2] = {0,0};
		else p[6] = {0,0};
		
		draw_cube();
		SDL_RenderPresent(R);
		SDL_Delay(delay);
	} return true;
}

bool Cube::move(int xx,int yy,int d) {
	//比较正常的move（蓝色传送带）
	double x = 360+40*xx-40*yy;
	double y = 190+30*xx+30*yy;

	for (double i=0;i<1;i+=0.1) {
		draw_map(true,false );
		if (gettouch(600,120,720,240))
			return false;
		double X = x+u[d]*i;
		double Y = y+v[d]*i;
		
		p[0] = {X,Y};
		p[2] = {X-B,Y-A};
		p[4] = {X+B,Y-A};
		p[6] = 	{p[2].x+p[4].x-p[0].x,p[2].y+p[4].y-p[0].y};
		p[1] = {p[0].x,p[0].y-C};
		p[3] = {p[2].x,p[2].y-C};
		p[5] = {p[4].x,p[4].y-C};
		p[7] = {p[6].x,p[6].y-C};
		p[6] = {0,0};
		
		draw_cube();
		SDL_RenderPresent(R);
		SDL_Delay(30);
	} return true;
}

bool Cube::turn(int xx,int yy) {
	//比较正常的turn（绿色旋转块）
	double x = 360+40*xx-40*yy;
	double y = 160+30*xx+30*yy;
	int a = A,b = B;
	
	for (int i=0;i<90;i+=9) {
		draw_map(true,false );
		if (gettouch(600,120,720,240))
			return false;
		a++;  b--;
		p[0] = rec(x,y,b,180+i);
		p[2] = rec(x,y,a,90+i);
		p[4] = rec(x,y,b,270+i);
		p[6] = rec(x,y,b,i);
		p[1] = {p[0].x,p[0].y-C};
		p[3] = {p[2].x,p[2].y-C};
		p[5] = {p[4].x,p[4].y-C};
		p[7] = {p[6].x,p[6].y-C};
		
		if (i<45) p[4] = {0,0};
		else p[0] = {0,0};
		draw_cube();
		SDL_RenderPresent(R);
		SDL_Delay(30);
	} return true;
}

bool Cube::magnify(int xx,int yy) {
	//magnify和shrink （紫色传送门）
	double X = 360+40*xx-40*yy;
	double Y = 160+30*xx+30*yy;
	point center = {X,Y-A-(C / 2)};
	
	for (double i=0;i<1;i+=0.1) {
		draw_map(true,false );
		if (gettouch(600,120,720,240))
			return false;
		
		p[0] = {X,Y};
		p[2] = {X-B,Y-A};
		p[4] = {X+B,Y-A};
		p[6] = 	{p[2].x+p[4].x-p[0].x,p[2].y+p[4].y-p[0].y};
		p[1] = {p[0].x,p[0].y-C};
		p[3] = {p[2].x,p[2].y-C};
		p[5] = {p[4].x,p[4].y-C};
		p[7] = {p[6].x,p[6].y-C};
		p[6] = {0,0};
		
		for (int j=0;j<8;j++) if (j != 6) {
			p[j].x *= i;
			p[j].y *= i;
			p[j].x += center.x * (1 - i);
			p[j].y += center.y * (1 - i);
		}
		draw_cube();
		SDL_RenderPresent(R);
		SDL_Delay(8);
	}
	
	for (int i=30;i>0;i-=5) {
		draw_map(true,false );
		if (gettouch(600,120,720,240))
			return false;
		p[0] = {X,Y - i};
		p[2] = {X-B,Y-A - i};
		p[4] = {X+B,Y-A - i};
		p[6] = 	{p[2].x+p[4].x-p[0].x,p[2].y+p[4].y-p[0].y};
		p[1] = {p[0].x,p[0].y-C};
		p[3] = {p[2].x,p[2].y-C};
		p[5] = {p[4].x,p[4].y-C};
		p[7] = {p[6].x,p[6].y-C};
		p[6] = {0,0};
		
		draw_cube();
		SDL_RenderPresent(R);
		SDL_Delay(15);
	} return true;
}

bool Cube::shrink(int xx,int yy) {
	double X = 360+40*xx-40*yy;
	double Y = 160+30*xx+30*yy;
	point center = {X,Y-A-(C / 2)};
	
	for (int i=0;i<30;i+=5) {
		draw_map(true,false );
		if (gettouch(600,120,720,240))
			return false;
		p[0] = {X,Y - i};
		p[2] = {X-B,Y-A - i};
		p[4] = {X+B,Y-A - i};
		p[6] = 	{p[2].x+p[4].x-p[0].x,p[2].y+p[4].y-p[0].y};
		p[1] = {p[0].x,p[0].y-C};
		p[3] = {p[2].x,p[2].y-C};
		p[5] = {p[4].x,p[4].y-C};
		p[7] = {p[6].x,p[6].y-C};
		p[6] = {0,0};
		
		draw_cube();
		SDL_RenderPresent(R);
		SDL_Delay(15);
	}
	
	for (double i=1;i>0;i-=0.1) {
		draw_map(true,false );
		if (gettouch(600,120,720,240))
			return false;
		
		p[0] = {X,Y};
		p[2] = {X-B,Y-A};
		p[4] = {X+B,Y-A};
		p[6] = 	{p[2].x+p[4].x-p[0].x,p[2].y+p[4].y-p[0].y};
		p[1] = {p[0].x,p[0].y-C};
		p[3] = {p[2].x,p[2].y-C};
		p[5] = {p[4].x,p[4].y-C};
		p[7] = {p[6].x,p[6].y-C};
		p[6] = {0,0};
		
		for (int j=0;j<8;j++) if (j != 6) {
			p[j].x *= i;
			p[j].y *= i;
			p[j].x += center.x * (1 - i);
			p[j].y += center.y * (1 - i);
		}
		draw_cube();
		SDL_RenderPresent(R);
		SDL_Delay(8);
	} return true;
}

bool Cube::appear(int xx,int yy) {
	double X = 360+40*xx-40*yy;
	double Y = 190+30*xx+30*yy;
	
	p[0] = {X,Y};
	p[2] = {X-B,Y-A};
	p[4] = {X+B,Y-A};
	p[6] = 	{p[2].x+p[4].x-p[0].x,p[2].y+p[4].y-p[0].y};
	p[1] = {p[0].x,p[0].y-C};
	p[3] = {p[2].x,p[2].y-C};
	p[5] = {p[4].x,p[4].y-C};
	p[7] = {p[6].x,p[6].y-C};
	p[6] = {0,0};
	
	for (int i=20;i<=220;i+=10) {
		draw_map(true,false );
		if (gettouch(600,120,720,240))
			return false;
		
		cube_alpha = i;
		draw_cube();
		SDL_RenderPresent(R);
		SDL_Delay(15);
	} return true;
}

bool Cube::disappear(int xx,int yy) {
	double X = 360+40*xx-40*yy;
	double Y = 190+30*xx+30*yy;
	
	p[0] = {X,Y};
	p[2] = {X-B,Y-A};
	p[4] = {X+B,Y-A};
	p[6] = 	{p[2].x+p[4].x-p[0].x,p[2].y+p[4].y-p[0].y};
	p[1] = {p[0].x,p[0].y-C};
	p[3] = {p[2].x,p[2].y-C};
	p[5] = {p[4].x,p[4].y-C};
	p[7] = {p[6].x,p[6].y-C};
	p[6] = {0,0};
	
	for (int i=220;i>=20;i-=10) {
		draw_map(true,false );
		if (gettouch(600,120,720,240))
			return false;
		
		cube_alpha = i;
		draw_cube();
		SDL_RenderPresent(R);
		SDL_Delay(15);
	} cube_alpha = 220;
	return true;
}
