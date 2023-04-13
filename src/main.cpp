#include "file_exchange.h"
#include "converter_json.h"
#include "search_server.h"

void paused();

int main() {
    if (FileExchange::readFromFile(JSON::config, "config.json") != status::OK ||
            ConfigInit::checkCorrectConfig() != status::OK) {
        paused();
        return 0;
    }

    InvertedIndex invertedIndex;
    invertedIndex.updateDocumentBase(ConverterJSON::getTextDocuments());

    SearchServer searchServer(invertedIndex);
    auto searchResult = searchServer.search(ConverterJSON::getRequests());

    if (ConverterJSON::putAnswers(SearchServer::convert(searchResult)) == status::OK)
        std::cout << "\nSearching is done." << std::endl;
    paused();
    return 0;
}

void paused() {
    std::cout << "\nPress Enter to exit.";
    getchar(); //!< make pause for engine does not close immediately
}