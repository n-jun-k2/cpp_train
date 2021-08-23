#pragma once
#include <iostream>
#include <string>

struct Object {
  std::string name;
  unsigned int age;
  bool sex;
};

struct Room {
  int number;
  unsigned long objectCount;
  Object* pObjectList;
};

struct Service {
  int type;
  unsigned long roomNumber;
};

struct Manager {
  unsigned long roomCount;
  Room* pRoomList;
  unsigned long serviceCount;
  Service* pServiceList;
};

bool ManagerRoomView(Manager* pManager) {

  if (pManager->roomCount <= 0 || pManager->pRoomList == nullptr) return false;

  Room* pRoom = nullptr;
  Object* pObject = nullptr;

  for (auto room_id = 0; room_id < pManager->roomCount; ++room_id) {
    pRoom = &pManager->pRoomList[room_id];
    std::cout << "room[" << room_id << "] type:" << pRoom->number << std::endl;
    std::cout << "room[" << room_id << "] object cout:" << pRoom->objectCount << std::endl;
    for(auto object_id = 0; object_id < pRoom->objectCount; ++object_id) {
      std::cout <<"["<<pRoom->pObjectList[object_id].name << "," << pRoom->pObjectList[object_id].age <<"] : " ;
    }
    std::cout << std::endl;
  }

  return true;
}

bool ManagerServiceView(Manager* pManager) {

  if (pManager->serviceCount <= 0 || pManager->pServiceList == nullptr) return false;

  for(auto sID = 0; sID < pManager->serviceCount; ++sID) {
    std::cout << pManager->pServiceList[sID].type << ":" << pManager->pServiceList[sID].roomNumber << std::endl;
  }
  return true;
}
