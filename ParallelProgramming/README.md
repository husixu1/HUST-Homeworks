* Please use UTF-8 encoding to display the chinese part.

## 概要
此project分别使用串行、pthread并行、openmp并行、openmpi并行以及cuda并行对于图像进行腐蚀和膨胀操作。并使用openmp并行对于数独求解进行加速。

## 目录结构
```
./                          project根目录
├── README.md               README文件（本文件）
├── Makefile                主Makefile，用于同时编译/清除所有lab
├── data                    数据目录
│   ├── Lenna.png           lab1 ~ lab5 所用图片
│   └── questions.txt       lab6 所用数独数据
├── lab1                    串行形态学图像处理
│   ├── Makefile            lab1 Makefile
│   └── main.cpp            lab1 源码
├── lab2                    使用pthread加速的多线程图像处理
│   ├── Makefile            lab2 Makefile
│   └── main.cpp            lab2 源码
├── lab3                    使用openmp加速的多线程图像处理
│   ├── Makefile            lab3 Makefile
│   └── main.cpp            lab3 源码
├── lab4                    使用openmpi加速的多线程图像处理
│   ├── Makefile            lab4 Makefile
│   ├── main.cpp            lab4 源码
│   └── run.sh              openmpi 运行环境的包装，使用mpirun执行编译输出
├── lab5                    使用cuda加速的多线程图像处理
│   ├── Makefile            lab5 Makefile
│   └── main.cu             lab5 源码
├── project1                数独求解程序
│   ├── Makefile            lab6 Makefile
│   ├── parallel.cpp        使用openmp加速的并行数独求解程序源码
│   └── serial.cpp          串行数独求解程序源码
├── lib                     第三方库文件夹
│   └── CImg-2.3.3          CImg图像处理库
│       └── ...             CImg图像处理库内容
└── report                  报告文件夹
        └── ...             报告内容
```

### 编译及运行说明
要编译所有的lab，需要在x64 Linux平台下编译，并满足以下依赖条件：

| 项目   |版本     |
|-------:|:--------|
| gcc    | >=8.1.1 |
| Make   | >=4.2.1 |
| openmp | >=6.0.1 |
| openmpi| >=3.1.0 |
| cuda   | >=9.2   |

并且，需要保证`gcc`、`nvcc`所在的目录在`$PATH`中。

要能够正确的编译并运行lab5，需要满足下列要求：
* `nvcc`在`$PATH`中
* PC上装有支持cuda的Nvidia显卡
* 正确版本的Nvidia驱动以正确安装并 **已加载**
* Nvidia显卡有足够的显存以及足够的Concurrent Thread数目以支持程序的运行。

此外，lab2、lab3可以使用`-n <number>`来指定线程数，使用`-s <number>` 来指定图片分块大小 (推荐在8~256之间) ；lab4可以通过更改`run.sh`更改进程数，进程数必须大于1，但不可超过CPU物理内核数。

## 时间统计
推荐使用fish shell自带的时间统计功能。如果使用bash或zsh，请自行编写wrapper script统计时间。为了各程序时间统计上的公平性，所有的程序均不使用程序内的时间统计。

## LICENSE
<a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="知识共享许可协议" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />本作品采用<a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">知识共享署名-非商业性使用 4.0 国际许可协议</a>进行许可。
