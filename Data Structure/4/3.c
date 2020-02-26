#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define STACKINITSIZE 100
#define STACKINCREMENT 10
#define MAXQSIZE 100
#define infinity 100000
#define max_vertex_num 20
typedef enum{
    DG,         //有向图
    DN,         //有向网
    UDG,        //无向图
    UDN         //无向网
} graphkind;
int visited[max_vertex_num];


/*************栈的相关操作*************/
typedef struct{
	int *base;
	int *top;
	int stacksize;
}sqstack;
int initstack(sqstack *S){
	//创建空栈 
	(*S).base=(int*)malloc(STACKINCREMENT*sizeof(int));
	if(!(*S).base) exit(0);
	(*S).top=(*S).base;
	(*S).stacksize=STACKINCREMENT;
	return 1;
}//initstack
int push(sqstack *S, int e){
	//将e入栈 
	if((*S).top-(*S).base>=(*S).stacksize){
		(*S).base=(int*)realloc((*S).base,((*S).stacksize+STACKINCREMENT)*sizeof(int));
		if(!(*S).base) exit(0);
		(*S).top=(*S).base+(*S).stacksize;
		(*S).stacksize+=STACKINCREMENT;
	}
	*(*S).top++=e;
	return 1;
}//push
int pop(sqstack *S, int *e){
	//出栈 
	if((*S).top==(*S).base) return 0;
	*e=*(*S).top--;
	return 1;
}//pop
int gettop(sqstack *S, int *e){
	//取栈顶元素
	if((*S).top==(*S).base) return 0;
	*e=*((*S).top-1);
	return 1; 
}//gettop
int stackempty(sqstack *S){
    //判断是否为空栈
    if((*S).top==(*S).base) 
        return 1;
    else
        return 0;
}


/************队列的相关操作***********/
typedef struct{
	int *base;
	int front;
	int rear;
}sqqueue;
int initqueue(sqqueue *Q){						
    //创建空队列 
	(*Q).base=(int*)malloc(MAXQSIZE*sizeof(char));
	if(!(*Q).base) exit(0);
	(*Q).front=(*Q).rear=0;
	return 1;
}
void enqueue(sqqueue *Q, int e){
    //入队列
    //保证不越界，每轮使用都初始化首尾
    (*Q).base[(*Q).rear++] = e;
}
void dequeue(sqqueue *Q, int *e){
    //出队列
    (*e) = (*Q).base[(*Q).front++];
}


/**********数组表示法存储无向网**********/
typedef struct arccell{
    //边
    //由于不需要每边的相关信息，此处未定义
    int         adj;                                //权值，infinity代表不相邻
} arccell, adjmatrix[max_vertex_num][max_vertex_num];
typedef struct {
    char            vexs[max_vertex_num];
    adjmatrix       arcs;
    int             vexnum, arcnum;                 //图的当前顶点数和弧数
    graphkind       kind;                           //图的种类
} MGraph;


/************创建算法************/
void CreatMGraph(MGraph *G){
    //通过直接读入点集与邻接矩阵来构建无向网
    int i, j, k, n;
    (*G).kind = UDN;
    printf("enter vexnum:");                        //顶点数
    scanf("%d", &((*G).vexnum));
    printf("enter arcnum:");                        //边数
    scanf("%d", &((*G).arcnum));
    printf("enter vexs:\n");                        //顶点信息:‘顶点1’‘ ’‘顶点二’‘ ’‘……’
    fflush(stdin);
    for (i = 0; i < (*G).vexnum; i++){
        (*G).vexs[i] = getchar();
        getchar();
    }
    for (i = 0; i < (*G).vexnum; i++)
        for (j = 0; j < (*G).vexnum; j++)
            (*G).arcs[i][j].adj = infinity;
    for(i = 0; i < (*G).arcnum; i++){
        printf("enter arc %d:",i);                 //边信息逐边输入:端点、端点、权值
        scanf("%d%d%d", &j, &k, &n);
        (*G).arcs[j][k].adj = n;
        (*G).arcs[k][j].adj = n;
    }
}

int final[max_vertex_num];
void ShortestPath_DIJ(MGraph G, int v0, char p[][max_vertex_num], int *D){
    //p存储各个最短路径中的顶点信息，D存储当前最短路径的带权长度
    //final[v]为1当且仅当v在S中
    int i, v, w, min, l;
    for (v = 0;v < G.vexnum; v++){
        final[v] = 0;
        D[v] = G.arcs[v0][v].adj;
        if(D[v]<infinity){
            p[v][0] = G.vexs[v0];
            p[v][1] = G.vexs[v];
            p[v][2] = '\0';
        }
    }
    D[v0] = 0;
    p[v0][0] = '\0';
    final[v0] = 1;
    for (i = 1; i < G.vexnum; i++){
        min = infinity;
        for (w = 0; w < G.vexnum; w++)
            if((!final[w])&&(D[w]<min)){
                v = w;
                min = D[w];
            }
        final[v] = 1;
        for (w = 0;w < G.vexnum; w++)
            if(!final[w]&&(min+G.arcs[v][w].adj<D[w])){
                D[w] = min + G.arcs[v][w].adj;
                strcpy(p[w], p[v]);
                l = strlen(p[w]);
                p[w][l] = G.vexs[w];
                p[w][l + 1] = '\0';
            }
    }
}



int main(){
    int i,j,v0;
    int D[max_vertex_num];
    char p[max_vertex_num][max_vertex_num];
    MGraph G;
    CreatMGraph(&G);
    for(i=0; i < G.vexnum; i++)
        for(j = 0; j < G.vexnum; j++)
            if(G.arcs[i][j].adj<infinity)
                printf("%c,%c,%d\n", G.vexs[i], G.vexs[j], G.arcs[i][j].adj);        //验证
    printf("enter v0:");
    scanf("%d", &v0);
    ShortestPath_DIJ(G, v0, p, D);
    for (i = 0; i < G.vexnum; i++){
        printf("%c->%c", G.vexs[v0], G.vexs[i]);
        printf(" length:%d path:", D[i]);
        puts(p[i]);
    }
}



