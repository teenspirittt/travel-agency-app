#include "HotelGateway.h"

HotelGateway::HotelGateway() {
  dbConnector = &DataBaseConnector::getInstance();
  sqlExecuter = &SqlExecuter::getInstance();
}

bool HotelGateway::insertHotel(const std::string &name, int hotelClass,
                               const std::string &roomCategory) {
  std::string query =
      "INSERT INTO Hotels (name, class, room_category) "
      "VALUES ('" +
      name + "', " + std::to_string(hotelClass) + ", '" + roomCategory + "')";

  return sqlExecuter->executeSQL(query);
}

bool HotelGateway::deleteHotel(int hotelId) {
  std::string query =
      "DELETE FROM Hotels WHERE id = " + std::to_string(hotelId);

  return sqlExecuter->executeSQL(query);
}

bool HotelGateway::getAllHotels(
    std::vector<std::tuple<int, std::string, int, std::string>> &hotelData) {
  std::string query = "SELECT id, name, class, room_category FROM Hotels";
  SQLHSTMT hstmt;

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      int id, hotel_class;
      char nameBuffer[256];
      char room_categoryBuffer[256];

      ret = SQLFetch(hstmt);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        ret = SQLGetData(hstmt, 1, SQL_C_LONG, &id, 0, NULL);
        ret = SQLGetData(hstmt, 2, SQL_C_CHAR, nameBuffer, sizeof(nameBuffer),
                         NULL);
        ret = SQLGetData(hstmt, 3, SQL_C_LONG, &hotel_class, 0, NULL);
        ret = SQLGetData(hstmt, 4, SQL_C_CHAR, room_categoryBuffer,
                         sizeof(room_categoryBuffer), NULL);

        std::string name(nameBuffer);
        std::string room_category(room_categoryBuffer);

        hotelData.emplace_back(id, name, hotel_class, room_category);
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !hotelData.empty();
  }

  return false;
}

bool HotelGateway::updateHotel(int hotelId, const std::string &name,
                               int hotelClass,
                               const std::string &roomCategory) {
  std::string query = "UPDATE Hotels SET name = '" + name +
                      "', class = " + std::to_string(hotelClass) +
                      ", room_category = '" + roomCategory +
                      "' WHERE id = " + std::to_string(hotelId);

  return sqlExecuter->executeSQL(query);
}

bool HotelGateway::getHotel(int hotelId, std::string &name, int &hotelClass,
                            std::string &roomCategory) {
  std::string query =
      "SELECT name, class, room_category FROM Hotels WHERE id = " +
      std::to_string(hotelId);

  SQLHSTMT hstmt;
  char nameBuffer[256];
  char roomCategoryBuffer[256];

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    ret = SQLFetch(hstmt);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
      SQLLEN nameLength, roomCategoryLength;
      ret = SQLGetData(hstmt, 1, SQL_C_CHAR, nameBuffer, sizeof(nameBuffer),
                       &nameLength);
      ret = SQLGetData(hstmt, 2, SQL_C_LONG, &hotelClass, 0, NULL);
      ret = SQLGetData(hstmt, 3, SQL_C_CHAR, roomCategoryBuffer,
                       sizeof(roomCategoryBuffer), &roomCategoryLength);

      name = nameBuffer;
      roomCategory = roomCategoryBuffer;

      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
      return true;
    } else {
      SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    }
  }

  return false;
}

bool HotelGateway::findHotelsByName(const std::string &name,
                                    std::vector<int> &matchingHotelIds) {
  std::string query = "SELECT id FROM Hotels WHERE name = '" + name + "'";

  SQLHSTMT hstmt;

  if (sqlExecuter->executeSQLWithResults(query, hstmt)) {
    SQLRETURN ret = SQL_SUCCESS;
    while ((ret = SQLFetch(hstmt)) == SQL_SUCCESS ||
           ret == SQL_SUCCESS_WITH_INFO) {
      int hotelId;
      ret = SQLGetData(hstmt, 1, SQL_C_LONG, &hotelId, 0, NULL);
      if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        matchingHotelIds.push_back(hotelId);
      }
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return !matchingHotelIds.empty();
  }

  return false;
}

int HotelGateway::getLastInsertedId() {
  SQLHSTMT hstmt;
  std::string query = "SELECT last_value FROM hotels_id_seq";

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