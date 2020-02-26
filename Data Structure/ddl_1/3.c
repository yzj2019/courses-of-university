//选择做题目2.6
//2019.12.4 23:07

#include<stdio.h>
#include<stdlib.h>
#include<time.h>


/************线性链表的相关操作***********/

typedef struct{
    int OccurTime;      //事件发生时刻
    int NType;          //事件类型，0表示到达事件，1、2表示q1、q2中的客户离开，3表示接待q1头的客户，4表示顺序检查q2的客户
} Event, ElemType;      //有序链表LinkList的数据元素类型
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
    while(p->next){
        //寻找待插入位置,循环结束后插到p的后面，此时p<e<=p->next，故同一时刻发生的事，后插入的先发生。
        if(cmp(e, (p->next)->data) == 1)
            p = p->next;
        else
            break;
    }
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
void GetTopList(LinkList L, ElemType *e){
    //取表头元素
    *e = (L->next)->data;
}//GetTopList


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
int QueueLength(LinkQueue Q){
    //返回队列长度
    int i = 0;
    QueuePtr p;
    p = (Q.front)->next;
    while(p){
        p = p->next;
        i++;
    }
    return i;
}//QueueLength




/***********主要部分**********/

EventList ev;               //事件表
Event en;                   //当前事件
Event en_next;              //下一事件
LinkQueue q[3];             //两个客户队列，0号不用
QElemType customer;         //当前客户
int del_time;               //顺序检查q2时有人能办业务，由此产生时间延迟
int TotalTime;              //累计客户逗留时间
int CustomerNum;            //客户总数
int total;                  //当钱资金总额
int pretotal;               //有一人存钱，之前的总钱数，作为参数传入下面
int CloseTime;              //关门时间
int max_Duration;           //最大办理业务所需时间
int min_Duration;           //最小办理业务所需时间
int max_intertime;          //最大到达时间间隔
int min_intertime;          //最小到达时间间隔
int max_money;              //交易的最大款额

void OpenForDay(){
    //初始化操作
    int i;
    TotalTime = 0;                  //初始化累计时间核客户数为0
    CustomerNum = 0;
    del_time = 0;
    srand((unsigned)time(NULL));    //重设随机数种子
    CreatList(&ev);                 //创建空事件链表
    for (i = 1; i <= 2; i++)
        InitQueue(&(q[i]));         //置空队列
    //读入数据
    printf("enter total money:");
    scanf("%d", &total);
    printf("enter CloseTime:");
    scanf("%d", &CloseTime);
    printf("enter max Duration:");
    scanf("%d", &max_Duration);
    printf("enter min Duration:");
    scanf("%d", &min_Duration);
    printf("enter max intertime:");
    scanf("%d", &max_intertime);
    printf("enter min intertime:");
    scanf("%d", &min_intertime);
    printf("enter max money:");
    scanf("%d", &max_money);
    en.OccurTime = rand() % (max_intertime - min_intertime + 1) + min_intertime;    //设定第一个客户到达事件
    en.NType = 0;
    putchar('\n');
    ListInsert(&ev, en);        //插入事件表
}//OpenForDay


int CustomerDeparture(){
    //处理客户离开事件，en.NType>0，每次处理完后产生下一客户接待事件。
    int i;
    i = en.NType;
    DeQueue(&(q[i]), &customer);
    pretotal = total;
    total += customer.Money;                                //办理业务
    TotalTime += en.OccurTime - customer.ArrivalTime;       //累计客户逗留时间
    if(i==1){
        //q1中客户处理完离开
        if(ListEmpty(ev)){
           //事件表空了，则银行关门前的所有客户都已到达，且刚才不是存钱，不必检查q2
           //可以接待下一客户
           if(!QueueEmpty(q[1])){
                //刚才处理q1时有人等待，则产生立即接待下一q1客户的事件
                en_next.NType = 3;
                en_next.OccurTime = en.OccurTime;
                ListInsert(&ev, en_next);
            }
        }
        else{
            //事件表非空
            GetTopList(ev, &en_next);
            if (en_next.NType != 4)
                //当前事件过后不是立即顺序检查q2，可以接待下一个客户
                if (!QueueEmpty(q[1])){
                    //刚才处理q1时有人等待，则产生立即接待下一q1客户的事件
                    en_next.NType = 3;
                    en_next.OccurTime = en.OccurTime;
                    ListInsert(&ev, en_next);
                }
        }
    }
    else{
        //q2中客户处理完离开
        del_time -= customer.Duration;
        if(!del_time)
            //完成此次顺序检查q2的所有业务，可以接待下一个q1客户
            if(!QueueEmpty(q[1])){
                //刚才处理q1时有人等待，则产生立即接待下一q1客户的事件
                en_next.NType = 3;
                en_next.OccurTime = en.OccurTime;
                ListInsert(&ev, en_next);
            }
    }
    return 1;
}

