#include "DataBaseConnector.h"
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

DataBaseConnector::DataBaseConnector() {
    henv = SQL_NULL_HENV;
    hdbc = SQL_NULL_HDBC;
}

DataBaseConnector::~DataBaseConnector() {
    disconnect();
}

bool DataBaseConnector::connect() {
    SQLRETURN ret;

    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        return false;
    }

    ret = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        return false;
    }

    ret = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        return false;
    }

    std::string connectionString = "DSN=" + dsn + ";Database=" + name + ";UID=" + user + ";PWD=" + password;
    ret = SQLDriverConnect(hdbc, NULL, (SQLCHAR*)connectionString.c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

    return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

void DataBaseConnector::disconnect() {
    if (hdbc != SQL_NULL_HDBC) {
        SQLDisconnect(hdbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
    }

    if (henv != SQL_NULL_HENV) {
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
    }
}

SQLHDBC DataBaseConnector::getConnection() {
    return hdbc;
}

bool DataBaseConnector::readConfigFile(const std::string& configFile) {
    boost::property_tree::ptree pt;
    
    try {
        boost::property_tree::ini_parser::read_ini(configFile, pt);
        
        dsn = pt.get<std::string>("database.dsn");
        name = pt.get<std::string>("database.name");
        user = pt.get<std::string>("database.user");
        password = pt.get<std::string>("database.password");
        
        return true;
    } catch (const boost::property_tree::ptree_error& e) {
        std::cerr << "Error reading configuration file: " << e.what() << std::endl;
        return false;
    }
}
