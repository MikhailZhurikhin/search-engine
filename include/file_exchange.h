//
// Created by Mihail on 14.03.2023.
//
/*!
 * \file
 * \brief Header file describing template methods for exchanging information with files
 *
 * Header file describing template methods for exchanging information with files.
 * Contains the FileExchange class for working with files located on the user's PC.
 */

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "status.h"

/*!
 * Class for data exchange (reading, writing) with files located on the user's PC.
 */
class FileExchange {
public:
    FileExchange() = default;

    /*!
     * The method for reading data from a file on the user's PC.
     * @tparam T data type (nlohmann::json, std::string)
     * @param [out] object variable in which the read data is stored
     * @param [in] path path to data file
     * @param [in] isText parameter to be set as follows:
     * isText = true if T = std::string
     * @return operation status:
     * OK - reading was successful;
     * ERROR - an error occurred while reading the file.
     */
    template<typename T>
    static status readFromFile(T &object, const std::string &path, bool isText = false);

    /*!
     * The method for writing data to a file on the user's PC.
     * @tparam T data type (nlohmann::json, std::string)
     * @param [in] object variable whose data is written to the file
     * @param [in] path path to data file. If this file did not exist before, it will be created
     * @return operation status:
     * OK - recording was successful;
     * ERROR - an error occurred while writing to file.
     */
    template <typename T>
    static status writeToFile(T &object, const std::string& path);
};

template<typename T>
status FileExchange::readFromFile(T &object, const std::string &path, bool isText) {
    std::ifstream file;
    file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    try {
        file.open(path);
        if (!isText)
            file >> object;
        else {
            std::string word;
            while (true) {
                file >> word;
                object += word + ' ';
                if (file.eof())
                    break;
            }
        }
        file.close();
        return status::OK;
    }
    catch (const std::ifstream::failure &ex) {
        std::cerr << "Can't open file: " << path << std::endl;
        if (file.is_open())
            file.close();
        return status::ERROR;
    }
}

template <typename T>
status FileExchange::writeToFile(T &object, const std::string& path) {
    std::ofstream file;
    file.exceptions(std::ofstream::badbit | std::ofstream::failbit);
    try {
        file.open(path);
        file << object;
        file.close();
        return status::OK;
    }
    catch (const std::ofstream::failure &ex) {
        std::cerr << "Can't write to file: " << path << std::endl;
        if (file.is_open())
            file.close();
        return status::ERROR;
    }
}

