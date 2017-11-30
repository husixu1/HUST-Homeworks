# This script is created by ck

#===================================
#     Simulation parameters setup
#===================================
set val(stop)   10.0                         ;# time of simulation end

#===================================
#        Initialization        
#===================================
#Create a ns simulator
set ns [new Simulator]

#定义不同数据流的颜色
$ns color 0 black  ;#0为默认黑色 
$ns color 1 Blue  ;#1为蓝色 tcp
$ns color 2 Red  ;#2为红色 tahoe
$ns color 3 Yellow  ;#3为黄色 reno
$ns color 4 Pink  ;#4为粉色 newReno
$ns color 5 Purple  ;#4为紫色 vegas
$ns color 6 Orange  ;#4为橙色 sack
#Open the NS trace file
set tracefile [open out.tr w]
$ns trace-all $tracefile

##拥塞窗口文件#阈值窗口文件
set Cwinfile0 [open CwinFile0 w]
set winfile0 [open ThreshFile0 w]

set Cwinfile1 [open CwinFile1 w]
set winfile1 [open ThreshFile1 w]

set Cwinfile2 [open CwinFile2 w]
set winfile2 [open ThreshFile2 w]

set Cwinfile3 [open CwinFile3 w]
set winfile3 [open ThreshFile3 w]

set Cwinfile4 [open CwinFile4 w]
set winfile4 [open ThreshFile4 w]

set Cwinfile5 [open CwinFile5 w]
set winfile5 [open ThreshFile5 w]

#Open the NAM trace file
set namfile [open out.nam w]
$ns namtrace-all $namfile

#===================================
#        Nodes Definition        
#===================================
#Create 28 nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]
set n7 [$ns node]
set n8 [$ns node]
set n9 [$ns node]
set n10 [$ns node]
set n11 [$ns node]
set n12 [$ns node]
set n13 [$ns node]
set n14 [$ns node]
set n15 [$ns node]
set n16 [$ns node]
set n17 [$ns node]
set n18 [$ns node]
set n19 [$ns node]
set n20 [$ns node]
set n21 [$ns node]
set n22 [$ns node]
set n23 [$ns node]
set n24 [$ns node]
set n25 [$ns node]
set n26 [$ns node]
set n27 [$ns node]

#===================================
#        Links Definition        
#===================================
#Createlinks between nodes


$ns duplex-link $n0 $n6 100.0Mb 10ms DropTail
$ns queue-limit $n0 $n6 50


$ns duplex-link $n6 $n1 100.0Mb 10ms DropTail
$ns queue-limit $n6 $n1 50


$ns duplex-link $n2 $n6 100.0Mb 10ms DropTail
$ns queue-limit $n2 $n6 50


$ns duplex-link $n4 $n6 100.0Mb 10ms DropTail
$ns queue-limit $n4 $n6 50


$ns duplex-link $n3 $n6 100.0Mb 10ms DropTail
$ns queue-limit $n3 $n6 50


$ns duplex-link $n5 $n6 100.0Mb 10ms DropTail
$ns queue-limit $n5 $n6 50


$ns duplex-link $n7 $n12 100.0Mb 10ms DropTail
$ns queue-limit $n7 $n12 50


$ns duplex-link $n8 $n12 100.0Mb 10ms DropTail
$ns queue-limit $n8 $n12 50


$ns duplex-link $n9 $n12 100.0Mb 10ms DropTail
$ns queue-limit $n9 $n12 50


$ns duplex-link $n12 $n10 100.0Mb 10ms DropTail
$ns queue-limit $n12 $n10 50


$ns duplex-link $n11 $n12 100.0Mb 10ms DropTail
$ns queue-limit $n11 $n12 50


$ns duplex-link $n13 $n12 100.0Mb 10ms DropTail
$ns queue-limit $n13 $n12 50


$ns duplex-link $n6 $n13 100.0Mb 10ms DropTail
$ns queue-limit $n6 $n13 50


$ns duplex-link $n13 $n14 10.0Mb 10ms DropTail
$ns queue-limit $n13 $n14 50


$ns duplex-link $n15 $n14 100.0Mb 10ms DropTail
$ns queue-limit $n15 $n14 50


