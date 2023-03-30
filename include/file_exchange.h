//
// Created by Mihail on 14.03.2023.
//
/*!
 * \file
 * \brief Заголовочный файл с описанием шаблонных методов для обмена информацией с файлами
 *
 * Заголовочный файл с описанием шаблонных методов для обмена информацией с файлами.
 * Содержит класс FileExchange для работы с файлами, находящимися на ПК пользователя.
 */

#ifndef SEARCH_ENGINE_FILE_EXCHANGE_H
#define SEARCH_ENGINE_FILE_EXCHANGE_H

#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "status.h"

/*!
 * Класс для обмена данными (чтение, запись) с файлами , находящимися на ПК пользователя.
 */
class FileExchange {
public:
    FileExchange() = default;

    /*!
     * Метод чтения данных из файла на ПК пользователя.
     * @tparam T тип данных (nlohmann::json, std::string)
     * @param [out] object переменная, в которую сохраняются считанные данные
     * @param [in] path путь к файлу с данными
     * @param [in] isText параметр, который должен быть установлен следующим образом:
     * isText = true ecли T = std::string
     * @return Возвращает статус операции:
     * OK - считывание прошло успешно;
     * ERROR - произошла ошибка при считывании файла.
     */
    template<typename T>
    static status readFromFile(T &object, const std::string &path, bool isText = false);

    /*!
     * Метод записи данных в файл на ПК пользователя.
     * @tparam T тип данных (nlohmann::json, std::string)
     * @param [in] object переменная, данные из которой записываются в файл
     * @param [in] path путь к файлу. Если данного файла раньше не существовало, то он будет создан
     * @return Возвращает статус операции:
     * OK - запись прошла успешно;
     * ERROR - произошла ошибка при записи в файл.
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
        return OK;
    }
    catch (const std::ifstream::failure &ex) {
        std::cerr << "Can't open file: " << path << std::endl;
        if (file.is_open())
            file.close();
        return ERROR;
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
        return OK;
    }
    catch (const std::ofstream::failure &ex) {
        std::cerr << "Can't write to file: " << path << std::endl;
        if (file.is_open())
            file.close();
        return ERROR;
    }
}

#endif //SEARCH_ENGINE_FILE_EXCHANGE_H
