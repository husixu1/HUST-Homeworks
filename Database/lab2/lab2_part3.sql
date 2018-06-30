source lab2_part2.sql

drop procedure if exists 获得平均工资;
drop procedure if exists 加薪;

system echo '[93m==> 获得平均工资过程[0m'
create procedure if not exists 获得平均工资 (
    in  部门名称 nvarchar(20),
    out 平均工资 decimal(8, 2)
)
select avg(工资) into 平均工资
from 职工 inner join 部门
on 职工.部门号=部门.部门号
where 名称=部门名称;

system echo '[93m==> 加薪过程[0m'
create procedure if not exists 加薪 ()
update 职工 inner join 部门
on 职工.部门号=部门.部门号
set 工资=工资*case
    when 职务='经理' then 1.5
    when 部门.名称='IT' then 1.5
    when 部门.名称='销售' then 1.2
    else 1.1
end;

