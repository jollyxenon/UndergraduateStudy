# CS100 Cheat Sheet

## 1. 编译、程序执行与基础输入输出

### 编译命令

常见写法：

```bash
gcc  -std=c17   -Wall -Wextra -O2 -g main.c   -o main
g++  -std=c++17 -Wall -Wextra -O2 -g main.cpp -o main
```

常用参数：

- `-o`：输出文件名
- `-c`：只编译，不链接
- `-E`：只预处理
- `-S`：生成汇编
- `-std=...`：指定语言标准
- `-DNAME=VALUE`：定义宏
- `-O0/-O1/-O2/-O3`：优化级别
- `-g`：调试信息
- `-Wall -Wextra -Werror`：警告设置
- `-fsanitize=address`：地址检查

### shell 重定向

```bash
./app > out.txt
./app >> out.txt
./app < input.txt
./app > out.txt 2> err.txt
./app > all.txt 2>&1
```

含义：

- `>` 覆盖输出
- `>>` 追加输出
- `<` 从文件读入
- `2>` 重定向错误输出
- `2>&1` 把错误也合并到标准输出
- `|` 管道，前一个程序输出接到后一个程序输入

C/C++ 里也可以用 `freopen()` 重定向 stdin/stdout。

### `freopen()`

```c
FILE *freopen(const char *path, const char *mode, FILE *stream);
```

- `path`：文件路径
- `mode`：打开方式，如 `"r"`, `"w"`, `"a"`, `"r+"`
- `stream`：要重定向的流，如 `stdin` / `stdout` / `stderr`

例子：

```c
freopen("in.txt", "r", stdin);
freopen("out.txt", "w", stdout);
```

常见返回值：成功返回 `stream`，失败返回 `NULL`。

### 输入输出函数

可以这样记：

- **`printf`**：按格式把内容输出到 `stdout`；返回**成功输出的字符数**。
- **`scanf`**：按格式从 `stdin` 读入并写到变量地址；返回**成功赋值的项数**，失败时可能是 `EOF`。
- **`getchar/putchar`**：分别读/写**单个字符**；`getchar` 返回读到的字符，失败返回 `EOF`。
- **`fgets`**：按长度读一行/一段字符串，**最多读 `count-1` 个字符**；成功返回 `str`，失败返回 `NULL`。
- **`puts`**：输出字符串并**自动补一个换行**。

`scanf format`：

- 格式串由三类组成：`%d/%s/...`、空白符、普通字符。
- **普通字符必须完全匹配**输入。
- **空白符**在格式串里都等价，都会吞掉输入中的连续空白。
- 常用占位符：`%d` `int`，`%ld` `long`，`%lf` `double`，`%c` `char`，`%s` 字符串。

空白符/换行/缓冲区差异：

- `scanf("%d")`、`scanf("%s")` 会**跳过前导空白**。
- `scanf("%c")` **不会跳过**，常写成 `scanf(" %c", &ch)`。
- `scanf("%s")` 读到空白就停，**空白还留在缓冲区里**。
- `fgets` 会把**换行符也读进来**，然后再补 `\0`。
- `scanf("%s")` 没有长度检查，**不安全**；`fgets` 更安全。

## 2. 基本类型、表示与 `sizeof`

### 类型

通常是：

- `char`：1 字节
- `short`：2 字节
- `int`：4 字节
- `long`：4 或 8 字节
- `long long`：8 字节
- `float`：4 字节
- `double`：8 字节
- `long double`：通常 16 字节（也可能 12/8）

大小关系一般是：

- `char <= short <= int <= long <= long long`
- `float <= double <= long double`

但**只保证顺序，不保证具体字节数**。

### `const`

- `const int* p` / `int const* p` 能改指向不能改 `*p`
- `int* const p` 不能改指向能改 `*p`

### C++ 新增类型相关

- `auto`：自动推导类型
- `decltype(...)`：按表达式推导类型
- `nullptr`：专门表示空指针
- `using T = ...`：类型别名
  - `using Vec = std::vector<int>;` 等价于旧写法 `typedef std::vector<int> Vec;`
- `enum class`：强类型枚举
  - `enum class Color { Red, Green, Blue };`
  - 用法：`Color c = Color::Red;`
