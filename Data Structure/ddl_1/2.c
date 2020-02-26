//选择做题目2.6
//2019.12.4 23:07

#include<stdio.h>
#include<stdlib.h>


/************线性链表的相关操作***********/

typedef struct{
    int OccurTime;      //事件发生时刻
    int NType;          //事件类型，0表示到达事件，1、2表示从1、2号队列离开
} Event, ElemType;      //事件类型，有序链表LinkList的数据元素类型
typedef struct LNode{
    ElemType data;
    struct LNode *next;
} LNode, *LinkList;
typedef LinkList EventList;
int cmp(Event a, Event b){
    //依事件a的发生时刻<、=或>事件b的发生时刻分别返回-1、0、1
    if(a.OccurTime < b.OccurTime)
        return -1;
    else if(a.OccurTime == b.OccurTime)
        return 0;
    else
        return 1;
}//cmp
int ListInsert(LinkList *L, ElemType e){
    //在带头结点的单链线性表中按时间顺序插入元素e
    LinkList p, s;
    p = *L;
    while(p->next && cmp(e, (p->next)->data))
        //寻找待插入位置,循环结束后插到p的后面
        p = p->next;
    s = (LinkList)malloc(sizeof(LNode));
    s->data = e;
    s->next = p->next;
    p->next = s;
    return 1;
}//ListInsert
int ListDelete(LinkList *L, int i, ElemType *e){
    //在带头结点的单链线性表L中，删除第i个元素，并由e返回其值
    LinkList p,q;
    int j = 0;
    p = *L;
    while(p->next && j<i-1){
        //寻找第i个结点，并令p指向其前驱
        p = p->next;
        ++j;
    }
    if(!(p->next) || j>i-1)
        return 0;       //删除位置不合理
    q = p->next;
    p->next = q->next;
    *e = q->data;
    free(q);
    return 1;
}//ListDelete
void CreatList(LinkList *L){
    //建立带表头结点的单链线性空表L
    *L = (LinkList)malloc(sizeof(LNode));
    (*L)->next = NULL;
}//CreatList
int ListEmpty(LinkList L){
    //检查是否为空表
    if(!(L->next))
        return 1;
    else
        return 0;
}//ListEmpty


/************队列的相关操作***********/

typedef struct{
    int ArrivalTime;    //到达时刻
    int Duration;       //办理事务所需时间，check不计时间
    int Money;          //此顾客待操作的钱数，若存钱则>0，取钱则<0。
} QElemType;            //队列的数据元素类型
typedef struct QNode{
    QElemType data;
    struct QNode *next;
} QNode, *QueuePtr;
typedef struct{
    QueuePtr front;     //队头指针
    QueuePtr rear;      //队尾指针
} LinkQueue;
int InitQueue(LinkQueue *Q){
    //构造带头结点的空队列Q
    Q->front = Q->rear = (QueuePtr)malloc(sizeof(QNode));
    if(!(Q->front))
        exit(0);
    (Q->front)->next = NULL;
    return 1;
}//InitQueue
int EnQueue(LinkQueue *Q, QElemType e){
    //插入元素e为Q的新的队尾元素
    QueuePtr p;
    p = (QueuePtr)malloc(sizeof(QNode));
    if(!p)
        exit(0);
    p->data = e;
    p->next = NULL;
    (Q->rear)->next = p;
    Q->rear = p;
    return 1;
}//EnQueue
int DeQueue(LinkQueue *Q, QElemType *e){
    //若队列不空，则删除Q的队头元素，用e返回其值
    QueuePtr p;
    if(Q->front==Q->rear)
        return 0;
    p = (Q->front)->next;
    *e = p->data;
    (Q->front)->next = p->next;
    if(Q->rear==p)
        Q->rear = Q->front;
    free(p);                    //删p
    return 1;
}//DeQueue
int GetTopQueue(LinkQueue Q, QElemType *e){
    //若队列不空，则取Q的队头元素，用e返回其值
    QueuePtr p;
    if(Q.front==Q.rear)
        return 0;
    p = (Q.front)->next;
    *e = p->data;
    return 1;
}//GetTopQueue
int QueueEmpty(LinkQueue Q){
    //检查队列Q是否为空
    if(Q.front==Q.rear)
        return 1;
    else
        return 0;
}//QueueEmpty




/***********主要部分**********/

EventList ev;               //事件表
Event en;                   //当前事件
LinkQueue q[3];             //两个客户队列，0号不用
QElemType customer;         //当前客户
int TotalTime;              //累计客户逗留时间
int CustomerNum;            //客户数
int total;                  //当钱资金总额
int curr_time;              //接待客户过程中的当前时间
int CloseTime;              //关门时间
int max_Duration;           //最大办理业务所需时间
int max_intertime;          //最大到达时间间隔
int max_money;              //交易的最大款额

void OpenForDay(){
    //初始化操作
    int i;
    TotalTime = 0;              //初始化累计时间核客户数为0
    CustomerNum = 0;
    CreatList(&ev);             //创建空事件链表
    en.OccurTime = 0;           //设定第一个客户到达事件
    en.NType = 0;
    ListInsert(&ev, en);        //插入事件表
    for (i = 1; i <= 2; i++)
        InitQueue(&(q[i]));     //置空队列
    //读入数据
    printf("enter total money:");
    scanf("%d", &total);
    printf("enter CloseTime:");
    scanf("%d", &CloseTime);
    printf("enter max Duration:");
    scanf("%d", &max_Duration);
    printf("enter max intertime:");
    scanf("%d", &max_intertime);
    printf("enter max money:");
    scanf("%d", &max_money);
    printf("%d,%d,%d,%d,%d\n", total, CloseTime, max_Duration, max_intertime, max_money);
}//OpenForDay


