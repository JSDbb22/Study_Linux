#pragma once 


#include <cstdio>
#include <cstdlib>
#include <mysql/mysql.h>
#include<jsoncpp/json/json.h>


namespace order_system
{
  // 创建 MySQL 句柄并建立连接
  static MYSQL* MySQLInit()
  {
    // 1.创建句柄
    MYSQL* mysql=mysql_init(NULL);
    
    // 2.建立连接
    if(mysql_real_connect(mysql,"127.0.0.1","root","","order_system",3306,NULL,0)==NULL)
    {
      printf("mysql 连接失败！%s\n",mysql_error(mysql));
      return NULL;
    }

    // 3.设置编码方式
    mysql_set_character_set(mysql,"utf8");

    return mysql;
  }

  static void MySQLRelease(MYSQL* mysql)
  {
    mysql_close(mysql);
  }
  
  class DishTable
  {
    public:
      DishTable(MYSQL* mysql):_mysql(mysql)
    {}

      // 使用JSON 作为函数参数
      bool Insert(const Json::Value& dish)
      {
        //拼装 SQL
        char sql[1024*4]={0};
        sprintf(sql,"insert into dish_table values(null,'%s',%d)",dish["name"].asCString(),dish["price"].asInt());

        //执行 SQL
        int ret =mysql_query(_mysql,sql);
        if(ret!=0)
        {
          printf("DishTable Insert failed! %s\n",mysql_error(_mysql));
          return false;
        }

        printf("DishTable Insert OK!\n");
        return true;
      }

      bool SelectAll(Json::Value* dishes)
      {
        char sql[1024*4]={0};
        sprintf(sql,"select dish_id,name,price from dish_table");
        int ret =mysql_query(_mysql,sql);
        if(ret !=0)
        {
          printf("DishTable SelectAll failed! %s\n",mysql_error(_mysql));
          return false;
        }
        printf("DishTable SelectAll ok! \n");

        // 构造结果
        MYSQL_RES* result =mysql_store_result(_mysql);
        if(result ==NULL)
        {
          printf("获取结果失败！%s\n",mysql_error(_mysql));
          return false;
        }

        // 获取行数
        int rows=mysql_num_rows(result);
        for(int i =0;i<rows;++i)
        {
          MYSQL_ROW row=mysql_fetch_row(result);
          Json::Value dish;
          // row[] 取出的结果都是 C 风格字符串
          dish["dish_id"]=atoi(row[0]);
          dish["name"]=row[1];
          dish["price"]=atoi(row[2]);
          // 遍历结果依次加入到 dishes 中
          dishes->append(dish);
        }

        mysql_free_result(result);
        return true;
      }

      bool SelectOne(int32_t dish_id,Json::Value* dish)
      {
        // 1.拼装 SQL 语句
        char sql[1024*4]={0};
        sprintf(sql,"select dish_id,name,price from dish_table where dish_id = %d",dish_id);

        // 2.执行 SQL
        int ret=mysql_query(_mysql,sql);
        if(ret!=0)
        {
          printf("DishTable SelectOne failed! %s\n",mysql_error(_mysql));
          return false;
        }

        MYSQL_RES* result=mysql_store_result(_mysql);
        int rows=mysql_num_rows(result);
        if(rows!=1)
        {
          printf("DishTable SelectOne result rows not 1!");
          mysql_free_result(result);
          return false;
        }
        // 取出这一行数据
        MYSQL_ROW row =mysql_fetch_row(result);
        (*dish)["dish_id"]=atoi(row[0]);
        (*dish)["name"]=row[1];
        (*dish)["price"]=atoi(row[2]);
        mysql_free_result(result);

        return true;
      }

      bool Update(const Json::Value& dish)
      {
        // 1.拼装 SQL
        char sql[1024*4]={0};
        sprintf(sql,"update dish_table SET name='%s',price=%d where dish_id=%d",dish["name"].asCString(),dish["price"].asInt(),dish["dish_id"].asInt());
        
        //printf("sql: %s\n",sql);

        // 2.执行 SQL
        int ret =mysql_query(_mysql,sql);
        if(ret!=0)
        {
          printf("DishTable Update failed! %s\n",mysql_error(_mysql));
          return false;
        }

        printf("DishTable Update OK!\n");
        return true;
      }

      bool Delete(int dish_id)
      {
        // 1.拼装 SQL
        char sql[1024*4]={0};
        sprintf(sql,"delete from dish_table where dish_id=%d",dish_id);

        // 2.执行 SQL
        int ret=mysql_query(_mysql,sql);
        if(ret!=0)
        {
          printf("DishTable Delete failed! %s\n",mysql_error(_mysql));
          return false;
        }

        printf("DishTable Delete OK!\n");
        return true;
      }

    private:
      MYSQL* _mysql;
  };



  class OrderTable
  {
    public:
      OrderTable(MYSQL* mysql):_mysql(mysql)
    {}


      bool Insert(const Json::Value& order)
      {
        // 1.拼装 SQL
        char sql[1024*4]={0};
        sprintf(sql,"insert into order_table values(null,'%s','%s','%s',%d)",order["table_id"].asCString(),order["time"].asCString(),order["dishes"].asCString(),order["status"].asInt());

        // 2.执行 SQL
        int ret=mysql_query(_mysql,sql);
        if(ret!=0)
        {
          printf("OrderTable Insert failed! %s\n",mysql_error(_mysql));
          return false;
        }
        
        printf("OrderTable Insert ok!\n");
        return true;
      }

      bool SelectAll(Json::Value* orders)
      {
        // 1.拼装 SQL
        char sql[1024*4]={0};
        sprintf(sql,"select order_id,table_id,time,dishes,status from order_table");

        // 2.执行 SQL 
        int ret =mysql_query(_mysql,sql);
        if(ret !=0)
        {
          printf("OrderTable SelectAll failed! %s\n",mysql_error(_mysql));
          return false;
        }

        // 3.拼装到输出型参数中
        MYSQL_RES* result =mysql_store_result(_mysql);
        int rows=mysql_num_rows(result);
        for(int i =0;i<rows;++i)
        {
          MYSQL_ROW row =mysql_fetch_row(result);
          Json::Value order;
          order["order_id"]=atoi(row[0]);
          order["table_id"]=row[1];
          order["time"]=row[2];
          order["dishes"]=row[3];
          order["status"]=row[4];
          // 遍历结果依次加入到 orderes 中
          orders->append(order);
        }

        mysql_free_result(result);
        return true;
      }

      bool ChangeState(const Json::Value& order)
      {
        // 1.拼装 SQL 
        char sql[1024*4]={0};
        sprintf(sql,"update order_table set status = %d where order_id = %d",order["status"].asInt(),order["order_id"].asInt());

        // 2.执行 SQL
        int ret=mysql_query(_mysql,sql);
        if(ret!=0)
        {
          printf("OrderTable ChangeState failed! %s\n",mysql_error(_mysql));
          return false;
        }

        printf("OrderTable ChangeState ok!\n");
        return true;
      }

    private:
      MYSQL* _mysql;
  };
} //end order_system 
