//
// Created by Mihail on 14.03.2023.
//
/*!
 * \file
 * \brief Header file declaring the structure of the processed data and the class for working with 
 * JSON files
 *
 * Header file declaring the structure of the processed data and the class for working with JSON files.
 * Contains a JSON structure consisting of a set of static fields for storing JSON-type data, as well as 
 * the ConverterJSON class for working with JSON files, the EmptyConfigException and WrongVersionException 
 * classes for throwing exceptions when working with configuration data.
 */

#pragma once

#include <vector>
#include <string>
#include "nlohmann/json.hpp"
#include "status.h"

/*!
 *\brief Structure for storing static data in JSON format
 */
struct JSON {
    static nlohmann::json config;
    static nlohmann::json requests;
    static nlohmann::json answers;
};

/*!
 *\brief The class that overrides the what method of the parent class std::exception
 *
 * The class that overrides the what method of the parent class std::exception.
 * Used to throw an exception when there is no "config" field in the JSON::config variable.
 */
class EmptyConfigException : public std::exception {
public:
    const char* what() const noexcept override;
};

/*!
 *\brief The class that overrides the what method of the parent class std::exception
 *
 * The class that overrides the what method of the parent class std::exception.
 * Used to throw an exception if the engine version specified in the "config" field of the 
 * JSON::config variable does not match the project version specified in the CMakelists.txt file
 */
class WrongVersionException: public std::exception {
public:
    const char* what() const noexcept override;
};

/*!
 *\brief Class containing the checkCorrectConfig() method to validate the data stored in the 
 * JSON::config variable
 */
class ConfigInit {
public:
    ConfigInit();

/*!
 *\brief Method for determining the validity of data stored in a JSON::config variable
 *
 * @return the data validity status:
 * OK - data is valid;
 * ERROR - the data is invalid or an error occurred while processing it
 */
    static status checkCorrectConfig();
};

/*!
 *
 * Class for working with JSON files
 */

class ConverterJSON {
public:
    ConverterJSON() = default;

/*!
 * File content retrieval method
 * @return list with contents of files listed in config.json
 */
    static std::vector<std::string> getTextDocuments();

/*!
 * The method reads the max_responses field to determine 
 * the maximum number of responses per request
 * @return limit on the number of responses per request
 */
    static size_t getResponsesLimit();

/*!
 * The method for receiving requests from the requests.json file
 * @return list of requests from requests.json file
 */
    static std::vector<std::string> getRequests();

/*!
 * The method for putting search results in the answers.json file
 */
    static status putAnswers(std::vector<std::vector<std::pair<size_t, double>>>
    answers);
};

