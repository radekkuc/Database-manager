#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <map>
#include <unordered_map>

class Table {
private:
    std::vector<std::string> columns;
    std::vector<std::map<std::string, std::string>> rows;

public:
    Table() : columns(columns), rows(rows) {}
    void addColumn(std::string col_name);
    void addRow(std::vector<std::string> row_name);
    void displayData();
    void alterData(int index, std::string colName, std::string newVal);
    void safeToFile(std::string fileName);
    void importFromFile(std::string importFileName);
    void dynamicSearch(std::string query);
};


#endif 


