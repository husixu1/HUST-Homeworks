## RIP 配置
将每个路由器所知道的网络加入RIP配置中即可，路由器会自动扫描对应的interface并进行RIP通信

## OSPF协议配置
### 在ClI界面设置OSPF

```
enable
config terminal
router ospf 1                                                       # 1 is process id (group)
network <address> <wildcard-mask(invert netmask)> area <area-id>    # area id 0 is backbone area
```

### 查看OSPF
```
enable
show ip ospf ?  # to see what can be seen
```
