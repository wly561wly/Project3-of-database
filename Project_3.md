# Project 3

主要内容

1. 评价指标
2. 根据评价指标，设计比较方案，具体的比较
3. 提供 openGuass的详细评估，包括优势和劣势
4. 报告和源码应该在 github 上公布

计划安排



​	

### 其他内容

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

