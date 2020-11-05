#include "Point.h"
#include "SnakeGame.h"

#include <ncurses.h>
#include <iostream>
#include <locale.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>

using namespace std;

int main(){
  setlocale(LC_ALL, "ko_KR.utf8");
  setlocale(LC_CTYPE, "ko_KR.utf8");//한글 출력 설정
  // struct winsize w;
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  char key;
  char userName[8];
  WINDOW *win1;
  WINDOW *win2;
  WINDOW *win3;
  WINDOW *win4;

  //터미널 크기 받아오기
  int height = w.ws_row;
  int width = w.ws_col;
  int height_win = w.ws_row-10;
  int width_win = w.ws_col/2;

  initscr();//curse 모드 시작
  keypad(stdscr, TRUE);
  curs_set(0);
  cbreak();
  echo();

  printw("User name: ");
  scanw("%s", userName);
  noecho();

  //게임 메뉴얼
  printw("\n        @ Notice @\n\n");
  printw("snake의 길이가 3미만 or 머리가 벽에 닿으면 실패\n");
  printw("화살표 키로 상하좌우 이동\n");
  printw("'dash'>> space 키를 눌러 대시를 할수 있다\n");
  printw("1 stage에서는 1개\n");
  printw("2 stage에서는 2개\n");
  printw("3,4 stage에서는 3개의 미션을 통과해야 미션 클리어\n");
  printw("4 stage를 클리어하면 게임 클리어\n");
  printw("#: wall, G: gate,  +,GI : growth item, -,PI:poison item, Pl: play time\n");
  printw("enter키를 눌러 게임 시작\n");

  getch();
  refresh();
  clear();


  start_color();
  init_color(COLOR_RED, 230*4, 100*4, 90*4);
  init_color(COLOR_CYAN, 70*4, 110*4, 110*4);
  init_color(COLOR_BLUE, 9*4, 100*4, 50*4);

  init_pair(1,COLOR_WHITE,COLOR_BLUE);//색 attribute 설정
  init_pair(2,COLOR_BLUE,COLOR_WHITE);//스코어
  init_pair(3,COLOR_WHITE,COLOR_RED);//미션
  init_pair(4,COLOR_GREEN,COLOR_WHITE);//뱀
  init_pair(5,COLOR_CYAN,COLOR_WHITE);//1벽
  init_pair(6,COLOR_RED,COLOR_WHITE);//아이템, 2벽
  init_pair(7,COLOR_MAGENTA,COLOR_WHITE);//gate

  init_color(COLOR_BLACK, 90*4, 120*4, 80*4);

// //숫자, 폰트 색, 폰트
  border('|','|','-','-','|','|','|','|');
  mvprintw(1,1, "SnakeGame");
  // mvprintw(2,1,"User : ");
  // mvprintw(2,2,userName);
  mvprintw(2,1,"User : ");
  printw(userName,"\n");
  refresh();
  getch();
/////////////////////////////////////////

  //게임 윈도우
  win1= newwin(height_win,width_win,3,3);
  wbkgd(win1,COLOR_PAIR(2));
  wattron(win1, COLOR_PAIR(2));
  SnakeGame s1 = SnakeGame(win1,height_win,width_win);
  wrefresh(win1);

  //스코어 윈도우
  win2 = newwin(height/3, width/3, 3, width/2 + 10);
  wbkgd(win2,COLOR_PAIR(1));
  wattron(win2, COLOR_PAIR(1));
  wborder(win2,'|','|','-','-','|','|','|','|');
  wrefresh(win2);

  //미션 윈도우
  win3 = newwin(height/3,width/3, height/2, width/2+10);
  wbkgd(win3,COLOR_PAIR(3));
  wattron(win3, COLOR_PAIR(3));
  wborder(win3,'|','|','-','-','|','|','|','|');
  wrefresh(win3);

  // getch();
  refresh();

  //게임 실행
  while(s1.get_stage()<=4){
    int pre = 4;
    getch();
    s1.make_gate();
    wrefresh(win1);
    s1.mission_show(win3);
    wrefresh(win3);
    s1.score_show(win2);
    wrefresh(win2);
    getch();
    nodelay(stdscr, TRUE);

    //시간측정
    time_t start, end;
    double result;
    start = time(NULL);
    time_t first,mid;
    first = time(NULL);

    //본격적인 게임 시작
    while (s1.get_started()){
      int input;
      mid = time(NULL);
      s1.mission_show(win3);
      wrefresh(win3);
      double t = mid-start;
      s1.score_show(win2);
      wrefresh(win2);

      //5초 지나면 아이템 옮겨가도록 설정
      if (mid-first>=5.0){
        s1.delete_item();
        s1.make_item();
        first = time(NULL);
      }

      input  = getch();
      switch (input){
        case KEY_LEFT:
          s1.snake_move(4,&pre);
          break;
        case KEY_RIGHT:
          s1.snake_move(3,&pre);
          break;
        case KEY_DOWN:
          s1.snake_move(2,&pre);
          break;
        case KEY_UP:
          s1.snake_move(1,&pre);
          break;
        case ' ':
          s1.dash_move(pre, &pre);
          break;
        case ERR:
          s1.snake_move(pre,&pre);
          break;
        }
      if(input == KEY_LEFT){
        pre = 4;
      }else if(input == KEY_RIGHT){
        pre = 3;
      }else if(input == KEY_DOWN){
        pre = 2;
      }else if(input == KEY_UP){
        pre = 1;
      }
      s1.game_end();

      wrefresh(win1);
      usleep(s1.get_speed());
    }
    end = time(NULL);
    result = (double)(end - start); // result == 플레이 타임
    s1.set_playtime(result);

    nodelay(stdscr, false);

    s1.score_show(win2);
    wrefresh(win2);
    getch();

    // 새로운 스테이지로 넘어갈 때
    if(s1.get_stage_clear() == true){
      s1.set_totalplaytime(s1.get_totalplaytime()+result);
      s1.inc_stage();
      werase(win2);
      werase(win3);
      wrefresh(win2);
      wrefresh(win3);
      if(s1.get_stage() < 5 ) {s1.next_stage();}
    }
    else{
      s1.set_totalplaytime(0);
      werase(win2);
      werase(win3);
      wrefresh(win2);
      wrefresh(win3);
      s1 = SnakeGame(win1,height_win,width_win);
    }
    getch();
  }

  // 게임 끝났으므로 모든 윈도우 지우기
  delwin(win1);
  delwin(win2);
  delwin(win3);

  // 모든 스테이지 클리어시 보여주는 윈도우
  win4 = newwin(height-5, width-5, 3, 3);
  wbkgd(win4,COLOR_PAIR(2));
  wattron(win4, COLOR_PAIR(2));
  // 윈도우 문구: GAME CLEAR
  mvwprintw(win4,3,3, s1.after_game);
  mvwprintw(win4,4,3, s1.after_game2);
  mvwprintw(win4,5,3, s1.after_game3);
  mvwprintw(win4,6,3, s1.after_game4);
  mvwprintw(win4,7,3, s1.after_game5);
  mvwprintw(win4,9,3, s1.after_game6);
  mvwprintw(win4,10,3, s1.after_game7);
  mvwprintw(win4,11,3, s1.after_game8);
  mvwprintw(win4,12,3, s1.after_game9);
  mvwprintw(win4,13,3, s1.after_game10);
  mvwprintw(win4,15,3,s1.after_game11);
  mvwprintw(win4,15,21, s1.after_game12);
  wrefresh(win4);
  getch();

  endwin();//curses 모드

  return 0;

}
