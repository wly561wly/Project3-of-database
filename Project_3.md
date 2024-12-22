# Project 3

主要内容

1. 评价指标
2. 根据评价指标，设计比较方案，具体的比较
3. 提供 openGuass的详细评估，包括优势和劣势
4. 报告和源码应该在 github 上公布

#### 主要评价指标

响应时间：

​		请求完成所需要的时间，通常使用平均响应时间、中位数、90% 等等来衡量；

​		基准测试：1个用户，200-500ms；压力测试：多个用户，2 秒

吞吐量：衡量网络上成功运输的数量，单位是 Byte/s

TPS：系统在一定时间内能够处理的事务，每秒事务数TPS

QPS：每秒的查询率

RPS：系统内能够同时使用的接口

HPS：每一秒的点击率

并发用户数：系统能同时处理的最大用户请求数量

事务：接口、流程都可以是事务；事务控制器

资源利用率：包括CPU使用率、内存使用情况、磁盘I/O 和网络I/O

错误率：测试过程中失败的请求所占的比例

稳定性：在长时间运行下系统性能的波动情况

场景模拟：影响测试规模、时间等等

### 测试内容

​	变量包括了用户数量、数据规模、查询sql语句的复杂度、index的影响、事务间隔级别设置(在测试计划的配置中)等等

```postgresql
DROP TABLE IF EXISTS table1, table2, table3, ...;//测试前先清空
TRUNCATE TABLE table_name;
```



##### 测试方式

​	使用数据生成器，生成不同规模的数据，

#### 基准测试

​	只使用一个用户，进行基本操作，包括不同规模的insert、和各种查询，创建 function，创建 trigger等等

​	比较上面标出的评价指标：响应时间、错误率、资源利用率、吞吐量、TPS，QPS等等

insert 测试

```
INSERT INTO ${test_id} VALUES ('${col1}','${col2}','${col3}','${col4}','${col5}');
聚合报告
D:\Desktop\Project3\test_case\test_1_insert_post.csv
CSV文件设置
D:/Desktop/Project3/test_case/test_insert.csv
test_id,col1,col2,col3,col4,col5
```

select 测试

```
${statement}
D:/Desktop/Project3/test_case/query_csv.csv
statement,value

${statement}, ${s1}
statement,s1,s2

a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w
${a},${b},${c},${d},${e},${f},${g},${h},${i},${j},${k},${l},${m},${n},${o},${p},${q},${r},${s},${t},${u},${v},${w}
```



#### 压力测试



​	

#### Reference

[jmeter中如何对cpu使用率和内存占用率进行压测_jmeter接口压力测试中,cpu和内存使用率如何获取、-CSDN博客](https://blog.csdn.net/m0_73575927/article/details/135082845)





### 其他内容

#### 性能压测脚本的生成

在internet属性中查看ip地址和端口

在线程组下 新建一个 HTTP Cookoe管理器。再在测试计划中 增加一个HTTP代理服务器，在 Request Filtering 下排除模式中，选择 添加建议排除，在末尾加上 `.*`，然后将这个脚本保存，在包含模式中输入 项目的ip；然后设置 测试计划，测试计划->线程组、在组间增加分隔



#### 配置 docker 容器

​	拉取镜像

```
docker pull docker.1panel.live/enmotech/opengauss:3.0.0
docker pull docker.1panel.live/ubuntu/postgres:14-22.04_beta
```

​	docker open-gauss

```
docker run --name project3-opengauss --privileged=true \
    -d -e GS_PASSWORD=<OpenGuass0220#> \
    -v <D:\docker_opengauss>:/var/lib/opengauss  -u root \
    -p 15432:5432 \
    docker.1panel.live/enmotech/opengauss:3.0.0
    
docker run --name opengauss --privileged=true -d -e GS_PASSWORD=!Open0220Gauss@# -v D:\\docker_opengauss:/var/lib/opengauss  -u root -p 15432:5432 docker.1panel.live/enmotech/opengauss:3.0.0  
```

​	docker postgres

```
docker run -d \--name project3-postgres \-e POSTGRES_USER=postgres-e POSTGRES_PASSWORD=<!!!your db password!!!> \-e POSTGRES_DB=postgres-e PGDATA=/var/lib/postgresql/data/pgdata \-p 5432:5432 \-v <!!!persist directory!!!>:/var/lib/postgresql/data \
 docker.1panel.live/ubuntu/postgres:14-22.04_beta
 
docker run -d --name project3-postgres -e POSTGRES_USER=postgres -e POSTGRES_PASSWORD=12345678 -e POSTGRES_DB=postgres -e PGDATA=/var/lib/postgresql/data/pgdata -p 8432:5432 -v D:\docker_postgres:/var/lib/postgresql/data docker.1panel.live/ubuntu/postgres:14-22.04_beta
```



### 云服务器

​	密码和电脑pin一样

#### 操作顺序

	1. 切换至omm操作系统：`[root@ecs-c9bf script]# su - omm `
	1. 服务是否启动

```
[omm@ecs-9a68 ~]$ gs_ctl status -D $GAUSSHOME/data/single_node -Z single_node
[2024-04-03 14:58:17.740][8117][][gs_ctl]: gs_ctl status,datadir is /opt/software/openGauss/data/single_node
gs_ctl: server is running (PID: 6111)
/opt/software/openGauss/bin/gaussdb "-D" "/opt/software/openGauss/data/single_node"
```

3. 启动数据库服务（可选操作，如未启动，请按此步骤启动）

```
[omm@ecs-c9bf ~]$ gs_ctl start -D $GAUSSHOME/data/single_node -Z single_node
……………………………….
[2024-04-03 11:05:22.435][4269][][gs_ctl]:  done
[2024-04-03 11:05:22.435][4269][][gs_ctl]: server started (/opt/software/openGauss/data/single_node)
```

​	停止数据库服务

```
gs_ctl stop -D $GAUSSHOME/data/single_node -Z single_node
```

​	重启数据库服务

```
gs_ctl restart -D $GAUSSHOME/data/single_node -Z single_node
```

4. 连接数据库

```
[omm@ecs-c9bf ~]$ gsql -d postgres -p 26000 -r 
gsql ((openGauss 5.0.1 build 33b035fd) compiled at 2023-12-15 20:28:19 commit 0 last mr  )
Non-SSL connection (SSL connection is recommended when requiring high-security)
Type "help" for help.
 
opengauss=# 
```

5. 修改用户密码

`alter role omm identified by '****新密码****' replace '****原密码****';`

6. 关于数据库用户

创建数据库用户，用户名为 joe，密码是 "Bigdata@123"

```
opengauss=# CREATE USER joe WITH PASSWORD "Bigdata@123";
```

赋予、收回用户权限



删除数据库用户



6. 关于数据库操作

创建数据库，以及确定所有者为 joe

```
opengauss=# CREATE DATABASE db_tpcc OWNER joe; 
```

退出数据库

`opengauss=# \q`

使用新用户连接到此数据库

```
[omm@ecs-c9bf ~]$ gsql -d db_tpcc -p 26000 -U joe -W Bigdata@123  -r
```

删除数据库



6. 数据库内部操作，和 PostgreSQL 类似

创建SCHEMA

```
db_tpcc=> CREATE SCHEMA joe AUTHORIZATION joe;
```

创建表

```
db_tpcc=> CREATE TABLE mytable (firstcol int);
```

插入数据

```
db_tpcc=> INSERT INTO mytable values (100);
```

#### Linux 常用命令