- 引用 `&` / 右值引用 `&&`

### `sizeof` 的各种用法

- **基本类型**：`sizeof(int)`、`sizeof(double)`
- **数组**：`sizeof(arr)` 得到**整个数组**大小；`sizeof(arr[0])` 是单个元素大小
- **指针**：`sizeof(p)` 只看**指针本身**，不是它指向的内容
- **结构体 / 类对象**：`sizeof(obj)` 得到对象整体大小，通常包含对齐填充
- **表达式 vs 类型名**：
  - `sizeof x` / `sizeof(x)`：对表达式求大小
  - `sizeof(int)`：对类型求大小

例子：

```c
int a[10];
int *p = a;

sizeof(a);   // 整个数组；= 10 * sizeof(int)
sizeof(p);   // 指针大小；= sizeof(int*)
sizeof(*p);  // int 的大小；= sizeof(int)
```

注意：

- `sizeof` 的结果类型是 `size_t`
- `sizeof` **不会执行表达式**，所以 `sizeof(i++)` 不会让 `i` 自增

### 位运算

位运算就是**按二进制位逐位操作**。

常见运算符：

- `&`：按位与，都为 1 才是 1
- `|`：按位或，有一个 1 就是 1
- `^`：按位异或，不同为 1，相同为 0
- `~`：按位取反，0 变 1，1 变 0
- `<<`：左移
- `>>`：右移

- **取某一位**：`x & (1 << k)`
- **把第 k 位置 1**：`x | (1 << k)`
- **把第 k 位清 0**：`x & ~(1 << k)`
- **翻转第 k 位**：`x ^ (1 << k)`

### 运算符

#### 优先级

##### 1. 最高：后缀类，左结合

```cpp
a[i]    a()    a.b    a->b    a++    a--
```

##### 2. 一元运算符，右结合

```cpp
++a   --a   +a   -a   !a   ~a
*a   &a   sizeof   new   delete   (type)
```

##### 3. 指向成员，左结合

```cpp
a.*p
a->*p
```

##### 4. 乘除模，左结合

```cpp
*   /   %
```

##### 5. 加减，左结合

```cpp
+   -
```

##### 6. 移位，左结合

```cpp
<<   >>
```

##### 7. 比较大小，左结合

```cpp
<   <=   >   >=
```

##### 8. 相等/不等，左结合

```cpp
==   !=
```

##### 9. 按位与，左结合

```cpp
&
```

##### 10. 按位异或，左结合

```cpp
^
```

##### 11. 按位或，左结合

```cpp
|
```

##### 12. 逻辑与，左结合

```cpp
&&
```

##### 13. 逻辑或，左结合

```cpp
||
```

##### 14. 条件运算符、赋值类，右结合

```cpp
?:
=   +=   -=   *=   /=   %=   <<=   >>=   &=   ^=   |=
```

例如：

```cpp
a = b = c;
```

等价于：

```cpp
a = (b = c);
```

##### 15. 最低：逗号，左结合

```cpp
,
```

#### 特性

整数除法向零截断，所以取模可能取到负数。

##### 类型提升

- `bool/char/short/enum -> int` 或 `unsigned int`
- 转 `float -> double -> long float`
- 同类型直接算，同号选范围更大的，异号按 signed/unsigned 规则判断
  - 哪个等级高转成哪个
  - 一样高就转成对应 unsigned

### CE，UB，RE

#### 编译错误

- 类型/参数不匹配：`scanf/printf` 形参类型不对、`std::cout << vector` 没有对应重载
- 不能绑定：非 `const` 左值引用绑右值
- 用了被删除的函数：如 `std::unique_ptr` 拷贝
- `const` 冲突：在 `const` 成员函数里改成员、`const int*` 赋给 `int*`
- 不完全类型：只前向声明却定义对象、访问成员、`sizeof`
- 语法/名字错误：拼错变量名、缺声明、函数原型不对

#### 未定义行为

- 解引用空指针/野指针/悬垂指针
- 返回局部变量地址后再用
- 数组越界、指针越界算术
- `free/delete` 用错：双重释放、`new`/`delete[]` 混用、`free` 非堆指针
- 读未初始化局部变量
- 非 `void` 函数漏 `return` 且结果被用到
- 有符号整数溢出
- 修改字符串字面量
- `printf/scanf` 格式与实参类型不匹配
- 同一表达式里对同一变量多次修改/修改+读取，且求值顺序不确定

