实验三-二叉树及其应用


/*********版本信息*********/
1.c和2.c为按照所需功能分步骤实现的测试版。
3.c为最终版。


/*********实验要求*********/

1、使用二叉链表作为二叉树的存储结构，编写程序，实现按照先序遍历的次序创建二叉树，并先序遍历该二叉树检验是否与输入一致。
2、使用三叉链表作为线索二叉树的存储结构，编写程序，实现按照先序遍历的次序创建二叉树，并将其后续线索化，最后根据线索非递归的后序遍历线索二叉树。
3、利用1中实现的功能，将输入的合法波兰式转化为表达式树。先序、中序、后续分别遍历打印该树，比较他们与实际的波兰式、中缀式和逆波兰式的区别。
4、将3中的表达式树输出成为对应的中缀表达式。


/*********实现思路*********/

1、	二叉链表存储结构容易实现。

2、	先序遍历创建二叉链表存储的二叉树：每次读入一个结点；如果结点为虚结点，则指向该结点的指针置空；否则将结点数据存入，然后递归地构建该结点的左子树、右子树。

3、	先序遍历并打印二叉树：如果该树为空，则打印空格；否则打印它的根中存储的数据，然后递归地遍历根的左子树、右子树。

4、	三叉链表存储结构容易实现。
先序遍历创建三叉链表存储的二叉树：在二叉链表创建算法的基础上，增加参数p指向当前结点的双亲；当前结点不为空时，将parent指针赋为p，然后将当前结点的两个tag域都置为link，方便后面线索化时修改。
将构建好的二叉树后续线索化：创建头结点，为了方便头结点和根结点的判断，将头结点的data域赋成特定的值；若树非空，则根结点的parent域指向头结点、头结点的lchild指向根结点、rchild指向后续遍历的第一个结点，否则头结点的lchild、rchild均回指；后序遍历该树，pre指向上一个访问的结点，判断当前结点的lchild、pre所指结点的rchild是否为空，若为空则进行相应的线索化。

5、	通过线索，非递归地后序遍历上题构造好的二叉树：从头结点的后继（即后序遍历中的第一个结点）开始遍历，重复下述过程，直到更新后的当前结点回到头结点时，完成遍历：
	遍历当前结点；
	寻找后继并更新当前节点：如果该结点是双亲的左孩子，且双亲有右子树，则该结点的后继是双亲的右子树后序遍历的第一个结点；否则该结点的后继是它的双亲；
6、	根据输入的合法波兰式，创建表达式树：
	在先序输入创建二叉树的基础上，增加对当前结点的判断：若当前结点的data域存储的是字母或数字，则当前结点的lchild、rchild域置空；否则递归调用此函数，创建当前结点的左右子树。

7、	先序、中序、后续遍历上题的表达式树，容易实现。

8、	表达式的求值与转换：
（1）、基于表达式树，对表达式进行求值：若当前结点是操作数，则返回当前节点的数值；否则当前结点是操作符，递归调用此函数求出左右子表达式树的值a、b，判断操作符的种类，并对a、b进行相应运算得到结果sum并返回。
（2）、将该表达式转换成标准的中缀表达式：分析中序遍历和中缀表达式的区别，发现中序遍历的输出缺少括号表明运算的优先顺序，则在中序遍历的基础上，增加对操作符和其左右子树的判断：如果当前结点存储的是乘除号，则判断左右子树的根节点是不是操作数，若是则正常递归调用此函数遍历，否则为操作符，即左/右子树存储的是一个表达式，输出时在表达式的左右两侧加上括号。
