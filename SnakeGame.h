//SnakeGame.h

#ifndef _SNAKEGAME_H
#define _SNAKEGAME_H

#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <vector>
#include <string>
#include "Point.h"


class SnakeGame{
  Point first_gate;// 첫번째 게이트의 좌표
  Point second_gate;// 두번째 게이트의 좌표
  int size_map;// map의 한 변의 길이(기본 21x21, 정사각형)
  int len_wall = 3;// map 내부에 생성되는 wall의 길이(기본 3)
  int count_wall = 3;// map 내부에 생성되는 wall의 개수(기본 3)
  // int height = w.ws_row;
  // int width = w.ws_col;
  int height_win;
  int width_win;
  int **map;
  bool len_mission, grow_mission, poison_mission, gate_mission;
  int snake_speed = 500*1000;// sec: 뱀 속도
  int num_growth=0;// growth item을 먹은 개수
  int num_poison=0;// poison item을 먹은 개수
  int num_gate=0;// gate를 통과한 횟수
  char play_time[3];// 종료 시간 - 시작시간. seconds로 계산한다.
  //int direction = 1; // 키보드 받아서 처리할 예정
  int current_head_row;// 뱀 머리
  int current_head_col;
  int tail_row;
  int tail_col;

  int len_snake = 4;// 뱀 길이
  int max_len = 4;
  std::vector<Point> snake;
  std::queue<Point> items; // poison_item Point  모음
  // vector<int> sna(len_snake);
  WINDOW *win;
  int stage=1;
  bool started = true;
  bool stage_clear = false;
  double total_playtime=0;

public:
char after_game[39]="00000000   0000   000     000  000000";
char after_game2[39]="00        00  00  0 00   00 0  00    ";
char after_game3[39]="00  0000  00  00  0  0   0  0  000000";
char after_game4[39]="00   00   000000  0   0 0   0  00    ";
char after_game5[39]="00000000  00  00  00  000   0  000000";
char after_game6[42] ="  00000  00      000000   0000   000000  ";
char after_game7[42] =" 00      00      00      00  00  00   00 ";
char after_game8[42] ="00       00      000000  00  00  000000  ";
char after_game9[42] =" 00      00      00      000000  00   00 ";
char after_game10[42] ="  00000  000000  000000  00  00  00    00";
char after_game11[18] = "total_play_time: ";
char after_game12[3];

  SnakeGame(WINDOW *w1, int h, int w);
  void make_snake();
  void snake_move(int dir, int *pre);
  void dash_move(int dir, int *pre);
  void goHead(int dir, int row, int col, int c=0);
  bool map_include(int row, int col);
  void make_stage();
  void meet_item(int row, int col, int dir);
  void make_item();
  void delete_item();
  void next_stage();
  int get_stage();
  void make_gate();
  void set_speed(double sec);
  int get_speed();
  void score_show(WINDOW *w);
  void mission_show(WINDOW *w);
  Point search_output(Point output_gate, int *pre);
  void meet_gate(int row, int col, int dir, int *pre);
  void set_playtime(double r);
  void set_totalplaytime(double r);
  double get_totalplaytime();
  bool get_stage_clear();
  bool get_started();
  void inc_stage();
  void game_end();
};
#endif