#### 运行时错误

- `v.at(i)` 越界
- `new int[n]{...}` 长度非法
- `malloc` 失败返回 `NULL`
- 主动 `throw / exit()`

## 3. C 字符串、字符数组与 `str` 函数

### 字符串

- 在 C 中每个 `"abc"` 都是类型为 `char [4]` 的字符串，存放在只读区域。
- **`char str[]`**：数组本体，通常可改内容，`sizeof(str)` 是整个数组大小（即[ ]里的内容）。
- **`char *str`**：指针，只保存首地址，`sizeof(str)` 是指针大小；`char *str = "abc"` 时不可通过 str 改，否则是 UB。
- **`\0`**：字符串结束标志，必须保留；函数会一直找它。少了它，很多字符串函数会越界，属于 UB。
- 在 C++ 里，`"abc"` 的类型是 `const char[4]`。

### C 中的 `str` 函数

- C 字符串本质是 **以 `'\0'` 结尾的 `char` 数组**
- 大多数 `str...` 函数都**依赖 `'\0'` 来判断字符串结束**
- 如果传进去的内容**不是合法的以 `'\0'` 结尾的字符串**，很多函数都会继续往后读，可能导致 **UB**
- 有些函数会**写入 `'\0'`**，有些只是**查找/比较并依赖它**

#### 长度类

| 函数                    | 输入       | 操作                        | 返回值   | 和 `\0` 的关系                                |
| ----------------------- | ---------- | --------------------------- | -------- | --------------------------------------------- |
| `strlen(const char *s)` | 字符串 `s` | 计算长度，不包括末尾 `'\0'` | `size_t` | **依赖** `'\0'`；不处理、不保留，只是一路找它 |

例子：

```c
strlen("abc")   // 3
```

注意：

- `strlen` 不看数组大小，只看什么时候遇到 `'\0'`
- 如果没有 `'\0'`，它会继续读，可能 UB

#### 复制类

| 函数                                            | 输入                         | 操作                  | 返回值 | 和 `\0` 的关系                 |
| ----------------------------------------------- | ---------------------------- | --------------------- | ------ | ------------------------------ |
| `strcpy(char *dst, const char *src)`            | 目标串、源串                 | 把 `src` 复制到 `dst` | `dst`  | **复制并保留** `src` 的 `'\0'` |
| `strncpy(char *dst, const char *src, size_t n)` | 目标串、源串、最多复制数 `n` | 最多复制 `n` 个字符   | `dst`  | **可能补 `'\0'`，也可能不补**  |

##### `strcpy`

```c
char a[10];
strcpy(a, "abc");
```

结果：

```c
a = {'a','b','c','\0', ...}
```

特点：

- 会一直复制到 `src` 的 `'\0'`
- 所以 `dst` 必须足够大
- 否则可能缓冲区溢出，UB

##### `strncpy`

```c
char a[10];
strncpy(a, "abc", 5);
```

结果大致是：

```c
{'a','b','c','\0','\0', ...}
```

但：

```c
char a[3];
strncpy(a, "abcd", 3);
```

结果是：

```c
{'a','b','c'}
```

**没有 `'\0'`！**

所以考试重点：

- `strncpy` **不是**“安全版 strcpy”
- 当源串长度 `>= n` 时，结果**可能不是合法字符串**
- 后面如果再 `printf("%s", a)`，就可能出问题

#### 拼接类

| 函数                                            | 输入                              | 操作                     | 返回值 | 和 `\0` 的关系                                                    |
| ----------------------------------------------- | --------------------------------- | ------------------------ | ------ | ----------------------------------------------------------------- |
| `strcat(char *dst, const char *src)`            | 目标串、源串                      | 把 `src` 接到 `dst` 末尾 | `dst`  | **先依赖 `dst` 中原有的 `'\0'` 找结尾，再复制 `src` 连同 `'\0'`** |
| `strncat(char *dst, const char *src, size_t n)` | 目标串、源串、最多追加 `n` 个字符 | 追加最多 `n` 个字符      | `dst`  | **依赖 `dst` 原有 `'\0'` 找结尾，并且会补自己的 `'\0'`**          |

