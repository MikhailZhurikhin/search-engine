//
// Created by Mihail on 16.03.2023.
//
/*!
 * \file
 * \brief Заголовочный файл с обьявлением структуры RelativeIndex и класса SearchServer
 */

#ifndef SEARCH_ENGINE_SEARCH_SERVER_H
#define SEARCH_ENGINE_SEARCH_SERVER_H

#pragma once

#include <unordered_set>
#include "inverted_index.h"


/*!
 * Структура, содержащая поле Id документа и поле rank, соответсвующее относительной релевантности данного документа
 * для определенного поискового запроса
 */
struct RelativeIndex {
    size_t docId;
    double rank;
    bool operator ==(const RelativeIndex &other) const {
        return (docId == other.docId && rank == other.rank);
    }
};

class SearchServer {
public:
/*!
* @param idx в конструктор класса передаётся ссылка на класс InvertedIndex,
* чтобы SearchServer мог узнать частоту слов встречаемых в запросе
*/
    SearchServer(InvertedIndex &inIndex);

/*!
* Метод обработки поисковых запросов
* @param [in] queriesInput поисковые запросы, взятые из файла requests.json
* @return возвращает отсортированный список релевантных ответов для заданных запросов
*/
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string> &queriesInput);
private:
    InvertedIndex index;

/*!
 * Метод выдаёт список уникальных слов для запроса
 * @param [in] request запрос
 * @return список уникальных слов
 */
    std::unordered_set<std::string> getUniqueWords(const std::string &request);

/*!
 * Метод сортирует уникальные слова в порядке увеличения их частоты встречаемости в соответствии с частотным словарём
 * @param [in] uniqueWords уникальные слова запроса
 * @return отсортированный список уникальных слов
 */
    std::vector<std::string> sortUniqueWords(const std::unordered_set<std::string> &uniqueWords);

/*!
 * Метод вычисляет значение абсолютной релевантности для одного документа
 * @param [in] Id документа, для которого рассчитывается значение абсолютной релевантности
 * @param [in] words набор слов, по которым вычисляется абсолютная релевантность документа
 * @return значение абсолютной релевантности
 */
    int calcAbsoluteForDoc(size_t &id, std::vector<std::string> &words);

/*!
 * Метод вычисляет относительную релевантность для каждого документа из вектора документов
 * @param [in] Ids вектор документов, по которым осуществляется поиск
 * @param [in] absolutes вектор абсолютной релевантности для вектора документов
 * @param [out] indexes вектор индекстов относительной релевантности для вектора документов
 */
    void calcRelative(std::vector<size_t> &Ids, std::vector<int> &absolutes, std::vector<RelativeIndex> &indexes);
public:
/*!
* Метод, осуществляющий конвертацию данных типа std::vector<std::vector<RelativeIndex>> в
* std::vector<std::vector<std::pair<int, double>>> для дальнейшей работы с методами класса ConverterJSON
* @param [in] searchResult входные данные типа std::vector<std::vector<RelativeIndex>>
* @return выходные данные типа std::vector<std::vector<std::pair<int, double>>>
*/
    static std::vector<std::vector<std::pair<int, double>>> convert(const std::vector<std::vector<RelativeIndex>> &searchResult);
};

#endif //SEARCH_ENGINE_SEARCH_SERVER_H
