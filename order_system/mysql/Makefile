all:insert select

insert:mysql_insert.cc 
	g++ $^ -o $@ -L/usr/lib64/mysql -lmysqlclient
select:mysql_select.cc 
	g++ $^ -o $@ -L/usr/lib64/mysql -lmysqlclient

.PHONY:clean
clean:
	rm insert select 
