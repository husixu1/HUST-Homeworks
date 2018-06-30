-- part 2 ---------------------------------------------------------------------
-------------------------------------------------------------------------------

system echo '[93m==> 清理表[0m'
drop table if exists 职工;
drop table if exists 部门;
drop view if exists 自己;
drop view if exists 工资;
drop trigger if exists 职工插入触发器;
drop trigger if exists 职工删除触发器;
drop trigger if exists 职工更新触发器;
drop trigger if exists 部门插入触发器;
drop trigger if exists 部门更新触发器;

system echo '[93m==> 清理用户[0m'
drop user if exists 王明@'localhost';
drop user if exists 李勇@'localhost';
drop user if exists 刘星@'localhost';
drop user if exists 张新@'localhost';
drop user if exists 周平@'localhost';
drop user if exists 杨兰@'localhost';

system echo '[93m==> 创建表[0m'
create table if not exists 职工 (
    职工号  nvarchar(20),
    姓名    nvarchar(10),
    年龄    integer,
    职务    nvarchar(20),
    工资    decimal(8,2),
    部门号  nvarchar(20)
);

create table if not exists 部门 (
    部门号  nvarchar(20),
    名称    nvarchar(20),
    经理名  nvarchar(10),
    电话号  nvarchar(30)
);

system echo '[93m==> 定义每个模式的主码[0m'
alter table 职工 add primary key(职工号);
alter table 部门 add primary key(部门号);

system echo '[93m==> 定义参照完整性约束，当部门号改变时，职工记录的部门号相应改变，[0m'
system echo '[93m==> 当部门被删除时，将该部门职工记录的部门号置空[0m'
alter table 职工 add foreign key(部门号) references 部门(部门号)
on update cascade
on delete set null;

system echo '[93m==> 职工年龄不能超过60岁[0m                            '
alter table 职工 add constraint check(年龄<=60);

system echo '[93m==> 职工姓名和部门名称都不允许取空值[0m                '
alter table 职工 modify 姓名 nvarchar(10) not null;
alter table 部门 modify 名称 nvarchar(20) not null;

system echo '[93m==> 部门名称不允许重复[0m'
alter table 部门 add constraint unique(名称);

system echo '[93m==> 设置触发器[0m'

delimiter //
create trigger if not exists 职工插入触发器
before insert
on 职工 for each row
begin
    if(@recurseLock is null) then
        set @recurseLock=1;
        if (new.职务='经理' and new.部门号 is not null) then
            if (exists (select 部门号
                from 职工
                where 部门号=new.部门号 and 职务='经理')) then
                signal sqlstate '45000'
                set message_text='此部门已有经理';
            else
                update 部门 set 经理名=new.姓名 where 部门号=new.部门号;
            end if;
        end if;
        set @recurseLock=null;
    end if;
end;//

create trigger if not exists 职工删除触发器
after delete
on 职工 for each row
begin
    if(@recurseLock is null) then
        set @recurseLock=1;
        update 部门
        set 经理名=null
        where 经理名=old.姓名 and 部门号=old.部门号;
        set @recurseLock=null;
    end if;
end;//

create trigger if not exists 职工更新触发器
after update
on 职工 for each row
begin
    if(@recurseLock is null) then
        set @recurseLock=1;
        if (old.职务 is null or new.职务 is null or
            old.部门号 is null or new.部门号 is null or
            old.职务!=new.职务 or old.部门号!=new.部门号) then
            -- a delete
            update 部门
            set 经理名=null
            where 经理名=old.姓名 and 部门号=old.部门号;
            -- an insert
            -- insert conflit only occurres when 职务 or 部门 updates
            if (new.职务='经理' and new.部门号 is not null) then
                if ((select count(*)
                    from 职工
                    where 部门号=new.部门号 and 职务='经理') > 1) then
                    signal sqlstate '45000'
                    set message_text='此部门已有经理';
                else
                    update 部门 set 经理名=new.姓名 where 部门号=new.部门号;
                end if;
            end if;
        end if;
        set @recurseLock=null;
    end if;
end;//

create trigger if not exists 部门插入触发器
before insert
on 部门 for each row
begin
    if(@recurseLock is null) then
        set @recurseLock=1;
        if (new.经理名 is not null) then
            signal sqlstate '45000'
            set message_text='经理名称必须为空，请在职工表中更新部门经理信息';
        end if;
        set @recurseLock=null;
    end if;
end;//

create trigger if not exists 部门更新触发器
after update
on 部门 for each row
begin
    if(@recurseLock is null) then
        set @recurseLock=1;
        if (old.经理名 is null or new.经理名 is null or old.经理名!=new.经理名) then
            signal sqlstate '45000'
            set message_text='经理名一旦修改即会与职工表冲突，请修改职工表';
        end if;
        set @recurseLock=null;
    end if;
end;//

delimiter ;

system echo '[93m==> 插入值[0m'
insert into 部门 values
('001', '管理', null, '13159322791'),
('002', 'IT'  , null, '13342816459'),
('003', '人力', null, '13947814621'),
('004', '销售', null, '13966241962');

insert into 职工 values
('001', '蔡荣', 20, '经理', 5483.54, '001'),
('002', '廖宁', 35, '员工', 6599.65, '001'),
('003', '孟羚', 39, '员工', 5886.58, '001'),

('004', '高恒', 24, '经理', 5784.57, '002'),
('005', '王光', 44, '员工', 5176.51, '002'),
('006', '廖炎', 55, '员工', 6326.63, '002'),

('007', '谢承', 22, '经理', 6113.61, '003'),
('008', '姚碫', 43, '员工', 6481.64, '003'),
('009', '卢旭', 39, '员工', 5446.54, '003'),

('010', '余棠', 28, '经理', 5320.53, '004'),
('018', '常汉', 48, '员工', 5703.57, '004'),
('012', '白孜', 19, '员工', 6834.68, '004');

