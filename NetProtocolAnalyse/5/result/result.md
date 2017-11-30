结果分析
=====

## 配置
![configure](./configure.png)

## 拥塞窗口
| TCP Agent | UDP Pack Size = 250                    | UDP Pack Size = 1000                 | UDP Pack Size = 2000                 |
|:----------|:---------------------------------------|:-------------------------------------|:-------------------------------------|
| tcp0/Tahoe| ![](./window_tcp0_25.png){witdth=600px}| ![](./window_tcp0.png){witdth=600px} |![](./window_tcp0_2.png){witdth=600px}|
| tcp1/Reno | ![](./window_tcp1_25.png){witdth=600px}| ![](./window_tcp1.png){witdth=600px} |![](./window_tcp1_2.png){witdth=600px}|
| tcp2/newR | ![](./window_tcp2_25.png){witdth=600px}| ![](./window_tcp2.png){witdth=600px} |![](./window_tcp2_2.png){witdth=600px}|
| tcp3/Vegas| ![](./window_tcp3_25.png){witdth=600px}| ![](./window_tcp3.png){witdth=600px} |![](./window_tcp3_2.png){witdth=600px}|
| tcp4/Sack | ![](./window_tcp4_25.png){witdth=600px}| ![](./window_tcp4.png){witdth=600px} |![](./window_tcp4_2.png){witdth=600px}|
| tcp5/Fack | ![](./window_tcp5_25.png){witdth=600px}| ![](./window_tcp5.png){witdth=600px} |![](./window_tcp5_2.png){witdth=600px}|


