#include "SnakeGame.h"
using namespace std;

/*생성자*/
SnakeGame::SnakeGame(WINDOW *w1, int h, int w){
  win = w1;
  height_win = h;
  width_win = w;
  len_mission= false; grow_mission= false; poison_mission = false; gate_mission=false;
  num_growth = 0; num_poison=0; len_snake=4;num_gate=0; stage_clear = false;

  make_stage();
  make_snake();
}//end constructor

/*map에서 뱀 위치, 뱀의 배열 초기화*/
void SnakeGame::make_snake(){
    snake.clear();
    if(map[height_win/2][width_win/2 - 2] == 1||map[height_win/2][width_win/2 - 1] == 1||map[height_win/2][width_win/2] == 1 || map[height_win/2][width_win/2 + 1] == 1// 벽생성된 곳에 뱀이 생성되지 않도록
      ||map[height_win/2][width_win/2 + 2] == 1 || map[height_win/2][width_win/2 + 3] == 1){
        int i = 0;
        while(map[height_win/2][width_win/2 - 2  - i] == 1||map[height_win/2][width_win/2 - 1 - i] == 1||map[height_win/2][width_win/2 - i] == 1 || map[height_win/2][width_win/2 + 1 - i] == 1
          ||map[height_win/2][width_win/2 + 2- i] == 1 || map[height_win/2][width_win/2 + 3 - i] == 1){
            i++;
          }

        // snake.push_back(Point(height_win/2,width_win/2-i));
        // snake.push_back(Point(height_win/2, width_win/2+1-i));
        // snake.push_back(Point(height_win/2, width_win/2+2-i));
        // snake.push_back(Point(height_win/2, width_win/2+3-i));
        for(int k=0; k<4; k++){
           snake.push_back(Point(height_win/2,width_win/2+k-i));
        }

      }
      else{
        for(int k=0; k<4; k++){
           snake.push_back(Point(height_win/2,width_win/2+k));
        }
      }
      current_head_row = snake[0].row;
      current_head_col = snake[0].col;
      tail_row = snake[len_snake-1].row; //row
      tail_col = snake[len_snake-1].row;

      init_pair(1,COLOR_WHITE,COLOR_BLUE);//색 attribute 설정
      init_pair(2,COLOR_BLUE,COLOR_WHITE);//스코어
      init_pair(3,COLOR_WHITE,COLOR_RED);//미션
      init_pair(4,COLOR_GREEN,COLOR_WHITE);//뱀
      init_pair(5,COLOR_CYAN,COLOR_WHITE);//1벽
      init_pair(6,COLOR_RED,COLOR_WHITE);//아이템, 2벽
      init_pair(7,COLOR_MAGENTA,COLOR_WHITE);//gate

      wattron(win, COLOR_PAIR(4));


      for (int i=0;i<len_snake;i++){
        if (i==0){
          mvwprintw(win, snake[i].row, snake[i].col, "O");
        }
        else{
          mvwprintw(win, snake[i].row,snake[i].col,"o");
        }
      }

      while (!items.empty()){
        items.pop();
      }
      make_item();
      wrefresh(win);

}//end make_snake

/*뱀의 움직임*/
void SnakeGame::snake_move(int dir,int *pre){
  int crow = snake[0].row;
  int ccol = snake[0].col;
  if(map[crow][ccol] == 9){
    meet_gate(crow, ccol, dir, pre);
  }
  else if (map[crow][ccol] == 5 ||map[crow][ccol]==6){
    meet_item(crow,ccol, dir);
  }
  else{
    goHead(dir, current_head_row, current_head_col);
  }

  Point *tmp = new Point[len_snake-1];
  tail_row = snake[len_snake-1].row;
  tail_col = snake[len_snake-1].col;

  for(int i=0; i<len_snake-1; i++){
      tmp[i] = snake[i];
  }

  snake[0] = Point(current_head_row, current_head_col);

  for(int i=1; i<len_snake; i++){
      snake[i] = tmp[i-1];
  }

  len_snake = snake.size();
  // init_pair(1,COLOR_WHITE,COLOR_BLUE);//색 attribute 설정
  // init_pair(2,COLOR_BLUE,COLOR_WHITE);//스코어
  // init_pair(3,COLOR_WHITE,COLOR_RED);//미션
  init_pair(4,COLOR_GREEN,COLOR_WHITE);//뱀
  // init_pair(5,COLOR_CYAN,COLOR_WHITE);//1벽
  // init_pair(6,COLOR_RED,COLOR_WHITE);//아이템, 2벽
  // init_pair(7,COLOR_MAGENTA,COLOR_WHITE);//gate
  wattron(win, COLOR_PAIR(4));

  for (int i=0;i<len_snake;i++){
    if (map[snake[i].row][snake[i].col]==0){
      if (i==0){
        mvwprintw(win,snake[i].row,snake[i].col,"O");
      }
      else{
        mvwprintw(win,snake[i].row,snake[i].col,"o");
      }
    }
  }

  if(map[tail_row][tail_col] != 9){
    mvwprintw(win,tail_row,tail_col," ");
  }
  wattroff(win,COLOR_PAIR(4));

  wrefresh(win);
}// end snake move

