//
// Created by Mihail on 16.03.2023.
//

#include <algorithm>
#include <utility>
#include "converter_json.h"
#include "file_exchange.h"
#include "search_server.h"

typedef std::vector<std::pair<std::string, size_t>> pairs_t;

SearchServer::SearchServer(InvertedIndex &inIndex) : index(inIndex) {}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queriesInput) {
    ///< Данное условие необходимо для правильной работы приложения при тестировании
    if (JSON::config == NULL)
        FileExchange::readFromFile(JSON::config, "..\\config.json");

    std::vector<std::vector<RelativeIndex>> result;

    for (auto const &request: queriesInput) {
        std::vector<std::string> sortedWords = sortUniqueWords(getUniqueWords(request));

        std::vector<size_t> docIds;
        for (size_t i = 0; i < index.getDocsAmount(); i++)
            docIds.push_back(i);

        std::vector<RelativeIndex> relativeIndexes(0);
        std::vector<int> absolutes;
        for (auto &docId: docIds)
            absolutes.push_back(calcAbsoluteForDoc(docId, sortedWords));

        calcRelative(docIds, absolutes, relativeIndexes);

        result.push_back(relativeIndexes);
    }
    return result;
}

std::unordered_set<std::string> SearchServer::getUniqueWords(const std::string &request) {
        std::stringstream wordsStream(request);
        std::unordered_set<std::string> uniqueWords;
        while (true) {
            std::string word;
            wordsStream >> word;
            uniqueWords.insert(word);
            if (wordsStream.eof())
                break;
        }
        return uniqueWords;
}

std::vector<std::string> SearchServer::sortUniqueWords(const std::unordered_set<std::string> &uniqueWords) {
    pairs_t sorted;
    for (auto const &uniqueWord: uniqueWords) {
        auto freqDictionary_ptr = index.getFreqDictionary_ptr();
        auto it = freqDictionary_ptr->find(uniqueWord);
        size_t sum = 0;
        if (it != freqDictionary_ptr->end()) {
            for (auto const &entry: it->second) {
                sum += entry.count;
            }
        }
        sorted.emplace_back(uniqueWord, sum);
    }

    std::sort(sorted.begin(), sorted.end(),
                    [] (const std::pair<std::string, size_t> &a , const std::pair<std::string, size_t> &b) {
        return a.second < b.second;
    });

    std::vector<std::string> result;
    for (auto &word : sorted) {
        result.push_back(word.first);
    }

    return result;
}

int SearchServer::calcAbsoluteForDoc(size_t &id, std::vector<std::string> &words) {
    int sum = 0;
    for (auto &word : words) {
        auto record = index.getFreqDictionary_ptr()->find(word);
        if (record != index.getFreqDictionary_ptr()->end()) {
            for (auto &entry : record->second) {
                if (entry.docId == id)
                    sum += entry.count;
            }
        }
    }
    return sum;
}

void SearchServer::calcRelative(std::vector<size_t> &Ids, std::vector<int> &absolutes, std::vector<RelativeIndex> &indexes) {
    int max = absolutes[0];
    for (int &absolute : absolutes) {
        if (max < absolute)
            max = absolute;
    }
    indexes.resize(Ids.size());
    for (int i = 0; i < Ids.size(); i++) {
        indexes[i].docId = Ids[i];
        indexes[i].rank = (absolutes[i] != 0 ? (double) absolutes[i] / max : 0);
    }

    std::sort(indexes.begin(), indexes.end(), [] (const auto &a , const auto &b) {
        return a.rank > b.rank || a.rank == b.rank && a.docId < b.docId;
    });


    auto limit = ConverterJSON::getResponsesLimit();
    if (indexes.size() > limit)
        indexes.resize(limit);

    while (indexes.back().rank == 0)
        indexes.pop_back();
}

std::vector<std::vector<std::pair<int, double>>> SearchServer::convert(
                                                const std::vector<std::vector<RelativeIndex>> &searchResult) {

    std::vector<std::vector<std::pair<int, double>>> answers;
    for (auto &oneRequestResult : searchResult) {
        std::vector<std::pair<int, double>> oneAnswer;

        for (auto &index : oneRequestResult) {
            std::pair<int, double> indexPair;

            indexPair.first = index.docId;
            indexPair.second = index.rank;

            oneAnswer.push_back(indexPair);
        }
        answers.push_back(oneAnswer);
    }
    return answers;
}