#include<stdio.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<termios.h>

#define CLEAR "\033[2J"
#define HIDE "\033[?25l"
#define SHOW "\033[?25h"

void raw_mode();
void normal_mode();
void move(int x, int y);
void draw();
void erase();


int main(){

struct winsize w;

raw_mode();
printf(CLEAR);
printf(HIDE);

ioctl(0, TIOCGWINSZ, &w);
int win_row = w.ws_row;
int win_col = w.ws_col;


int row = win_row/2;
int col = win_col/2;

int show;
show = 1;
char c;


int prev_row = row;
int prev_col = col;
while(1){
	
	move(prev_row,prev_col);
	erase();

	move(row, col);
	draw();


prev_row = row; prev_col = col;

	usleep(16666);
	
	show ^= 1;
       if(read(0,&c,1 ) > 0){
		switch(c){
		case 'w' : row--; break;
		case 's' : row++; break;
		case 'a' : col--; break;
		case 'd' : col++; break; 
		case 'q' : goto leave;break;
		}
    }

}//while loop completed

leave: 
normal_mode();
printf(SHOW);
return 0;


}


void draw(){

printf("^_^");
fflush(stdout);

}



void move(int x, int y){

printf("\033[%d;%dH", x, y);

}


void erase(){

printf("    ");
fflush(stdout);

}

void raw_mode(){
struct termios t;
tcgetattr(0, &t);
t.c_lflag &= ~ICANON;
t.c_lflag &= ~ECHO;
t.c_cc[VMIN] = 0;
t.c_cc[VTIME] = 0;
tcsetattr(0, TCSANOW, &t);
}




void normal_mode(){
struct termios t;
tcgetattr(0, &t);
t.c_lflag |= ICANON;
t.c_lflag |= ECHO;
tcsetattr(0, TCSANOW, &t);
}
