#include "ClientGateway.h"

ClientGateway::ClientGateway() {
  dbConnector = &DataBaseConnector::getInstance();
  sqlExecuter = &SqlExecuter::getInstance();
}

bool ClientGateway::insertClient(const std::string &fullName,
                                 const std::string &phone,
                                 const std::string &orderDate,
                                 const std::string &clientClass,
                                 const std::string &seat) {
  std::string query =
      "INSERT INTO Clients (full_name, phone, order_date, class, seat) "
      "VALUES ('" +
      fullName + "', '" + phone + "', '" + orderDate + "', '" + clientClass +
      "', '" + seat + "')";

  return sqlExecuter->executeSQL(query);
}

bool ClientGateway::deleteClient(int clientId) {
  std::string query =
      "DELETE FROM Clients WHERE id = " + std::to_string(clientId);
  return sqlExecuter->executeSQL(query);
}

bool ClientGateway::getClient(int clientId, std::string &fullName,
                              std::string &phone, std::string &orderDate,
                              std::string &clientClass, std::string &seat) {
  std::string query =
      "SELECT full_name, phone, order_date, class, seat FROM Clients "
      "WHERE id = " +
      std::to_string(clientId);

  SQLHSTMT hstmt;
  char fullNameBuffer[256];
  char phoneBuffer[20];
  char orderDateBuffer[256];
  char clientClassBuffer[50];
  char seatBuffer[10];

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    ret = SQLFetch(hstmt);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      SQLLEN fullNameLength, phoneLength, orderDateLength, clientClassLength,
          seatLength;
      ret = SQLGetData(hstmt, 1, SQL_C_CHAR, fullNameBuffer,
                       sizeof(fullNameBuffer), &fullNameLength);
      ret = SQLGetData(hstmt, 2, SQL_C_CHAR, phoneBuffer, sizeof(phoneBuffer),
                       &phoneLength);
      ret = SQLGetData(hstmt, 3, SQL_C_CHAR, orderDateBuffer,
                       sizeof(orderDateBuffer), &orderDateLength);
      ret = SQLGetData(hstmt, 4, SQL_C_CHAR, clientClassBuffer,
                       sizeof(clientClassBuffer), &clientClassLength);
      ret = SQLGetData(hstmt, 5, SQL_C_CHAR, seatBuffer, sizeof(seatBuffer),
                       &seatLength);

      fullName = fullNameBuffer;
      phone = phoneBuffer;
      orderDate = orderDateBuffer;
      clientClass = clientClassBuffer;
      seat = seatBuffer;

      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return true;
    } else {
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    }
  }

  return false;
}

bool ClientGateway::updateClient(int clientId, const std::string &fullName,
                                 const std::string &phone,
                                 const std::string &orderDate,
                                 const std::string &clientClass,
                                 const std::string &seat) {
  std::string query =
      "UPDATE Clients "
      "SET full_name = '" +
      fullName + "', phone = '" + phone + "', order_date = '" + orderDate +
      "', class = '" + clientClass + "', seat = '" + seat +
      "' "
      "WHERE id = " +
      std::to_string(clientId);

  return sqlExecuter->executeSQL(query);
}

bool ClientGateway::getAllClients(std::vector<int> &clientIds) {
  std::string query = "SELECT id FROM clients";

  SqlExecuter &sqlExecuter = SqlExecuter::getInstance();

  SQLHSTMT hstmt;
  if (sqlExecuter.executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret;

    while ((ret = SQLFetch(hstmt)) == SQL_SUCCESS ||
           ret == SQL_SUCCESS_WITH_INFO) {
      int clientId;
      ret = SQLGetData(hstmt, 1, SQL_C_LONG, &clientId, 0, NULL);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        clientIds.push_back(clientId);
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !clientIds.empty();
  }

  std::cout << "Failed to execute SQL or no flights found." << std::endl;
  return false;
}

bool ClientGateway::findClientsByFullName(const std::string &fullName,
                                          std::vector<int> &matchingClientIds) {
  std::string query =
      "SELECT id FROM clients WHERE full_name = '" + fullName + "'";

  std::cout << query;
  SQLHSTMT hstmt;
  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int clientId;
      ret = SQLFetch(hstmt);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        ret = SQLGetData(hstmt, 1, SQL_C_LONG, &clientId, 0, NULL);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
          matchingClientIds.push_back(clientId);
        }
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !matchingClientIds.empty();
  }

  return false;
}

bool ClientGateway::findClientsByOrderDate(
    const std::string &orderDate, std::vector<int> &matchingClientIds) {
  std::string query =
      "SELECT id FROM clients WHERE order_date = '" + orderDate + "'";

  std::cout << query;
  SQLHSTMT hstmt;
  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int clientId;
      ret = SQLFetch(hstmt);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        ret = SQLGetData(hstmt, 1, SQL_C_LONG, &clientId, 0, NULL);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
          matchingClientIds.push_back(clientId);
        }
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !matchingClientIds.empty();
  }

  return false;
}