#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int frequency = 0, time = 0, user = 0, UserCnt = 0;
int OneNum = 0, TwoNum = 0, FourNum = 0, EightNum = 0; 
int CurTime = 0;  // make sure adequate time
int TimeIsOne = 0, TimeIsTwo = 0, TimeIsFour = 0, TimeIsEight = 0, TimeIsSixteen = 0; 
int PlaceOne = 0, PlaceTwo = 0, PlaceFour = 0, PlaceEight = 0, PlaceSixteen = 0, RecordTime = 0;// record place item and current time 
int previous = 0; // pervious time

struct User {
  int ID, frequency[6], time[6];                      
};
struct User users[1000] = {0};
struct Ans {
  int ID, frequency, time;                      
};
struct Ans ans[1000] = {0};

void SetCondition();
int check(struct User users);
struct Ans place(struct User users, struct Ans ans);

int main (void) {
  SetCondition();

  for(int i = 0; i < user; i++) {   // Can they put in the resource table?
    if(check(users[i])) {   // can place
      ans[UserCnt].ID = i;  // record ID
      ans[UserCnt] = place(users[i], ans[UserCnt]); // place item
      previous = users[i].time[0];  // record its time for next item checking
      UserCnt++;            
    }
  }

  printf("%d\n", UserCnt);
  for(int i = 0; i < UserCnt; i++) {// print available situations
    printf("%d %dx%d %d %d\n", users[ans[i].ID].ID, users[ans[i].ID].frequency[0], users[ans[i].ID].time[0], ans[i].frequency, ans[i].time);
  }
  return 0;
}

// set initial condition & sort
void SetCondition() {
  int c = 0;
  char str[50];
  scanf("%d %d %d", &frequency, &time, &user);
  OneNum = frequency / 16, TwoNum = frequency / 8, FourNum = frequency / 4, EightNum = frequency / 2;  
  // compute the number of the item can place  
  CurTime = time;

  for(int i = 0; i < user; i++) {
    scanf("%d", &users[i].ID);
    int j = 0;
    while((c = getchar()) != '\n') {
      scanf("%s", str);
      char *location = strtok(str, "x");      // frequency
      users[i].frequency[j] = atoi(location); // string to integer
      location = strtok(NULL, " ");           // time
      users[i].time[j] = atoi(location);      // string to integer
      j++;  // record the number of input f x time
    }

    for(int idx = 0; idx < j; idx++) {  // bubble sort each item(time from small to big)
      for(int jdx = 0; jdx < j - idx - 1; jdx++) {
        if(users[i].time[jdx] > users[i].time[jdx+1]) {
          users[i].time[5] = users[i].time[jdx];
          users[i].time[jdx] = users[i].time[jdx+1];
          users[i].time[jdx+1] = users[i].time[5]; 
          users[i].frequency[5] = users[i].frequency[jdx];
          users[i].frequency[jdx] = users[i].frequency[jdx+1];
          users[i].frequency[jdx+1] = users[i].frequency[5]; 
        }
      }
    }
  }

  for(int i = 0; i < user; i++) {   // bubble sort each user(time[0] from small to big)
    for(int j = 0; j < user - i - 1; j++) {
      if(users[j].time[0] > users[j+1].time[0]) {
        struct User temp = users[j];
        users[j] = users[j+1];
        users[j+1] = temp; 
      }
    }
  }
}

