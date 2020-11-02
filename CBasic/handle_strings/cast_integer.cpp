#include <stdio.h>
#include <cstring>
#include <cstdlib>
/**
 * -----------
 * 总结:sprintf和sscanf两个方法足够
 * 基本类型转char*,用 sprintf
 * char*转基本类型用sscanf
 * 格式化输出用sprintf,还原格式化的字符串用sscanf
 * -----------
 */
/**
 * 整形转换为指定进制的字符串
 */
void int2String(){
    int size=123;
    char fileSizeStr[20] = {0};
    sprintf(fileSizeStr,"%d",size);//size按照10进制写入fileSizeStr
    puts(fileSizeStr);//123

    sprintf(fileSizeStr,"%x",size);//16进制
    puts(fileSizeStr);//7b

    sprintf(fileSizeStr,"%X",size);//16进制,大写
    puts(fileSizeStr);//7B
}

/**
 * atoi方法,在stdlib.h中
 */
void string2int(){
    char *num2 = "71";
    char fileSizeStr[20] = {0};
    sscanf(num2,"%02d",fileSizeStr);//'71'按照10进制理解,表示0x47,放到fileSizeStr里.
    puts(fileSizeStr);//输出:G,因为十进制的71对应ASCII的G
    sscanf(num2,"%02X",fileSizeStr);//'71'按照16进制理解,放到fileSizeStr里.
    puts(fileSizeStr);//输出:q,因为十六进制的71对应ASCII的q


    char *num = "45000";
    int n=atoi(num);//将'45000'按照10进制理解,转成int
    printf("%d\n",n);

    //如果45000表示的是16进制,翻译成int
    int data;
    sscanf(num,"%x",&data);
    printf("data=%d\n",data);//282624,45000从16进制转算成10进制是282624

    char *numf = "45.25";
    float f=atof(numf);
    printf("%f\n",f);
}


/**
 * 字符串转换为16进制保存到数组
 */
void charStr2HexStr() {
    char *src = "abcdefg";
    char fileSizeStr[20] = {0};
    for (int i = 0; i < strlen(src); i++) {
        //将src按照format的格式发送到字符串
        sprintf(fileSizeStr+strlen(fileSizeStr), "%02X:", (unsigned char)src[i]);//02表示对齐到两位,不足补0,X表示大写的16进制
    }
    puts(fileSizeStr);//61:62:63:64:65:66:67:
}

/**
 * 16进制的字符串转换为原来形式
 */
void hexStr2charStr() {
    char *src = "616263";
    char fileSizeStr[20] = {0};
    for (int i = 0; i < strlen(src)/2; i++) {
        //与sprintf相反,从字符串读取格式化输入
        sscanf(src + i*2,"%02X",fileSizeStr + i);
    }
    puts(fileSizeStr);//abc
}