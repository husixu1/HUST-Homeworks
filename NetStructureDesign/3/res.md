## Vlan划分
* 通过GUI设置，在switch上，将PC段的interface设置为access并选择所处vlan（2和3），将Switch端设置为trunc并仅允所有刚刚设置的vlan通过。vlan就设置完成了。


* 要各个vlan之间进行通信，需要一个单臂路由链接到这个网络，并在物理interface上派生两个逻辑interface，然后设置对应的vlan和address即可。使用如下命令：
```
enable
config terminal

interface Gi<Tab>0/1        # select interface
interface Gi<Tab>0/1.1      # important step. the digit after '.' indicates the subinterface num
encapsulation dot1Q 2       # set protocol, 2 is vlan nuber
ip address 192.168.1.1 255.255.255.0
no shutdown

## Repeat the last section with vlan 3 and corresponding ip and done!

```
