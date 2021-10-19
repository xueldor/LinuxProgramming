#include <iostream>
#include <memory>//包含头文件
#include <vector>

using namespace std;

/*
 * 智能指针
 * C++ 11: 包含在头文件<memory>中
 * 有三个智能指针，分别为shared_ptr,unique_ptr,weak_ptr
 *
 * unique_ptr 独占指针对象，并保证指针所指对象生命周期与其一致
 * shared_ptr 可共享指针对象，可以赋值给shared_ptr或weak_ptr。指针指向的对象，在所有的相关联的shared_ptr生命周期结束时结束，是强引用。
 * weak_ptr 它不能决定所指对象的生命周期，引用所指对象时，需要lock()成shared_ptr才能使用。
 */
namespace f2 {

    void testUniquePtr(){
        {
            //错误的使用unique_ptr案例
            int *pInt = new int(10);
            unique_ptr<int> uptr(pInt);//unique_ptr独占指针对象,所以下面uptr2也指向pInt是错误的。
            unique_ptr<int> uptr2(pInt);
            cout << *uptr << "   " << *uptr2 << endl;
//            unique_ptr<int> uptr3 = uptr;//unique_ptr是禁止赋值的，始终保持一个 unique_ptr 管理一个对象

            //语句块执行完，uptr和uptr2的生命周期结束，作为unique_ptr，自动释放指向的动态对象。uptr和uptr2在同一个内存上释放两次，因此报错：free(): double free detected in tcache 2
        }
        //前面语句块末尾崩溃，不会执行下面的打印。
        cout<<"will not print"<<endl;


        //正确：
        int *pInt = new int(10);
        unique_ptr<int> uptr(pInt);//unique_ptr独占指针对象,所以下面uptr2也指向pInt是错误的。
        unique_ptr<int> uptr2(pInt);
        cout << *uptr << "   " << *uptr2 << endl;
        uptr.release();//通过release方法释放所有权，这样就只有uptr2一个独占指针，程序正常
    }

    void testUniquePtr2(){
        //为了避免testUniquePtr那样，我们总是直接把new语句写在unique_ptr初始化里面
        unique_ptr<int> uptr(new int(11));
        cout << *uptr << endl;
        unique_ptr<int> uptr2 = move(uptr);//对象所有权通过move函数转移给uptr2
//        cout << *uptr<<endl;//uptr不能用了。直接崩溃退出

        cout<<"uptr2 "<<*uptr2 << endl;
        uptr2.reset();//unique_ptr 提前释放指针
//        cout<<"不能用啦 "<<*uptr2 << endl;
    }

    void testSharedPtr(){
        string* str = new string("abcd");
        weak_ptr<string> w1;
        {
            shared_ptr<string> s1(str);
            //引用计数，有几个shared_ptr引用对象。“string* str”也引用对象，但这就不可能被shared_ptr知道了。
            // 因此跟unique_ptr一样，因尽量把new语句写在shared_ptr的初始化里：shared_ptr<string> s1(new string("abcd"));
            cout<<"use count " <<s1.use_count()<<" ,str="<<*s1<<endl;
            w1 = s1;//weak_ptr，是配合shared_ptr而引入的的智能指针
            {
                shared_ptr<string> s2 = s1;
                {
                    shared_ptr<string> s3 = s2;
                    cout<<"use count " << s1.use_count()<<"  "<<s2.use_count()<<"  "<<s3.use_count()<<endl;

                    unique_ptr<string> uptr(str);
                    //unique_ptr引用了对象，但shared_ptr不知道，计数不增加。这里打印跟上面一行相同
                    cout<<"unique_ptr use count " << w1.use_count()<<endl;
                    uptr.release();
                }
                cout<<"use count " << s1.use_count()<<"  "<<s2.use_count()<<" weak:" << w1.use_count()<<endl;
            }
            //weak_ptr的use_count函数，也是返回有几个shared_ptr引用。因为weak_ptr作为弱引用，不参与引用计数
            cout<<"use count  weak:" << w1.use_count()<<endl;
        }
        cout<<"use count  weak:" << w1.use_count()<<endl;//0个引用

        cout<<"expired: "<<w1.expired()<<endl;//引用计数为0，相当于被释放，返回1即true

        //使用weak_ptr时，应先将他转成强指针。否则指向的对象随时可能被释放
        shared_ptr s_ptr2 = w1.lock();//由于对象已经释放，故lock返回NULL
        cout<<"s_ptr2 " <<(s_ptr2==NULL)<<endl;
    }
    void testSharedPtr2(){
        shared_ptr<string> s0;
        cout<<s0.use_count()<<endl;
        cout<<"s0 is NULL: "<<(s0==NULL?"true":"false")<<endl;
        cout<<"s0 is NULL: "<<(s0?"false":"true")<<endl;//same
        if(s0)cout<<"NOT NULL";else cout<<"NULL"<<endl;
        //<----s0对象本身不是动态分配的，当然不是null,否则上一行如何能调用use_count函数？显然这里s0==NULL,实际是s0这个智能指针引用的目标对象是NULL。
        //下面可以看到，我们可以直接调用模板类型的方法

        shared_ptr<string> s1(new string("abcdefg"));
        //智能指针使用方法跟正常指针一样,用*号解引用
        cout<<"s1="<<*s1<<endl;
        //调用的是string的empty方法
        cout<<"s1 is empty?"<<s1->empty()<<endl;

        shared_ptr<string> s2(s1);
        shared_ptr<string> s3=s1;
        s1.reset();//解除s1对对象的引用
// wrong:       cout<<"s1="<<*s1<<endl;
        cout<<"s2="<<*s2<<" ,s3=" <<*s3<<endl;//OK

        string* strPtr = s2.get();//返回shared_ptr里面保存的指针
        cout<<"strPtr="<<*strPtr<<endl;

        //最安全的使用方式是使用make_shared标准库函数
        shared_ptr<int> i0 = make_shared<int>(3);
        cout<<"is unique "<<i0.unique()<<endl;//当use_count==1时返回true

        //用auto，代码简洁
        auto p1 = make_shared<vector<string>>();
    }

    int main() {
// 异常演示       testUniquePtr();
        testUniquePtr2();
        testSharedPtr();
        testSharedPtr2();
        return 0;
    }
}