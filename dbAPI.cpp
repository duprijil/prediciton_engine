#include <dbAPI.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char tmp_response_buff[256];

int id_callback(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = 0;
    for (int i = 0; i < argc; i++) {
        snprintf(tmp_response_buff, sizeof(tmp_response_buff), "%s", argv[i] ? argv[i] : "NULL");
    }
    return 0;
}

dbAPI::dbAPI(const char* filepath,const char* table_name) {
    this->table_name = (char*)malloc(sizeof(char) * strlen(table_name));
    strcpy(this->table_name, table_name);
    rc = sqlite3_open(filepath, &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error opening db: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    }
    error_msg = nullptr;
}

dbAPI::~dbAPI() {
    sqlite3_close(db);
    if (error_msg) free(error_msg);
    //free(this->table_name);
}

char* dbAPI::selectById(int id, const char* column_name, const char* table_name) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT %s FROM %s WHERE id == %d", column_name, table_name, id);
    rc = sqlite3_exec(db, query, id_callback, 0, &error_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", error_msg);
        sqlite3_free(error_msg);
        sqlite3_close(db);
        return nullptr;
    }
    return tmp_response_buff;
}

double dbAPI::selectById__annual_inc(int id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT annual_inc FROM %s WHERE id == %d", table_name, id);
    rc = sqlite3_exec(db, query, id_callback, 0, &error_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", error_msg);
        sqlite3_free(error_msg);
        sqlite3_close(db);
        return -1;
    }
    return strtod(tmp_response_buff, NULL);
}

/* double dbAPI::selectById__emp_length(int id) {} */

double dbAPI::selectById__funded_amnt(int id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT funded_amnt FROM %s WHERE id == %d", table_name, id);
    rc = sqlite3_exec(db, query, id_callback, 0, &error_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", error_msg);
        sqlite3_free(error_msg);
        sqlite3_close(db);
        return -1;
    }
    return strtod(tmp_response_buff, NULL);
}
double dbAPI::selectById__funded_amnt_inv(int id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT funded_amnt_inv FROM %s WHERE id == %d", table_name, id);
    rc = sqlite3_exec(db, query, id_callback, 0, &error_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", error_msg);
        sqlite3_free(error_msg);
        sqlite3_close(db);
        return -1;
    }
    return strtod(tmp_response_buff, NULL);
}
double dbAPI::selectById__installment(int id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT installment FROM %s WHERE id == %d", table_name, id);
    rc = sqlite3_exec(db, query, id_callback, 0, &error_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", error_msg);
        sqlite3_free(error_msg);
        sqlite3_close(db);
        return -1;
    }
    return strtod(tmp_response_buff, NULL);
}
double dbAPI::selectById__int_rate(int id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT int_rate FROM %s WHERE id == %d", table_name, id);
    rc = sqlite3_exec(db, query, id_callback, 0, &error_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", error_msg);
        sqlite3_free(error_msg);
        sqlite3_close(db);
        return -1;
    }
    return strtod(tmp_response_buff, NULL);
}
double dbAPI::selectById__loan_amnt(int id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT loan_amnt FROM %s WHERE id == %d", table_name, id);
    rc = sqlite3_exec(db, query, id_callback, 0, &error_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", error_msg);
        sqlite3_free(error_msg);
        sqlite3_close(db);
        return -1;
    }
    return strtod(tmp_response_buff, NULL);
}

int dbAPI::selectById__grade(int id) {
    char query[256];
    snprintf(query, sizeof(query), "SELECT grade FROM %s WHERE id == %d", table_name, id);
    rc = sqlite3_exec(db, query, id_callback, 0, &error_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", error_msg);
        sqlite3_free(error_msg);
        sqlite3_close(db);
        return -1;
    }
    return tmp_response_buff[0];
    //return strtod(tmp_response_buff, NULL);
}