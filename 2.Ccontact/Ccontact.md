# Lab 2：动态通讯录

## 1. 目标

练习这些能力：

- `struct` 设计
- 动态数组
- `malloc/realloc/free`
- 文件读写

## 2. 推荐数据结构

```c
typedef struct {
    char name[32];
    char phone[20];
    int age;
} Contact;

typedef struct {
    Contact *data;
    size_t size;
    size_t capacity;
} ContactBook;
```

## 3. 你要完成的功能

- 添加联系人
- 删除联系人
- 查找联系人
- 修改联系人
- 列出所有联系人
- 保存到文件
- 从文件加载

## 4. 硬性要求

- 不能一开始就用超大静态数组糊过去
	- 必须自己写扩容逻辑
- `size` 和 `capacity` 必须分开管理
- 退出前必须释放堆内存

## 5. 重点难点

- `realloc` 之后旧指针可能失效
- 删除元素时要处理元素搬移
- 文件加载时要处理空文件和格式错误

## 6. 验收标准

- 连续添加很多联系人时程序仍然正确
- 删除第一个、最后一个、中间元素都正确
- 文件保存后重新加载结果一致
- 开 `AddressSanitizer` 时无明显内存问题

## 7. 建议搭配阅读

- [[C/c进阶/AddressSanitizer：开启条件、用法与适用场景|AddressSanitizer]]
- [[C/c进阶/GCC 常用编译参数：-Wall -Wextra -Werror -g -o|GCC 常用编译参数]]

## 8.输入格式

	1. + 蒋雨乐 12xxxxxx11 18
	2. - XXX
	3. f XXX
	4. l