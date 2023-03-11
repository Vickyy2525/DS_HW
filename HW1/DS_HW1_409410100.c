#include <stdio.h>
#include <stdlib.h>
#define N 1000             // for global variables
#define MAX 0x3f3f3f3f            // define infinity as MAX

int NodeNum = 0, LinkNum = 0, ReqNum = 0;  // Node數、link數、request數
int graph[N][N];                           // 圖的連結 儲存在 2D陣列
int channel[N][N];                         // 邊的通道數 儲存在 2D陣列
int visited[N];                            // 訪問 ? 1 : 0
int PriorNode[N];                          // 放最短路徑，放上個最短路徑的節點
int distance[N] = {MAX};                   // 起點到各點最短結點數
int ansID[N];                              // 放可以走完的request的ID  
int node[N];

struct Link {
  int LinkID;             // 放link的ID
  int LinkID0, LinkID1;   // 存連接節點0 & 1的ID
  int LinkChannel;        // 放連接的通道數                   
};
struct Link link[N];

struct Req {                              
  int id;                 
  int src;
  int dst;
  int path[N];            // 可行Req可走的節點               
  int cnt;                // 紀錄可以走的節點個數
};
struct Req request[N];              

void SetGraph(void);               // 讀輸入
void Dijkstra(struct Req req);    
struct Req judge(struct Req req);  // 找每個request所走路徑 
int check(struct Req req);         // 確認占用到的記憶體和通道數夠不夠走
void operate(struct Req req);      // 扣掉走過占用到的記憶體和通道數

int main (void) {
  int AcceptCnt = 0;                    // 記可行的request個數
  scanf("%d %d %d", &NodeNum, &LinkNum, &ReqNum);

  SetGraph();

  for(int i = 0; i < ReqNum; i++) {  // 填request
    scanf("%d %d %d", &request[i].id, &request[i].src, &request[i].dst);
  }

  for(int i = 0; i < ReqNum; i++) {           
    Dijkstra(request[i]);            // request 做 Dijkstra演算法
    request[i] = judge(request[i]);  // 回傳可走節點跟節點個數
  }

  for(int i = 0; i < ReqNum; i++) {  // bubble sort req 可走的點
    for(int j = 0; j < ReqNum - i - 1; j++) {
      if(request[j].cnt > request[j+1].cnt) {
        struct Req temp = request[j];
        request[j] = request[j+1];
        request[j+1] = temp; 
      }
    }
  }

  for(int i = 0; i < ReqNum; i++) {
    if(check(request[i]) == 1) {
      operate(request[i]);
      ansID[AcceptCnt] = i;                    // 紀錄可走Request的ID
      AcceptCnt++;
    }
  }


  printf("%d\n", AcceptCnt);
  for(int i = 0; i < AcceptCnt; i++) {                // 印出可行request
    printf("%d", request[ansID[i]].id);
    for(int j = (request[ansID[i]].cnt); j >= 0; j--) {
      printf(" %d", request[ansID[i]].path[j]); // 印出可走的起點到終點
    }
    printf("\n");     
  }
  return 0;
}

void SetGraph() {  
  for(int i = 0; i < NodeNum; i++) {      // 填nodes的memories大小
    int id = 0, mem = 0;
    scanf("%d %d", &id, &mem);
    node[id] = mem;                       
  }

  for(int i = 0; i < NodeNum; i++) {      
    for(int j = 0; j < NodeNum; j++) {
      graph[i][j] = MAX;                  // 初始化成皆未連接
      channel[i][j] = MAX;
    }
  }

  for(int i = 0; i < LinkNum; i++) {      // 存通道數 跟 存有接起來的邊
    scanf("%d %d %d %d", &link[i].LinkID, &link[i].LinkID0, &link[i].LinkID1, &link[i].LinkChannel);
    channel[link[i].LinkID0][link[i].LinkID1] = link[i].LinkChannel; 
    channel[link[i].LinkID1][link[i].LinkID0] = link[i].LinkChannel; 
    graph[link[i].LinkID0][link[i].LinkID1] = 1;
    graph[link[i].LinkID1][link[i].LinkID0] = 1;
  }
}

void Dijkstra(struct Req req) { 
  for(int i = 0; i < NodeNum; i++) {      
    visited[i] = 0;          // X訪  
    distance[i] = MAX;   
  }
  PriorNode[req.src] = -1; 
  distance[req.src] = 0;     // 經過結點數為1

  for(int i = 0; i < NodeNum; i++) {  
    int CurShortPath = MAX;                 // traverse到的最短路徑長
    int idx = -1, jdx = -1;                 // 紀錄traverse到的節點
    for(int j = 0; j < NodeNum; j++) {
      if(!visited[j] && distance[j] < CurShortPath) {
        CurShortPath = distance[j];
        idx = j;
      }
    }
    if(idx == -1) break;              // 路徑都找完
    if(CurShortPath == MAX) break;    // 最短路徑是MAX == 沒有連通的路徑
    visited[idx] = 1;                 // 已拜訪
    for(jdx = 0; jdx < NodeNum; jdx++) {   // 可更新PriorNode?(窮舉方式，找一個不在最短路徑樹上、且與idx點相鄰的點jdx)
      if(!visited[jdx] && graph[jdx][idx] == 1 && distance[jdx] > distance[idx] + graph[idx][jdx]) {
        distance[jdx] = distance[idx] + graph[idx][jdx];
        PriorNode[jdx] = idx;              // 紀錄上個路徑
      }
    }
  }
}

struct Req judge(struct Req req) {
  req.cnt = 0;
  int tmp = req.dst;                    // 紀錄節點的idx

  req.path[req.cnt] = req.dst; 
  while(PriorNode[tmp] != -1) {         // 還沒找到終點
    for(int j = 0; j < NodeNum; j++) {  // 終點 -> ... -> 起點
      if(PriorNode[tmp] == j) {
        req.cnt++;
        req.path[req.cnt] = j;
        tmp = j;
      }
    }
  }
  return req;                           // 回傳這個Req大小的req
}

int check(struct Req req) {
  //先確認起點終點跟接起點的邊通道數
  if(channel[req.path[req.cnt-1]][req.path[req.cnt]] && node[req.path[0]] >= 1 && node[req.path[req.cnt]] >= 1){  
    for(int i = 0; i < req.cnt - 1;) {
      if(node[req.path[i+1]] >= 2 && channel[req.path[i]][req.path[i+1]]) { // 確認各結點還有邊通道數可不可走下去      
        i++;
      } else {
        return 0;                       
      }
    }
    return 1;     // 都可走
  }
  return 0;
}

void operate(struct Req req) {
  for(int i = 0; i < req.cnt; i++) {
    node[req.path[i]]--;
    channel[req.path[i]][req.path[i+1]]--;
    channel[req.path[i+1]][req.path[i]]--;
    node[req.path[i+1]]--;
  }
}