void CustomerDeparture(){
    //处理客户离开事件，en.NType>0。
    int i;
    i = en.NType;
    DeQueue(&(q[i]), &customer);
    TotalTime += en.OccurTime - customer.ArrivalTime;       //累计客户逗留时间
}

int check(int pretotal){
    //顺序检查和处理q2的客户
    QElemType m,n;
    if(GetTopQueue(q[2], &m))
        n = m;
    while(!QueueEmpty(q[2])){
        DeQueue(&(q[2]), &n);
        if (n.Money + total >= 0){
            if(curr_time + n.Duration >= CloseTime)
                break;                          //超时，没法办理了
            total += n.Money;                   //办理业务
            curr_time += n.Duration;            //更新当前时间
            TotalTime += curr_time - n.ArrivalTime;      //累计客户逗留时间
        }
        else
            EnQueue(&(q[2]), n);
        if(total <= pretotal)
            break;
        if(GetTopQueue(q[2], &n))
            if(n.ArrivalTime == m.ArrivalTime)
                break;
    }
    return 1;
}

int Reception(){
    //接待q1中的客户
    int pretotal;                                   //有一人存钱，之前的总钱数，作为参数传入下面
    int durtime;                                    //当前客户办理业务所需时间
    curr_time = 0;                                  //初始化当前时间为0
    while(!QueueEmpty(q[1])){
        //处理窗口1面前的顾客
        DeQueue(&(q[1]), &customer);                //此时customer为窗口1前的顾客
        if(curr_time < customer.ArrivalTime)
            curr_time = customer.ArrivalTime;
        durtime = customer.Duration;
        if(customer.Money <= 0){
            if(total + customer.Money < 0)
                EnQueue(&(q[2]), customer);         //取钱过多，入q2，不耗时
            else{
                if(curr_time + durtime >= CloseTime)
                    break;                          //超时，没法办理了
                total += customer.Money;            //办理业务
                curr_time += durtime;               //更新当前时间
                TotalTime += curr_time - customer.ArrivalTime;      //累计客户逗留时间
            }
        }
        else{
            if(curr_time + durtime >= CloseTime)
                break;                              //超时，没法办理了
            pretotal = total;
            total += customer.Money;                //办理业务
            curr_time += durtime;                   //更新当前时间
            TotalTime += curr_time - customer.ArrivalTime;          //累计客户逗留时间
            check(pretotal);                        //检查q2
        }
    }
    //关门时间到了，其他人出队列。(可能q1没人了，q2没法处理)
    printf("Bank closed!");
    while(!QueueEmpty(q[1])){
        DeQueue(&(q[1]), &customer);
        TotalTime += CloseTime - customer.ArrivalTime;
    }
    while(!QueueEmpty(q[2])){
        DeQueue(&(q[2]), &customer);
        TotalTime += CloseTime - customer.ArrivalTime;
    }
    return 1;
}


void CustomerArrived(){
    //处理客户到达事件，en.NType=0。
    //q1队列为正常办理窗口，暂时不能办理则入q2，每次存钱就检查一遍q2
    int durtime;                                    //此时到达的客户办理事务所需时间
    int intertime;                                  //下一客户将到达的时间间隔
    int t, tag;
    Event en_next;                                  //下一事件
    ++CustomerNum;
    durtime = rand() % max_Duration + 1;            //产生随机数
    intertime = rand() % max_intertime + 1;
    printf("%d,%d\n", durtime, intertime);
    t = en.OccurTime + intertime;                   //下一客户到达时刻
    if(t < CloseTime){
        //产生下一个客户到达事件并插入事件表
        en_next.NType = 0;
        en_next.OccurTime = t;
        ListInsert(&ev, en_next);                   
    }
    //产生当前客户结点并入队列
    customer.ArrivalTime = en.OccurTime;
    customer.Duration = durtime;
    printf("random money?");
    scanf("%d", &tag);
    if(tag){
        customer.Money = rand() % max_money + 1;    //怎么产生负数？
        printf("%d\n", customer.Money);
    }
    else{
        printf("enter money:");
        scanf("%d", &(customer.Money));
    }
    EnQueue(&(q[1]), customer);
    if(t >= CloseTime){
        //此时在关门前能到达的所有客户都在q1排好了，开始接待客户，产生所有的离队或转队事件。
        Reception();
    }
}


int main(){
    OpenForDay();                   //先把事件表按顺序生成了，再一个个处理
    while(!ListEmpty(ev)){
        ListDelete(&ev, 1, &en);
        if(en.NType==0)
            CustomerArrived();      //客户到达
        else
            CustomerDeparture();    //客户离开
    }
    //计算并输出平均逗留时间
    printf("The Average Time is %f\n", (float)TotalTime / CustomerNum);
}//main




//先把关门前能到达的所有客户都排好队，再接待？
//无限random money循环？？？不是，输的太多了
//成功实现，但是需要改成事件驱动，还需产生负钱数