##### `strcat`

```c
char a[20] = "abc";
strcat(a, "de");
```

结果：

```c
"abcde"
```

特点：

- 先找到 `a` 里原来的 `'\0'`
- 从那里开始拷贝 `"de\0"`
- 所以 `dst` 必须本来就是合法字符串，且空间足够

##### `strncat`

```c
char a[20] = "abc";
strncat(a, "defgh", 2);
```

结果：

```c
"abcde"
```

注意：

- 只追加前 `n` 个字符
- 然后**自己补一个 `'\0'`**
- 但仍然要保证 `dst` 空间足够

#### 比较类

| 函数                                              | 输入                            | 操作              | 返回值            | 和 `\0` 的关系                 |
| ------------------------------------------------- | ------------------------------- | ----------------- | ----------------- | ------------------------------ |
| `strcmp(const char *a, const char *b)`            | 两个字符串                      | 按字典序比较      | `<0` / `0` / `>0` | **依赖** `'\0'` 判断结束       |
| `strncmp(const char *a, const char *b, size_t n)` | 两个字符串、最多比较 `n` 个字符 | 比较前 `n` 个字符 | `<0` / `0` / `>0` | 比较时也会遇到 `'\0'` 提前结束 |

##### `strcmp`

```c
strcmp("abc", "abc")   // 0
strcmp("abc", "abd")   // < 0
strcmp("abe", "abd")   // > 0
```

它比较的是**第一个不同字符的大小**。

##### `strncmp`

```c
strncmp("abcdef", "abcxyz", 3)   // 0
strncmp("abcdef", "abcxyz", 4)   // < 0
```

特点：

- 最多比较 `n` 个字符
- 并不是“长度比较”
- 如果前 `n` 个都一样，就返回 0

#### 查找类

| 函数                                     | 输入         | 操作                   | 返回值                                | 和 `\0` 的关系                                         |
| ---------------------------------------- | ------------ | ---------------------- | ------------------------------------- | ------------------------------------------------------ |
| `strchr(const char *s, int c)`           | 字符串、字符 | 查找字符第一次出现位置 | 指向该字符的指针；找不到返回 `NULL`   | **依赖** `'\0'` 扫描结束；如果找 `'\0'` 本身也可以找到 |
| `strstr(const char *s, const char *sub)` | 字符串、子串 | 查找子串第一次出现位置 | 指向起始位置的指针；找不到返回 `NULL` | **依赖** 两边的 `'\0'`                                 |

##### `strchr`

```c
char *p = strchr("abcde", 'c');   // 指向 "cde"
```

特别点：

```c
strchr("abc", '\0')
```

会返回指向末尾那个 `'\0'` 的指针。

##### `strstr`

```c
char *p = strstr("abcde", "cd");   // 指向 "cde"
```

如果查空串：

```c
strstr("abc", "")
```

通常返回原串起始位置。

## 4. 数组、指针与动态内存

### 指针

#### 指针退化

数组经常退化成指针。`int a[10]` 中的 `a` 经常退化成 `&a[0]`。

以下情况为**例外**：

- `sizeof(a)`
- `&a`
- 用字符串字面量初始化数组

#### 指针数组和数组指针

`int *arr[5];` 是指针数组，数组里装的每个元素都是指针；`int (*p)[5];` 是数组指针，这个指针指向一个数组。

#### 二级指针

```c
int x = 42;
int *p = &x;
int **pp = &p;
```

##### 分配内存

```c
int **a = malloc(3 * sizeof(int *));
for (int i = 0; i < 3; ++i)
    a[i] = malloc(4 * sizeof(int));
```

##### 释放内存

```c
for (int i = 0; i < 3; ++i)
    free(a[i]);
free(a);
```

### 重复释放

#### `malloc` 和 `free`

- `malloc/calloc/realloc` 分配的内存，只能用 `free`
- 同一块内存只能 `free` 一次
- `free` 之后，这个指针就成了悬垂指针
- `free(NULL)` 是安全的，什么也不做

易错点：

