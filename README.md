# rbtree
用c实现的泛型的红黑树。受到linux中list的实现的启发，用类似的思想来实现，也就是不是在一个红黑树节点中保存值的数据，而是在一个数据结构中内置一个红黑树节点，靠这个节点可以定位出结构体实例的位置。这样值的类型和大小等都是可以改变的，同一棵红黑树之中可以有多种数据类型。

在test.c中有测试的例子，用了两种不同的结构体。

写完之后看了看linux的红黑树实现，果然也是这样做的哈哈哈。

