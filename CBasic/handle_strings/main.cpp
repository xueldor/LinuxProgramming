#include <iostream>
#include "cstring"

using namespace std;

void testSomeStringFun(){
    int arr1[50];
    memset(arr1,0,sizeof(arr1));//把一块内存区清零
    cout<<sizeof(arr1)<<"   "<<arr1[49]<<endl;
    memset(arr1,'a',sizeof(arr1));//把一块内存区重置为'a'
    cout<<"arr1[49]="<<arr1[49]<<endl;//int是4字节,每个字节都是'a'=0x61,所以是0x61616161=1,633,771,873

    char* str1 = "123456";
    std::size_t len = strlen(str1);//从str的开头地址到\0为止,不包括\0
    cout<<"str1="<<len<<endl;//6

    char src[20] = "hello world\n";
    char dest[20];
    memcpy(dest, src, 13);//参数的指针类型是void *而非char *,一定会拷贝完n个字节,而不是遇到'\0'就结束
    //src和dest所指的内存区间不能有重叠,否则无法保证正确拷贝
    printf(dest);

    char buf[20] = "hello world\n";
    memmove(buf + 2, buf, 13);//内存区间可以有重叠
    printf(buf);//hehello world

    char buf2[20] = "hello world\n";
    char* newBuf = strdup(buf2);//strdup调用malloc动态分配内存,因此必须free释放
    printf(newBuf);
    free(newBuf);//remember

    char d[10] = "foo";//调用者必须确保dest缓冲区足够大
    char s[10] = "bar";
    strcat(d, s);
    printf("%s %s\n", d, s);

    strncat(d, s,1);//如果src中前n个字符没有出现'\0',strncat方法会自动添加
    printf("%s %s\n", d, s);
}

/**
 * 三个方法:
 * #include <string.h>
 * int memcmp(const void *s1, const void *s2, size_t n);
 * int strcmp(const char *s1, const char *s2);
 * int strncmp(const char *s1, const char *s2, size_t n);
 */
void testCompareStringFun(){
    printf("%d\n",strcmp("abc","abd"));//-1
    printf("%d\n",strcmp("abd","abc"));//1
    printf("%d\n",strcmp("abc","abc"));//0

    int a=-1,b=1;
    printf("%d\n",memcmp(&a,&b,4));//虽然a<b,但是这里是按逐字节比较的.转成二进制按字节拆开,a是大于b的.
}
/**
 * char *strchr(const char *s, int c)
 */
void testSearchStringFun(){
    printf("%s\n",strchr("abcdecfg",'c'));//cdecfg.从左往右搜索c的位置返回指向该位置的指针

    printf("%s\n",strrchr("abcdecfg",'c'));//output:cfg .从右往左
}

/**
 * 分割字符串
 * 注意该方法会修改src
 *
 * 由于strtok函数里面使用了静态变量,因此是不可重入的.即不能多个任务调用同一个函数.尽量避免使用
 */
void testStrtok(){
    char str[] = "root:x::0:root:/root:/bin/bash:";
    char *token;
    printf("str=%s\n", str);

    //我们会发现str数组会不断被修改.分割符被替换为\0.
    token = strtok(str, ":");
    printf("%s\n", token);
    while ( (token = strtok(NULL, ":")) != NULL) {
        printf("%s\n", token);
        printf("str=%s\n", str);
    }
}

void int2String();
void charStr2HexStr();
void string2int();
void hexStr2charStr();
int main() {
    testSomeStringFun();
    testCompareStringFun();
    testSearchStringFun();
    testStrtok();
    int2String();
    charStr2HexStr();
    string2int();
    hexStr2charStr();
    return 0;
}
