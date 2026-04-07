#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "display.h"

#define BLACK 1
#define WHITE -1
#define NONE 0

typedef struct Othero
{
  int color;
  int value;
} Othero;

void display(Othero O[8][8]); //表示関数
void judge(Othero O[8][8]);  //勝敗関数
bool stop(Othero O[8][8]);  //盤面がどちらかの色のみ、またはNONEがなくなったとき止める
int canput(Othero O[8][8], int h, int w,bool turn,bool humanIsBlack); //置ける場所を探す。
bool searchDirection(Othero O[8][8], int y, int x, int dy, int dx, int mine, int opponent); //置ける場所を探すときの、特定の方向を探す関数
bool changeB(Othero O[8][8],int minplace[],bool turn,bool humanIsBlack,int count);//ユーザ入力で置く場所決定
void changeW(Othero O[8][8],int minplace[],bool turn,bool humanIsBlack,int count);//cpu側の置く場所決定
void changeB2(Othero O[8][8],int minplace[],bool turn,bool humanIsBlack,int count);//ユーザ入力しないで置く場所決定

void flipDiscs(Othero O[8][8],bool turn,bool humanIsBlack,int num);//裏返す。

bool isValidMoove(int num, int minplace[], int count);//ユーザ入力の判断

int cpu1(Othero O[8][8],int minplace[],int count);
int cpu2(Othero O[8][8],int minplace[],int count);

void amount(Othero O[8][8], int minplace[], int count, double score[], double weight);//裏返す量＋
void corner(int minplace[], int count, double score[], double weight);//角＋
void cornerFront(int minplace[], int count, double score[],double weight);//角の前ー
void edge(int minplace[], int count, double score[],double weight);//端＋
void edgeFront(int minplace[], int count, double score[],double weight);//端手前ー
void around(Othero O[8][8], int minplace[], int count, double score[],double weight);//周りがどのくらい埋まってるか＋

int currentStone(bool turn, bool humanIsBlack); //自分の石を返す
int opponentStone(bool turn, bool humanIsBlack); //相手の石を返す
bool isInside(int y, int x); //盤面内かどうか

int currentStone(bool turn, bool humanIsBlack) {
    if (turn == true) {
        return humanIsBlack ? BLACK : WHITE;
    } else {
        return humanIsBlack ? WHITE : BLACK;
    }
}

int opponentStone(bool turn, bool humanIsBlack) {
  int mine = currentStone(turn, humanIsBlack);
  return (mine == BLACK) ? WHITE : BLACK;
}

//端・角で落ちないようにする
bool isInside(int y, int x) {
    return (y >= 0 && y < 8 && x >= 0 && x < 8);
}

int main(){
  // 配列の初期配置
  int i, j, k, num1,num2;
  bool turn;
  bool humanIsBlack;
  int count=0;
  int passCount = 0; // パスの回数をカウント
  int can_put[28];
  Othero O[8][8];

  // 構造体配列初期化
  for (i = 0; i < 8; i++){
    for (j = 0; j < 8; j++){
      O[i][j].color = NONE;
    }
  }

  O[3][3].color = WHITE; 
  O[3][4].color = BLACK; 
  O[4][3].color = BLACK; 
  O[4][4].color = WHITE;

  // ターン決め
  printf("先攻(1)、後攻(2)を選択してください。:");
  scanf("%d", &num1);

  if (num1 == 1) { 
    humanIsBlack = true; // 人間が黒
    turn = true; // 黒が先手なので人間から開始
  }  else {  
    humanIsBlack = false; // 人間が白
    turn = false; // 黒が先手なのでCPUから開始
  }

  

  // 60回繰り返す
  for(i=0;i<60;i++){
    display(O); // 盤面表示
    // can_putの初期化
    count = 0;
    for (k = 0; k < 28; k++){
     can_put[k] = -1;
    }
    k = 0;

    //置ける場所探索
    for (i = 0; i < 8; i++){
      for (j = 0; j < 8; j++){
        if (O[i][j].color == NONE){

          num2 = canput(O, i, j, turn, humanIsBlack);
          if(num2!=-1){
            can_put[k]=num2;
            k++;
          }
          count=k;
        }
      }
    }

    if (count == 0) { // 置ける場所がない場合
      passCount++;
      printf("置ける場所がありません。パスします。\n");

      if (passCount == 2){
        printf("両社ともおける場所がないため終了します。\n");
        break;
      }
      turn = !turn; // ターンを切り替える
      continue; // 次のループ
    }

    passCount = 0;


    //can_putを最小にする
    int minplace[count];
    for(int i=0;i<count;i++){
      minplace[i]=can_put[i];
    }

    //置ける場所表示
    for (k = 0; k < count; k++){
        printf("[%d,%d]", minplace[k] / 10, minplace[k] % 10);
    }
    printf("\n");
    
    //置く場所決める、盤面更新
    if(turn==true){  
      changeB(O, minplace, turn, humanIsBlack, count);
    }
    else {
      changeW(O, minplace, turn, humanIsBlack, count);
    }

    turn = !turn; // ターンを切り替える

    //盤面をみて、終了するかどうか判断
    if(stop(O)==false) break;

  }
  judge(O); // 終了判定
  printf("終了しました。Enterキーを押すと閉じます。");
  getchar(); 
  getchar();
  return 0;
}

