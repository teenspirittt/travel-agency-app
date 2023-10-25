#include "EmployeeTransfers.h"

EmployeeTransfers::EmployeeTransfers() {
  dbConnector = &DataBaseConnector::getInstance();
  sqlExecuter = &SqlExecuter::getInstance();
}

bool EmployeeTransfers::insertTransfer(int employeeId,
                                       const std::string &oldPosition,
                                       const std::string &transferReason,
                                       const std::string &orderNumber) {
  std::string query =
      "INSERT INTO Employee_Transfers (employee_id, old_position, "
      "transfer_reason, order_number) "
      "VALUES (" +
      std::to_string(employeeId) + ", '" + oldPosition + "', '" +
      transferReason + "', '" + orderNumber + "')";

  return sqlExecuter->executeSQL(query);
}

bool EmployeeTransfers::deleteTransfer(int transferId) {
  std::string query =
      "DELETE FROM Employee_Transfers WHERE id = " + std::to_string(transferId);

  return sqlExecuter->executeSQL(query);
}

bool EmployeeTransfers::getAllTransfers(std::vector<int> &transferIds) {
  std::string query = "SELECT id FROM Employee_Transfers";
  SQLHSTMT hstmt;

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int transferId;
      ret = SQLFetch(hstmt);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        ret = SQLGetData(hstmt, 1, SQL_C_LONG, &transferId, 0, NULL);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
          transferIds.push_back(transferId);
        }
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !transferIds.empty();
  }

  std::cerr << "Failed to get all employee transfers." << std::endl;
  return false;
}

bool EmployeeTransfers::updateTransfer(int transferId, int employeeId,
                                       const std::string &oldPosition,
                                       const std::string &transferReason,
                                       const std::string &orderNumber) {
  std::string query = "UPDATE Employee_Transfers SET employee_id = " +
                      std::to_string(employeeId) + ", old_position = '" +
                      oldPosition + "', transfer_reason = '" + transferReason +
                      "', order_number = '" + orderNumber +
                      "' WHERE id = " + std::to_string(transferId);

  std::cout << query << std::endl;
  return sqlExecuter->executeSQL(query);
}

bool EmployeeTransfers::getTransfer(int transferId, int &employeeId,
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

bool EmployeeTransfers::findTransfersByEmployee(
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