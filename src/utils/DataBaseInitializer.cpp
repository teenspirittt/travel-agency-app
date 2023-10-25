#include "DataBaseInitializer.h"

DataBaseInitializer::DataBaseInitializer(DataBaseConnector &connector)
    : dbConnector(connector) {}

bool DataBaseInitializer::initializeDataBase() {
  if (checkTableExists("employees") && checkTableExists("employee_transfers") &&
      checkTableExists("hotels") && checkTableExists("flights") &&
      checkTableExists("route") && checkTableExists("clients") &&
      checkTableExists("carriers") && checkTableExists("aircraft") &&
      checkTableExists("client_route")) {
    std::cout << "All necessary tables exist." << std::endl;
    return true;
  } else {
    if (createTables()) {
      std::cout << "Tables have been created." << std::endl;
      return true;
    } else {
      std::cerr << "Failed to create tables." << std::endl;
      return false;
    }
  }
}

bool DataBaseInitializer::checkTableExists(const std::string &tableName) {
  SQLHSTMT hstmt;
  SQLRETURN ret;

  ret = SQLAllocHandle(SQL_HANDLE_STMT, dbConnector.getConnection(), &hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    return false;
  }

  std::string query =
      "SELECT table_name FROM information_schema.tables WHERE table_name = ?";
  ret = SQLPrepare(hstmt, (SQLCHAR *)query.c_str(), SQL_NTS);

  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    std::cerr << "An error occurred while preparing the SQL query."
              << std::endl;
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLBindParameter(hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0,
                         0, (SQLPOINTER)tableName.c_str(), 0, NULL);

  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    std::cerr << "Error binding parameter to SQL query." << std::endl;
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLExecute(hstmt);

  if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
    SQLLEN rowCount;
    ret = SQLRowCount(hstmt, &rowCount);
    if (rowCount > 0) {
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return true;
    } else {
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return false;
    }
  } else {
    std::cerr << "Error executing SQL query." << std::endl;
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }
}

bool DataBaseInitializer::createTables() {
  SQLHSTMT hstmt;
  SQLRETURN ret;

  ret = SQLAllocHandle(SQL_HANDLE_STMT, dbConnector.getConnection(), &hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    return false;
  }

  std::string createEmployeesTable =
      "CREATE TABLE IF NOT EXISTS employees ("
      "id serial primary key,"
      "full_name varchar(255) not null,"
      "address text,"
      "date_of_birth date,"
      "position varchar(255),"
      "salary decimal(10, 2)"
      ")";

  std::string createEmployeeTransfersTable =
      "CREATE TABLE IF NOT EXISTS employee_transfers ("
      "id serial PRIMARY KEY,"
      "employee_id int NOT NULL,"
      "old_position varchar(255),"
      "transfer_reason text,"
      "order_number varchar(50),"
      "order_date date"
      ");";

  std::string createHotelsTable =
      "CREATE TABLE IF NOT EXISTS hotels ("
      "id serial PRIMARY KEY,"
      "name varchar(255) NOT NULL,"
      "class int,"
      "room_category varchar(255)"
      ");";

  std::string createFlightsTable =
      "CREATE TABLE IF NOT EXISTS flights ("
      "id serial PRIMARY KEY,"
      "flight_number varchar(50) NOT NULL,"
      "departure_date timestamp NOT NULL,"
      "aircraft_id int,"
      "class varchar(50),"
      "available_seats int,"
      "carrier_id int"
      ");";

  std::string createRouteTable =
      "CREATE TABLE IF NOT EXISTS route ("
      "id serial PRIMARY KEY,"
      "name varchar(255) NOT NULL,"
      "country varchar(255),"
      "city varchar(255),"
      "duration int,"
      "hotel_id int,"
      "flight_id int,"
      "employee_id int,"
      "agency_representative_name varchar(255),"
      "agency_representative_phone varchar(20)"
      ");";

  std::string createClientsTable =
      "CREATE TABLE IF NOT EXISTS clients ("
      "id serial PRIMARY KEY,"
      "full_name varchar(255) NOT NULL,"
      "phone varchar(20) NOT NULL,"
      "order_date timestamp NOT NULL,"
      "class varchar(50),"
      "seat varchar(10)"
      ");";

  std::string createCarriersTable =
      "CREATE TABLE IF NOT EXISTS carriers ("
      "id serial PRIMARY KEY,"
      "name varchar(255) NOT NULL"
      ");";

  std::string createAircraftTable =
      "CREATE TABLE IF NOT EXISTS aircraft ("
      "id serial PRIMARY KEY,"
      "aircraft_type varchar(255) NOT NULL,"
      "carrier_id int,"
      "manufacturer varchar(255),"
      "capacity int"
      ");";

  std::string createClientRoute =
      "CREATE TABLE IF NOT EXISTS client_route ("
      "client_id int,"
      "route_id int,"
      "primary key(client_id, route_id)"
      ");";

  std::string createReferences =
      "ALTER TABLE employee_transfers "
      "ADD FOREIGN KEY (employee_id) REFERENCES employees (id);"
      "ALTER TABLE flights "
      "ADD FOREIGN KEY (aircraft_id) REFERENCES aircraft (id);"
      "ALTER TABLE route "
      "ADD FOREIGN KEY (hotel_id) REFERENCES hotels (id);"
      "ALTER TABLE route "
      "ADD FOREIGN KEY (flight_id) REFERENCES flights (id);"
      "ALTER TABLE route "
      "ADD FOREIGN KEY (employee_id) REFERENCES employees (id);"
      "ALTER TABLE aircraft "
      "ADD FOREIGN KEY (carrier_id) REFERENCES carriers (id);"
      "ALTER TABLE client_route "
      "ADD FOREIGN KEY (client_id) REFERENCES clients (id);"
      "ALTER TABLE client_route "
      "ADD FOREIGN KEY (route_id) REFERENCES route (id);";

  ret = SQLExecDirect(hstmt, (SQLCHAR *)createEmployeesTable.c_str(), SQL_NTS);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLExecDirect(hstmt, (SQLCHAR *)createEmployeeTransfersTable.c_str(),
                      SQL_NTS);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLExecDirect(hstmt, (SQLCHAR *)createHotelsTable.c_str(), SQL_NTS);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLExecDirect(hstmt, (SQLCHAR *)createFlightsTable.c_str(), SQL_NTS);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLExecDirect(hstmt, (SQLCHAR *)createRouteTable.c_str(), SQL_NTS);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLExecDirect(hstmt, (SQLCHAR *)createClientRoute.c_str(), SQL_NTS);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLExecDirect(hstmt, (SQLCHAR *)createClientsTable.c_str(), SQL_NTS);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLExecDirect(hstmt, (SQLCHAR *)createCarriersTable.c_str(), SQL_NTS);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLExecDirect(hstmt, (SQLCHAR *)createAircraftTable.c_str(), SQL_NTS);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLExecDirect(hstmt, (SQLCHAR *)createReferences.c_str(), SQL_NTS);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return false;
  }

  ret = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
  if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
    return false;
  }

  return true;
}