- `free` 完没把指针设成 `NULL`，后面容易再 `free`
- 对不是 `malloc` 返回的地址调用 `free` 也是 UB
  例如 `int x; free(&x);` 是 UB

#### `new` 和 `delete`

- `new` 对应 `delete`
- `new[]` 对应 `delete[]`
- 也只能释放一次

##### `new` 的传参

```c
int *p = new int;
int *p2 = new int(42);
int *p3 = new int{42};

int *a = new int[10];
int *b = new int[10]();
int *c = new int[10]{1, 2, 3};
```

易错点：

- 混用
- `malloc/free` 和 `new/delete` 不能交叉混用，是 UB

#### `new` 与 `malloc` 对比

| 对比项           | `new`                   | `malloc`                 |
| ---------------- | ----------------------- | ------------------------ |
| 语言             | C++                     | C                        |
| 返回类型         | 直接返回对应类型指针    | 返回 `void *`            |
| 是否调用构造函数 | 会                      | 不会                     |
| 是否初始化对象   | 可以                    | 不会自动按对象语义初始化 |
| 失败时           | 抛异常 `std::bad_alloc` | 返回 `NULL`              |
| 释放方式         | `delete` / `delete[]`   | `free`                   |

例子：

```cpp
int *p = new int(42);
int *q = (int*)malloc(sizeof(int));
```

- `new int(42)`：得到值为 42 的 `int` 的地址
- `malloc(sizeof(int))`：只拿到一块原始内存，里面内容未定义

对类对象差别更明显：

```cpp
std::string *p = new std::string("abc");   // 正常构造
std::string *q = (std::string*)malloc(sizeof(std::string)); // 很危险
```

因为 `malloc` 不会调用构造函数。

#### C++ 智能指针

尽量避免手写释放

##### `std::unique_ptr`

- 独占所有权
- 离开作用域自动释放
- 不能拷贝，只能移动

```c
std::unique_ptr<int> p = std::make_unique<int>(42);
```

无需手动 free。

##### `std::shared_ptr`

- 多个智能指针共享所有权
- 内部靠引用计数决定何时释放

```c
int *p = new int(42);
std::shared_ptr<int> a(p);
std::shared_ptr<int> b(p);   // 错：会各自认为自己拥有 p
```

正确写法是：

```c
auto a = std::make_shared<int>(42);
auto b = a;
```

##### `make_unique`

一般使用中更推荐 `auto p = std::make_unique<T>(args...);` 而不是 `std::unique_ptr<T> p(new T(args...));`：

- **更安全**：少直接写 `new`，不容易把裸指针管理搞乱
- **更简洁**：代码短，意图更清楚
- **异常安全更好**：在更复杂的表达式里，`make_unique` 更不容易因为中途抛异常而把资源管理写得危险
- **不容易写错类型**：尤其配合 `auto`

例如：

```cpp
auto p = std::make_unique<int>(42);
```

比：

```cpp
std::unique_ptr<int> p(new int(42));
```

更现代。

例外：

- 需要**自定义 deleter 的构造方式**
- 代码场景必须显式接收一个裸指针

才可能直接写 `unique_ptr<T>(new T(...))`。

## 5. `struct` / 默认行为 / 过渡到对象

### 默认结构体

- **默认结构体 / `init` / 逐成员行为**：`struct` 和 `class` 在这些语义上几乎一样，主要只差默认访问权限。若你没自己写构造函数，内置类型成员默认初始化不一定清零；但如果你写 `T x{};`，通常是值初始化。编译器默认生成的拷贝构造、拷贝赋值、析构，做的基本都是逐成员操作：每个成员按它自己的规则拷贝/销毁。
- **默认拷贝会发生什么**：对普通值成员，这通常正好；但对指针成员，默认拷贝只会复制地址，不会复制指针指向的那块资源。于是两个对象看起来各有一份成员，实际上共享同一块堆内存，这叫浅拷贝。
- **为什么“默认拷贝 + 指针成员”埋雷**：因为一旦对象析构里 `delete ptr;`，两个对象都会以为自己拥有这块内存。结果要么 double free，要么一个对象释放后，另一个对象里留下悬垂指针。

### 资源管理

