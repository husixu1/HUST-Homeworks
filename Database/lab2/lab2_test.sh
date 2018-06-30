#!/bin/bash

dbName=db_lab2
rootUserName=dblab
rootPassword=dblabdblab
script_part1=lab2_part1.sql
script_part2=lab2_part2.sql
script_part3=lab2_part3.sql
currentWindow=$(xdotool getactivewindow)
inputDelay=80 # this value should be greater than 60 or someting will be wrong

# if pause between each test
nopause=
while (( $# )); do
    if [[ $1 == "--nopause" ]]; then
        nopause=true
    fi
    shift
done

# delay 1 sec and automatically type a command
typeCommand(){
    sleep 0.3;
    xdotool type --window "${currentWindow}" --delay ${inputDelay} "$1"
    xdotool key --window "${currentWindow}" --delay ${inputDelay} Return
}

# Usage: sqlRunCommandAndExit [username [password]] <command>
# start mysql with username and password, and type command into it in another
# thread, then exit
sqlRunCommandAndExit(){
    if [[ $# == 1 ]]; then
        typeCommand "$1; exit;" &
        mysql -s -t --prompt="[\\u] >" -D "${dbName}" -u"${rootUserName}" -p"${rootPassword}"
    elif [[ $# == 3 ]]; then
        typeCommand "$3; exit;" &
        mysql -s -t --prompt="[\\u] >" -D "${dbName}" -u"$1" -p"$2"
    elif [[ $# == 2 ]]; then
        typeCommand "$2; exit;" &
        mysql -s -t --prompt="[\\u] >" -D "${dbName}" -u"$1"
    else
        echo "==WRONG COMMAND=="
    fi
    wait
}

pause() {
    if [[ ! ${nopause} ]]; then
        read -r
    else
        echo ""
    fi
}

test_lab1() {
tput rmam
echo \
"[96m-- PART I -------------------------------------------------------------\
-------------------------------------------------------------------------[0m"
tput smam
sqlRunCommandAndExit "${rootUserName}" "${rootPassword}" "source ${script_part1}"

tput rmam
echo \
"[96m-- PART I TEST --------------------------------------------------------\
-------------------------------------------------------------------------[0m"
tput smam
echo -n "[1m[32m==> test-1. 用户王明对两个表有SELECT权限[0m"
pause
sqlRunCommandAndExit '王明' "select * from 职工"
sqlRunCommandAndExit '王明' "select * from 部门"

echo -n "[1m[32m==> test-2. 用户李勇对两个表都有INSERT和DELETE权限[0m"
pause
sqlRunCommandAndExit '李勇' 'insert into 职工 values("test", "测试", 18, "开发", 1200.0, "001")'
sqlRunCommandAndExit "${rootUserName}" "${rootPassword}" 'select * from 职工'
sqlRunCommandAndExit '李勇' 'delete from 职工'
sqlRunCommandAndExit "${rootUserName}" "${rootPassword}" 'select * from 职工'
sqlRunCommandAndExit "${rootUserName}" "${rootPassword}" "source ${script_part1}"

echo -n "[1m[32m==> test-3. 每个职工对于自己的记录有SELECT权限[0m"
pause
sqlRunCommandAndExit '王明' 'select * from 自己'
sqlRunCommandAndExit '李勇' 'select * from 自己'
sqlRunCommandAndExit '刘星' 'select * from 自己'
sqlRunCommandAndExit '张新' 'select * from 自己'

echo -n "[1m[32m==> test-4. 用户刘星对自职工表有SELECT权限，对工资字段有更新权限[0m"
pause
sqlRunCommandAndExit '刘星' 'select * from 职工'
sqlRunCommandAndExit '刘星' 'update 职工 set 工资=1234 where 职工号="001"'
sqlRunCommandAndExit '刘星' 'select * from 职工'

echo -n "[1m[32m==> test-5. 用户张新具有修改这两个表的结构的权限[0m"
pause
sqlRunCommandAndExit '张新' 'alter table 职工 add column test integer'
sqlRunCommandAndExit "${rootUserName}" "${rootPassword}" 'select * from 职工'
sqlRunCommandAndExit '张新' 'alter table 职工 drop column test'
sqlRunCommandAndExit '张新' 'alter table 部门 add column test integer'
sqlRunCommandAndExit "${rootUserName}" "${rootPassword}" 'select * from 部门'
sqlRunCommandAndExit '张新' 'alter table 部门 drop column test'

echo -n "[1m[32m==> test-6. 用户周平具有对两个表的所有权限，并可给其他用户权限[0m"
pause
sqlRunCommandAndExit '周平' 'grant all privileges on db_lab2.职工 to 王明@"localhost"'
sqlRunCommandAndExit '王明' 'update 职工 set 工资=4567 where 职工号="001"'
sqlRunCommandAndExit '周平' 'select * from 职工'
sqlRunCommandAndExit '周平' 'revoke all privileges on db_lab2.职工 from 王明@"localhost"'

echo -n "[1m[32m==> test-7. 用户杨兰具有从每个部门职工中SELECT最高工资，最低工资，平均工资的权限，但他不能查看每个人的工资[0m"
pause
sqlRunCommandAndExit '杨兰' 'select * from 工资'
sqlRunCommandAndExit '杨兰' 'select 工资 from 职工'
}


test_lab2 () {

tput rmam
echo \
"[96m-- PART II ------------------------------------------------------------\
-------------------------------------------------------------------------[0m"
tput smam

pause
sqlRunCommandAndExit "source ${script_part2}"

tput rmam
echo \
"[96m-- PART II TEST -------------------------------------------------------\
-------------------------------------------------------------------------[0m"
tput smam
sqlRunCommandAndExit 'select * from 职工'
sqlRunCommandAndExit 'select * from 部门'

echo -n "[1m[32m==> test 参照完整性约束。[0m"
pause
sqlRunCommandAndExit 'update 部门 set 部门号="999" where 部门号="004"'
sqlRunCommandAndExit 'select * from 职工'
sqlRunCommandAndExit 'update 部门 set 部门号="004" where 部门号="999"'

echo -n "[1m[32m==> test 职工年龄不能超过60岁。[0m"
pause
sqlRunCommandAndExit 'update 职工 set 年龄=90 where 职工号="001"'

echo -n "[1m[32m==> test 职工姓名和部门名称都不允许取空值。[0m"
pause
sqlRunCommandAndExit 'insert into 职工 values("999", null, 12, "test", 123.00, "001")'
sqlRunCommandAndExit 'insert into 部门 values("999", null, null, "123456789")'

echo -n "[1m[32m==> test 部门名称不允许重复。[0m"
pause
sqlRunCommandAndExit 'insert into 部门 values("999", "管理", null, "123456789")'

echo -n "[1m[32m==> test 同一个部门不应该有一个以上的经理[0m"
pause
sqlRunCommandAndExit 'insert into 职工 values("999", "test", 12, "经理", 123.00, "001")'
sqlRunCommandAndExit 'update 职工 set 职务="经理" where 职工号="002"'


echo -n "[1m[32m==> test 部门插入测试[0m"
pause
sqlRunCommandAndExit 'insert into 部门 values("100", "测试部", "白孜", "123456789")'
sqlRunCommandAndExit 'insert into 部门 values("100", "测试部", null, "123456789")'

echo -n "[1m[32m==> test 部门更新测试[0m"
pause
sqlRunCommandAndExit 'update 部门 set 经理名="白孜" where 部门号="100"'

echo -n "[1m[32m==> test 职工插入测试[0m"
pause
sqlRunCommandAndExit 'insert into 职工 values("100", "测试1", 30, "员工", 5000.00, "100")'
sqlRunCommandAndExit 'insert into 职工 values("101", "测试2", 30, "经理", 6000.00, "100")'

echo -n "[1m[32m==> test 职工更新测试[0m"
pause
sqlRunCommandAndExit 'update 职工 set 职务="经理" where 职工号="100"'
sqlRunCommandAndExit 'update 职工 set 部门号="001" where 职工号="100"'
sqlRunCommandAndExit 'update 职工 set 部门号="001" where 职工号="101"'
sqlRunCommandAndExit 'select * from 职工'
sqlRunCommandAndExit 'select * from 部门'

echo -n "[1m[32m==> test 部门删除测试[0m"
pause
sqlRunCommandAndExit 'delete from 部门 where 部门号="100"'
sqlRunCommandAndExit 'select * from 职工'
sqlRunCommandAndExit 'select * from 部门'

echo -n "[1m[32m==> test 职工删除测试[0m"
pause
sqlRunCommandAndExit 'delete from 职工 where 职工号="100"'
sqlRunCommandAndExit 'delete from 职工 where 职工号="101"'
sqlRunCommandAndExit 'select * from 职工'

}

test_lab3() {
tput rmam
echo \
"[96m-- PART III -----------------------------------------------------------\
-------------------------------------------------------------------------[0m"
tput smam
pause
sqlRunCommandAndExit "source ${script_part3}"

tput rmam
echo \
"[96m-- PART III test ------------------------------------------------------\
-------------------------------------------------------------------------[0m"
tput smam

echo -n "[1m[32m==> test 平均工资测试[0m"
pause
sqlRunCommandAndExit 'call 获得平均工资("管理", @管理_平均工资); select @管理_平均工资'
sqlRunCommandAndExit 'call 获得平均工资("IT"  , @IT_平均工资);   select @IT_平均工资'
sqlRunCommandAndExit 'call 获得平均工资("人力", @人力_平均工资); select @人力_平均工资'
sqlRunCommandAndExit 'call 获得平均工资("销售", @销售_平均工资); select @销售_平均工资'
sqlRunCommandAndExit 'select 部门号,avg(工资) from 职工 group by 部门号'

echo -n "[1m[32m==> test 加薪测试[0m"
pause
sqlRunCommandAndExit 'select * from 职工'
sqlRunCommandAndExit 'call 加薪'
sqlRunCommandAndExit 'select * from 职工'

}

test_lab1
test_lab2
test_lab3

