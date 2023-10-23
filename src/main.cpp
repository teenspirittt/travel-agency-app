#include <iostream>
#include "DataBaseConnector.h"

int main() {
    DataBaseConnector dbConnector;

    if (!dbConnector.readConfigFile("./database.properties")) {
        std::cerr << "Failed to read configuration file." << std::endl;
        return 1;
    }

    if (dbConnector.connect()) {
        std::cout << "Connected to the database." << std::endl;

  

        dbConnector.disconnect();
    } else {
        std::cerr << "Failed to connect to the database." << std::endl;
        return 1;
    }

    return 0;
}