-- part 1 ---------------------------------------------------------------------
-------------------------------------------------------------------------------

system echo '[93m==> 清理表[0m'
drop table if exists 职工;
drop table if exists 部门;
drop view  if exists 自己;
drop view  if exists 工资;

system echo '[93m==> 清理用户[0m'
drop user if exists 王明@'localhost';
drop user if exists 李勇@'localhost';
drop user if exists 刘星@'localhost';
drop user if exists 张新@'localhost';
drop user if exists 周平@'localhost';
drop user if exists 杨兰@'localhost';

system echo '[93m==> 创建表[0m'
create table if not exists 部门 (
    部门号  nvarchar(20) not null primary key,
    名称    nvarchar(20) not null,
    经理名  nvarchar(10),
    地址    nvarchar(50),
    电话号  nvarchar(30)
);

create table if not exists 职工 (
    职工号  nvarchar(20) not null primary key,
    姓名    nvarchar(10) not null,
    年龄    integer      not null,
    职务    nvarchar(20),
    工资    decimal(8,2) default 0.0,
    部门号  nvarchar(20),
    foreign key (部门号) references 部门(部门号)

);

system echo '[93m==> 创建用户[0m'
create user if not exists 王明@'localhost';
create user if not exists 李勇@'localhost';
create user if not exists 刘星@'localhost';
create user if not exists 张新@'localhost';
create user if not exists 周平@'localhost';
create user if not exists 杨兰@'localhost';

system echo '[93m==> 1. 用户王明对两个表有SELECT权限[0m'
grant select on db_lab2.职工 to 王明@'localhost';
grant select on db_lab2.部门 to 王明@'localhost';

system echo '[93m==> 2. 用户李勇对两个表都有INSERT和DELETE权限[0m'
grant insert, delete on 职工 to 李勇@'localhost';
grant insert, delete on 部门 to 李勇@'localhost';

system echo '[93m==> 3. 每个职工对于自己的记录有SELECT权限[0m'
create view if not exists 自己 as (
    select *
    from 职工
    where 姓名=substr(user(), 1, instr(user(), '@') - 1)
);

grant select on 自己 to 王明@'localhost';
grant select on 自己 to 李勇@'localhost';
grant select on 自己 to 刘星@'localhost';
grant select on 自己 to 张新@'localhost';
grant select on 自己 to 周平@'localhost';
grant select on 自己 to 杨兰@'localhost';

system echo '[93m==> 4. 用户刘星对自职工表有SELECT权限，对工资字段有更新权限[0m'
grant select, update(工资) on 职工 to 刘星@'localhost';

system echo '[93m==> 5. 用户张新具有修改这两个表的结构的权限'
grant alter on 职工 to 张新@'localhost';
grant alter on 部门 to 张新@'localhost';

system echo '[93m==> 6. 用户周平具有对两个表的所有权限，并可给其他用户权限[0m'
grant all privileges on 职工 to 周平@'localhost' with grant option;

system echo '[93m==> 7. 用户杨兰具有从每个部门职工中SELECT最高工资，最低工资，平均工资的权限，但他不能查看每个人的工资[0m'
create view if not exists 工资 as (
    select 职工.部门号, 名称, max(工资), min(工资), avg(工资)
    from 职工 inner join 部门
    on 职工.部门号=部门.部门号
    group by 职工.部门号
);
grant select on 工资 to 杨兰@'localhost';

system echo '[93m==> 插入值[0m'
insert into 部门 values
('001', '管理', '李勇', '珞喻路1038号', '13159322791'),
('002', '研发', '刘星', '珞喻路1039号', '13342816459'),
('003', '销售', '周平', '珞喻路1040号', '13947814621'),
('004', '人力', '张新', '珞喻路1041号', '13966241962'),
('005', '产品', '周平', '珞喻路1042号', '13846834591');

insert into 职工 values
('001', '王明', 23, '研究员',   2300.01, '001'),
('002', '李勇', 26, '经理',     4547.42, '001'),
('003', '刘星', 37, '工程师',   2234.51, '002'),
('004', '张新', 45, '咨询师',   4394.35, '004'),
('005', '周平', 25, '产品经理', 3388.45, '003'),
('006', '杨兰', 42, '总监',     4342.33, '005');

