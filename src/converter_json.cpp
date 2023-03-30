//
// Created by Mihail on 14.03.2023.
//

#include <iostream>
#include "file_exchange.h"
#include "app_version.h"
#include "converter_json.h"


nlohmann::json JSON::config = NULL;
nlohmann::json JSON::requests = NULL;
nlohmann::json JSON::answers = NULL;

const char* EmptyConfigException::what() const noexcept {
    return "Config file is empty";
}

const char* WrongVersionException::what() const noexcept {
    return "config.json has incorrect file version";
}

ConfigInit::ConfigInit() = default;

status ConfigInit::checkCorrectConfig() {
    double currentVersion = 0.0;
    try {
        if (JSON::config["config"].empty())
            throw EmptyConfigException();
        currentVersion = JSON::config["config"]["version"];
        if (currentVersion != APP_VERSION)
            throw WrongVersionException();
        std::cout << "Welcome to the " << JSON::config["config"]["name"] << " application!" << std::endl;
        return OK;
    }
    catch (const EmptyConfigException &ex) {
        std::cerr << ex.what() << std::endl;
    }
    catch (const WrongVersionException &ex) {
        std::cerr << ex.what() << std::endl;
        std::cout << std::fixed << std::setprecision(1);
        std::cout << "Current version: " << currentVersion << ". Need version: " << (double) APP_VERSION << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown Error!" << std::endl;
    }
    return ERROR;
}

std::vector<std::string> ConverterJSON::getTextDocuments() {

    std::vector<std::string> texts;
    for (int i = 0; i < JSON::config["files"].size(); i++) {
        std::string text;
        std::string path = JSON::config["files"][i];
        if (FileExchange::readFromFile(text, path, true) == OK)
            texts.push_back(text);
    }
    return texts;
}

int ConverterJSON::getResponsesLimit() {
    if (JSON::config["config"]["max_responses"].empty() || JSON::config["config"]["max_responses"] < 1)
        return 5;

    return JSON::config["config"]["max_responses"];
}

std::vector<std::string> ConverterJSON::getRequests() {
    std::vector<std::string> requestsVec;
    if (FileExchange::readFromFile(JSON::requests, "requests.json") == OK) {
        for (int i = 0; i < JSON::requests["requests"].size(); i++) {
            requestsVec.push_back(JSON::requests["requests"][i]);
        }
    }
    return requestsVec;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, double>>> answers) {
    std::vector<nlohmann::json> requestJsonAnswers(answers.size());
    for (int i = 0; i < answers.size(); i++) {
        std::string request = "request" + (i < 100 ? (i < 10 ? "00" + std::to_string(i + 1) : "0" + std::to_string(i + 1)) : std::to_string(i + 1));
        if (answers[i].empty()) {
            (requestJsonAnswers[i])[request]["result"] = false;
        } else if (answers[i].size() == 1) {
            (requestJsonAnswers[i])[request]["result"] = true;
            (requestJsonAnswers[i])[request]["docid"] = answers[i][0].first;
            (requestJsonAnswers[i])[request]["rank"] = answers[i][0].second;
        } else {
            (requestJsonAnswers[i])[request]["result"] = true;
            for (int j = 0; j < answers[i].size(); j++) {
                nlohmann::json docRank;
                docRank["docid"] = answers[i][j].first;
                docRank["rank"] = answers[i][j].second;
                (requestJsonAnswers[i])[request]["relevance"].push_back(docRank);
            }
        }
    }
    nlohmann::json jsonAnswers;
    for (auto &requestJsonAnswer : requestJsonAnswers)
        jsonAnswers["answers"].push_back(requestJsonAnswer);
        JSON::answers = jsonAnswers;

    FileExchange::writeToFile(JSON::answers, "answers.json");

}