int check(){
    //顺序检查q2的客户，产生q2中客户离开事件
    //返回在这个过程中的耗时
    int count;
    int total_sim = total;
    QElemType n;
    del_time = 0;
    count = QueueLength(q[2]);
    if(!count){
        //当前q2没人
        if (!QueueEmpty(q[1])){
            //刚才处理q1时有人等待，则产生立即接待下一q1客户的事件
            en_next.NType = 3;
            en_next.OccurTime = en.OccurTime;
            ListInsert(&ev, en_next);
        }
    }
    while(count){
        GetTopQueue(q[2], &n);
        if (n.Money + total_sim >= 0){
            //能办
            if(en.OccurTime + del_time + n.Duration >= CloseTime)
                break;                                      //超时，没法办理了
            else{
                del_time += n.Duration;                     //更新延迟时间
                total_sim += n.Money;                       //模拟办理业务，作为判断
                en_next.NType = 2;                          //产生q2客户离开事件
                en_next.OccurTime = en.OccurTime + del_time;
                ListInsert(&ev, en_next);
            }
        }
        else{
            //办不了，转至队尾
            DeQueue(&(q[2]), &n);
            EnQueue(&(q[2]), n);
        }
        if(total_sim <= pretotal)
            break;
        count--;
    }
    return 1;
}

int Reception(){
    //接待q1中的客户，每次接待完后产生接待下一q1客户的事件
    int durtime;                                    //当前客户办理业务所需时间
    int judge = 0;                                  //判断是否关门
    GetTopQueue(q[1], &customer);                   //此时customer为窗口1前的顾客
    durtime = customer.Duration;                    //此时en代表接待，还没处理
    if(customer.Money <= 0){
        if(total + customer.Money < 0){
            //取钱过多，入q2，不耗时
            DeQueue(&(q[1]), &customer);
            EnQueue(&(q[2]), customer);                         
        }
        else{
            if(en.OccurTime + durtime >= CloseTime)
                //超时，办不完
                judge = 1;                                      
            else{
                //产生q1客户离开事件
                en_next.NType = 1;
                en_next.OccurTime = en.OccurTime + durtime;
                ListInsert(&ev, en_next);
            }
        }
    }
    else{
        if(en.OccurTime + durtime >= CloseTime)
            judge = 1;                                      //超时，办不完
        else{
            //产生顺序检查q2的事件
            en_next.NType = 4;                                           
            en_next.OccurTime = en.OccurTime + durtime;
            ListInsert(&ev, en_next);
            //产生q1客户离开事件，先发生
            en_next.NType = 1;
            en_next.OccurTime = en.OccurTime + durtime;
            ListInsert(&ev, en_next);
        }
    }
    return 1;
}


void CustomerArrived(){
    //处理客户到达事件，en.NType=0。
    //q1队列为正常办理窗口，暂时不能办理则入q2，每次存钱就检查一遍q2
    int durtime;                                    //此时到达的客户办理事务所需时间
    int intertime;                                  //下一客户将到达的时间间隔
    int t;
    //int tag;
    ++CustomerNum;
    durtime = rand() % (max_Duration - min_Duration + 1) + min_Duration;            //产生随机数
    intertime = rand() % (max_intertime - min_intertime + 1) + min_intertime;
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
    customer.Money = rand() % (2 * max_money + 1) - max_money;
    EnQueue(&(q[1]), customer);
    //输出客户数据方便进行验证
    printf("arrivaltime:%d\n", customer.ArrivalTime);
    printf("duration:%d\n", customer.Duration);
    printf("money:%d\n", customer.Money);
    printf("intertime:%d\n", intertime);
    putchar('\n');
    //产生当前客户被立即接待的事件
    if (!del_time)
        //没有在检查q2
        if (QueueLength(q[1]) == 1){
            //队列q1中唯一的客户，则立即接待，由此来驱动产生其它的接待事件
            en_next.NType = 3;
            en_next.OccurTime = customer.ArrivalTime;
            ListInsert(&ev, en_next);
        }
}


int main(){
    OpenForDay();                   //先把事件表按顺序生成了，再一个个处理
    while(!ListEmpty(ev)){
        ListDelete(&ev, 1, &en);    //事件表头事件发生
        printf("%d,%d\n", en.NType, en.OccurTime);
        if(en.NType==0)
            CustomerArrived();      //客户到达
        else if(en.NType==3)
            Reception();            //处理q1头的客户
        else if(en.NType==4)
            check();                //顺序检查q2的客户
        else
            CustomerDeparture();    //客户离开
    }
    //关门时间到了，其他人出队列。(可能q1没人了，q2没法处理)
    printf("Bank closed!\n");
    while (!QueueEmpty(q[1])){
        DeQueue(&(q[1]), &customer);
        TotalTime += CloseTime - customer.ArrivalTime;
    }
    while (!QueueEmpty(q[2])){
        DeQueue(&(q[2]), &customer);
        TotalTime += CloseTime - customer.ArrivalTime;
    }
    //计算并输出平均逗留时间
    printf("%d,%d\n", TotalTime, CustomerNum);
    printf("The Average Time is %f\n", (float)TotalTime / CustomerNum);
}//main



//还需完善check，完善Reception的产生下一......  完成
//srand(seed)，完成
//测试1：到达时间间隔很短，客户交易时间很长，成功，输出为从1到599累加再除以599，即300
//测试2_1：没有正确输出，需要判断事件表是否为空！！！Line217
//测试2_2：到达时间间隔很长，客户交易时间很短，成功，输出为第一人的逗留时间
//2019.12.10 17:13