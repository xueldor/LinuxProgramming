/*
 * 推荐用C标准库进行文件读写，尽量不要用系统的open/write方法,因为非常复杂有许多坑
 */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

void testReadOneByte(){
	FILE *stream;
	stream = fopen("/home/xue/test/testReadByte.txt", "r");//只读模式打开文件
	if (!stream){
        perror("file open");
        return;
	}
	int c = fgetc(stream);
	if(c == EOF){
        printf("End of file");
	}else{
		printf("c=%c\n",(char)c);
	}

	fclose(stream);
}
void testReadLine(){
	int fd;
	fd = open ("/home/xue/test/testReadByte.txt", O_RDONLY);
	FILE *stream;
	stream = fdopen(fd, "r");//通过文件描述符打开，模式必须与open传入的一致
	if (!stream){
        perror("file open");
        return;
	}
	const int len = 100;
	char str[len + 1];
	memset(str,0,len + 1);
	char* line;
	while(!feof(stream)){
		line = fgets(str,len,stream);//读一行(包括换行符)，并加\0
		if(line == NULL){
	        printf("read nothing");
		}else{
			printf("line=%s,line %p,str %p\n",line,(void*)line,(void*)str);
		}
	}

	fclose(stream);
}
int main_stdfileio(void) {
    testReadOneByte();
    testReadLine();
    return 0;
}
