#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//用fgetc读取源文件，fputc解压文件

typedef struct ALPHA_FREQ{
    unsigned char alpha;
    int freq;
} ALPHA_FREQ;

ALPHA_FREQ *getAlphaFreq(char *sourceFileName, char *out, int *alphaVariety){
    int freq[256] = {0};
    int i;
    int index;
    ALPHA_FREQ *alphafreq = NULL;
    FILE *fpIN, *fpOUT;
    int ch;             //返回的所读取字符的ASCII码值

    //二进制只读打开待压缩文件
    fpIN  = fopen(sourceFileName, "rb");
    fpOUT = fopen(out, "wb+");

    //统计字符频度
    ch = fgetc(fpIN);
    while(!feof(fpIN)){
        freq[ch]++;
        fputc(ch, fpOUT);
        ch = fgetc(fpIN);       //读入下一个
    }
    fclose(fpIN);

    //统计字符的种类，alphaVariety需要提前初始化为0
    for (i = 0; i < 256; i++){
        if(freq[i]){
            (*alphaVariety)++;
        }
    }

    alphafreq = (ALPHA_FREQ *)calloc(sizeof(ALPHA_FREQ), *alphaVariety);
    for (i = index = 0; i < 256; i++){
        if(freq[i]){
            alphafreq[index].alpha = i;
            alphafreq[index].freq = freq[i];
            index++;
        }
    }

    fclose(fpIN);
    fclose(fpOUT);
    return alphafreq;
}

int main(int argc, char *argv[]){
    //命令行参数按照：“模式 输入文件名 输出文件名”的格式输入，模式1代表压缩，2代表解压缩
    int alphanum;
    int i;
    char *a,*b,*c;
    ALPHA_FREQ *Tab;    //字符频率表
    a = argv[1];        //模式
    b = argv[2];        //输入文件名
    c = argv[3];        //输出文件名

    if(a[0]=='1')
        printf("ya suo\n");
    else
        printf("jie ya suo\n");

    alphanum = 0;       //初始化
    Tab = getAlphaFreq(b, c, &alphanum);
    for (i = 0; i < alphanum; i++)
        printf("%c,%d\n", Tab[i].alpha, Tab[i].freq);

    return 0;
}

//12.16 17:28 实现二进制读写，将数据类型转为1字节数据块，读出后写入，证实无误。