/*빠르게 움직일 수 있게 해주는 움직임 구현*/
void SnakeGame::dash_move(int dir, int *pre){
  for (int i=0;i<5;i++){
    if(1>current_head_row||current_head_row>height_win-2||1>current_head_col||current_head_col>width_win-2
    || map[current_head_row][current_head_col]==1)
      break;
    else
      snake_move(dir, pre);
  }
}//end dash_move

/*다음 이동해야하는 위치 설정*/
void SnakeGame::goHead(int dir, int row, int col, int c){
  if (c==0){

    if (dir == 1){ //위쪽
      current_head_row = row-1;
    }
    if (dir == 2){ //아래
      current_head_row = row+1;
    }
    if (dir == 3){ //오른쪽
      current_head_col = col+1;
    }
    if (dir == 4){ //왼쪽
      current_head_col = col-1;
    }
  }
  else{
    current_head_row = row;
    current_head_col = col;
  }
}// end goHead

bool SnakeGame::map_include(int row, int col){
  for (int i=0; i<len_wall; i++){
    if(map[row + i][col] == 1){
      return false;
    }
    if(map[row][col + i] == 1){
      return false;
    }
  }
  return true;
}// end map_include

/*map 만들기: 벽구현*/
void SnakeGame::make_stage(){

  map = new int*[height_win];
  for (int i=0;i<height_win;i++){
    map[i] = new int[width_win];
  }//벽 크기 생성(화면 높이 * 화면 너비)

  for(int i=0; i<height_win; i++){
    for(int j = 0; j<width_win; j++){
      if((i == 0 && (j == 0|| j == width_win -1))
      ||(i == height_win -1 &&(j == 0 || j == width_win -1))){
        map[i][j] = 2;//4개의 꼭짓점은 2
      }
      else if(i == 0 || i == height_win - 1){
        map[i][j] = 1;
      }
      else if(j == 0|| j == width_win -1){
        map[i][j] = 1;
      }
      else{
        map[i][j]=0;
      }

    }
  }//map 값
  // init_pair(1,COLOR_WHITE,COLOR_BLUE);//색 attribute 설정
  init_pair(2,COLOR_BLUE,COLOR_WHITE);//스코어
  // init_pair(3,COLOR_WHITE,COLOR_RED);//미션
  // init_pair(4,COLOR_GREEN,COLOR_WHITE);//뱀
  init_pair(5,COLOR_CYAN,COLOR_WHITE);//1벽
  init_pair(6,COLOR_RED,COLOR_WHITE);//아이템, 2벽
  // init_pair(7,COLOR_MAGENTA,COLOR_WHITE);//gate
  wattron(win, COLOR_PAIR(2));

  for(int i = 0; i<height_win; i++){
    for(int j = 0; j<width_win; j++){
      if(map[i][j] == 2){
        wattron(win, COLOR_PAIR(6));
        mvwprintw(win, i, j, "#");
      }
      else if(map[i][j] == 1){
        wattron(win, COLOR_PAIR(5));
        mvwprintw(win, i, j, "#");
      }
      else{
        mvwprintw(win, i, j, " ");
      }
    }
  }//화면에 표시
  wrefresh(win);
  getch();

  int numrow;
  int numcol;
  srand(static_cast<unsigned int>(time(NULL)));
  int plus = 1;

  for (int i=0;i<count_wall;++i){
        int num1 = rand();
        int random1 = (int)num1%height_win; // num1은 0~width_win-1까지의 난수
        int num2 = rand();
        int random2 = (int)num2%width_win;

        if(random1 <= height_win - (len_wall + 2) && 2 <= random1 && random1 != height_win/2
          &&random2 <= width_win  - (len_wall + 2) && 2 <= random2
          &&map_include(random1, random2)){
            numrow = random1; numcol = random2;
            for (int k=0;k<count_wall;k++){
              if (map[numrow][numcol] == 0){
               for(int h = 0; h < len_wall; h++){
                 wattron(win, COLOR_PAIR(5));
                 if(plus < 0){
                   mvwprintw(win,numrow +h,numcol,"#");
                   map[numrow+h][numcol] = 1;
                 }else{
                   mvwprintw(win,numrow,numcol+h,"#");
                   map[numrow][numcol +h] = 1;
                 }

                }
                plus *= -1;
             }

           }
        }else{
          i--;
        }

      }


 //교차하는 벽생성시 교차부분은 ImmuneWall로 바꿈///
  for(int i = 1; i<height_win - 1; i++){
    for(int j = 1; j<width_win - 1; j++){
      if(map[i][j] == 1){
        if(map[i - 1][j] == 1 &&map[i][j - 1] == 1
          && map[i][j + 1] == 1 && map[i + 1][j] == 1){
            wattron(win, COLOR_PAIR(6));
            mvwprintw(win,i, j,"#");
            map[i][j] == 2;
        }
      }
    }
  }

  wrefresh(win);

}// end make_stage