//(1)盤面を表示する関数。
void display(Othero O[8][8]){
  int i, j;
  printf(" ０１２３４５６７\n");
  for (i = 0; i < 8; i++)
  {
    printf("%d",i);
    for (j = 0; j < 8; j++)
    {
      // printf(array[i][j]);
      if (O[i][j].color == WHITE)
        printf("〇");
      else if (O[i][j].color == BLACK)
        printf("Ｘ");
      else
        printf("・");
    }
    printf("\n");
  }
}

//(2)終了判定関数
void judge(Othero O[8][8])
{
  int i, j;
  int b=0;
  int w=0;
  int sum = 0;
  for (i = 0; i < 8; i++)
  {
    for (j = 0; j < 8; j++)
    {
      if(O[i][j].color==BLACK) b+=O[i][j].color;
      else w+=O[i][j].color;
    }
  }
  sum=b+w;

  if (sum > 0)
    printf("あなたの勝ちです。黒：%d、白：%d",abs(b),abs(w));
  else if (sum < 0)
    printf("あなたの負けです。黒：%d、白：%d",abs(b),abs(w));
  else if (sum == 0)
    printf("引き分けです。黒：%d、白：%d",abs(b),abs(w));
}

//色が一色になったとき、またはNONEがなくなったとき止める。
bool stop(Othero O[8][8]){
  int black=0;
  int white=0;
  int none=0;
  for(int i=0;i<8;i++){
    for(int j=0;j<8;j++){
      if(O[i][j].color==BLACK) black++;
      if(O[i][j].color==WHITE) white++;
      if(O[i][j].color==NONE) none++;  
    }
  }
  if(black==0||white==0) return false;
  else if(none!=0) return true;
  else return false;
}


bool searchDirection(Othero O[8][8], int y, int x, int dy, int dx, int mine, int opponent) {
  int ny = y + dy;
  int nx = x + dx;
  bool foundOpponent = false;

  while (isInside(ny, nx)) {
    if (O[ny][nx].color == opponent) {
      foundOpponent = true;
      ny += dy;
      nx += dx;
      continue;
    }
    if (O[ny][nx].color == mine) {
      return foundOpponent; // 敵の石を見つけた後に自分の石があれば置ける
    }
    return false; // 空白または盤面外に出た場合は置けない
  }
  return false; // 盤面外に出た場合は置けない
}

//(3)置ける場所を探す関数
int canput(Othero O[8][8], int y, int x, bool turn, bool humanIsBlack){
  int mine = currentStone(turn, humanIsBlack);
  int opponent = opponentStone(turn, humanIsBlack);

  if (!isInside(y, x)) return -1;
  if (O[y][x].color != NONE) return -1;

  for (int dy = -1; dy <= 1; dy++){
    for (int dx = -1; dx <= 1; dx++){
      if (dy == 0 && dx == 0) continue;

      if (searchDirection(O, y, x, dy, dx, mine, opponent)) {
        return y * 10 + x;
      }
    }
  }

  return -1;
}


//(4)ユーザ入力で盤面更新黒
bool changeB(Othero O[8][8],int minplace[],bool turn,bool humanIsBlack,int count){
  int num;
  printf("置く場所を入力してください。[3,4] → 34 : ");
  scanf("%d", &num);

  if(isValidMoove(num,minplace,count)==true){
    flipDiscs(O,turn,humanIsBlack,num);
    return true;
  }

  return false;
}

//cpu盤面更新
void changeW(Othero O[8][8],int minplace[], bool turn,bool humanIsBlack,int count){
  int num = cpu2(O,minplace,count);
  flipDiscs(O,turn,humanIsBlack,num);
}

//ユーザ入力なし盤面更新黒
void changeB2(Othero O[8][8],int minplace[], bool turn,bool humanIsBlack,int count){
  int num = cpu1(O,minplace,count);
  flipDiscs(O,turn,humanIsBlack,num);
}

//裏返す
void flipDiscs(Othero O[8][8],bool turn,bool humanIsBlack,int num){
  int x = num % 10;
  int y = num / 10;
  int dx,dy;
  int mine = currentStone(turn, humanIsBlack);
  int opponent = opponentStone(turn, humanIsBlack);

  printf("%d,%d\n",y,x);
  O[y][x].color=mine;

  for(dx=-1;dx<=1;dx++){
    for (dy = -1; dy <= 1; dy++) {
      if (dx == 0 && dy == 0) continue; // x,yと同じ座標はスキップ

      // 一つ隣の座標
      int nx = x + dx;
      int ny = y + dy;

      // 隣が盤面の内部であり、かつ敵の石がある間ループを続ける
      while (isInside(ny, nx) && O[ny][nx].color == opponent) {
          nx += dx;
          ny += dy;
      }

      // ループが終わった場所が盤面内で、かつ自分の石があるなら
      if (isInside(ny, nx) && O[ny][nx].color == mine) {
          // ひっくり返す処理を行う
          while (true) {
              nx -= dx;
              ny -= dy;
              if (nx == x && ny == y) break;
              O[ny][nx].color = mine;
          }
      }
    }
  }
}

