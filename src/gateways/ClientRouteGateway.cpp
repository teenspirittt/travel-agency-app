#include "ClientRouteGateway.h"

ClientRouteGateway::ClientRouteGateway() {
  dbConnector = &DataBaseConnector::getInstance();
  sqlExecuter = &SqlExecuter::getInstance();
}

bool ClientRouteGateway::insertClientRoute(int clientId, int routeId) {
  std::string query =
      "INSERT INTO Client_Route (client_id, route_id) VALUES (" +
      std::to_string(clientId) + ", " + std::to_string(routeId) + ")";
  return sqlExecuter->executeSQL(query);
}

bool ClientRouteGateway::deleteClientRoute(int clientId, int routeId) {
  std::string query =
      "DELETE FROM Client_Route WHERE client_id = " + std::to_string(clientId) +
      " AND route_id = " + std::to_string(routeId);
  return sqlExecuter->executeSQL(query);
}

bool ClientRouteGateway::updateClientRoute(int clientId, int routeId,
                                           int newCID, int newRID) {
  std::string query =
      "UPDATE Client_Route SET client_id =" + std::to_string(clientId) +
      ", route_id = " + std::to_string(routeId) +
      " WHERE client_id =  " + std::to_string(newCID) +
      " AND route id =" + std::to_string(newRID);

  return sqlExecuter->executeSQL(query);
}

bool ClientRouteGateway::getClientRoute(int clientId, int routeId) {
  std::string query = "SELECT * FROM Client_Route WHERE client_id = " +
                      std::to_string(clientId) +
                      " AND route_id = " + std::to_string(routeId);
  SQLHSTMT hstmt;

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    ret = SQLFetch(hstmt);
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
  }

  return false;
}

bool ClientRouteGateway::getAllClientRoutes(
    std::vector<std::pair<int, int>> &clientRoutePairs) {
  std::string query = "SELECT client_id, route_id FROM Client_Route";
  SQLHSTMT hstmt;

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int clientId, routeId;
      ret = SQLGetData(hstmt, 1, SQL_C_LONG, &clientId, 0, NULL);
      ret = SQLGetData(hstmt, 2, SQL_C_LONG, &routeId, 0, NULL);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        clientRoutePairs.push_back(std::make_pair(clientId, routeId));
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return true;
  }

  return false;
}

bool ClientRouteGateway::findRoutesByClient(
    int clientId, std::vector<int> &matchingRouteIds) {
  std::string query = "SELECT route_id FROM Client_Route WHERE client_id = " +
                      std::to_string(clientId);
  SQLHSTMT hstmt;

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int routeId;
      ret = SQLGetData(hstmt, 1, SQL_C_LONG, &routeId, 0, NULL);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        matchingRouteIds.push_back(routeId);
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return true;
  }

  return false;
}