**RAII 是什么**：RAII = Resource Acquisition Is Initialization。意思是“资源的获取绑定到对象初始化，资源的释放绑定到对象析构”。换句话说，让对象生命周期自动管理资源生命周期。典型例子是 `std::string`、`std::vector`、`std::unique_ptr`：创建对象就拿到资源，离开作用域自动释放，不靠你手写 `free/delete`。

## 6. C++ 基础替代 C 风格写法

### C++ 中字符串

`std::string` 理论上不依靠 `'\0'` 判定字符串长度。例如，`std::string s("ab\0cd", 5);` 的长度就是 5。

C String 必须依靠 `\0` 判断长度。

### `auto`

```c
int a = 1;
const int ca = 2;
const int* p = &ca;
int& r = a;

auto x1 = a;   // int
auto x2 = ca;  // int
auto x3 = p;   // const int*
auto x4 = r;   // int

auto& y1 = a;  // int&
auto& y2 = ca; // const int&
```

### range-based for

原生数组和有 `begin()` 与 `end()` 的类可以用 range-based for。

注意：如果数组退化为普通指针（例如作为函数参数传递的 `int* arr`），则不能使用 range-based for，因为指针本身丢失了长度信息。

- `for (T x : range)`：拷贝元素
- `for (T& x : range)`：引用元素，可修改
- `for (const T& x : range)`：只读引用，不拷贝
- `auto` 常用写法：
- `auto x`
- `auto& x`
- `const auto& x`
- 遍历 `vector<string>` 时，通常优先 `for (const auto& s : vs)`
- 遍历时别随便 `push_back` / `erase` 改容器结构

### `static`

当 `static` 修饰局部变量时，这个变量作为一个静态局部变量存在：

- 只初始化一次
- 生命周期是整个程序运行期
- 但作用域仍然只在这个函数里

当 `static` 修饰全局变量/函数时，这个变量/函数变成一个内部链接：

- 仍然是全局生命周期
- 但只在当前源文件可见
- 别的 `.c/.cpp` 文件不能直接用它

在 C++ 类中，它标识一个静态成员。

```c
class A {
public:
    static int cnt;
};
```

- 属于整个类，不是某个对象独有
- 所有对象共享这一份

- `static` 局部变量：第一次执行到定义处时初始化（C++ 中常这样讲）
- 全局 / 文件作用域静态变量：程序开始前完成初始化

凡是有静态存储期的对象：

- `static` 局部变量
- 全局变量
- `static` 全局变量

如果你没显式初始化，都会先做零初始化。

## 7. 类、对象与构造语义

### `class`

#### `private`、`protected`、`public`

- `private` 只有自己能用
- `protected` 自己和子类能用，类外不能用
- `public` 都可以用

#### `explicit` 和 `noexcept`

`explicit` 用来阻止隐式转换和某些隐式构造。

```c
class A {
public:
    explicit A(int x) {}
};

A a = 3;         // 错
A a(3);          // 对
A a{3};          // 对
```

也可用于转换运算符：`explicit operator bool() const;`

`noexcept` 承诺不抛出异常，通常在移动构造/赋值中使用。否则，容器扩容时可能倾向于拷贝而非移动。

```c
class T {
public:
    T(T&& other) noexcept { ... }
};
```

#### `const`

成员函数参数列表后面、`{` 前面的 `const` 关键字用来声明该函数为一个常量成员函数。

- 在普通的非 `const` 成员函数中：`this` 的类型是 `Dynarray * const`（即一个指向非常量对象的常指针，指针本身不能指向其他地方，但可以通过它修改对象的数据成员）。
- 在 `const` 成员函数中：`this` 的类型变成了 `const Dynarray * const`（即指向常量对象的指针）。

#### 构造函数

默认构造函数只有未显式声明任何构造函数时，或写明 `T() = default` 时出现。

默认构造函数并不不一定是零初始化。

```cpp
class Foo{
  int x;
};
```

在这种情况下，`Foo f;` 不会零初始化；然而，`Foo f();` 或 `Foo f{}` 会零初始化 `f.x`。

#### 函数参数形式（左值右值）

##### 情况 A：只是“读入参数”，不需要保存副本

```cpp
Foo(const T& x)
```

优点：

- 左值能传
- 右值也能传
- 不额外拷贝一个参数对象
- 适合“只读使用”

