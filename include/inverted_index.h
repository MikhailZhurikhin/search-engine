//
// Created by Mihail on 14.03.2023.
//
/*!
 * \file
 * \brief Заголовочный файл с обьявлением структуры Entry и класса InvertedIndex
 */

#ifndef SEARCH_ENGINE_INVERTED_INDEX_H
#define SEARCH_ENGINE_INVERTED_INDEX_H

#pragma once

#include <string>
#include <vector>
#include <map>

/*!
 * Структура, содержащая поле Id документа и поле count, соответсвующее количеству встречаемого конктретного слова в данном документе
 */
struct Entry {
    size_t docId, count;
// Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
        return (docId == other.docId &&
                count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex();

/*!
* Метод заполняет базу документов, по которой совершается поиск
* @param [in] inputDocs содержимое документов
*/
    void updateDocumentBase(std::vector<std::string> inputDocs);

/*!
* Метод определяет количество вхождений слова word в загруженной базе
документов
* @param [in] word слово, частоту вхождений которого необходимо определить
* @return возвращает подготовленный список с частотой слов
*/
    std::vector<Entry> getWordCount(const std::string &word);

/*!
 * Метод получения указателя на частотный словарь.
 * @return возвращает указатель на частотный словарь
 */
    std::map<std::string, std::vector<Entry>>* getFreqDictionary_ptr();

/*!
 * Метод получения количества документов, по которым осуществляется поиск.
 * @return возвращает количество документов, по которым осуществляется поиск.
 */
    size_t getDocsAmount();

private:

    std::vector<std::string> docs; /// список содержимого документов
    std::map<std::string, std::vector<Entry>> freqDictionary; /// частотный словарь

/*!
 * Метод заполнения частотного словаря словами из документов, по которым осуществляется поиск
 */
    void fillDictionaryByWords();

    /*!
     * Метод получения количества вхождений слова в текст.
     * @param [in] text тест, по которому прогисходит поиск слова
     * @param [in] word слово, которое ищется в тексте
     * @return количество входждений слова word в текст text
     */
    int getCount(const std::string &text, const std::string &word);

/*!
 * \brief Метод группирует заданное количество элементов в заданное колчество групп.
 *
 * Метод группирует заданное количество элементов в заданное колчество групп.
 * Данный метод необходим для оптимального распределения обрабатываемых документов на потоки.
 * Например, при входных данных amount = 7, groups = 3, получим результат
 * {{0, 3, 6}, {1, 4}, {2, 5}}.
 * @param [in] amount количество элементов, которые нужно разбить на группы
 * @param [in] groups количество групп
 * @return вектор векторов сгруппированных номеров элементов
 */
    std::vector<std::vector<int>> getGroupsOfElements(int amount, int groups);
};

#endif //SEARCH_ENGINE_INVERTED_INDEX_H
