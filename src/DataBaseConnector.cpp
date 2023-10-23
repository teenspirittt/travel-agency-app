#include "DataBaseConnector.h"


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
    std::ifstream file(configFile);
    if (!file.is_open()) {
        return false; // Не удалось открыть файл
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string name = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            if (name == "database.dsn") {
                dsn = value;
            } else if (name == "database.name") {
                name = value;
            } else if (name == "database.user") {
                user = value;
            } else if (name == "database.password") {
                password = value;
            }
        }
    }

    file.close();
    return true;
}