// Can it put into the table?
int check(struct User person) {
  if(person.frequency[0] > frequency) return 0;     // check frequency
  if(person.time[0] == 1) {       // 16x1
    TimeIsOne++;                  // count 16x1
    if(TimeIsOne == 1 && CurTime-(person.time[0]) >= 0) {
      CurTime -= person.time[0];  // can place
      if(TimeIsOne == OneNum) TimeIsOne = 0;
    }
    else if(TimeIsOne == OneNum) TimeIsOne = 0;               // need to restart to count
    else if(TimeIsOne == 1 && CurTime-(person.time[0]) < 0) { // can't place
      TimeIsOne = 0;
      return 0;
    }
    return 1;
  }if(person.time[0] == 2) {      // 8x2
    TimeIsTwo++;                  // count 8x2
    if(TimeIsTwo == 1 && CurTime-(person.time[0]) >= 0) CurTime -= person.time[0];  // can place
    else if(TimeIsTwo == TwoNum) TimeIsTwo = 0;               // need to restart to count
    else if(TimeIsTwo == 1 && CurTime-(person.time[0]) < 0) { // can't place
      TimeIsTwo = 0;
      return 0;
    }
    return 1;
  }else if(person.time[0] == 4) { // 4x4
    TimeIsFour++;                 // count 4x4
    if(TimeIsFour == 1 && CurTime-(person.time[0]) >= 0) CurTime -= person.time[0]; // can place
    else if(TimeIsFour == FourNum) TimeIsFour = 0;            // need to restart to count
    else if(TimeIsFour == 1 && CurTime-(person.time[0]) < 0) {// can't place
      TimeIsFour = 0;
      return 0;
    }
    return 1;
  }else if(person.time[0] == 8) { // 2x8
    TimeIsEight++;                // count 2x8
    if(TimeIsEight == 1 && CurTime-(person.time[0]) >= 0) CurTime -= person.time[0]; // can place
    else if(TimeIsEight == EightNum) TimeIsEight = 0;          // need to restart to count
    else if(TimeIsEight == 1 && CurTime-(person.time[0]) < 0) {// can't place
      TimeIsEight = 0;
      return 0;
    }
    return 1;
  }else if(person.time[0] == 16) { // 16x1
    TimeIsSixteen++;                // count 16x1
    if(TimeIsSixteen == 1 && CurTime-(person.time[0]) >= 0) CurTime -= person.time[0]; // can place
    else if(TimeIsSixteen == frequency) TimeIsSixteen = 0;          // need to restart to count
    else if(TimeIsSixteen == 1 && CurTime-(person.time[0]) < 0) {// can't place
      TimeIsSixteen = 0;
      return 0;
    }
    return 1;
  }    
  return 0;
}

// place the item of each accepted user
struct Ans place(struct User person, struct Ans answer) {
  // different from last item!
  if(previous != person.time[0] && person.time[0] != 1) {
    if(PlaceOne != 0) {
      PlaceOne = 0;
      RecordTime += previous;
    }else if(PlaceTwo != 0) {
      PlaceTwo = 0;
      RecordTime += previous;
    }else if(PlaceFour != 0) {
      PlaceFour = 0;
      RecordTime += previous;
    }else if(PlaceEight != 0) {
      PlaceEight = 0;
      RecordTime += previous;
    }
  }

  // place
  if(person.time[0] == 1) {       // 16x1
    PlaceOne++;                   // the number of place increase
    answer.frequency = (PlaceOne-1) * person.frequency[0]; 
    answer.time = RecordTime;

    if(PlaceOne == OneNum) {      // place to the fullest frequency
      RecordTime += person.time[0];
      PlaceOne = 0;               // clean the counter to zero
    }
    return answer;
  }else if(person.time[0] == 2) { // 8x2
    PlaceTwo++;                   // the number of place increase
    answer.frequency = (PlaceTwo-1) * person.frequency[0]; 
    answer.time = RecordTime;

    if(PlaceTwo == TwoNum) {      // place to the fullest frequency
      RecordTime += person.time[0];
      PlaceTwo = 0;               // clean the counter to zero
    }
    return answer;
  }else if(person.time[0] == 4) { // 4x4
    PlaceFour++;                  // the number of place increase
    answer.frequency = (PlaceFour-1) * person.frequency[0];   
    answer.time = RecordTime;

    if(PlaceFour == FourNum) {    // place to the fullest frequency
      RecordTime += person.time[0];
      PlaceFour = 0;              // clean the counter to zero
    }
    return answer;
  }else if(person.time[0] == 8) { // 2x8
    PlaceEight++;                 // the number of place increase
    answer.frequency = (PlaceEight-1) * person.frequency[0]; 
    answer.time = RecordTime;   

    if(PlaceEight == EightNum) {  // place to the fullest frequency
      RecordTime += person.time[0];
      PlaceEight = 0;             // clean the counter to zero
    }
    return answer;
  }else if(person.time[0] == 16) { // 1x16
    PlaceSixteen++;                 // the number of place increase
    answer.frequency = (PlaceSixteen-1) * person.frequency[0]; 
    answer.time = RecordTime;   

    if(PlaceSixteen == frequency) {  // place to the fullest frequency
      RecordTime += person.time[0];
      PlaceSixteen = 0;             // clean the counter to zero
    }
    return answer;
  }
  return answer;  
}