// 409410100 HW4 PS. the last test data fails 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define N 1000

int StringsNum = 0, StrMaxLen = 0, Incorrect = 0, levelNum = 0;
int nodeNum = 0, Position = 0, CurNum = 0;
int ans[N] = {0}, copyData[N] = {0};  // 存錯字串位置 & 是不是被複製的 data  
char str[N][N] = {0};         // 存讀入字串
unsigned long data[N] = {0};  // 存 hash 後的值

void SetUp(void);
void BiTree(void);
unsigned long MurmurOAAT32(char * );
void Ask(int, int);

int main (void) {
  SetUp();
  levelNum--;   // 已建好最底層

  BiTree();
  Ask(0, 0);
  
  // output
  printf("2 %d\n", Incorrect);
  for(int i = 0; i < Incorrect; i++) printf("%s\n", str[ans[i]]);

  return 0;
}
// 讀輸入 & 初始化
void SetUp () {
  scanf("%d %d", &StringsNum, &StrMaxLen);
  double StrNum = (double) StringsNum;    // 只是要轉型字串長用的
  levelNum = (int)(ceil(log(StrNum) / log(2.0))) + 1;
  double buf = (double) levelNum;         // 只是要轉型樹的高度用的
  nodeNum = ((int) pow(2.0, buf)) - 1;
  int baseNum = (int) pow(2.0, levelNum-1);
  for(int i = 0; i < StringsNum; i++) {
    char tmpArr[N];
    scanf("%s", tmpArr);        // 讀字串
    strncpy(str[i], tmpArr, StrMaxLen);
    str[i][StrMaxLen] = '\0';   // 結尾補 '\0'
    data[nodeNum - i - 1] = MurmurOAAT32(str[i]);   // 建成 data 陣列
    Position = nodeNum - i - 1, baseNum--;          // Position 記位置 baseNum 記還有幾個葉子沒有存
  }
  while(baseNum != 0) {
    data[Position - 1] = data[Position];  // copy 左邊的 data
    copyData[Position - 1] = 1;           // 是 copy 的 data
    baseNum--, Position--;                // 往右一次 & 多存一個 data
  }
}
// hash Fn
unsigned long MurmurOAAT32(char * key) {
  unsigned long h = 3323198485ul;
  for (; *key; ++key) {
    h ^= *key;
    h *= 0x5bd1e995;
    h ^= h >> 15;
  }
  return h;
}

// 建剩下層數的樹
void BiTree() {
  CurNum = ((int) pow(2.0, levelNum - 1));  // 目前所要建層的節點數
  for(; levelNum > 0; levelNum--) {         // 一層一層建樹
    for(int i = Position - 1, j = 0; j < CurNum; i--, j++) {
      // printf("%d %d QQ\n", i, j);
      if(copyData[2*i + 1] == 1 && copyData[2*i + 2] == 1) {
        data[i] = data[i + 1];  // copy 左邊的 data
        copyData[i] = 1;        // i 是 copy 的
        continue;
      }
      unsigned long tmpArr = data[2 * i + 2] + data[2 * i + 1];   // 要合併的 data
      char temp[N];
      memset(temp, '\0', N);    // 初始化
      sprintf(temp, "%lu", tmpArr);
      data[i] = MurmurOAAT32(temp); // hash
    }
    Position -= CurNum;
    CurNum /= 2; // 少一半要加的 node 數 & 往上一層合併
  }
}
// 問 server & 得到 feedback 做出判斷
void Ask(int idx, int level) {
  int base = ((int) (pow(2.0, level + 1)) - 2); // index of the left leaf  
  if(idx >= nodeNum) return;
  if(Incorrect == StringsNum) return;
  if(copyData[idx] == 1) return;
  unsigned long feedback = 0;
  printf("1 %d %d\n", level, base - idx);   // ask the judge
  fflush(NULL);
  scanf("%lu", &feedback);
    
  if(feedback == data[idx]) return;      // correct string
  if(idx*2 + 2 < nodeNum) {
    Ask(idx*2 + 2, level + 1); // left traverse
    if(idx*2 + 1 < nodeNum) {
      Ask(idx*2 + 1, level + 1); // right traverse 
    }
    return;
  }
  if(feedback != data[idx] && idx*2 + 1 >= nodeNum && idx*2 + 2 >= nodeNum) {
    ans[Incorrect] = base - idx; // find wrong string
    Incorrect++;    
    return;
  } 
}