#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<random>
#define il inline
#define re register int
#define int long long
using namespace std;
il int read()
{
	int x=0,f=1;char ch=getchar();
	while(!isdigit(ch)&&ch!='-')ch=getchar();
	if(ch=='-')f=-1,ch=getchar();
	while(isdigit(ch))x=(x<<1)+(x<<3)+ch-'0',ch=getchar();
	return x*f;
}
// 生成随机整数
il int random_int(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

// 生成随机字符串
il string random_string(int len) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 25);
    string result;
    for (int i = 0; i < len; ++i) {
        result += 'a' + distrib(gen);
    }
    return result;
}
//T表示生成数据类型，N表示表的数量，M表示每个表的行数
const int T=1,N=20,M=20000,inf=1e9+5;
//表名，test_id，col1(integer自增)，col2(0-99999999),col3(varchar(20)),col4(varchar(20)),col5(varchar(20));
//依次随机生成数据
il void generate_csv() {
    freopen("test_insert.csv", "w", stdout);
    for (re i = 1; i <= N; i++) {
        for (re j = 1; j <= M; j++) {
            int test_id = i;
            int col1 = j; // 自增整数
            int col2 = random_int(0, 2000); // 0-99999999
            string col3 = random_string(20); // varchar(20)
            string col4 = random_string(20);
            string col5 = random_string(20);
            printf("test_%d,%d,%d,%s,%s,%s\n", test_id, col1, col2, col3.c_str(), col4.c_str(), col5.c_str());
        }
    }
    fclose(stdout);
}
il void generate_sql() {
    freopen("test_sql.sql", "w", stdout);
    for (re i = 1; i <= N; i++) {
        for (re j = 1; j <= M; j++) {
            int test_id = i;
            int col1 = j; // 自增整数
            int col2 = random_int(0, 2000); // 0-99999999
            string col3 = random_string(20); // varchar(20)
            string col4 = random_string(20);
            string col5 = random_string(20);
            printf("INSERT INTO test_%d (col1, col2, col3, col4, col5) VALUES (%d, %d, '%s', '%s', '%s');\n",
                   test_id, col1, col2, col3.c_str(), col4.c_str(), col5.c_str());
        }
    }
    fclose(stdout);
}
void generator_drop_create() {
    freopen("test_tables.sql", "w", stdout);
    for (int i = 1; i <= N; ++i) {
        printf("DROP TABLE IF EXISTS test_%d;\n", i);
        printf("CREATE TABLE test_%d (\n", i);
        printf("    col1 INT NOT NULL,\n");
        printf("    col2 INT NOT NULL,\n");
        printf("    col3 VARCHAR(20) NOT NULL,\n");
        printf("    col4 VARCHAR(20) NOT NULL,\n");
        printf("    col5 VARCHAR(20) NOT NULL,\n");
        printf("    PRIMARY KEY (col1)\n");
        printf(");\n\n");
    }
    fclose(stdout);
}
signed main() {
	generator_drop_create();
    generate_csv();
    generate_sql();
    return 0;
}