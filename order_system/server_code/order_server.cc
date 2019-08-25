#include "httplib.h"
#include <jsoncpp/json/json.h>
#include "db.hpp"



const char* CONTENT_TYPE="application/json";

MYSQL* mysql =NULL;

int main()
{
  using namespace httplib;
  mysql=order_system::MySQLInit();
  signal(SIGINT,[](int){order_system::MySQLRelease(mysql);exit(0);});
  order_system::DishTable dish_table(mysql);
  order_system::OrderTable order_table(mysql);
  

  Server server;
  server.Post("/dish",[&dish_table](const Request& req,Response& resp)
      {
      Json::Value req_json;
      Json::Value resp_json;
      Json::Reader reader;
      Json::FastWriter writer;
      printf("新增菜品!\n");

      // 1.获取到数据并解析成 JSON 格式
      bool ret=reader.parse(req.body,req_json);
      if(!ret)
      {
      printf("parse body failed!\n");
      resp_json["ok"]=false;
      resp_json["reason"]="parse body failed!";
      resp.status=400;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }

      // 2.校验 JSON 中的信息
      if(req_json["name"].empty()||req_json["price"].empty())
      {
      printf("Request json error format!\n");
      resp_json["ok"]=false;
      resp_json["reason"]="Request json error format!";
      resp_json["ok"]=false;
      resp.status=400;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }

      // 3.调用数据库操作来进行插入数据
      ret=dish_table.Insert(req_json); 
      if(!ret)
      {
      printf("dish_table insert failed!\n");
      resp_json["ok"]=false;
      resp_json["reason"]="Request json error format!";
      resp.status=500;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }

      // 4.构造正确响应
      resp_json["ok"]=true;
      resp.status=200;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      });

  server.Get("/dish",[&dish_table](const Request& req,Response& resp)
      {
      (void) req;
      Json::Value resp_json;
      Json::FastWriter writer;
      printf("查看所有菜品!\n");

      // 1.不需要处理输入数据，无需校验，直接调用数据库操作
      bool ret =dish_table.SelectAll(&resp_json);
      if(!ret)
      {
      printf("SelectAll dish failed!\n");
      resp_json["ok"]=false;
      resp_json["reason"]="SelectAll dish failed!";
      resp.status=500;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }

      // 2.构造正确的响应
      resp.status=200;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      });

  server.Get(R"(/dish(\d+))",[&dish_table](const Request& req,Response& resp)
      {
      Json::FastWriter writer;
      Json::Value resp_json;
      // 1.获取到 dish_id
      int32_t dish_id=std::stoi(req.matches[1]);
      printf("获取到编号为 %d 的菜品\n",dish_id);

      // 2.执行数据库操作
      bool ret=dish_table.SelectOne(dish_id,&resp_json);
      if(!ret)
      {
      printf("SelectOne dish failed!\n");
      resp_json["ok"]=false;
      resp_json["reason"]="SelectOne dish failed!";
      resp.status=500;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }

      // 3.构造一个正确的响应
      resp.status=200;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      });

  server.Get(R"(/dish(\d+))",[&dish_table](const Request& req,Response& resp)
      {
      Json::FastWriter writer;
      Json::Value resp_json;
      // 1.获取到 dish_id
      int32_t dish_id=std::stoi(req.matches[1]);
      printf("获取到编号为 %d 的菜品\n",dish_id);

      // 2.执行数据库操作
      bool ret=dish_table.SelectOne(dish_id,&resp_json);
      if(!ret)
      {
      printf("SelectOne dish failed!\n");
      resp_json["ok"]=false;
      resp_json["reason"]="SelectOne dish failed!";
      resp.status=500;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }

      // 3.构造一个正确的响应
      resp.status=200;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      });

  server.Put(R"(/dish/(\d+))",[&dish_table](const Request& req,Response& resp)
      {
      Json::Value req_json;
      Json::Value resp_json;
      Json::Reader reader;
      Json::FastWriter writer;
      printf("修改菜品!\n");

      // 1.获取到 dish_id
      int32_t dish_id=std::stoi(req.matches[1]);
      printf("修改编号为 %d 菜品!\n",dish_id);

      // 2.获取到数据并解析成 JSON 格式
      bool ret=reader.parse(req.body,req_json);
      if(!ret)
      {
      printf("parse body failed!\n");
      resp_json["ok"]=false;
      resp_json["reason"]="parse body failed!";
      resp.status=400;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }

      // 3.校验 JSON 中的信息
      if(req_json["name"].empty()||req_json["price"].empty())
      {
      printf("Request json error format!\n");
      resp_json["ok"]=false;
      resp_json["reason"]="Request json error format!";
      resp.status=400;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }

      // 4.调用数据库操作来进行插入数据
      req_json["dish_id"]=dish_id;    //要把 dish_id 添加到 json中
      ret=dish_table.Update(req_json);
      if(!ret)
      {
      printf("dish_table update failed!\n");
      resp_json["ok"]=false;
      resp_json["reason"]="dish_table update failed!";
      resp.status=500;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }

      // 5.构造正确的响应
      resp_json["ok"]=true;
      resp.status=200;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      });

  server.Delete(R"(/dish/(\d+))",[&dish_table](const Request& req,Response& resp)
      {
      Json::Value resp_json;
      Json::FastWriter writer;
        
      // 1.获取到 dish_id
      int32_t dish_id=std::stoi(req.matches[1]);
      printf("删除编号为 %d 菜品!\n",dish_id);

      // 2.执行数据库操作
      bool ret =dish_table.Delete(dish_id);
      if(!ret)
      {
      printf("dish_table delete failed!\n");
      resp_json["ok"]=false;
      resp_json["reason"]="dish_table delete failed!";
      resp.status=500;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }

      // 3.构造正确响应
      resp_json["ok"]=true;
      resp.status=200;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      });
  server.Post("/order",[](const Request& req,Response& resp)
      {
      });
  server.Get("(/order",[](const Request& req,Response& resp)
      {
      });
  server.Put(R"(/order/(\d+))",[](const Request& req,Response& resp)
      {
      });

  server.set_base_dir("./wwwroot");
  server.listen("0.0.0.0",9094);

  return 0;
}