/*뱀이 아이템 먹었을 때의 로직*/
void SnakeGame::meet_item(int row, int col, int dir){
  // init_pair(1,COLOR_WHITE,COLOR_BLUE);//색 attribute 설정
  // init_pair(2,COLOR_BLUE,COLOR_WHITE);//스코어
  // init_pair(3,COLOR_WHITE,COLOR_RED);//미션
  init_pair(4,COLOR_GREEN,COLOR_WHITE);//뱀
  // init_pair(5,COLOR_CYAN,COLOR_WHITE);//1벽
  // init_pair(6,COLOR_RED,COLOR_WHITE);//아이템, 2벽
  // init_pair(7,COLOR_MAGENTA,COLOR_WHITE);//gate
  wattron(win, COLOR_PAIR(4));
  if (map[row][col]==5){
    //growth item
    mvwprintw(win,row,col,"O");
    len_snake +=1;
    num_growth+=1;
    snake.push_back(Point(tail_row,tail_col));

  }
  else if (map[row][col]=6){
    //poison item
    Point p = snake.back();
    mvwprintw(win,p.row,p.col," ");
    len_snake-=1;
    num_poison+=1;
    snake.pop_back();
  }

  goHead(dir,row,col);

  map[row][col] = 0;
  Point i = Point(row,col);
  queue<Point> temp;
  while(!items.empty()){
    Point c = items.front();
    if (c!=i){
      temp.push(c);
    }
    items.pop();
  }
  items = temp;
  make_item();
} // end meet_item

/*map에 아이템 생성*/
void SnakeGame::make_item(){//growth item,  poison item은 wall에
  // init_pair(1,COLOR_WHITE,COLOR_BLUE);//색 attribute 설정
  // init_pair(2,COLOR_BLUE,COLOR_WHITE);//스코어
  // init_pair(3,COLOR_WHITE,COLOR_RED);//미션
  // init_pair(4,COLOR_GREEN,COLOR_WHITE);//뱀
  // init_pair(5,COLOR_CYAN,COLOR_WHITE);//1벽
  init_pair(6,COLOR_RED,COLOR_WHITE);//아이템, 2벽
  // init_pair(7,COLOR_MAGENTA,COLOR_WHITE);//gate
  wattron(win, COLOR_PAIR(6));

  srand(static_cast<unsigned int>(time(NULL)));
  //growth item
  int num=rand();
  int choice = (int)num%2;

  int nums[2]={0,};
  Point item = Point();
  while(items.size()<3){
    while(map[item.row][item.col]!=0||item.isInclude(len_snake, snake)){
       for (int i=0;i<2;i++){
         nums[i] = rand();
         if (i%2==0){

           item.row = (int)nums[i]%height_win;
         }
         else{
           item.col = (int)nums[i]%width_win;
         }
      }
    }//end while
   if (choice == 0){
     map[item.row][item.col]=5;
     mvwprintw(win,item.row, item.col, "+");
   }
   else{
     map[item.row][item.col]=6;
     mvwprintw(win, item.row, item.col, "x");
   }


  items.push(item);
  }//end while
  wrefresh(win);
}// end make_item

/*map에 아이템 삭제*/
void SnakeGame::delete_item(){
  Point item = items.front();
  map[item.row][item.col]=0;
  mvwprintw(win,item.row,item.col," ");
  items.pop();
  wrefresh(win);
}// end delete_item

