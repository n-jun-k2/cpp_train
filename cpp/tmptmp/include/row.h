#pragma once

struct CLASS_A{
  int x;
  int y;
  int z;
};

struct CLASS_B{
  float r;
  float g;
  float b;
};

struct CLASS_C{
  int type;
  float priority;
  unsigned int a_count;
  CLASS_A* p_a;
  unsigned int b_count;
  CLASS_B* p_b;
};