#include <cstdio>
#include <cstdlib>
#include <mysql/mysql.h>


int main()
{
  // 1.创建一个数据库的操作 "句柄" (遥控器)
  MYSQL* mysql = mysql_init(NULL);

  // 2.建立句柄和数据库服务器之间的联系(连接服务器)
  if(mysql_real_connect(mysql,"127.0.0.1","root","","order_system",3306,NULL,0) == NULL)
  {
    printf("connect failed! %s\n",mysql_error(mysql));
    return 1;
  }

  // 3.设置编码格式(让 mysql 服务器的编码方式和客户端的方式匹配)
  // utf-8
  mysql_set_character_set(mysql,"utf8");

  // 4.拼装一个插入数据的 SQL
  char sql[1024] = {0};
  int price =2000;
  sprintf(sql,"insert into dish_table values(null,'京酱肉丝',%d)",price);

  // 5.执行 SQL 语句
  int ret=mysql_query(mysql,sql);
  if(ret != 0)
  {
    printf("sql 执行失败！ %s\n",mysql_error(mysql));
    mysql_close(mysql);
    return 1;
  }

  // 6.关闭句柄
  mysql_close(mysql);
  printf("执行成功！\n");

  return 0;
}