$ns duplex-link $n14 $n22 100.0Mb 10ms DropTail
$ns queue-limit $n14 $n22 50


$ns duplex-link $n16 $n15 100.0Mb 10ms DropTail
$ns queue-limit $n16 $n15 50


$ns duplex-link $n15 $n17 100.0Mb 10ms DropTail
$ns queue-limit $n15 $n17 50


$ns duplex-link $n18 $n15 100.0Mb 10ms DropTail
$ns queue-limit $n18 $n15 50


$ns duplex-link $n19 $n15 100.0Mb 10ms DropTail
$ns queue-limit $n19 $n15 50


$ns duplex-link $n15 $n20 100.0Mb 10ms DropTail
$ns queue-limit $n15 $n20 50


$ns duplex-link $n21 $n15 100.0Mb 10ms DropTail
$ns queue-limit $n21 $n15 50


$ns duplex-link $n22 $n23 100.0Mb 10ms DropTail
$ns queue-limit $n22 $n23 50


$ns duplex-link $n22 $n24 100.0Mb 10ms DropTail
$ns queue-limit $n22 $n24 50


$ns duplex-link $n22 $n25 100.0Mb 10ms DropTail
$ns queue-limit $n22 $n25 50


$ns duplex-link $n22 $n26 100.0Mb 10ms DropTail
$ns queue-limit $n22 $n26 50


$ns duplex-link $n27 $n22 100.0Mb 10ms DropTail
$ns queue-limit $n27 $n22 50

#Give node position (for NAM)
$ns duplex-link-op $n0 $n6 orient right-down
$ns duplex-link-op $n6 $n1 orient left-up
$ns duplex-link-op $n2 $n6 orient right
$ns duplex-link-op $n4 $n6 orient right-up
$ns duplex-link-op $n3 $n6 orient right-up
$ns duplex-link-op $n5 $n6 orient right-up
$ns duplex-link-op $n7 $n12 orient right-down
$ns duplex-link-op $n8 $n12 orient right-down
$ns duplex-link-op $n9 $n12 orient right
$ns duplex-link-op $n12 $n10 orient left-down
$ns duplex-link-op $n11 $n12 orient right-up
$ns duplex-link-op $n13 $n12 orient left-down
$ns duplex-link-op $n6 $n13 orient right-down
$ns duplex-link-op $n13 $n14 orient right
$ns duplex-link-op $n15 $n14 orient left-down
$ns duplex-link-op $n14 $n22 orient right-down
$ns duplex-link-op $n16 $n15 orient left-down
$ns duplex-link-op $n15 $n17 orient right-up
$ns duplex-link-op $n18 $n15 orient left-down
$ns duplex-link-op $n19 $n15 orient left

$ns duplex-link-op $n15 $n20 orient right-down
$ns duplex-link-op $n21 $n15 orient left-up
$ns duplex-link-op $n22 $n23 orient right-up
$ns duplex-link-op $n22 $n24 orient right-up
$ns duplex-link-op $n22 $n25 orient right
$ns duplex-link-op $n22 $n26 orient right-down
$ns duplex-link-op $n27 $n22 orient left-up

#===================================
#        Agents Definition        
#===================================
#Setup a TCP connection
set tcp0 [new Agent/TCP]
$ns attach-agent $n0 $tcp0
set sink16 [new Agent/TCPSink]
$ns attach-agent $n16 $sink16
$ns connect $tcp0 $sink16
	$tcp0 set packetSize_ 1500.0
	$tcp0 set fid_ 0
	$sink16 set packetSize_ 1500.0

#Setup a TCP connection
set tcp1 [new Agent/TCP]
$ns attach-agent $n1 $tcp1
set sink17 [new Agent/TCPSink]
$ns attach-agent $n17 $sink17
$ns connect $tcp1 $sink17
	$tcp1 set packetSize_ 1500.0
	$tcp1 set fid_ 0
	$sink17 set packetSize_ 1500.0

