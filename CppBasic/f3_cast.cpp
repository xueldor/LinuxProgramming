#include <iostream>
#include <vector>

using namespace std;

/*
 * C++中的四种类型转换
 */
namespace f3 {

    class A{
    public:
        int fa = 4;
        virtual ~A() = default;
    };
    class B:public A{
    public:
        int val = 9;
        ~B(){}
    };
    /*
     * static_cast大致相当于C语言的强转
     * 1. 基类和派生类之间指针或引用的转换
     * 2. 基本数据类型之间的转换，如把int转换成char，把int转换成enum。
     * 3. 把空指针转换成目标类型的空指针
     * 4. 把任何类型的表达式转换成void类型
     */
    void test_static_cast(){
        float f1 = 10.5f;
        int i1 = f1;//float直接赋给int，有的编译器会给出一个警告
        int i2 = (int)f1;//C语言风格的强转
        int i3 = static_cast<int>(f1);
        cout<<"i1="<<i1<<",i2="<<i2<<", i3="<<i3<<endl;

        {
            A *a = new B;
            B *b = (B *) a;//a 指向对象实际类型是B,因此强转为B指针类型是OK的
            B *b1 = static_cast<B *>(a);
            cout << b1 << "  " << b->val << endl;
            delete a;
        }

        {//错误示例(不安全的转换)：
            A *a = new A;
            B *b = (B *) a;//父类型的对象转成子类型，虽然编译器没有报错, 但它是不安全的，理论上是不允许的。导致程序运行不符合预期。
            B *b1 = static_cast<B *>(a);//因为B是子类，B的空间包含了A，new A只构造了空间A的部分，而强转成B*后，B比A多的空间实际不属于B，未经构造初始化，故val打印0
            cout << b1 << "  " << b->val << endl;
            delete a;
        }
    }
    /*
     * dynamic_cast只接受 "基于类对象的指针和引用" 的类转换
     * 和static_cast的区别是，会检查 downcast 是否安全
     *
     * 注意：class A 要有虚函数。这是由于运行时类型检查需要运行时类型信息，而这个信息存储在类的虚函数表中。
     * static_cast则没有这个限制
     */
    void test_dynamic_cast(){
        A *a = new B;
        B *b = dynamic_cast<B*>(a);
        cout<<"print b "<<b->val<<endl;
        delete b;

        A *a2 = new A;
        B* b2 = dynamic_cast<B*>(a2);
        if(b2){
            cout<<"print b2 "<<b2->val<<endl;
        } else{//new A 不能转成B，故走到else里
            cout<<"b2 is null "<<endl;
        }
        delete a2;
    }
    void test_dynamic_cast2(){
        B b;
        A &a = dynamic_cast<A&>(b);
        cout<<"print a.fa="<<a.fa<<endl;//OK

        A a2;
        try {//跟指针类型不同，引用类型因为不存在所谓空引用，故引用转换失败时，会抛出std::bad_cast异常
            B& b2 = dynamic_cast<B&>(a2);
        } catch (std::bad_cast bad) {
            cout<<bad.what()<<endl;
//            cerr<<bad.what()<<endl;
        }
    }
    /*
     * const_cast用来去掉对象上的const属性
     *
     */
    void test_const_cast(){
        //指针
        const int g = 20;
//wrong:        int* h = &g;
        int* h = const_cast<int*>(&g);//常量指针被转化成非常量的指针，并且仍然指向原来的对象
        *h = 30;
        cout<<h<<" ,"<<&g<<endl;//地址是一样的
        cout<<"g="<<g<<" , h="<<*h<<endl;//g++ 7.5编译器，输出g=20 , h=30,似乎有点奇怪，既然g和h是同一块内存，那为什么值不同？原因在于，const常量在编译期间就替换成字面量了。没人规定所有编译器都要这么做，故输出其实是未知的。
        //<----由上可见，const_cast不是这么用的。目的不是为了让你去修改一个本身被定义为const的值，如果非这么做，虽然编译没问题，但后果是无法预期的。

        //引用
        int& h2 = const_cast<int&>(g);
        h2 = 40;
        cout<<"g="<<g<<",h2="<<h2<<endl;//g=20,h2=40

        //s指向的hello是readonly，s指针本身不是
        const char *s = "hello";
        char*s2 = const_cast<char*>(s);
        try {
//            s2[1] = 'o';//不知道为什么执行这一步，程序直接挂了。异常也捕获不住。只好注释掉
        } catch (...) {
            //程序直接挂了,进一步证明了前面的观点，即const_cast不应该这么用
            cout<<"s2 "<<endl;
        }

        //sp本身是readonly，指向的目标不是
        char* const sp = "hello";//不能sp = "";
        char*sp2 = sp;//OK
        cout<<"sp2="<<sp2<<endl;

        //下面演示一个const_cast的正确用法场景
        void test_const_cast_well();//declare
        test_const_cast_well();

    }

    const string &shortString(const string& s1, const string& s2){
        return s1.size() <= s2.size() ? s1 : s2;
    }
    void test_const_cast_well(){
        string mystr1 = "test";
        string mystr2 = "testtest";
        //shortString返回一个const string &，shortString函数可能来自外部类库，也可能是我们自己封装的工具类，总之我们不能直接修改函数。
        //返回值实际上是我们传进来的，不是const。我们需要返回值也不是const。这种情况用const_cast是合理的。
        string &theShorter = const_cast<string&>(shortString(mystr1, mystr2));
        theShorter[1] = 'a';
        cout<<theShorter<<endl;
    }
    /*
     * reinterpret_cast为对象的位模式提供低层次上的重新解释，本质上依赖于机器和编译器，这也就表示它不可移植。
     * 使用reinterpret_cast是非常危险的，安全使用必须对涉及的类型和编译器实现转换过程都非常了解。
     * IBM的C++指南、C++之父Bjarne Stroustrup的FAQ网页和MSDN的Visual C++也都指出：错误的使用reinterpret_cast很容易导致程序的不安全，只有将转换后的类型值转换回到其原始类型，这样才是正确使用reinterpret_cast方式。
     *
     * 应用场景：哈希函数
     */
    void test_reinterpret_cast_well(){
        //1. 将一个地址转成long
        int i = 100;
        int *p = &i;
        long addr = reinterpret_cast<long>(p);
        //打印指针地址
        cout<<"p addr="<<p<<"  "<<addr<<endl;//print: p addr=0x7fff5cf99f94  140734753251220
        //<----0x7fff5cf99f94转换成10进制正是140734753251220
    }


    int main() {
        test_static_cast();
        test_dynamic_cast();
        test_dynamic_cast2();
        test_const_cast();
        test_reinterpret_cast_well();
        return 0;
    }
}