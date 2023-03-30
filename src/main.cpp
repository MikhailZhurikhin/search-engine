#include "file_exchange.h"
#include "converter_json.h"
#include "search_server.h"


int main() {
    if (FileExchange::readFromFile(JSON::config, "config.json") != OK)
        return -1;
    if (ConfigInit::checkCorrectConfig() != OK)
        return -1;

    ConverterJSON converterJson;

    InvertedIndex invertedIndex;
    invertedIndex.updateDocumentBase(converterJson.getTextDocuments());

    SearchServer searchServer(invertedIndex);
    auto searchResult = searchServer.search(converterJson.getRequests());

    converterJson.putAnswers(SearchServer::convert(searchResult));

    return 0;
}
