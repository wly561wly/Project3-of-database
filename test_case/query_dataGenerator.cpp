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
//T表示生成数据类型，N表示表的数量，M表示每个表的行数
const int T=1,N=20,M=20000,P=200000,inf=1e9+5,A=100;//比例为A/100，随机数mod 100 小于A则select
int tot=M+1;
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
inline const char* random_operation() {
    int op = random_int(1, 100);
    if (op <= A) {
        return "SELECT";
    } else {
        return "UPDATE";
    }
}
// 随机选择比较操作符
inline const char* random_operator() {
    int op = random_int(1, 4);
    switch (op) {
        case 1: return "=";
        case 2: return "<";
        case 3: return ">";
        case 4: return "<>";
        default: return "=";
    }
}
//select,update 混合
void query_sql_easy() {
    freopen("query_sql.sql", "w", stdout);
    for (int i = 1; i <= P; i++) {
            int test_id = random_int(1,N);
            int col1 = random_int(1,M);
            std::string operation = random_operation();
            std::string col3 = random_string(random_int(6, 15)); // varchar(4-10)

            if (operation == "SELECT") {
                const char* operator_col1 = random_operator();
                printf("SELECT * FROM test_%d WHERE col1 %s %d", test_id, operator_col1, col1);
                printf(";\n");
            } else {
                printf("UPDATE test_%d SET col3 = '%s' WHERE col2 = %d;\n", test_id, col3.c_str(), col1);
            }
    }
    fclose(stdout);
}
void query_csv_easy() {
    freopen("query_csv.csv", "w", stdout);
    for (int i = 1; i <= P; i++) {
            int test_id = random_int(1,N);
            int col1 = random_int(1,M); // 0-99999999
            std::string value1 = std::to_string(col1); // col1作为value1
            std::string value3 = random_string(20); // 生成随机字符串作为value3
            std::string operation = random_operation();
            const char* compare_operator = random_operator();

            if (operation == "SELECT") {
                printf("SELECT * FROM test_%d WHERE col1 = %d;,1\n", test_id, col1);
            } else {
                printf("UPDATE test_%d SET col3 = '%s' WHERE col1 = %d;,1\n", test_id, value1.c_str(), col1);
            }
    }
    fclose(stdout);
}
void query_csv_jh() {
	freopen("query_csv.csv", "w", stdout);
    for (int i = 1; i <= P; i++) {
            int test_id = random_int(1,N);
            int x = random_int(1,3); // 0-99999999
            int y = random_int(1,2000);
            int oper = random_int(1,3);
            if (oper == 1) {
                printf("SELECT col2 , SUM(col2) AS sum_col2 FROM test_%d GROUP BY col2;,1\n",test_id);
            } else if(oper == 3){
                printf("SELECT col2 , AVG(col1) AS avg_col1 FROM test_%d GROUP BY col2;,1\n",test_id);
            } else if(oper == 2){
                printf("SELECT col2 , MAX(col1) AS max_col1 FROM test_%d GROUP BY col2;,1\n",test_id);
            } 
        }
    fclose(stdout);
}
void query_sql_jh() {
	freopen("query_sql.sql", "w", stdout);
    for (int i = 1; i <= P; i++) {
            int test_id = random_int(1,N);
            int x = random_int(1,3); // 0-99999999
            int y = random_int(1,2000);
            int oper = random_int(1,3);
            if (oper == 1) {
                printf("SELECT col2 , SUM(col2) AS sum_col2 FROM test_%d GROUP BY col2;\n",test_id);
            } else if(oper == 3){
                printf("SELECT col2 , AVG(col1) AS avg_col1 FROM test_%d GROUP BY col2;\n",test_id);
            } else if(oper == 2){
                printf("SELECT col2 , MAX(col1) AS max_col1 FROM test_%d GROUP BY col2;\n",test_id);
            } 
        }
    fclose(stdout);
}
void query_csv_join() {
	freopen("query_csv.csv", "w", stdout);
    for (int i = 1; i <= P; i++) {
            int test_id = random_int(1,N);
            int test_id22 =random_int(1,N);
            if(test_id==test_id22) test_id22=random_int(1,test_id-1);
            int x = random_int(500,1500); // 0-99999999
            int y = random_int(500,10000);
            printf("SELECT t2.col1, t3.col3, t3.col4, t3.col5 "
			"FROM test_%d t2 LEFT JOIN test_%d t3 ON t2.col2 = t3.col2 WHERE t2.col2 = %d AND t2.col1 < %d ;,1\n",test_id,test_id22,x,y);
        }
    fclose(stdout);
}
void query_sql_join() {
	freopen("query_sql.sql", "w", stdout);
    for (int i = 1; i <= P; i++) {
            int test_id = random_int(1,N);
            int test_id22 =random_int(1,N);
            if(test_id==test_id22) test_id22=random_int(1,test_id-1);
            int x = random_int(500,1500); // 0-99999999
            int y = random_int(500,10000);
            printf("SELECT t2.col1, t3.col3, t3.col4, t3.col5 "
			"FROM test_%d t2 LEFT JOIN test_%d t3 ON t2.col2 = t3.col2 WHERE t2.col2 = %d AND t2.col1 < %d ;\n",test_id,test_id22,x,y);
        }
    fclose(stdout);
}
void query_csv_trans() {
    freopen("query_csv.csv", "w", stdout);
    for (int i = 1; i <= P; i++) {
        // 显式开始事务
        //printf("BEGIN;");

        for (int j = 0; j < 10; j++) {
            int test_id = random_int(1, N);
            int x = random_int(1, 20000); // Assuming col1 values range from 1 to 2000
            if(j>=8)x=++tot;
            int y = random_int(1, 2000); // Assuming col2 values range from 1 to 2000
            string col3 = random_string(20); // varchar(20)
            string col4 = random_string(20);
            string col5 = random_string(20);

            if (j<5) { // 50% SELECT
                printf("EXPLAIN ANALYZE SELECT * FROM test_%d WHERE col1 = %d;", test_id, x);
            } else{ // 30% UPDATE
                printf("EXPLAIN ANALYZE UPDATE test_%d SET col3 = '%s', col4 = '%s', col5 = '%s' WHERE col1 = %d;", test_id, col3.c_str(), col4.c_str(), col5.c_str(), x);
            }
            if (j < 9) {
                printf(" ");
            }
            else printf("\n");
        }
		//printf("COMMIT;\n");
        /*
        int t=random_int(1,10);
        if(t<=5)
        else printf("rollback;\n");*/
    }
    fclose(stdout);
}
signed main()
{
    //query_sql();
	query_csv_trans();
    return 0;
}
/*
select col3 from test_id where col2=value2;
update test_id SET col3='${value1}' where col2=value2;
"select,test_id,col2,col3,value2,value3\n"
"update,test_id,col2,col3,value2,value3\n"
*/