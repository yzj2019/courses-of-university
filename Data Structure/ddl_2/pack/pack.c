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


ALPHA_FREQ *getAlphaFreq(char *infilename, int *alphaVariety){
    int freq[256] = {0};
    int i;
    int index;
    ALPHA_FREQ *alphafreq = NULL;       //0号位置也用了
    FILE *fpIN;
    unsigned char ch;                   //返回的所读取字符的ASCII码值

    //二进制只读打开待压缩文件
    fpIN  = fopen(infilename, "rb");

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

int HuffmanCoding(HuffmanTab **HT, ALPHA_FREQ *Tab, HuffmanCode **HC, int alphanum){
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
    (*HT) = (HuffmanTab *)malloc(sizeof(HuffmanTab)*m);     //分配的空间都能用到，跟书上不同，所以注意判断条件的不同
    for (i = 0; i < alphanum; i++){
        //0~alphanum-1占用，存储叶子结点
        (*HT)[i].weight = Tab[i].freq;
        (*HT)[i].parent = -1;
        (*HT)[i].lchild = -1;
        (*HT)[i].rchild = -1;
    }
    for (; i < m; i++){
        (*HT)[i].weight = 0;
        (*HT)[i].parent = -1;
        (*HT)[i].lchild = -1;
        (*HT)[i].rchild = -1;
    }

    printf("total:%d\n", alphanum);
    //构建Huffman树，由构建过程可知，根存在末尾结点上
    for (i = alphanum; i < m; i++){
        //在HT[0~i-1]中选择parent为-1且weight最小和次小的两个结点，其序号分别为j、k
        t = 0;
        while((*HT)[t].parent >= 0 && t < i)
            t++;
        j = t;              //第一个没马结点
        t++;
        while((*HT)[t].parent >= 0 && t < i)
            t++;
        k = t;              //第二个没马结点
        for (; t < i; t++)
            if((*HT)[t].parent < 0){
                if ((*HT)[t].weight < (*HT)[j].weight){
                    k = j;
                    j = t;
                }
                else if ((*HT)[t].weight < (*HT)[k].weight)
                    k = t;
            }

        //构建新结点
        (*HT)[j].parent = i;
        (*HT)[k].parent = i;
        (*HT)[i].lchild = j;
        (*HT)[i].rchild = k;
        (*HT)[i].weight = (*HT)[j].weight + (*HT)[k].weight;
    }
    //将Huffman表打印来验证
    for (i = 0; i < m; i++)
        printf("%d,%d,%d,%d\n", (*HT)[i].weight, (*HT)[i].parent, (*HT)[i].lchild, (*HT)[i].rchild);

    //从叶子到根，逆向求每个字符的Huffman编码
    (*HC) = (HuffmanCode *)malloc(sizeof(HuffmanCode)*alphanum);      //0号元素也用
    cd = (char *)malloc(sizeof(char)*alphanum);                    //编码长度最大为alphanum-1，最后一位存放结束符
    cd[alphanum - 1] = '\0';
    for (i = 0; i < alphanum; i++){
        start = alphanum - 1;
        for (c = i, f = (*HT)[i].parent; f != -1; c = f, f = (*HT)[f].parent){
            //从叶子到根逆向求编码
            if((*HT)[f].lchild == c)       //上一步向左走
                cd[--start] = '0';
            else                        //上一步向右走
                cd[--start] = '1';
        }
        (*HC)[i] = (HuffmanCode)malloc(sizeof(char) * (alphanum - start));
        strcpy((*HC)[i], &(cd[start]));    //从cd复制编码到HC，有问题
        printf("%d,%d:", i, Tab[i].freq);
        puts((*HC)[i]);                    //输出验证
    }
    free(cd);                           //释放工作空间
    return 1;
}//HuffmanCoding


int pack(char *infilename, char *outfilename, ALPHA_FREQ *Tab, int alphanum, HuffmanCode *HC){
    //将字符种类、字符频率表写入压缩文件头，然后将文件数据转码后写入压缩文件
    int i;                                  //拼凑位计数
    int j, k;                               //辅助变量
    unsigned char ch_in;                    //返回所读取字符的ASCII码值
    unsigned char ch_out;                   //拼凑的待写入字节
    FILE *fpIN, *fpOUT;
    fpIN  = fopen(infilename, "rb");
    fpOUT = fopen(outfilename, "wb+");      //新建压缩文件
    fwrite(&alphanum, sizeof(int), 1, fpOUT);           //将总字符种类写入文件头
    fwrite(Tab, sizeof(ALPHA_FREQ), alphanum, fpOUT);   //继续向后写入字符频率表

    i = 0;
    ch_out = 0x00;
    ch_in = fgetc(fpIN);
    while(!feof(fpIN)){
        //怎么拼？编码从头到尾对应数据位的从高到低的循环，对应到文件中就是从左到右的二进制编码，每存入8bit写一次
        //读取的时候整块读出，每次根据最高位来寻址，每8bit读一次，找到则输出
        for (j = 0; j < alphanum; j++)
            if(Tab[j].alpha == ch_in)
                break;
        //1
        //printf("wow:%d\n",j);
        for (k = 0; HC[j][k] != '\0'; k++){
            //写入当前字符的Huffman编码
            if(HC[j][k] == '1')
                //待输入位(即当前最低位)置1，否则直接为0不需赋
                ch_out |= 0x01;
            if(i == 7){
                //当前待写入字节已满，则写入并置空
                fputc(ch_out, fpOUT);
                //printf("0x%x\n", ch_out);
                ch_out = 0x00;
                i = 0;
            }
            else{
                //未满，可填入下一位，将最末尾再次变为空位置。
                i++;
                ch_out = (ch_out << 1);             //解压缩读出的时候也左移
            }
        }
        ch_in = fgetc(fpIN); //读入下一个
    }
    if(i!=0){
        //最后没填满，则其余位置0，组成一块写入
        ch_out = (ch_out << (7 - i));
        fputc(ch_out, fpOUT);
        //printf("0x%x\n", ch_out);
    }

    fclose(fpIN);
    return 1;
}


int main(int argc, char *argv[]){
    //命令行参数按照：“模式 输入文件名 输出文件名”的格式输入，模式1代表压缩，2代表解压缩
    int alphanum = 0;
    int tag;
    char *a,*b,*c;
    ALPHA_FREQ *Tab = NULL;     //字符频率表，存储范围为0~alphanum-1
    a = argv[1];                //模式
    b = argv[2];                //输入文件名
    c = argv[3];                //输出文件名
    HuffmanCode *HC = NULL;     //存储Huffman编码
    HuffmanTab  *HT = NULL;     //存储Huffman表

    //从待处理文件中得到字符频率表
    if(a[0]=='1'){
        printf("ya suo\n");
        alphanum = 0;           //初始化
        Tab = getAlphaFreq(b, &alphanum);
    }   
    else{
        printf("jie ya suo\n");
        //gethead();
    }

    tag = HuffmanCoding(&HT, Tab, &HC, alphanum);
    if(!tag){
        //文件为空
        printf("File Empty\n");
        return 0;
    }

    if(HC==NULL)
        printf("ERROR\n");              //为什么仍然是ERROR？

    if (a[0] == '1'){
        pack(b, c, Tab, alphanum, HC);  //有问题，没生成任何
    }
    else{

    }

    return 0;
}




//根据输入的文件，生成字符频率表
//12.16 17:28 实现二进制读写，将数据类型转为1字节数据块，读出后写入，证实无误。
//根据字符频率表，生成Huffman树
//12.17 13:30 完成Huffman编码
//将字符种类、字符频率写入压缩文件头
//将文件数据转码后写入压缩文件
//12.18 11:06 成功实现大小合适的压缩，下一步事实解压验证是否正确