#Setup a TCP connection
set tcp2 [new Agent/TCP]
$ns attach-agent $n2 $tcp2
set sink18 [new Agent/TCPSink]
$ns attach-agent $n18 $sink18
$ns connect $tcp2 $sink18
	$tcp2 set packetSize_ 1500.0
	$tcp2 set fid_ 0
	$sink18 set packetSize_ 1500.0

#Setup a TCP connection
set tcp3 [new Agent/TCP]
$ns attach-agent $n3 $tcp3
set sink19 [new Agent/TCPSink]
$ns attach-agent $n19 $sink19
$ns connect $tcp3 $sink19
	$tcp3 set packetSize_ 1500.0
	$tcp3 set fid_ 0
	$sink19 set packetSize_ 1500.0

#Setup a TCP connection
set tcp4 [new Agent/TCP]
$ns attach-agent $n4 $tcp4
set sink20 [new Agent/TCPSink]
$ns attach-agent $n20 $sink20
$ns connect $tcp4 $sink20
	$tcp4 set packetSize_ 1500.0
	$tcp4 set fid_ 0
	$sink20 set packetSize_ 1500.0

#Setup a TCP connection
set tcp5 [new Agent/TCP]
$ns attach-agent $n5 $tcp5
set sink21 [new Agent/TCPSink]
$ns attach-agent $n21 $sink21
$ns connect $tcp5 $sink21
	$tcp5 set packetSize_ 1500.0
	$tcp5 set fid_ 0
	$sink21 set packetSize_ 1500.0

#Setup a UDP connection
set udp6 [new Agent/UDP]
$ns attach-agent $n7 $udp6
set null11 [new Agent/Null]
$ns attach-agent $n23 $null11
$ns connect $udp6 $null11
	$udp6 set packetSize_ 1500.0

#Setup a UDP connection
set udp7 [new Agent/UDP]
$ns attach-agent $n8 $udp7
set null12 [new Agent/Null]
$ns attach-agent $n24 $null12
$ns connect $udp7 $null12
	$udp7 set packetSize_ 1500.0

#Setup a UDP connection
set udp8 [new Agent/UDP]
$ns attach-agent $n9 $udp8
set null13 [new Agent/Null]
$ns attach-agent $n25 $null13
$ns connect $udp8 $null13
	$udp8 set packetSize_ 1500.0

#Setup a UDP connection
set udp9 [new Agent/UDP]
$ns attach-agent $n10 $udp9
set null14 [new Agent/Null]
$ns attach-agent $n26 $null14
$ns connect $udp9 $null14
	$udp9 set packetSize_ 1500.0

#Setup a UDP connection
set udp10 [new Agent/UDP]
$ns attach-agent $n11 $udp10
set null15 [new Agent/Null]
$ns attach-agent $n27 $null15
$ns connect $udp10 $null15
	$udp10 set packetSize_ 1500.0


#===================================
#        Applications Definition        
#===================================
#Setup a FTP Application over TCP connection
set ftp6 [new Application/FTP]
$ftp6 attach-agent $tcp0
$ns at 0.0 "$ftp6 start"
$ns at 10.0 "$ftp6 stop"

#Setup a FTP Application over TCP connection
set ftp7 [new Application/FTP]
$ftp7 attach-agent $tcp1
$ns at 0.0 "$ftp7 start"
$ns at 10.0 "$ftp7 stop"

#Setup a FTP Application over TCP connection
set ftp8 [new Application/FTP]
$ftp8 attach-agent $tcp2
$ns at 0.0 "$ftp8 start"
$ns at 10.0 "$ftp8 stop"

#Setup a FTP Application over TCP connection
set ftp9 [new Application/FTP]
$ftp9 attach-agent $tcp3
$ns at 0.0 "$ftp9 start"
$ns at 10.0 "$ftp9 stop"

#Setup a FTP Application over TCP connection
set ftp10 [new Application/FTP]
$ftp10 attach-agent $tcp4
$ns at 0.0 "$ftp10 start"
$ns at 10.0 "$ftp10 stop"

#Setup a FTP Application over TCP connection
set ftp11 [new Application/FTP]
$ftp11 attach-agent $tcp5
$ns at 0.0 "$ftp11 start"
$ns at 10.0 "$ftp11 stop"