/*스테이지 클리어시 다음 스테이지의 난이도 설정 및 초기화*/
void SnakeGame::next_stage(){
  len_mission= false; grow_mission= false; poison_mission = false; gate_mission=false;
  num_growth = 0; num_poison=0; len_snake=4;num_gate=0; stage_clear = false;
  started = true;
  snake_speed += 1000;
  //pre=4;
  set_playtime(0);
  if(stage == 2){
    set_speed(0.5);
    len_wall = 4;
    count_wall = 6;
  }
  else if(stage == 3){
    set_speed(0.2);
    len_wall = 5;
    count_wall = 9;
  }
  else if(stage == 4){
    set_speed(0.2);
    len_wall = 5;
    count_wall = 11;
  }
  make_stage();
  make_snake();
} //미션 생성 + 벽 새로 만들기

int SnakeGame::get_stage(){
  return stage;
}// end next_stage

/*게이트 만들기 및 map에 표시*/
void SnakeGame::make_gate(){//게이트 생성(wall의 9가 게이트)
  // init_pair(1,COLOR_WHITE,COLOR_BLUE);//색 attribute 설정
  // init_pair(2,COLOR_BLUE,COLOR_WHITE);//스코어
  // init_pair(3,COLOR_WHITE,COLOR_RED);//미션
  // init_pair(4,COLOR_GREEN,COLOR_WHITE);//뱀
  // init_pair(5,COLOR_CYAN,COLOR_WHITE);//1벽
  // init_pair(6,COLOR_RED,COLOR_WHITE);//아이템, 2벽
  init_pair(7,COLOR_MAGENTA,COLOR_WHITE);//gate
  wattron(win, COLOR_PAIR(7));
  int count = 0;
  for(int i = 0; i<height_win; i++){
    for(int j = 0; j<width_win; j++){
      if(map[i][j] == 1){
        count++;
      }
    }
  }
  srand(static_cast<unsigned int>(time(NULL)));
  int random1 = (int)rand()%count;

  int check = 0;
  for(int i = 0; i<height_win; i++){//첫번째 게이트 생성
    if(check == -1){
      break;
    }
    for(int j = 0; j<width_win; j++){
      if(map[i][j] == 1 && check == random1){
        first_gate = Point(i, j);
        map[i][j] = 9;
        mvwprintw(win,i, j,"G");
        check = -1;
        break;
      }else if(map[i][j] == 1){
        check++;
      }
    }
  }

  int random2 = (int)rand()%count;
  while(random1 ==random2){
    random2 = (int)rand()%count;
  }
  check = 0;
  for(int i = 0; i<height_win; i++){//두번째 게이트 생성
    if(check == -1){
      break;
    }
    for(int j = 0; j<width_win; j++){
      if(map[i][j] == 1 && check == random2){
        second_gate = Point(i, j);
        map[i][j] = 9;
        mvwprintw(win,i, j,"G");
        check = -1;
        break;
      }else if(map[i][j] == 1){
        check++;
      }
    }
  }
}// end make_gate

/*뱀의 속도 설정. 인자값이 작아질수록 속도 올라감*/
void SnakeGame::set_speed(double sec){
  snake_speed = sec*1000*1000;
}//end set_speed

int SnakeGame::get_speed(){
  return snake_speed;
}// end get_speed

/*점수 윈도우 설정*/
void SnakeGame::score_show(WINDOW *w){
  wborder(w,'|','|','-','-','|','|','|','|');
  char body[] = "Body: ";
  char body_char[2]; sprintf(body_char, "%d", len_snake);
  if(max_len < len_snake){
    max_len = len_snake;
  }
  char max_len_char[2]; sprintf(max_len_char, "%d" , max_len);
  char growth[] = "GI: "; char poison[] = "PI: "; char gate[] = "Gate: "; char pl[] = "P: ";
  char growth_char[2]; sprintf(growth_char, "%d", num_growth);
  char poison_char[2]; sprintf(poison_char, "%d", num_poison);
  char gate_char[2]; sprintf(gate_char, "%d", num_gate);
  mvwprintw(w, 2, 2, body); mvwprintw(w, 2, 8, body_char);mvwprintw(w, 2, 10, "/"); mvwprintw(w, 2, 11, max_len_char);
  mvwprintw(w, 3, 2, growth); mvwprintw(w, 3, 6, growth_char);
  mvwprintw(w, 4, 2, poison); mvwprintw(w, 4, 6, poison_char);
  mvwprintw(w, 5, 2, gate); mvwprintw(w, 5, 8, gate_char);
  if(started == false){ mvwprintw(w, 6, 2, pl); mvwprintw(w, 6, 5, play_time); }
} // end score_show

