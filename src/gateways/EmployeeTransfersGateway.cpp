#include "EmployeeTransfersGateway.h"

EmployeeTransfersGateway::EmployeeTransfersGateway() {
  dbConnector = &DataBaseConnector::getInstance();
  sqlExecuter = &SqlExecuter::getInstance();
}

bool EmployeeTransfersGateway::insertTransfer(int employeeId,
                                              const std::string &oldPosition,
                                              const std::string &transferReason,
                                              const std::string &orderNumber,
                                              const std::string &orderDate) {
  std::string query =
      "INSERT INTO Employee_Transfers (employee_id, old_position, "
      "transfer_reason, order_number, order_date) "
      "VALUES (" +
      std::to_string(employeeId) + ", '" + oldPosition + "', '" +
      transferReason + "', '" + orderNumber + "', '" + orderDate + "')";

  return sqlExecuter->executeSQL(query);
}

bool EmployeeTransfersGateway::deleteTransfer(int transferId) {
  std::string query =
      "DELETE FROM Employee_Transfers WHERE id = " + std::to_string(transferId);

  return sqlExecuter->executeSQL(query);
}
bool EmployeeTransfersGateway::getAllTransfers(
    std::vector<std::tuple<int, int, std::string, std::string, std::string,
                           std::string>> &transferData) {
  std::string query =
      "SELECT id, employee_id, old_position, transfer_reason, order_number, "
      "order_date FROM Employee_Transfers";
  SQLHSTMT hstmt;

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int id, employee_id;
      char old_positionBuffer[256];
      char transfer_reasonBuffer[65536];  // Изменено с 256 на 65536 для
                                          // поддержки текстового поля.
      char order_numberBuffer[51];  // Изменено с 50 на 51 для поддержки полей в
                                    // 50 символов.
      char order_dateBuffer[11];  // Изменено с 10 на 11, так как длина даты 10
                                  // символов (например, "yyyy-mm-dd").

      ret = SQLFetch(hstmt);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        ret = SQLGetData(hstmt, 1, SQL_C_LONG, &id, 0, NULL);
        ret = SQLGetData(hstmt, 2, SQL_C_LONG, &employee_id, 0, NULL);
        ret = SQLGetData(hstmt, 3, SQL_C_CHAR, old_positionBuffer,
                         sizeof(old_positionBuffer), NULL);
        ret = SQLGetData(hstmt, 4, SQL_C_CHAR, transfer_reasonBuffer,
                         sizeof(transfer_reasonBuffer), NULL);
        ret = SQLGetData(hstmt, 5, SQL_C_CHAR, order_numberBuffer,
                         sizeof(order_numberBuffer), NULL);
        ret = SQLGetData(hstmt, 6, SQL_C_CHAR, order_dateBuffer,
                         sizeof(order_dateBuffer), NULL);

        std::string old_position(old_positionBuffer);
        std::string transfer_reason(transfer_reasonBuffer);
        std::string order_number(order_numberBuffer);
        std::string order_date(order_dateBuffer);

        transferData.emplace_back(id, employee_id, old_position,
                                  transfer_reason, order_number, order_date);
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !transferData.empty();
  }
  return false;
}

bool EmployeeTransfersGateway::updateTransfer(int transferId, int employeeId,
                                              const std::string &oldPosition,
                                              const std::string &transferReason,
                                              const std::string &orderNumber,
                                              const std::string &orderDate) {
  std::string query = "UPDATE Employee_Transfers SET employee_id = " +
                      std::to_string(employeeId) + ", old_position = '" +
                      oldPosition + "', transfer_reason = '" + transferReason +
                      "', order_number = '" + orderNumber +
                      "', order_date = '" + orderDate +
                      "' WHERE id = " + std::to_string(transferId);

  std::cout << query << std::endl;
  return sqlExecuter->executeSQL(query);
}

bool EmployeeTransfersGateway::getTransfer(int transferId, int &employeeId,
                                           std::string &oldPosition,
                                           std::string &transferReason,
                                           std::string &orderNumber) {
  std::string query =
      "SELECT employee_id, old_position, transfer_reason, order_number FROM "
      "Employee_Transfers "
      "WHERE id = " +
      std::to_string(transferId);

  SQLHSTMT hstmt;
  char oldPositionBuffer[256];
  char transferReasonBuffer[256];
  char orderNumberBuffer[256];

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    ret = SQLFetch(hstmt);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      SQLLEN oldPositionLength, transferReasonLength, orderNumberLength;
      ret = SQLGetData(hstmt, 1, SQL_C_LONG, &employeeId, 0, NULL);
      ret = SQLGetData(hstmt, 2, SQL_C_CHAR, oldPositionBuffer,
                       sizeof(oldPositionBuffer), &oldPositionLength);
      ret = SQLGetData(hstmt, 3, SQL_C_CHAR, transferReasonBuffer,
                       sizeof(transferReasonBuffer), &transferReasonLength);
      ret = SQLGetData(hstmt, 4, SQL_C_CHAR, orderNumberBuffer,
                       sizeof(orderNumberBuffer), &orderNumberLength);

      oldPosition = oldPositionBuffer;
      transferReason = transferReasonBuffer;
      orderNumber = orderNumberBuffer;

      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return true;
    } else {
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    }
  }

  return false;
}

bool EmployeeTransfersGateway::findTransfersByEmployee(
    int employeeId, std::vector<int> &matchingTransferIds) {
  matchingTransferIds.clear();

  std::string query = "SELECT id FROM Employee_Transfers WHERE employee_id = " +
                      std::to_string(employeeId);

  SQLHSTMT hstmt;
  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int transferId;
      ret = SQLFetch(hstmt);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        SQLGetData(hstmt, 1, SQL_C_LONG, &transferId, 0, NULL);
        matchingTransferIds.push_back(transferId);
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !matchingTransferIds.empty();
  }

  return false;
}

int EmployeeTransfersGateway::getLastInsertedId() {
  SQLHSTMT hstmt;
  std::string query = "SELECT last_value FROM employee_transfers_id_seq";

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    int lastInsertedId = -1;

    ret = SQLFetch(hstmt);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      SQLLEN idLength;
      ret = SQLGetData(hstmt, 1, SQL_C_SLONG, &lastInsertedId, 0, &idLength);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        return lastInsertedId;
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
  }

  std::cerr << "Failed to get the last inserted ID.\n";
  return -1;
}