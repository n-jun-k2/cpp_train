#include "../include/data.h"
#include "../include/data_wrapper.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

int main() {

  const int room_size = 3;
  const int room_object_size = 5;

  std::vector<int> ages(room_size * room_object_size);
  std::iota(std::begin(ages), std::end(ages), 10);

  wrap::ManagerWrapper manager;
  manager
  .roomInit(room_size)
  .objectsInit(room_object_size)
  .objectsUpdate([&](wrap::ObjectWrap&& item){
    static int idx = 0;
    item.age(ages[idx]).name("test user").sex(true);
    ++idx;
  })
  .flush();

  ManagerRoomView(&manager.manager);
  /*
    room[0] type:0
    room[0] object cout:5
    [test user,10] : [test user,11] : [test user,12] : [test user,13] : [test user,14] :
    room[1] type:0
    room[1] object cout:5
    [test user,15] : [test user,16] : [test user,17] : [test user,18] : [test user,19] :
    room[2] type:0
    room[2] object cout:5
    [test user,20] : [test user,21] : [test user,22] : [test user,23] : [test user,24] :
   */

  manager.flush(); std::cout << "-----" << std::endl;

  ManagerRoomView(&manager.manager);
  /*
    room[0] type:0
    room[0] object cout:5
    [test user,10] : [test user,11] : [test user,12] : [test user,13] : [test user,14] :
    room[1] type:0
    room[1] object cout:5
    [test user,15] : [test user,16] : [test user,17] : [test user,18] : [test user,19] :
    room[2] type:0
    room[2] object cout:5
    [test user,20] : [test user,21] : [test user,22] : [test user,23] : [test user,24] :
   */


  return 0;
}