//
// Created by Mihail on 14.03.2023.
//
/*!
 * \file
 * \brief Header file declaring the Entry structure and the Inverted Index class
 */

#pragma once

#include <string>
#include <vector>
#include <map>

/*!
 * A structure containing the field of the document's Id  and a count field 
 * corresponding to the number of occurrences of a specific word in this document
 */
struct Entry {
    size_t docId, count;
//!< This operator is required for test scenarios
    bool operator ==(const Entry& other) const {
        return (docId == other.docId &&
                count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex();

/*!
* The method fills the base of documents on which the search is performed
* @param [in] inputDocs document content
*/
    void updateDocumentBase(const std::vector<std::string>& inputDocs);

/*!
* The method determines the number of occurrences of the word in the loaded 
* document base
* @param [in] word the word whose occurrence frequency is to be determined
* @return prepared list with word frequency
*/
    std::vector<Entry> getWordCount(const std::string &word);

/*!
 * The method for getting a frequency dictionary.
 * @return frequency dictionary
 */
    std::map<std::string, std::vector<Entry>> getFreqDictionary();

/*!
 * The method for getting the number of documents to search.
 * @return number of documents to search.
 */
    size_t getDocsAmount();

private:

    std::vector<std::string> docs; //!< document content list
    std::map<std::string, std::vector<Entry>> freqDictionary; //!< frequency dictionary

/*!
 * The method of filling the frequency dictionary with words from the documents 
 * that are being searched
 */
    void fillDictionaryByWords();

    /*!
     * The method for getting the number of occurrences of a word in a text.
     * @param [in] text word search test
     * @param [in] word the word to be searched for in the text
     * @return the number of occurrences of the word in the text
     */
    static size_t getCount(const std::string &text, const std::string &word);

/*!
 * \brief The method groups the specified number of elements into the specified number 
 * of groups.
 *
 * The method groups the specified number of elements into the specified number of groups.
 * This method is necessary for the optimal distribution of processed documents into threads.
 * For example, with the input data amount = 7, groups = 3, we get the result
 * {{0, 3, 6}, {1, 4}, {2, 5}}.
 * @param [in] amount the number of elements to be divided into groups
 * @param [in] groups the number of groups
 * @return vector of vectors of grouped element numbers
 */
    static std::vector<std::vector<size_t>> getGroupsOfElements(size_t amount, int groups);
};

