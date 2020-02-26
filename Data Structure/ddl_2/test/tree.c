#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//用fgetc读取源文件，fputc解压文件


//考虑将频率表存在压缩文件的头部



//根据输入的文件，生成字符频率表

typedef struct ALPHA_FREQ{
    //字符频率表的表结点
    unsigned char alpha;
    int freq;
} ALPHA_FREQ;


ALPHA_FREQ *getAlphaFreq(char *sourceFileName, int *alphaVariety){
    int freq[256] = {0};
    int i;
    int index;
    ALPHA_FREQ *alphafreq = NULL;       //0号位置也用了
    FILE *fpIN;
    unsigned char ch;                   //返回的所读取字符的ASCII码值

    //二进制只读打开待压缩文件
    fpIN  = fopen(sourceFileName, "rb");

    //统计字符频度
    ch = fgetc(fpIN);
    while(!feof(fpIN)){
        freq[ch]++;
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
    return alphafreq;
}//getAlphaFreq



//根据字符频率表，生成Huffman树

typedef struct{
    //Huffman表的表结点，方便构建Huffman树，同时存储每个字符的编码
    int weight;
    int parent;
    int lchild;
    int rchild;
} HuffmanTab;

typedef char* HuffmanCode;

int HuffmanCoding(HuffmanTab *HT, ALPHA_FREQ *Tab, HuffmanCode *HC, int alphanum){
    //根据频率表Tab，构造Huffman树HT（这里其实称作表或树都可），并求n个字符的Huffman编码HC
    //HT和TAB和HC通过i相联系，0号位都用了，所以不需要错位
    int m;                  //表的总大小
    int i, j, k, t;         //计数参量
    int start, c, f;        //辅助求编码的参量
    char *cd;               //求编码的工作空间
    if(alphanum <= 1)
        return 0;           //是否必要？
    
    //初始化Huffman表
    m = 2 * alphanum - 1;
    HT = (HuffmanTab *)malloc(sizeof(HuffmanTab)*m);     //分配的空间都能用到，跟书上不同，所以注意判断条件的不同
    for (i = 0; i < alphanum; i++){
        //0~alphanum-1占用，存储叶子结点
        HT[i].weight = Tab[i].freq;
        HT[i].parent = -1;
        HT[i].lchild = -1;
        HT[i].rchild = -1;
    }
    for (; i < m; i++){
        HT[i].weight = 0;
        HT[i].parent = -1;
        HT[i].lchild = -1;
        HT[i].rchild = -1;
    }

    printf("total:%d\n", alphanum);
    //构建Huffman树，由构建过程可知，根存在末尾结点上
    for (i = alphanum; i < m; i++){
        //在HT[0~i-1]中选择parent为-1且weight最小和次小的两个结点，其序号分别为j、k
        t = 0;
        while(HT[t].parent >= 0 && t < i)
            t++;
        j = t;              //第一个没马结点
        t++;
        while(HT[t].parent >= 0 && t < i)
            t++;
        k = t;              //第二个没马结点
        for (; t < i; t++)
            if(HT[t].parent < 0){
                if (HT[t].weight < HT[j].weight){
                    k = j;
                    j = t;
                }
                else if (HT[t].weight < HT[k].weight)
                    k = t;
            }

        //构建新结点
        HT[j].parent = i;
        HT[k].parent = i;
        HT[i].lchild = j;
        HT[i].rchild = k;
        HT[i].weight = HT[j].weight + HT[k].weight;
    }
    //将Huffman表打印来验证
    for (i = 0; i < m; i++)
        printf("%d,%d,%d,%d\n", HT[i].weight, HT[i].parent, HT[i].lchild, HT[i].rchild);

    //从叶子到根，逆向求每个字符的Huffman编码
    HC = (HuffmanCode *)malloc(sizeof(HuffmanCode)*alphanum);      //0号元素也用
    cd = (char *)malloc(sizeof(char)*alphanum);                    //编码长度最大为alphanum-1，最后一位存放结束符
    cd[alphanum - 1] = '\0';
    for (i = 0; i < alphanum; i++){
        start = alphanum - 1;
        for (c = i, f = HT[i].parent; f != -1; c = f, f = HT[f].parent){
            //从叶子到根逆向求编码
            if(HT[f].lchild == c)       //上一步向左走
                cd[--start] = '0';
            else                        //上一步向右走
                cd[--start] = '1';
        }
        HC[i] = (HuffmanCode)malloc(sizeof(char) * (alphanum - start));
        strcpy(HC[i], &(cd[start]));    //从cd复制编码到HC，有问题
        printf("%d,%d:", i, Tab[i].freq);
        puts(HC[i]);                    //输出验证
    }
    free(cd);                           //释放工作空间
    return 1;
}//HuffmanCoding



int main(int argc, char *argv[]){
    //命令行参数按照：“模式 输入文件名 输出文件名”的格式输入，模式1代表压缩，2代表解压缩
    int alphanum;
    char *a,*b,*c;
    ALPHA_FREQ *Tab;            //字符频率表，存储范围为0~alphanum-1
    a = argv[1];                //模式
    b = argv[2];                //输入文件名
    c = argv[3];                //输出文件名
    HuffmanCode *HC = NULL;     //存储Huffman编码
    HuffmanTab  *HT = NULL;     //存储Huffman表

    if(a[0]=='1')
        printf("ya suo\n");
    else
        printf("jie ya suo\n");

    alphanum = 0;       //初始化
    Tab = getAlphaFreq(b, &alphanum);
    HuffmanCoding(HT, Tab, HC, alphanum);

    return 0;
}




//根据输入的文件，生成字符频率表
//12.16 17:28 实现二进制读写，将数据类型转为1字节数据块，读出后写入，证实无误。
//根据字符频率表，生成Huffman树
//12.17 13:30 完成Huffman编码
//将字符种类、字符频率写入压缩文件头
//将文件数据转码后写入压缩文件