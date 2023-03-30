//
// Created by Mihail on 14.03.2023.
//
/*!
 * \file
 * \brief Заголовочный файл с обьявлением структуры статических данных и класса для работы с JSON-файлами
 *
 * Заголовочный файл с обьявлением структуры статических данных и класса для работы с JSON-файлами.
 * Содержит структуру struct JSON, состоящую из набора статических полей для хранения данных JSON-типа,
 * а также класс ConverterJSON для работы с JSON-файлами,
 * классы EmptyConfigException и WrongVersionException для генерации исключений при работе с данными конфигурации.
 */
#ifndef SEARCH_ENGINE_CONVERTER_JSON_H
#define SEARCH_ENGINE_CONVERTER_JSON_H

#pragma once

#include <vector>
#include <string>
#include "nlohmann/json.hpp"
#include "status.h"

/*!
 *\brief Структура для хранения статических данных в JSON-формате
 */
struct JSON {
    static nlohmann::json config;
    static nlohmann::json requests;
    static nlohmann::json answers;
};

/*!
 *\brief Класс, переопределяющий метод what родительского класса std::exception
 *
 * Класс, переопределяющий метод what родительского класса std::exception.
 * Используется для генерации исключения при отсутствии поля "config" в переменной JSON::config.
 */
class EmptyConfigException : public std::exception {
public:
    const char* what() const noexcept override;
};

/*!
 *\brief Класс, переопределяющий метод what родительского класса std::exception
 *
 * Класс, переопределяющий метод what родительского класса std::exception.
 * Используется для генерации исключения при несоответствии версии движка, указанной в поле "config" переменной JSON::config и
 * версии проекта, указанной в файле CMakelists.txt
 */
class WrongVersionException: public std::exception {
public:
    const char* what() const noexcept override;
};

/*!
 *\brief Класс, содержащий метод checkCorrectConfig() для валидации данных, хранящихся в переменной JSON::config
 */
class ConfigInit {
public:
    ConfigInit();

/*!
 *\brief Метод для определения валидности данных, хранящихся в переменной JSON::config
 *
 * @return Возвращает статус валидности данных:
 * OK - данные валидны;
 * ERROR -данные невалидны либо возникла ошибка при их обработке
 */
    static status checkCorrectConfig();
};

/*!
*
* Класс для работы с JSON-файлами
*/

class ConverterJSON {
public:
    ConverterJSON() = default;

/*!
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных
* в config.json
*/
    std::vector<std::string> getTextDocuments();

/*!
* Метод считывает поле max_responses для определения предельного
* количества ответов на один запрос
* @return возвращает предельное количество ответов на один запрос
*/
    static int getResponsesLimit();

/*!
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
    std::vector<std::string> getRequests();

/*!
* Метод помещения в файл answers.json результатов поисковых запросов
*/
    void putAnswers(std::vector<std::vector<std::pair<int, double>>>
    answers);
};

#endif //SEARCH_ENGINE_CONVERTER_JSON_H
