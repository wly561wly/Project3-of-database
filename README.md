# 使用 JMeter 和 DataGrip 测试 PostgreSQL 和 OpenGauss

本项目旨在展示如何使用 JMeter 和 DataGrip 对 PostgreSQL 和 OpenGauss 数据库进行基本的性能测试和数据操作。

### 安装和配置 JMeter

#### 配置 Java8 环境

需要安装和下载 `Java 8` 的可以去 Oracle 官方网站下载 Java 8：[Oracle Java 8](https://www.oracle.com/java/technologies/javase/javase8-archive-downloads.html)

如果存在多个Java版本，还需要在环境变量中修改，将Java的优先级调高

1. **修改环境变量**：右键点击“此电脑”，依次选择“属性”、“高级系统设置”、“环境变量”。

2. **配置 JAVA_HOME**：在“系统变量”中点击“新建”，新建变量，变量名（`JAVA_HOME`），变量值（Java 的安装目录，例如 `C:\Program Files\Java\jdk1.8.0_231`）。

3. **配置 PATH**：在“系统变量”中找到`Path`变量，点击“编辑”、点击“新建”，新增`Path`变量，值为`%JAVA_HOME%\bin`。

   ​	同时可以选择“上移” 将对应 java8 版本优先级提高

4. **验证 Java 安装**：打开命令行（Win + R，输入`cmd`，回车），输入`java -version`，回车，如果显示 Java 版本信息，说明安装成功。

#### 下载Jmeter

1. **下载 JMeter**：访问 JMeter 官方网站下载 JMeter：[Apache JMeter](https://jmeter.apache.org/download_jmeter.cgi)，选择适合你操作系统的版本并下载。
2. **安装 JMeter**：将下载的 JMeter 安装包解压到本地目录（例如 `C:\JMeter\apache-jmeter-5.5`）。
3. **运行 JMeter**：该文件夹下的 `"bin\jmeter.bat"` 就是对应的脚本程序，可以设置快捷方式方便打开。或者也可以选择打开命令行，输入`jmeter`，回车，如果显示 JMeter 界面，说明安装成功。

#### 下载 JDBC 驱动

在本项目中，你不需要单独下载 JDBC 驱动，因为它们已经包含在项目的 `lib` 文件夹中。以下是 JDBC 驱动的文件及其对应的数据库：

- `"postgresql.jar"`：这是 OpenGauss 的 JDBC 驱动。
- `"postgresql-42.7.4.jar"`：这是 PostgreSQL 的 JDBC 驱动，版本为 42.7.4。

这些 `.jar` 文件已经准备好，可以直接用于你的测试环境。如果你需要更新或替换 JDBC 驱动，可以从以下官方网站下载最新版本：

- **PostgreSQL JDBC 驱动**：[PostgreSQL 官方下载页面]([Download | pgJDBC](https://jdbc.postgresql.org/download/))
- **OpenGauss JDBC 驱动**：[OpenGauss 官方下载页面](https://opengauss.org/zh/download.html)

最后将下载的 `.jar` 文件放入 JMeter 安装目录的 `lib` 文件夹中。

### 使用 Docker 拉取的数据库版本

Docker 容器提供了一个隔离的环境，用来运行软件或者程序。对于数据库来说，这意味着你可以轻松地在 Windows、macOS 或 Linux 系统上运行 PostgreSQL 和 OpenGauss 数据库。（个人理解：docker就是在windows系统上提供一个虚拟的linux环境）

（Docker 的安装这里就不过多赘述，可在网上找到大量相关资料）

#### 拉取 Docker 镜像

在开始之前，确保你已经安装了 Docker 并且 Docker 服务正在运行。你可以使用以下命令来拉取 PostgreSQL 和 OpenGauss 的 Docker 镜像：

```bash
docker pull docker.1panel.live/enmotech/opengauss:3.0.0
docker pull docker.1panel.live/ubuntu/postgres:14-22.04_beta
```

这里也可以选择其他适配的版本，OpenGauss 的最新版本已经到了 6.0.0

#### 运行 OpenGauss 和 Postgres 容器

使用以下命令来运行 PostgreSQL 容器。请确保将 `<!!!your db password!!!>` 替换为你想要设置的 PostgreSQL 数据库密码，并且将 `<!!!persist directory!!!>` 替换为你本地的持久化存储路径。

```
docker run --name project3-opengauss --privileged=true \
    -d -e GS_PASSWORD=<!!!your db password!!!> \
    -v <!!!persist directory!!!>:/var/lib/opengauss  -u root \
    -p 15432:5432 \
    docker.1panel.live/enmotech/opengauss:3.0.0
```

```
docker run -d \--name project3-postgres \-e POSTGRES_USER=postgres-e POSTGRES_PASSWORD=<!!!your db password!!!> \-e POSTGRES_DB=postgres-e PGDATA=/var/lib/postgresql/data/pgdata \-p 5432:5432 \-v <!!!persist directory!!!>:/var/lib/postgresql/data \
 docker.1panel.live/ubuntu/postgres:14-22.04_beta
```

**端口映射**：使用了 `-p` 选项来将容器内部的数据库端口映射到宿主机的端口。对于 OpenGauss，我们映射了 15432 到 5432 端口，也可以选择其他未被占用的端口；对于 PostgreSQL，映射了 5432 到 5432 端口。

### 使用 JMeter 连接数据库和制作测试计划

1. 启动 JMeter：打开命令行，输入 `JMeter` 即可启动。

2. **创建测试计划**：在 JMeter 的欢迎界面，选择“Create New...”来创建一个新的测试计划。

3. **添加 JDBC 连接配置**：

   右键点击测试计划，选择“Add” -> “Config Element” -> “JDBC Connection Configuration”

   在弹出的配置窗口中，填写以下信息：

   - **Name**：给连接配置一个名称，例如“PostgreSQL Connection”
   - **Variable Name**：设置一个变量名，用于在测试计划中引用这个连接配置，例如“jdbcPostgreSQL”
   - **Database URL**：输入数据库的 URL，格式通常为 `jdbc:postgresql://<hostname>:<port>/<database_name>`
   - **Username**：输入数据库的用户名
   - **Password**：输入数据库的密码
   - **Driver Class**：选择对应的 JDBC 驱动类，对于 PostgreSQL 和 OpenGauss 都是 `org.postgresql.Driver`

   点击“OK”保存配置。

   <img src="https://github.com/wly561wly/Project3-of-database/blob/main/picture/config.png?raw=true" alt="config" style="zoom:67%;" />

4. **添加 JDBC 请求**：

   右键点击线程组，选择“Add” -> “Sampler” -> “JDBC Request”。

   在“JDBC Request”采样器中，选择之前创建的 JDBC 连接配置变量名，例如“jdbcPostgreSQL”。

   在“SQL query”字段中，输入你想要执行的 SQL 查询语句。

   <img src="https://github.com/wly561wly/Project3-of-database/blob/main/picture/request.png?raw=true" alt="request" style="zoom:67%;" />

   同时，这里可以使用自带的随机脚本或者导入csv文件，使用对应文本进行替换；为了测试更加多样，还可以使用 事务控制器 来进行模拟事务：

    a) 可以使用 JMeter 的内置变量来动态生成 SQL 查询，例如使用 `${__RandomString(10, 0123456789,)}` 来生成一个随机的字符串作为查询参数。

   b) 如果要从 CSV 文件中读取数据来执行 SQL 查询，可以添加一个“CSV Data Set Config”元素到你的线程组。其中，指定 CSV 文件的路径和变量名，这些变量名将在 SQL 查询中被使用

​		c) 为了进行并发测试，还可以添加一个“Transaction Controller”到你的线程组。在“Transaction Controller”中，你可以设置事务的名称和其他相关选项

5. **添加查看结果树**：

​	在"线程组"->"Add"-"监听器"中选择添加，可以帮助你查看每个请求的详细信息，包括请求和响应的详细内容。（如图，可以看到对应查询得到的结果表）

<img src="https://github.com/wly561wly/Project3-of-database/blob/main/picture/result_tree%E8%81%9A%E5%90%88.png?raw=true" alt="result_tree" style="zoom:67%;" />

6. **添加结果监听器**：

​		右键点击线程组，选择“Add” -> “Listener” -> “View Results in Table”。这将添加一个结果监听器，用于显示测试结果

7. **设置线程组参数**：

- 线程数：定义同时运行的虚拟用户数量。
- Ramp-Up 时间：所有线程启动并开始执行测试所需的时间。
- 持续时间：测试计划的总持续时间。
- 启动延迟：测试计划开始执行前的等待时间。
- 循环次数：每个线程将执行测试计划的次数。

同时，当使用 CSV 文件导入数据时，需要考虑 CSV Data Set Config 中的线程共享模式的选择。有“Current Thread”和“All Threads”等多种选择。选择“Current Thread”时，每个线程将独立地从 CSV 文件中读取数据，而选择“All Threads”时，所有线程将共享 CSV 文件中的数据，并且每个线程将从文件的不同部分读取数据，实现交替获取。

<img src="https://github.com/wly561wly/Project3-of-database/blob/main/picture/thread_group.png?raw=true" alt="thread_group" style="zoom:67%;" />

8. **运行测试计划**：

- 点击工具栏中的“Start”按钮来运行测试计划。
- 观察结果监听器中的输出，以查看测试结果。

9. **分析测试结果**：

- **响应时间**：观察不同请求的响应时间，了解哪些请求响应较慢，可能是数据库性能瓶颈所在。
- **吞吐量**：观察不同时间段的吞吐量变化，了解数据库在高并发下的处理能力。
- **错误率**：观察错误率的变化，了解数据库的稳定性和可靠性

<img src="https://github.com/wly561wly/Project3-of-database/blob/main/picture/insert_test_og.png?raw=true" alt="thread_group" style="zoom:67%;" />

10. **保存配置**：在完成一个配置后，可以选择保存为 `.jmx` 文件，方便下次使用（在 “test_case” 文件夹下就保存了我的测试配置 `"Opengauss_test.jmx"`,`"Postgres_test1.jmx"`）



#### 其他 tips：

1. 设置 **JMeter 简体中文界面**：

​	可以在 JMeter 的选项中选择简体中文，不过这个设置不能保存。或者，你可以通过编辑 JMeter 安装目录下 `bin` 文件夹中的 `jmeter.properties` 文件，并将 `language` 属性设置为 `zh_CN` 来永久更改界面语言，记得使用管理员权限打开该文件。

	2. 使用 **ServerAgent** 监控 `CPU`、`Memory`

- ​	需要去 JMeter 官网上下载 插件 `jmeter-plugins-manager.jar`,`JMeterPlugins-Standard.jar`,`JMeterPlugins-Extras.jar`，放到 `lib/ext` 文件夹下。
- ​	然后打开 JMeter，进入菜单栏 `Options` -> `Plugins Manager`，安装以下插件：**PerfMon Metrics Collector**
- ​	访问 [ServerAgent GitHub 仓库](https://github.com/undera/perfmon-agent) 下载 `ServerAgent-2.2.3.zip`
- ​	右键点击线程组，选择“Add” -> “Listener” -> “jp@gc - PerfMon Metrics Collector”。

​			在“PerfMon Metrics Collector”中，配置以下参数：

​				**Host/IP**：被测服务器的 IP 地址。

​				**Port**：ServerAgent 的端口，默认为 4444。

​				**Metric to collect**：选择要监控的指标，如 `CPU`、`Memory` 等

​	在使用时，保持 ServerAgent 程序正在运行。

### 结论

##### OpenGauss **优势**：

1. **资源利用率**：OpenGauss的单进程多线程架构在高并发场景下表现出色，能够更有效地利用CPU和内存资源，尤其是在多核CPU环境下。
2. **扩展性**：OpenGauss将Xid从int32扩展到int64，显著增加了事务ID的范围，减少了Xid耗尽的风险，适合高并发事务处理。
3. **增量检查点**：引入增量检查点机制，减少了对业务的影响，平滑了IO，降低了数据库性能波动。

**劣势**：

1. **性能表现**：在一些测试场景中，OpenGauss的性能表现不如PostgreSQL，尤其是在复杂查询和大数据量处理方面。（也可能是测试软硬件问题）
2. **功能支持**：OpenGauss在某些高级功能，如逻辑解码和并行处理方面，可能不如PostgreSQL成熟，同时使用 C++ 后，移植性降低了。

​	通过本次对 `OpenGauss` 和 `PostgreSQL` 的比较分析，我们得以管中窥豹，了解了这两种数据库系统在不同场景下的表现。虽然 `OpenGauss` 可能在安全、高并发等领域存在优势，而这部分我不能很好地测试出来，但在性能和生态系统方面仍有提升空间。`PostgreSQL` 作为一个成熟且广泛使用的数据库系统，其稳定性和丰富的功能集得到了市场的验证，在多项比较中都表现出色。

Reference：[JMeter连接MYSQL数据库并进行操作详解](https://www.cnblogs.com/mrgavin/p/12808447.html)

测试环境

|   测试环境   |                    型号或者版本                    |    备注     |
| :----------: | :------------------------------------------------: | :---------: |
|  PostgreSQL  |  PostgreSQL 14.12 (Ubuntu 14.12-0ubuntu0.22.04.1)  | Docker 镜像 |
|  OpenGauss   |           openGauss 3.0.0 build 02c14696           | Docker 镜像 |
|    JMeter    |                    JMeter 5.6.3                    |             |
|   DataGrip   |                      2024.2.2                      |             |
|     Java     |                       1.8.0                        | 适配 JMeter |
| 笔记本处理器 | AMD Ryzen 9 7940H w/ Radeon 780M Graphics 4.00 GHz |             |
|   机带 RAM   |              16.0 GB （15.2 GB可用）               |             |
|  笔记本系统  |               Windows 11 家庭中文版                |             |

代码、数据文件说明：

|          文件名          |                  功能                   | 备注 |
| :----------------------: | :-------------------------------------: | :--: |
|    Opengauss_test.jmx    |       JMeter中 openGauss 配置文件       |      |
|    Postgres_test1.jmx    |      JMeter中 PostgreSQL 配置文件       |      |
| query_dataGenerator.cpp  | 随机生成select/update对应的 csv/sql文件 |      |
| insert_dataGenerator.cpp |   随机生成 insert 对应的 csv/sql文件    |      |
|        data.xlsx         |          记录了所有的测试数据           |      |
|     picture(folder)      |        存储了本文档中的对应图片         |      |
|     test_tables.sql      |      包含 更新测试table 的sql 命令      |      |
|      query_sql.sql       |       包含 SELECT 测试中 sql 命令       |      |