##### 情况 B：要把参数存到成员里

```cpp
class Person {
    std::string name;
};
```

###### 写法 1：`const T&`

```cpp
Person(const std::string& s) : name(s) {}
```

- 传左值：一次拷贝到成员
- 传右值：右值先绑定到 `const&`，再拷贝进成员。右值优势浪费了

###### 写法 2：`T&&`

```cpp
Person(std::string&& s) : name(std::move(s)) {}
```

- 传右值：好，能移动
- 传左值：不行，除非手动 `std::move`

###### 写法 3：重载两份

```cpp
Person(const std::string& s) : name(s) {}
Person(std::string&& s) noexcept : name(std::move(s)) {}
```

- 左值来：走 `const&`，拷贝
- 右值来：走 `&&`，移动

###### 写法 4：按值传参

```cpp
Person(std::string s) : name(std::move(s)) {}
```

这也是现代 C++ 很常见的写法。

效果：

- 左值传入：先拷贝到形参 `s`，再移动到成员
- 右值传入：先移动构造形参 `s`，再移动到成员

优点：

- 代码只写一份
- 对右值表现不错
- 简洁

缺点：

- 对左值通常比 `const T&` 多一步“形参对象”的成本

#### 左值和右值定义

左值：

```cpp
int x = 3;
x          // 左值
++x        // 左值
*ptr       // 左值
arr[0]     // 左值
"abc"      // 在 C++ 里字符串字面量是左值
```

右值：

```cpp
42
x + 1
foo()      // 若返回非引用，通常是右值
std::string("abc")
```

##### 更细的分类

C++11 以后更细分成：

- **lvalue**
- **xvalue**
- **prvalue**

而：

- **glvalue = lvalue + xvalue**
- **rvalue = prvalue + xvalue**

###### lvalue

例子：

```cpp
int x = 0;
x
arr[0]
*ptr
++x
func_returning_lref()
```

###### prvalue

“纯右值”，通常是临时结果、字面量、返回的非引用对象。

例子：

```cpp
42
true
3.14
x + y
std::string("abc")
func_returning_by_value()
```

###### xvalue

“将亡值”，本质上是**快要被拿走资源的对象**。

例子：

```cpp
std::move(x)
static_cast<T&&>(x)
func_returning_rref()
```

`std::move(x)` 本身不会移动，它只是把 `x` 转成一个 xvalue，告诉编译器：**你可以把它当右值对待。**

##### 各种典型例子归类

```cpp
3.14          // prvalue
'a'           // prvalue

int x;
x             // lvalue

int x = 0;
int& r = x;
r             // lvalue

++x           // lvalue
x++           // prvalue

*ptr          // lvalue

arr[0]        // lvalue
vec[0]        // 通常是 lvalue（若返回引用）

T f();        // f() 是 prvalue
T& g();       // g() 是 lvalue
T&& h();      // h() 是 xvalue

std::string("abc")   // prvalue

std::move(x)         // xvalue
```

在 **C++** 里 `abc` 的类型是 `const char[4]`，并且它是**左值**。

##### 左值 / 右值和引用绑定规则

左值引用 `T&` 只能绑左值；`const T&` 能绑左值，也能绑右值；右值引用 `T&&` 只能绑右值。

## 8. 资源管理、移动语义与智能指针

### `std::move()`

移动完对象还活着，但状态变成已移动；只能安全地析构、重新赋值、再初始化，别依赖原值。

`std::move` 只是把参数强转成右值引用。

- `T&` → `T&&`，通常触发移动
- `const T&` → `const T&&`，通常不能走移动构造/赋值，多半退回拷贝
- `T&&` → 还是 `T&&`
- 临时对象本来就是右值，通常不需要 `std::move`

#### class 的示例

