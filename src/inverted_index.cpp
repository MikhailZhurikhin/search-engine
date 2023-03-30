//
// Created by Mihail on 14.03.2023.
//

#include <thread>
#include <mutex>
#include <cassert>
#include <sstream>
#include "inverted_index.h"

InvertedIndex::InvertedIndex() = default;

std::vector<std::vector<int>> InvertedIndex::getGroupsOfElements(int amount, int groups) {
    assert (amount > 0 && groups > 0);
    std::vector<std::vector<int>> result;
    if (amount < groups)
        groups = amount;

    result.resize(groups);

    int i = 0;
    for(int j = 0; j < amount; j++) {
        result[i].push_back(j);
        if (++i == groups)
            i = 0;
    }

    return result;
}

void InvertedIndex::updateDocumentBase(std::vector<std::string> inputDocs) {
    assert (!inputDocs.empty());
    docs = inputDocs;
    freqDictionary.clear();

    fillDictionaryByWords();

    for (auto &it : freqDictionary) {
       it.second = getWordCount(it.first);
    }
}


std::vector<Entry> InvertedIndex::getWordCount(const std::string &word) {
    std::vector<Entry> result;
    std::mutex resultAccess;
    ///< hardwareThreadsNum равно количеству потоков, которые возможно создать на данном ПК
    int hardwareThreadsNum = std::thread::hardware_concurrency() != 0 ? std::thread::hardware_concurrency() : 1;
    auto docGroups = getGroupsOfElements(docs.size(), hardwareThreadsNum);
    std::vector<std::thread> threads(docGroups.size());

    for(int i = 0; i < threads.size(); i++) {
        threads[i] = std::thread([i, &docGroups, this, &word, &result, &resultAccess]() {
            for (size_t docId : docGroups[i]) {
                Entry entry{};
                entry.docId = docId;
                entry.count = getCount(docs[docId], word);

                if (entry.count != 0) {
                    resultAccess.lock();
                    result.push_back(entry);
                    resultAccess.unlock();
                }
            }
        });
    }

    for (auto &thr : threads)
        thr.join();

    return result;
}


void InvertedIndex::fillDictionaryByWords() {
    std::mutex resultAccess;
    ///< hardwareThreadsNum равно количеству потоков, которые возможно создать на данном ПК
    int hardwareThreadsNum = std::thread::hardware_concurrency() != 0 ? std::thread::hardware_concurrency() : 1;
    auto docGroups = getGroupsOfElements(docs.size(), hardwareThreadsNum);
    std::vector<std::thread> threads(docGroups.size());

    for(int i = 0; i < threads.size(); i++) {
        threads[i] = std::thread([i, &docGroups, this, &resultAccess]() {
            for (size_t docId : docGroups[i]) {
                std::stringstream docStream(docs[docId]);
                std::string word;
                while(true) {
                    docStream >> word;
                    if (docStream.eof())
                        break;
                    if (freqDictionary.find(word) == freqDictionary.end()) {
                        resultAccess.lock();
                        freqDictionary.insert(std::pair<std::string, std::vector<Entry>>(word, NULL));
                        resultAccess.unlock();
                    }
                }
            }
        });
    }
    for (auto &thr : threads)
        thr.join();
}

int InvertedIndex::getCount(const std::string &text, const std::string &word) {
    if (text.empty() || text.length() < word.length())
        return 0;

    int counter = 0;
    std::stringstream textStream(text + ' '); ///< Пробел добавляется для корректной проверки на eof
    std::string wordFromText;
    while(true) {
        textStream >> wordFromText;
        if (textStream.eof())
            break;
        if (wordFromText == word)
            counter++;
    }

    return counter;
}

std::map<std::string, std::vector<Entry>>* InvertedIndex::getFreqDictionary_ptr() {
    return &freqDictionary;
}

size_t InvertedIndex::getDocsAmount() {
    return docs.size();
}