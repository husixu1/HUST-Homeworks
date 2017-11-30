# C++ Experiment Check System Plus Plus

Due to the lack of functionality and the user-unfriendly interface of the official check system v3, I created this C++ check system, hoping it'll be useful for CS 2015 students in HUST.

## Features
Compared with the old check system v3.0 program, this program has features presented below:

### user interface:
* resizeable: The original program is not resizeable, but this program resizes smoothly.
* bilingual support: This program supports Chinese and English, providing some convenience for those who have difficulties reading Chinese.
* comfortable layout: Borrowed from some websites, I created a comfortable layout for controlling the program.
* intuitive test result: the test result is displayed with colors, providing a quick locate to erroneous answers.

### functionality
* robustness: This program refuse to do tests when conditions are not satisfied. This program won't get stuck even if your program is a dead loop. it simply kill your program after 3 seconds time out.
* load test cases from file: This program provides a functionality to load test cases from file, which means you can custom your test cases without using them just once.
* single shot test case: input, run, get answer. fast.
* cross platform: The original program's source code is not provided, thus it can only run in windows. This program runs on windows, linux, mac osx. The release binaries are statically linked, so it runs [out of the box][1].

## Usage
Simply drag-drop your executable to the box above, and(optionally) drag-drop your custom test case to the box below, then choose a standard program and start testing!

Note: the program will search for `$ROOT/refProg/a.exe`, `$ROOT/refProg/b.exe`, `$ROOT/refProg/c.exe`(where `$ROOT` is the directory containing this program). So if the standard program is missing, simply get them somewhere(maybe from the old check system) and copy them to the corresponding directory. If the directory itself is missing, create one with the name of `refProg`.

You can load test cases from a file, file should not exceeds 1MB. in the file, each line is recognized as a test case, illegal line (lines not matching regexp `/((^|\\s+)-[SIOCAGN](\\s+\\d+\\s*)*)+(#.*)?/` ) will be discarded. contents after '#' will be taken as comment, and will be ignored. a valid sample file is presented below:
```
# this is samplefile.txt
# this line is a comment

-S 1 -I 1 2
-S 6 -I 1 2 -O 2  # this is a test case with comment
-S 4 -I 1 2 3 4 5 # another test case with coment

f!aa@#sad#%$      <-- this line will be discarded
```


## Complie guide
```
1. make sure you have qt5(or above) installed
2. qmake && make
3. I think users should have the ability to solve depenency issue themselves.
4. Why not just use pre-built binary?
```

## Licence
This program is published under [GPLv3][2].

![GPL-Logo](https://www.gnu.org/graphics/gplv3-88x31.png)


---

# C++ 实验检查系统++

由于官方的C++实验检查系统v3.0缺少一些功能，界面也并不非常人性化，我编写了这个C++实验检查系统，希望它会对华中科技大学计算机学院2015级同学有所帮助。

## 功能
与官方的C++实验检查系统相比，这个检查系统有如下的功能：

### 用户界面
* 可调整大小：原检查系统不可调整大小，本程序可以流畅的调整大小。
* 双语支持：提供了英语和中文支持，为阅读中文有些困难的同学提供了些许便利。
* 舒适的布局：借鉴了部分网站的布局，我创建了一个舒适的布局用于控制这个程序。
* 直观的测试结果：测试结果由元素标注，可以快速找到出错的用例。

### 功能性
* 健壮性/鲁棒性：如果条件未得到满足，此程序拒绝将拒绝运行测试。即便你的程序是一个死循环也不会造成本程序的卡死，它将在3秒超时后杀掉你的程序。
* 从文件读取测试用例：本程序提供了从文件读取测试用例的功能，这意味着你可以定制你的测试用例并多次使用他们而不是每次都需要重新输入。
* 单次测试：输入，运行，得到结果，超快！
* 跨平台：原测试程序的源码未提供，所以只能执行它所提供的windows可执行文件。本程序可以在windows，linux，mac osx下运行。本项目发布的二进制是静态链接的，[开箱即用][1]。

## 使用方法
只需把你的可执行文件拖到上面哪个框框里，然后（可选的）把你的测试用例文件拖到下面那个框框里，然后运行测试就好了！

注：本程序会搜索`$ROOT/refProg/a.exe`, `$ROOT/refProg/b.exe`, `$ROOT/refProg/c.exe` （`$ROOT`是本程序所在的文件夹路径）作为标准程序（其输出即为标准输出）。所以如果这些程序不见了，只需在其他地方弄到这些程序（有可能是从官方的测试系统里面），然后放到对应的文件夹下就好了。如果文件夹也没有了，创建一个名为`refProg`的文件夹就行。

你可以从文件加载测试样例，文件大小不应该超过1MB，在文件中，每一行被视作一个测试样例，非法输入（不匹配正则表达式`/((^|\\s+)-[SIOCAGN](\\s+\\d+\\s*)*)+(#.*)?/`的行）会被丢弃。每一行'#'后面的内容会被当作注释并忽略。一个有效的测试样例文件如下：
```
# this is samplefile.txt
# this line is a comment

-S 1 -I 1 2
-S 6 -I 1 2 -O 2  # this is a test case with comment
-S 4 -I 1 2 3 4 5 # another test case with coment

f!aa@#sad#%$      <-- this line will be discarded
```

## 编译指南
```
1. 确保你已安装qt5（及以上）
2. qmake && make
3. 我认为用户应该有能力自己解决依赖问题。
4. 为什么不用编译好的版本呢？
```

## Licence
本程序使用[GPLv3][2]许可.

![GPL-Logo](https://www.gnu.org/graphics/gplv3-88x31.png)


[1]:https://en.wikipedia.org/wiki/Out_of_the_box_(feature)
[2]:https://www.gnu.org/licenses/
