//
// Created by Mihail on 16.03.2023.
//
/*!
 * \file
 * \brief Header file declaring the RelativeIndex structure and 
 * the SearchServer class
 */

#pragma once

#include <unordered_set>
#include "inverted_index.h"


/*!
 * A structure containing the document's Id field and a rank field corresponding to 
 * the relative relevance of this document for a particular search query.
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
 * @param idx a reference to the InvertedIndex class is passed to the class constructor 
 * so that SearchServer can find out the frequency of words found in the request
 */
    explicit SearchServer(InvertedIndex &inIndex);

/*!
 * The method of processing search queries
 * @param [in] queriesInput search requests taken from the requests.json file
 * @return sorted list of relevant responses for given queries
 */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string> &queriesInput);
private:
    InvertedIndex index;

/*!
 * The method returns a list of unique words for the query
 * @param [in] request the query
 * @return list of unique words
 */
    static std::unordered_set<std::string> getUniqueWords(const std::string &request);

/*!
 * The method sorts unique words in order of increasing frequency of occurrence according to 
 * the frequency dictionary.
 * @param [in] uniqueWords unique query words
 * @return sorted list of unique words
 */
    std::vector<std::string> sortUniqueWords(const std::unordered_set<std::string> &uniqueWords);

/*!
 * The method calculates the absolute relevance value for one document
 * @param [in] Id of the document for which the absolute relevance value is calculated
 * @param [in] words a set of words by which the absolute relevance of a document is calculated
 * @return absolute relevance value
 */
    size_t calcAbsoluteForDoc(size_t &id, std::vector<std::string> &words);

/*!
 * The method calculates the relative relevance for each document from a vector of documents
 * @param [in] Ids vector of documents to be searched
 * @param [in] absolutes absolute relevance vector for document vector
 * @param [out] indexes vector of indexes of relative relevance for the vector of documents
 */
    static void calcRelative(std::vector<size_t> &Ids, std::vector<size_t> &absolutes, 
		std::vector<RelativeIndex> &indexes);
public:
/*!
 * Method that converts data of type std::vector<std::vector<RelativeIndex>> to 
 * std::vector<std::vector<std::pair<int, double>>> for further work with ConverterJSON class methods
 * @param [in] searchResult input data of type std::vector<std::vector<RelativeIndex>>
 * @return output data of type std::vector<std::vector<std::pair<int, double>>>
 */
    static std::vector<std::vector<std::pair<size_t, double>>> convert(
		const std::vector<std::vector<RelativeIndex>> &searchResult);
};