#Setup a CBR Application over UDP connection
set cbr12 [new Application/Traffic/CBR]
$cbr12 attach-agent $udp6
	$cbr12 set packetSize_ 1000
	$cbr12 set rate_ 1.0Mb
	$cbr12 set interval_ 0.005
	$cbr12 set maxpkts_ 2000.0
	$cbr12 set random_ false
$ns at 0.0 "$cbr12 start"
$ns at 10.0 "$cbr12 stop"

#Setup a CBR Application over UDP connection
set cbr13 [new Application/Traffic/CBR]
$cbr13 attach-agent $udp7
	$cbr13 set packetSize_ 1000
	$cbr13 set rate_ 1.0Mb
	$cbr13 set interval_ 0.005
	$cbr13 set maxpkts_ 2000.0
	$cbr13 set random_ false
$ns at 0.0 "$cbr13 start"
$ns at 10.0 "$cbr13 stop"

#Setup a CBR Application over UDP connection
set cbr14 [new Application/Traffic/CBR]
$cbr14 attach-agent $udp8
	$cbr14 set packetSize_ 1000
	$cbr14 set rate_ 1.0Mb
	$cbr14 set interval_ 0.005
	$cbr14 set maxpkts_ 2000.0
	$cbr14 set random_ false
$ns at 0.0 "$cbr14 start"
$ns at 10.0 "$cbr14 stop"

#Setup a CBR Application over UDP connection
set cbr15 [new Application/Traffic/CBR]
$cbr15 attach-agent $udp9
	$cbr15 set packetSize_ 1000
	$cbr15 set rate_ 1.0Mb
	$cbr15 set interval_ 0.005
	$cbr15 set maxpkts_ 2000.0
	$cbr15 set random_ false
$ns at 0.0 "$cbr15 start"
$ns at 10.0 "$cbr15 stop"

#Setup a CBR Application over UDP connection
set cbr16 [new Application/Traffic/CBR]
$cbr16 attach-agent $udp10
	$cbr16 set packetSize_ 1000
	$cbr16 set rate_ 1.0Mb
	$cbr16 set interval_ 0.005
	$cbr16 set maxpkts_ 2000.0
	$cbr16 set random_ false
$ns at 0.0 "$cbr16 start"
$ns at 10.0 "$cbr16 stop"


#===================================
#        Termination        
#===================================
#Define a 'finish' procedure
proc finish {} {
    global ns tracefile namfile
    $ns flush-trace
    close $tracefile
    close $namfile
    exec nam out.nam &
    exit 0
}

#===================================
#        create congestion window        
#===================================
proc plotWindow {tcpSource file_Cwnd file_wnd} {
    global ns
#设置抽样距离，每过0.1s后再调用自己
    set time 0.1
    set now [$ns now]
#拥塞窗口
    set cwnd [$tcpSource set cwnd_]
#通告窗口
    set wnd [$tcpSource set ssthresh_]
#把当前时间数据和cwnd的数据记录到file_Cwnd所指向的文件中
    puts $file_Cwnd "$now $cwnd"
#把当前时间数据和wnd的数据记录到file_wnd所指向的文件中
    puts $file_wnd "$now $wnd"
#这是一个递归过程，在过了0.1秒后继续调用本函数，并记录时间数据和cwnd数据
    $ns at [expr $now+$time] "plotWindow $tcpSource $file_Cwnd $file_wnd"
}
#===================================
#调用函数捕获tcp0的窗口数据
$ns at 0.1 "plotWindow $tcp0 $Cwinfile0 $winfile0"
$ns at 0.1 "plotWindow $tcp1 $Cwinfile1 $winfile1"
$ns at 0.1 "plotWindow $tcp2 $Cwinfile2 $winfile2"
$ns at 0.1 "plotWindow $tcp3 $Cwinfile3 $winfile3"
$ns at 0.1 "plotWindow $tcp4 $Cwinfile4 $winfile4"
$ns at 0.1 "plotWindow $tcp5 $Cwinfile5 $winfile5"
$ns at $val(stop) "$ns nam-end-wireless $val(stop)"
$ns at $val(stop) "finish"
$ns at $val(stop) "puts \"done\" ; $ns halt"
$ns run
