# 干掉PC1
在PC1直接连接的router上配置
```
enable
config terminal

access-list 1 deny 192.168.1.100 0.0.0.255  # deny pc in match list
access-list 1 permit any                    # allow any others

interface <interface-connected-to-PC1>
ip access-group 1 in                        # apply list to the interface
```

# 只干掉PC1 <-> PC2
在PC2直接连接的router上配置
```
enable
config terminal

access-list 1 deny host 192.168.1.100 # deny PC1 (poor PC1 ... )
access-list 1 permit any              # allow any others

interface <interface-connected-to-PC2>
ip accress-group 1 out
```
and DONE!
