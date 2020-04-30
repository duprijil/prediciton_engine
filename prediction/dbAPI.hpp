#ifndef __DBAPI_HPP__
#define __DBAPI_HPP__

#include <sqlite3.hpp>
#include <stdio.h>

class dbAPI
{
private:
    sqlite3* db;
    char *error_msg;
    char* table_name;
    int rc;
public:
    dbAPI(const char* filepath,const char* table_name);
    char* selectById(int id, const char* column_name, const char* table_name);

    double selectById__annual_inc(int id);
    //double selectById__emp_length(int id);
    double selectById__funded_amnt(int id);
    double selectById__funded_amnt_inv(int id);
    double selectById__installment(int id);
    double selectById__int_rate(int id);
    double selectById__loan_amnt(int id);
    int selectById__grade(int id);
    ~dbAPI();
};



#endif // __DBAPI_HPP__