### 演示C++语法
1. C++ 14 和 C++ 17 中新增的一些特性。
2. 过去不太熟悉的语法

每个cpp演示一些例子，每个cpp文件包含一个main方法。而一个项目只能有一个main函数，所以执行其中一个例子的时候，需要把其它文件里的main函数全部注释掉。为了方便，我在此项目采用了非常规的方法，正式的项目不应该这么组织。

方法是，main.cpp之外的每个cpp文件，加上namespace，空间名可以是文件名，以保证独一无二。然后，main.cpp文件引入#include "xxx.cpp",通过namespace::main()调用xxx.cpp的main方法。

因为被include的文件中只应该包含声明，而这里把cpp引入进来，因此会导致cpp中的代码重复定义。因此，CMakeLists.txt中，add_executable(CppBasic main.cpp)只add一个main.cpp，其它cpp文件移除掉。