/*미션 윈도우 설정*/
void SnakeGame::mission_show(WINDOW *w){
  //len_snake, num_growth, num_poison, num_gate, stage 사용
  mvwprintw(w,1,1,"Clear");mvwprintw(w,1,13,"End");
  wborder(w,'|','|','-','-','|','|','|','|');
  char stageNum[1];   sprintf(stageNum, "%d", stage);

  mvwprintw(w,2,1,"B: "); mvwprintw(w,2,13,"-: ");
  mvwprintw(w,3,1,"+: ");
  mvwprintw(w,4,1,"G: ");


  mvwprintw(w, 6, 1, "stage: "); mvwprintw(w, 6, 9, stageNum);
  switch (stage) {
    case 1://뱀 몸 길이 6이상, growth_item 3이상, poison_item 5 이하, gate 5 이상
      //clear mission
      mvwprintw(w, 2, 4, "6");
      if(len_snake >= 6){
        mvwprintw(w, 2, 6, "(V)");
        len_mission = true;
      }else{mvwprintw(w, 2, 6, "( )");}

      mvwprintw(w, 3, 4, "3");
      if(num_growth >= 3){
        mvwprintw(w, 3, 6, "(V)");
        grow_mission = true;
      }else{mvwprintw(w, 3, 6, "( )");}

      mvwprintw(w, 4, 4, "5");
      if(num_gate >= 5){//////////////////////// 5
        mvwprintw(w, 4, 6, "(V)");
        gate_mission = true;
      }else{mvwprintw(w, 4, 6, "( )");}

      //end mission(true가 되면 게임 오버)
      mvwprintw(w, 2, 16, "5");
      if(num_poison >= 5){
        mvwprintw(w, 2, 18, "(V)");
        poison_mission = true;
      }else{mvwprintw(w, 2, 18, "( )");}
      break;
    case 2://뱀 몸 길이 8이상, growth_item 5이상, poison_item 4 이하, gate 5 이상
    //clear mission
    mvwprintw(w, 2, 4, "8");
    if(len_snake >= 8){
      mvwprintw(w, 2, 6, "(V)");
      len_mission = true;
    }else{mvwprintw(w, 2, 6, "( )");}

    mvwprintw(w, 3, 4, "5");
    if(num_growth >= 5){
      mvwprintw(w, 3, 6, "(V)");
      grow_mission = true;
    }else{mvwprintw(w, 3, 6, "( )");}

    mvwprintw(w, 4, 4, "5");
    if(num_gate >= 5){
      mvwprintw(w, 4, 6, "(V)");
      gate_mission = true;
    }else{mvwprintw(w, 4, 6, "( )");}

    //end mission(true가 되면 게임 오버)
    mvwprintw(w, 2, 16, "4");
    if(num_poison >= 4){
      mvwprintw(w, 2, 18, "(V)");
      poison_mission = true;
    }else{mvwprintw(w, 2, 18, "( )");}
    break;
    case 3://뱀 몸 길이 13이상, growth_item 8이상, poison_item 3 이하, gate 10 이상
    //clear mission
    mvwprintw(w, 2, 4, "13");
    if(len_snake >= 13){
      mvwprintw(w, 2, 6, "(V)");
      len_mission = true;
    }else{mvwprintw(w, 2, 6, "( )");}

    mvwprintw(w, 3, 4, "8");
    if(num_growth >= 8){
      mvwprintw(w, 3, 6, "(V)");
      grow_mission = true;
    }else{mvwprintw(w, 3, 6, "( )");}

    mvwprintw(w, 4, 4, "10");
    if(num_gate >= 10){
      mvwprintw(w, 4, 6, "(V)");
      gate_mission = true;
    }else{mvwprintw(w, 4, 6, "( )");}

    //end mission(true가 되면 게임 오버)
    mvwprintw(w, 2, 16, "3");
    if(num_poison >= 3){
      mvwprintw(w, 2, 18, "(V)");
      poison_mission = true;
    }else{mvwprintw(w, 2, 18, "( )");}
    break;
    case 4://뱀 몸 길이 18이상, growth_item 12이상, poison_item 3 이하, gate 15 이상
    //clear mission
    mvwprintw(w, 2, 4, "18");
    if(len_snake >= 18){
      mvwprintw(w, 2, 6, "(V)");
      len_mission = true;
    }else{mvwprintw(w, 2, 6, "( )");}

    mvwprintw(w, 3, 4, "12");
    if(num_growth >= 12){
      mvwprintw(w, 3, 6, "(V)");
      grow_mission = true;
    }else{mvwprintw(w, 3, 6, "( )");}

    mvwprintw(w, 4, 4, "15");//테스트용 게이트
    if(num_gate >= 15){
      mvwprintw(w, 4, 6, "(V)");
      gate_mission = true;
    }else{mvwprintw(w, 4, 6, "( )");}

    //end mission(true가 되면 게임 오버)
    mvwprintw(w, 2, 16, "3");
    if(num_poison >= 3){
      mvwprintw(w, 2, 18, "(V)");
      poison_mission = true;
    }else{mvwprintw(w, 2, 18, "( )");}
    break;
  }
}// end mission_show