```cpp
class Dynarray {
  std::size_t n;
  int* p;
public:
  //构造函数
  Dynarray() : n(0), p(nullptr) {}
  explicit Dynarray(std::size_t n_) : n(n_), p(n_ ? new int[n_]{} : nullptr) {}
  Dynarray(std::size_t n_, int val) : n(n_), p(n_ ? new int[n_] : nullptr) {
    for (std::size_t i = 0; i < n; ++i) p[i] = val;
  }
  Dynarray(const Dynarray& other) : n(other.n), p(n ? new int[n] : nullptr) {
    for (std::size_t i = 0; i < n; ++i) p[i] = other.p[i];
  }
  Dynarray(Dynarray&& other) noexcept : n(other.n), p(other.p) {
    other.n = 0;
    other.p = nullptr;
  }
  //析构函数
  ~Dynarray() {
    delete[] p;
  }
  //拷贝赋值
  Dynarray& operator=(const Dynarray& other) {
    if (this != &other) {
      int* newp = other.n ? new int[other.n] : nullptr;
      for (std::size_t i = 0; i < other.n; ++i) newp[i] = other.p[i];
      delete[] p;
      p = newp;
      n = other.n;
    }
    return *this;
  }
  //移动赋值
  Dynarray& operator=(Dynarray&& other) noexcept {
    if (this != &other) {
        delete[] p;
        p = other.p;
        n = other.n;

        other.p = nullptr;
        other.n = 0;
    }
    return *this;
  }
};
```

## 9. 运算符重载、lambda 与算法

### 重载运算符

```cpp
class X {
  int v;
public:
  X(int x = 0) : v(x) {}
  // 成员版
  X operator+(const X& rhs) const { return X(v + rhs.v); }
  X& operator+=(const X& rhs) { v += rhs.v; return *this; }
  bool operator<(const X& rhs) const { return v < rhs.v; }
  X& operator++() { ++v; return *this; }      // 前置++
  X operator++(int) { X t(*this); ++v; return t; } // 后置++
  int& operator[](std::size_t i) { return arr[i]; } // 若有数组成员
  X& operator=(const X& rhs) { v = rhs.v; return *this; }
  explicit operator bool() const { return v != 0; } // 类型转换
  // 友元版
  friend X operator+(const X& a, const X& b) {
    return X(a.v + b.v);
  }
  friend std::ostream& operator<<(std::ostream& os, const X& x) {
    return os << x.v;
  }
  friend std::istream& operator>>(std::istream& is, X& x) {
    return is >> x.v;
  }
};
```

### lambda 与 `operator()`

`[](int a, int b) -> int { return a + b; }` 等价于 `int operator()(int a, int b) const;`。

`[x]() mutable { x++; }` 等价于 `int operator()() { x++; };`。

`auto f = [] { return 1; };` 中的 `f` 是一个函数对象。真正干活的是 `f.operator()();`。

```cpp
[x] { ... }   // 按值捕获
[&x] { ... }  // 按引用捕获
[=] { ... }   // 全部按值
[&] { ... }   // 全部按引用
```

### `std::sort` / `find`

```cpp
std::sort(v.begin(), v.end(),
          [](const int& a, const int& b) { return a < b; });
```

```cpp
auto it = std::find_if(v.begin(), v.end(),
                       [](int x) { return x % 2 == 0; });
```

### STL 容器

#### 顺序容器

- `vector<T>`：动态连续数组
- `deque<T>`：双端队列，头尾插删快
- `array<T,N>`：固定长度容器版数组，不会 decay
- `list<T>`：双向链表
- `forward_list<T>`：单向链表

#### 统一接口

- `size()` `empty()` `clear()`
- `front()` `back()`
- `push_back()` `pop_back()`
- `insert()` `erase()`
- `begin()` `end()`

注意：

- `array` 不能改长度
- 链表没 `operator[]`
- `forward_list` 无 `size()`

##### `set`

- 有序集合，不允许重复
- 插入/删除/查找：`O(log n)`
- 元素自动有序
- 元素不能直接改值（迭代器解引用得到 `const`）

常见：

- `insert`
- `erase`
- `find`
- `count`

##### `map`

- `map<Key, Value>`：键值对，按 key 有序
- 元素类型：`pair<const Key, Value>`
- `find(key)`：只查找，不插入
- `m[key]`：若 key 不存在，会先插入 `{key, Value{}}` 再返回引用

计数例子：

```cpp
map<string, int> counter;
string word;
while (cin >> word)
    ++counter[word];
```

##### `unordered_*`

- `unordered_set` / `unordered_map`
- 哈希表
- 平均 `O(1)`
- 需要可哈希
