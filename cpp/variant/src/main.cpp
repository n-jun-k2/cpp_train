#include <iostream>
#include <vector>
#include <variant>
#include <type_traits>

using namespace std;

struct Point2d {
  float x;
  float y;
};

struct Object2 {
    using pointer_type = vector<Point2d>*;
    using value_type = vector<Point2d>;
    variant<pointer_type, value_type> values;

    void set(vector<Point2d>&& rvalue) {
        values = rvalue;
    }

    void set(vector<Point2d>& lvalue) {
        values = &lvalue;
    }
};

int main(void){

    cout << "size :" << sizeof(Point2d) << ":" << sizeof(Object2)
          << ":" << sizeof(Object2::value_type) << ":" << sizeof(Object2::pointer_type) << std::endl;

    Object2 obj2;
    obj2.set({ Point2d{0, 1}, Point2d{1, 3} });

    auto data = get<vector<Point2d>>(obj2.values);
    for (auto &item : data) {
        cout << item.x << ":" << item.y << endl; // 0:1 1:3
    }

    cout << "-----" << endl;

    {
        auto local_data = vector<Point2d> { Point2d{0, 1}, Point2d{1, 2} };
        obj2.set(local_data);
        auto local = get<vector<Point2d>*>(obj2.values);
        for (auto &item : *local) {
            cout << item.x << ":" << item.y << endl; // 0:1 1:2
        }
    }

    cout << "------" << endl;

    auto non_data = get<vector<Point2d>*>(obj2.values);
    for (auto &item : *non_data) {
        cout << item.x << ":" << item.y << endl; //Are broken
    }

}