/*게이트 들어갔을 때, 나와야하는 방향 설정*/
Point SnakeGame::search_output(Point output_gate,int *pre){
    bool w = true, a = true, s = true, d = true;
     //w = 1, a = 4, s = 2, d = 3
    if(output_gate.row == 0){
      w = false;
    }
    if(output_gate.col == 0){
      a = false;
    }
    if(output_gate.row == height_win -1){
      s = false;
    }
    if(output_gate.col == width_win - 1){
      d = false;
    }
    int number = 0;
    string sequence[4] = {"1342", "2431", "3214", "4123"};
    for(int i = 0; i < 4; i++){
      if(sequence[i].at(0) == (char)(*pre + 48)){
        number = i;
        break;
      }
    }

    for(int i = 0; i < 4; i++){
       switch(sequence[number].at(i)){
         case '1':
           if(w && map[output_gate.row - 1][output_gate.col] == 0){
             *pre = 1;
             return Point(output_gate.row-1, output_gate.col);
           }
           break;
         case '2':
           if(s && map[output_gate.row + 1][output_gate.col] == 0){
             *pre = 2;
             return Point(output_gate.row + 1, output_gate.col);
           }
           break;
         case '3':
           if(d && map[output_gate.row][output_gate.col +1] == 0){
             *pre = 3;
             return Point(output_gate.row, output_gate.col +1);
           }
           break;
         case '4':
           if(a && map[output_gate.row][output_gate.col - 1] == 0){
             *pre = 4;
             return Point(output_gate.row, output_gate.col - 1);
           }
           break;
       }
    }
}// end search_output

/*게이트 만났을 때 처리하는 로직*/
void SnakeGame::meet_gate(int row, int col, int dir, int *pre){
  num_gate++;
  Point input_gate, output_gate;
  if(first_gate.row == row && first_gate.col == col){
   input_gate = Point(row, col);
   output_gate = Point(second_gate.row, second_gate.col);
  }else{
   input_gate = Point(row, col);
   output_gate = Point(first_gate.row, first_gate.col);
  }
  goHead(dir, search_output(output_gate,pre).row,search_output(output_gate,pre).col,1);
}// end meet_gate

void SnakeGame::set_playtime(double r){
  sprintf(play_time,"%.2f", r);
}

bool SnakeGame::get_started(){
  return started;
}

bool SnakeGame::get_stage_clear(){
  return stage_clear;
}

void SnakeGame::inc_stage(){
  stage+=1;
}

void SnakeGame::set_totalplaytime(double r){
	total_playtime = r;
  	sprintf(after_game12, "%.2f", total_playtime);
}

double SnakeGame::get_totalplaytime(){
	return total_playtime;
}

/*게임이 끝나는 조건: 실패, 성공 조건 모두 있음*/
void SnakeGame::game_end(){
  int crow = snake[0].row;
  int ccol = snake[0].col;
  if(map[crow][ccol] == 1){
    started = false;
  }
  if(len_snake < 3){
    started = false;
  }
  for(int i=1; i<len_snake; i++){
    if (snake[0]==snake[i]){
      started = false;
    }
  }
  if(poison_mission == true){
    started = false;
  }

  int mission = 0;
  if(len_mission == true){mission++;}
  if(grow_mission == true){mission++;}
  if(gate_mission == true){mission++;}
  switch (stage){
    case 1:
      if(mission >= 1){stage_clear = true; started = false;}
      break;

    case 2:
      if(mission >= 2){stage_clear = true; started = false;}/////2
      break;
    case 3:
      if(mission >= 3){stage_clear = true; started = false;}//////3
      break;
    case 4:
      if(mission >= 3){stage_clear = true; started = false;}////3
      break;
  }
}// end game_end
