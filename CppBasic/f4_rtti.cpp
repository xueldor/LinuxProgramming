#include <iostream>
#include <vector>

using namespace std;

/*
 * C++中的运行时类型识别
 */
namespace f4 {

    class MyJob{

    };
    void test_typeid(){
        cout<<"typeid(int) "<<typeid(int).name()<<endl;

        int a = 5;
        const type_info& typeI = typeid(a);
        cout<<"int type is " << typeI.name()<<endl;//int type is i
        const type_info& type = typeid(a + 1L);
        cout<<"long type name is " <<type.name()<<endl;//long type name is l
        //<-----表明我的系统里面，int类型，type_info.name()是i，long类型是l。

        MyJob job;
        cout<<"typeid(MyJob) " <<typeid(MyJob).name()<<endl;//typeid(MyJob) N2f45MyJobE。鬼知道怎么生成的。
        cout<<"job type name is " <<typeid(job).name()<<endl;//job type name is N2f45MyJobE
        MyJob* jobPtr = &job;
        cout<<"job type name is " <<typeid(jobPtr).name()<<endl;//job type name is PN2f45MyJobE
        MyJob& jobRef = job;
        cout<<"job type name is " <<typeid(jobRef).name()<<endl;//job type name is N2f45MyJobE,引用即别名，故和typeid(job).name()一样

        int b = 9;
        if(typeid(a) == typeid(b)){
            cout<<"a and b are the same type"<<endl;
        } else{
            cout<<"a and b are not the same type"<<endl;
        }
        long c = 2;
        if(typeid(a) == typeid(c)){
            cout<<"a and c are the same type"<<endl;
        } else{
            cout<<"a and c are not the same type"<<endl;
        }
    }

    int main() {
        test_typeid();
        return 0;
    }
}