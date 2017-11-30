结果分析
=====

## 配置
![configure](./configure.png)

## 重传率 & 端到端延时
|Type        | Node         | Rate         | Delay     |
|:-----------|:-------------|:-------------|:----------|
|tcp/Thole   | n0  → n15    | 0.053672317  | 0.101725  |
|tcp/Reno    | n1  → n16    | 0.040871933  | 0.102925  |
|tcp/newReno | n2  → n17    | 0.05437352   | 0.104125  |
|tcp/Vegas   | n3  → n18    | 0.0125       | 0.107725  |
|tcp/Sack    | n4  → n19    | 0.07266436   | 0.105325  |
|tcp/Fack    | n10 → n20    | 0.115384616  | 0.106525  |
|            |              |              |           |
|udp         | n5  → n21    | 0.0020181634 | 0.0       |
|            | n6  → n22    | 0.0060544903 | 0.0       |
|            | n7  → n23    | 0.018677436  | 0.0       |
|            | n8  → n24    | 0.050984353  | 0.0       |
|            | n9  → n25    | 0.1207851    | 0.0       |

## 网络抖动
|node|jitter                             |node|jitter                              |
|:--:|:---------------------------------:|:--:|:----------------------------------:|
| n0 |![n0](./jitter_n0.png){width=600px}| n1 |![n0](./jitter_n1.png){width=600px} |
| n2 |![n2](./jitter_n2.png){width=600px}| n3 |![n3](./jitter_n3.png){width=600px} |
| n4 |![n4](./jitter_n4.png){width=600px}| n10|![nA](./jitter_nA.png){width=600px}|

## 吞吐量
|node|throughput                             |node|throughput                             |
|:--:|:-------------------------------------:|:--:|:-------------------------------------:|
| n0 |![n0](./throughput_n0.png){width=600px}| n1 |![n1](./throughput_n1.png){width=600px}|
| n2 |![n2](./throughput_n2.png){width=600px}| n3 |![n3](./throughput_n3.png){width=600px}|
| n4 |![n4](./throughput_n4.png){width=600px}| n5 |![nA](./throughput_nA.png){width=600px}|

## 发送/接收
|node|send/receive                         |node|send/receive                         |
|:--:|:-----------------------------------:|:--:|:-----------------------------------:|
| n0 |![n0](./sendrecv_n0.png){width=600px}| n1 |![n1](./sendrecv_n1.png){width=600px}|
| n2 |![n2](./sendrecv_n2.png){width=600px}| n3 |![n3](./sendrecv_n3.png){width=600px}|
| n4 |![n4](./sendrecv_n4.png){width=600px}| nA |![n5](./sendrecv_nA.png){width=600px}|

## 拥塞窗口
|node|window                               |node|window                               |
|:--:|:-----------------------------------:|:--:|:-----------------------------------:|
|tcp0|![t0](./window_tcp0.png){width=600px}|tcp1|![t1](./window_tcp1.png){width=600px}|
|tcp2|![t2](./window_tcp2.png){width=600px}|tcp3|![t3](./window_tcp3.png){width=600px}|
|tcp4|![t4](./window_tcp4.png){width=600px}|tcp5|![t5](./window_tcp5.png){width=600px}|

