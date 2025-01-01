#include <iostream>
#include <vector>
#include <map>
#include "Table.h"
#include <stdexcept>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>


void Table::addColumn(std::string col_name) {
    if (std::find(columns.begin(), columns.end(), col_name) != columns.end()) {
        throw std::runtime_error("This name already exist in database");
    }
    else {
        columns.push_back(col_name);
    }
}

void Table::addRow(std::vector<std::string> row_name) {
    if (columns.empty()) {
        throw std::runtime_error("No columns added yet");
    }
    else if (row_name.size() != columns.size()) {
        throw std::runtime_error("Wrong number of parameters, " + std::to_string(columns.size()) + " parameters needed");
    }
    else {
        std::map<std::string, std::string> row;
        for (int i = 0; i < row_name.size(); i++) {
            row[columns[i]] = row_name[i];
        }
        rows.push_back(row);
    }
}

void Table::displayData() {
    for (int i = 0; i < columns.size(); i++) {
        std::cout << std::setw(10) << columns[i];
    }
    std::cout << std::endl;
    
    for (int z = 0; z < columns.size(); z++) {
        std::cout << std::setw(10) << "==========";
    }
    std::cout << std::endl;

    for (int j = 0; j < rows.size(); j++) {     
        for (int s = 0; s < columns.size(); s++ ){
            if(rows[j].count(columns[s]) > 0) {
                std::cout << std::setw(10) << rows[j][columns[s]];
            }
            else {
                std::cout << std::setw(10) << "-";
            }
        } 
        std::cout << std::endl;
    }
}

void Table::alterData(int index, std::string colName, std::string newVal) {
    if (index < 0 || index >= rows.size()) {
        throw std::runtime_error("Given index doesnt exist");
    }
    else if (std::find(columns.begin(), columns.end(), colName) == columns.end()) {
        throw std::runtime_error("Given column doesnt exist");
    }
    else {
        rows[index][colName] = newVal;
    }
}

void Table::safeToFile(std::string fileName) {
    std::ofstream file(fileName);
    if (!file) {
        throw std::runtime_error("Cant open a file");
    }

    if (columns.empty()) {
        throw std::runtime_error("No columns added");
        return;
    }

    for (int i = 0; i < columns.size(); i++) {
        file << columns[i];
        if (i < columns.size() - 1) file << ",";
    }
    file << "\n";

    for (int j = 0; j < rows.size(); j++) {
    
        for (int s = 0; s < columns.size(); s++) {
            if (rows[j].count(columns[s]) > 0) {
                file << rows[j][columns[s]];
                if (s < columns.size() - 1) file << ",";
            }
            else {
                file << "-";
            }
        }
        file << "\n";
    }
    file.close();
}

void Table::importFromFile(std::string importFileName) {
    std::ifstream file(importFileName);
    if (!file) {
        throw std::runtime_error("Cant open a file");
    }

    std::string line, word;
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        while (std::getline(ss, word, ',')) {
            columns.push_back(word);
        }
    }

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::map<std::string, std::string> tempMap;
        int idx = 0;
        while (std::getline(ss, word, ',')) {
            if (idx < columns.size()) {
                tempMap[columns[idx]] = word;
                idx++;
            }
        }
        rows.push_back(tempMap);
    }
}

void Table::dynamicSearch(std::string query) {
    std::string column, row,  op;
    int opPos;
    if (query.find(">") != std::string::npos) {
        op = ">";
        opPos = query.find(">");
    }
    else if (query.find("<") != std::string::npos) {
        op = "<";
        opPos = query.find("<");
    }
    else if (query.find("=") != std::string::npos) {
        op = "=";
        opPos = query.find("=");
    }
    else if (query.find("*") != std::string::npos) {
        op = "*";
        opPos = query.find("*");
    }
    else {
        throw std::runtime_error("Invalid syntax or input");
    }

    column = query.substr(0, opPos);
    row = query.substr(opPos + 1);
    column.erase(column.find_last_not_of(" ") + 1);
    std::size_t startPos = row.find_first_not_of(" ");

    if (startPos != std::string::npos) {
        row.erase(0, startPos);
    }
    else {
        row.clear();
    }

    std::size_t endPos = row.find_last_not_of(" ");
    if (endPos != std::string::npos) {
        row.erase(endPos + 1);
    }
    else {
        row.clear();
    }

    if (std::find(columns.begin(), columns.end(), column) == columns.end()) {
        throw std::runtime_error("Given name doesnt exist");
    }

    for (int i = 0; i < columns.size(); i++) {
        std::cout << std::setw(10) << columns[i];
    }
    std::cout << std::endl;

    if (op == ">") {
        for (int j = 0; j < rows.size(); j++) {
            if (std::stoi(rows[j][column]) > std::stoi(row)) {
                for (const auto& col : columns) {
                    std::cout << std::setw(10) << rows[j][col];
                }
            }
            std::cout << std::endl;
        }
    }
    else if (op == "<") {
        for (int j = 0; j < rows.size(); j++) {
            if (std::stoi(rows[j][column]) < std::stoi(row)) {
                for (const auto& col : columns) {
                    std::cout << std::setw(10) << rows[j][col];
                }
            }
            std::cout << std::endl;
        }
    }
    else if (op == "=") {
        for (int j = 0; j < rows.size(); j++) {
            if (std::stoi(rows[j][column]) == std::stoi(row)) {
                for (const auto& col : columns) {
                    std::cout << std::setw(10) << rows[j][col];
                }
            }
            std::cout << std::endl;
        }
    }
    else if (op == "*") {
        for (int j = 0; j < rows.size(); j++) {
            if ((rows[j][column]).rfind(row,0) == 0) {
                for (const auto& col : columns) {
                    std::cout << std::setw(10) << rows[j][col];
                }
            }
            std::cout << std::endl;
        }
    }
}