//ユーザ入力の判定
bool isValidMoove(int num, int minplace[], int count){
  for(int i=0; i<count; i++){
    if(minplace[i]==num) return true;
  }
  printf("そこにはおけません。\n");
  return false;
}

//cpu(今はランダム)
int cpu1(Othero O[8][8],int minplace[],int count){
  (void)O;
  int num=(int)(rand()*(count)/(1+RAND_MAX));
  return minplace[num];
}

//cpu(コスト計算)
int cpu2(Othero O[8][8], int minplace[],int count){
  int num=0;
  double score[count];
  double max=-10;

  for(int i=0; i<count; i++){
    score[i]=0; //score初期化
  }
  
  amount(O,minplace,count,score,0.9);  //裏返す量＋
  corner(minplace,count,score,10);     //角＋
  cornerFront(minplace,count,score,10);//角手前-
  edge(minplace,count,score,5);        //端＋
  edgeFront(minplace,count,score,5);   //端手前-
  around(O,minplace,count,score,0.9);  //周りの駒の量＋

  //scoreが一番大きいものを探す。
  for(int i=0;i<count; i++){
    if(max<score[i]){
      max=score[i];
      num=i;
    }
  }
  printf("[%f]\n",score[num]);
  return minplace[num];
}


//ひっくり返せる数でスコアを決める。
void amount(Othero O[8][8], int minplace[], int count, double score[], double weight){
  int dx,dy;
  int opponent=BLACK;
  int mine=WHITE;
  for(int i=0;i<count;i++){
    int x = minplace[i] % 10;
    int y = minplace[i] / 10;

    for(dx=-1;dx<=1;dx++){
      for (dy = -1; dy <= 1; dy++) {
        // x,yと同じ座標はスキップ
        if (dx == 0 && dy == 0) continue;

        // 一つ隣の座標
        int nx = x + dx;
        int ny = y + dy;

        // 隣が盤面の内部であり、かつ敵の石がある間ループを続ける
        while (isInside(ny, nx) && O[ny][nx].color == opponent) {
          nx += dx;
          ny += dy;
        }
        // ループが終わった場所が盤面内で、かつ自分の石があるなら
        if (isInside(ny, nx) && O[ny][nx].color == mine) {
          // ひっくり返す処理を行う
          while (true) {
            nx -= dx;
            ny -= dy;
            if (nx == x && ny == y) break;
            O[ny][nx].color = mine;
            score[i]=score[i]+(1*weight);//重み
            }
          }
      }
    }
  }
}

//角だったら加点
void corner(int minplace[], int count, double score[], double weight){
  for(int i=0;i<count;i++){
    int x = minplace[i] % 10;
    int y = minplace[i] / 10;
    if(x==(1||7)&&y==(1||7)){
      score[i]=score[i]+(1*weight);
    }
  }
}

//角の手前だったら減点
void cornerFront(int minplace[], int count, double score[], double weight){
  for(int i=0;i<count;i++){
    int x = minplace[i] % 10;
    int y = minplace[i] / 10;
    if((y==(0||1||6||7)&&(x==(1||6)))||(y==(1||6)&&(x==(0||7)))){
      score[i]=score[i]-(1*weight);
    }
  }
}

//端だったら加点
void edge(int minplace[], int count, double score[], double weight){
  for(int i=0; i<count; i++){
    int x = minplace[i] % 10;
    int y = minplace[i] / 10;
    if((y==(0||7)&&x==(2||3||4||5))||(y==(2||3||4||5)&&(x==(0||7)))){
      score[i]=score[i]+(1*weight);
    }
  }
}

//端の手前だったら減点
void edgeFront(int minplace[], int count, double score[], double weight){
  for(int i=0; i<count; i++){
    int x = minplace[i] % 10;
    int y = minplace[i] / 10;
    if(y==(1||6)||x==(1||6)){
      score[i]=score[i]-(1*weight);
    }
  }
}

//周りに駒があるかどうか
void around(Othero O[8][8], int minplace[], int count, double score[],double weight){
  int dx,dy,x,y;
  int point;

  for(int i=0;i<count;i++){
    x = minplace[i] % 10;
    y = minplace[i] / 10;
    point=0;

    for(dx=-1;dx<=1;dx++){
      for (dy = -1; dy <= 1; dy++) {
        // x,yと同じ座標はスキップ
        if (dx == 0 && dy == 0) continue;

        // 一つ隣の座標
        int nx = x + dx;
        int ny = y + dy;

        // 盤面内の時だけ参照
        if(isInside(ny, nx) && O[ny][nx].color != NONE){
          point++;
        }
      }
    }
    score[i]=score[i]+(point*weight);
  }
}




