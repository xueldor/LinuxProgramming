#include <iostream>
using namespace std;

//自动推导类型
namespace f1 {
    long add(int a, int b) {
        return a + b;
    }

    int main() {
        //编译器自动推导出sum的类型是long
        auto sum = add(2, 4);

        cout << sizeof sum << ", int size=" << sizeof(int) << endl;
        return 0;
    }
}
/*
 * 输出： 8, int size=4
 * 表明sum是long类型，且当前机器上，long占8个字节，int占4字节
 */