#include <stdio.h>
#include <math.h>     // for sqrt
#define N 1000

// 節點數、邊數量、計DFS步數、無人機數
int nodeNum = 0, edgeNum = 0, DFSpathCnt = 0, UAVnum = 0;     
// 父節點、MST的連接邊、拜訪與否、記DFS的路徑
int parent[N] = {0}, MST[N][N] = {0}, isVisited[N] = {0}, DFSpath[N] = {0}; 
// 油耗預算、目前耗油量、計各點距離的圖
double budget = 0, CurSum = 0, graph[N][N] = {0}; 
// UAV 路徑 及 各條路徑的步數
int UAVpath[N][N] = {0}, step[N] = {0};   

void SetUp();
int findParent(int x);
void mergeSet(int x, int y);
void Kruskal();
void DFS(int x);
void check();

struct Node {
  int ID;
  double X, Y;                   
};
struct Node node[N];    // for SetUp 初始 input
struct Edge {
  int from, to;   // from point A to point B
  double distance;// AB的距離                   
};
struct Edge edge[N];    // 紀錄邊

int main (void) {
  SetUp();
  Kruskal();

  for(int i = 0; i < nodeNum; i++) isVisited[i] = 0;
  for (int i = 0; i < nodeNum; i++) {
    if(isVisited[i] == 0) {
      isVisited[i] = 1;
      DFS(i);
    }
  }
  check();
  printf("%d\n", UAVnum);               // 可以走的無人機數
  for(int i = 0; i < UAVnum; i++) {     // 依序印出無人機可走的路徑
    printf("%d", i);
    for(int j = 0; j < step[i]; j++) {
      printf(" %d", UAVpath[i][j]);
    }
    printf("\n");
  }
  return 0;
}
// 初始的設定
void SetUp() {
  scanf("%d %lf", &nodeNum, &budget); // input
  for(int i = 0; i < nodeNum; i++) {
    scanf("%d %lf %lf", &node[i].ID, &node[i].X, &node[i].Y);// input
  }

  for(int i = 0; i < nodeNum; i++) {  // 算距離
    for(int j = i + 1; j < nodeNum; j++){
      graph[i][j] += (node[i].X - node[j].X) * (node[i].X - node[j].X) + (node[i].Y - node[j].Y) * (node[i].Y - node[j].Y);
      graph[i][j] = sqrt(graph[i][j]);
      graph[j][i] = graph[i][j];
    }
  }

  int cnt = 0;
  for(int i = 0; i < nodeNum; i++) {  // 輸入邊距離、起點、終點
    for(int j = i+1; j < nodeNum; j++){
      edge[cnt].from = i;
      edge[cnt].to = j;
      edge[cnt].distance = graph[i][j];
      cnt++;
    }
  }
  edgeNum = cnt;

  for(int i = 0; i < edgeNum; i++) {  // bubble sort 
    for(int j = 0; j < edgeNum - i - 1; j++) {
      if(edge[j].distance > edge[j+1].distance) {
        struct Edge temp = edge[j];
        edge[j] = edge[j+1];
        edge[j+1] = temp; 
      }
    }
  }
  for(int i = 0; i < 1000; i++) {
    step[i] = 0;
    for(int j = 0; j < 1000; j++) {
      UAVpath[i][j] = 0;
    }
  }
  
  return;
}
// 找父節點
int findParent(int x) {
  if(x == parent[x]) return x;
  else return (parent[x] = findParent(parent[x]));
}
// 合併兩節點
void mergeSet(int x, int y) {
  x = findParent(x);
  y = findParent(y);
  parent[y] = x;  // x--->y  x接y
  return;
}
// Kruskal's Algorithm 會用到 findParent and mergeSet
void Kruskal() {
  for(int i = 0; i < nodeNum; i++) parent[i] = i; // initialize

  for (int i = 0, j = 0; i < nodeNum-1 && j < edgeNum; i++) { // 對邊做找 parent node
    while(findParent(edge[j].from) == findParent(edge[j].to)) j++;
    mergeSet(edge[j].from, edge[j].to);  // 合併
    MST[edge[j].from][edge[j].to] = 1;
    MST[edge[j].to][edge[j].from] = 1;
    j++;	// 累計索引值
  }
}
// Depth first search
void DFS(int x) {   
  DFSpath[DFSpathCnt] = x;
  DFSpathCnt++;
  for(int i = 0; i < nodeNum; i++) {
    if(MST[x][i] == 1 && graph[x][i] < 0.5 * budget && isVisited[i] == 0) {
      isVisited[i] = 1;
      DFS(i);     // 繼續逡巡下去
    }
  }
  return;
}
// 檢查耗油量 決定無人機數量跟走過的點
void check() {
  UAVnum = 0;
  UAVpath[0][0] = DFSpath[0], step[0]++;                          // 走第一條路第一個點
  for(int i = 0; i < nodeNum - 1; i++) {                          // 一個一個走 存DFS走出的路徑
    if(CurSum + graph[DFSpath[i]][DFSpath[i+1]] < 0.5 * budget) { // 不超過 0.5 倍的Budget
      CurSum += graph[DFSpath[i]][DFSpath[i+1]];                  // 加上耗油
      UAVpath[UAVnum][step[UAVnum]] = DFSpath[i+1];               // 放下一個可以走的點
      step[UAVnum]++;                                             // 步數+1
    }else if(i+2 < DFSpathCnt && graph[DFSpath[i]][DFSpath[i+1]] > graph[DFSpath[i]][DFSpath[i+2]]
    && CurSum + graph[DFSpath[i]][DFSpath[i+2]] < 0.5 * budget) { 
      CurSum += graph[DFSpath[i]][DFSpath[i+2]];                  // 加上耗油
      UAVpath[UAVnum][step[UAVnum]] = DFSpath[i+2];               // 放下一個可以走的點
      step[UAVnum]++;                                             // 步數+1
      int temp = DFSpath[i+2];
      DFSpath[i+2] = DFSpath[i+1];
      DFSpath[i+1] = temp;
    }else if(i+3 < DFSpathCnt && graph[DFSpath[i]][DFSpath[i+1]] > graph[DFSpath[i]][DFSpath[i+3]]
    && CurSum + graph[DFSpath[i]][DFSpath[i+3]] < 0.5 * budget) { 
      CurSum += graph[DFSpath[i]][DFSpath[i+3]];                  // 加上耗油
      UAVpath[UAVnum][step[UAVnum]] = DFSpath[i+3];               // 放下一個可以走的點
      step[UAVnum]++;                                             // 步數+1
      int temp = DFSpath[i+3];
      DFSpath[i+3] = DFSpath[i+1];
      DFSpath[i+1] = temp;
    }else{
      UAVnum++;                                                   // 無人機 +1
      CurSum = 0;                                                 // 油耗歸 0
      UAVpath[UAVnum][0] = DFSpath[i+1];                          // 下條路徑是不能走的這個 i+1
      step[UAVnum]++;                                             // 下條路步數先加 1
    }
  }
  if(UAVpath[UAVnum][0] != 0) UAVnum++;
  return;
}