# Search Engine

This engine was written as a graduation work of [Skillbox](https://skillbox.ru/) course ["C++ Developer"](https://skillbox.ru/course/profession-c-plus-plus/).

[General overview](#general-overview)

[Technology stack](#technology-stack)

[Description](#description)

[How to use the engine](#how-to-use-the-engine)

[Conclusion](#conclusion)

## General overview

![everything_will_found](https://x-lines.ru/letters/i/cyrillictechno/0537/219ea1/60/1/ei5gkhu3qtwg15u8rbms15dcrbdg67mqco.png)

This search engine, called ***"Everything Will Found"***, is a console application that searches for user-specified text queries in user-specified text files with
issuing search results in JSON format.

## Technology stack
* [C++14](https://en.cppreference.com/w/cpp/14)
* [CMake 3.22.3](https://cmake.org/)
* [MinGW w64 9.0](https://www.mingw-w64.org/)
* [Nlohmann JSON](https://github.com/nlohmann/json)
* [Google Test](https://github.com/google/googletest)

## Description

### Common description

1. In the configuration JSON file (_config.json_) before starting the application, the names of the files that the engine will search for are set.

2. The search engine independently bypasses all files and indexes them so that later on any search query to find the most relevant documents.

3. The user sets requests through the _requests.json_ JSON file. The request is a set of words to search for documents.

4. The query is transformed into a list of words.

5. The index looks for those documents that contain all these words.

6. Search results are ranked, sorted and given to the user, the maximum number of possible documents in the response is set in configuration file.

7. At the end, the program generates the _answers.json_ file, in which it writes searching results.

#### File Description Examples

* _An example of config.json file description_:
```json
{
    "config": {
    "name": "SearchEngine",
    "version": "0.1",
    "max_responses": 5
},
    "files": [
        "../resources/file001.txt",
        "../resources/file002.txt",
        "../resources/file003.txt",
        "../resources/file004.txt"
    ]
}
```
- _config_ — general information, without which the application does not start.

  **if the _config.json_ file is missing, or if the "_config_" field in the _config.json_ file is missing, the application will generate an appropriate error and stop working.**

- _name_ — field with the name of the search engine. In this case, _"Everything Will Found"_. User can change this name.

- _version_ — field with the version number of the search engine.

  **If the application version specified in the _config.json_ file and the application version specified in _CMakeLists.txt_ do not match, the application will generate an appropriate error and stop working.**

- _max_responses_ — field that determines the maximum number of responses to one request. The default value of this field is 5.

- _files_  — the paths to the files to be searched.

  **If there is no file in the path specified in the "_files_" field, the application will issue a warning to the console and this file will be ignored during the document indexing process.**

* _An example of requests.json file description_:
```json
{
    "requests": [
        "some words..",
        "some words..",
        "some words..",
        "some words.."
    ]
}
```

* _An example of answers.json file description_:
```json
{
    "answers": {
        "request001": {
            "result": "true",
            "relevance": {
                "docid": 0, "rank" : 0.989,
                "docid": 1, "rank" : 0.897,
                "docid": 2, "rank" : 0.750,
                "docid": 3, "rank" : 0.670,
                "docid": 4, "rank" : 0.561
            }
        },
        "request002": {
            "result": "true",
            "docid": 0, "rank" : 0.769
        },
        "request003": {
            "result": "false"
        }
    }
}
```
- _answers_ — the base field in this file that contains responses to requests.

- _request001 … 003_ — the identifier of the request on which the response was generated. The request ID is generated automatically in the order in which the requests are in the _requests_ field of the _requests.json_ file. For example:
```json
{
  "requests": [
    "some words.. (the request ID is equal “request001” for this string)",
    "some words.. (the request ID is equal “request002” for this string)",
    "some words.. (the request ID is equal “request003” for this string)",
    "some words.. (the request ID is equal “request004” for this string)"
  ]
}
```
- _result_ – query search result. If it evaluates to true, then at least one document was found for this query. If the result is false, then no documents were found. Then there are no other fields in the response to this request.

- _relevance_  –  this field included in the _answers.json_ file if more than one document was found by this query.

Next, there are correspondences between the response rating and the name of the id of the document in which the search was carried out:
- _<document id>("docid")_ — ID of the document in which the response to the request was found. It is generated automatically when indexing all documents based on the order in which the documents are located in the _files_ field in the _config.json_ file. For example, if in the _config.json_ field, the _files_ field contains:
```json
{
  "files": [
    "../resources/file001.txt (docid will be equal to 0 for a given file)",
    "../resources/file002.txt (docid will be equal to 1 for a given file)",
    "../resources/file003.txt (docid will be equal to 2 for a given file)",
    "../resources/file004.txt (docid will be equal to 3 for a given file)"
  ]
}
```
- _<response rank>(“rank”)_ — search ranking. This number shows how good the document is for the given query. In the response, document id's are arranged in order of decreasing search ranking.

### Application principles

1. Serialization and deserialization of JSON structures.
To serialize and deserialize JSON structures, the _[Nlohmann JSON](https://github.com/nlohmann/json)_ library and the _ConverterJSON_ class are used.

2. Creating an inverted index for documents.
An inverted index is a data structure. In it, for each word in the collection of documents, the corresponding list lists all the documents in the collection in which it occurs.
To create an inverted index, the _InvertedIndex_ class is used, which stores document identifiers, lists of unique words for each of the documents, as well as calculated search indices.

3. Implementation of document indexing.
Indexing is the process of forming a search index for a certain amount of information, determining the relevance of answers for given queries.
For the existence of document indexing, the _SearchServer_ class is used, which calculates the relevance of the found documents relative to the given search queries.

## How to use the engine

1. Create or edit file _config.json_ with the desired application settings and the paths to the files you want to search.
2. Create or edit file _requests.json_ according to your search queries.
3. Add the following paths to the PATH environment variable:
- Path to bin folder when _MinGW.exe_ is contained (For example D:\Programs\MinGW\bin);
- Path to bin folder when _cmake.exe_ is contained (For example D:\Programs\JetBrains\CLion 2022.1.2\bin\cmake\win\bin).
4. Launch the standard console and enter the following commands:
```
cd (specify the full path to the search_engine folder)
mkdir build
cd build
cmake ../
cmake --build ./ --target search_engine
cd ../
search_engine.exe
```
5. If building happens without error, you have got a file _search_engine.exe_ in your project folder. You can execute next searching just running this file.
6. After running, file _answers.json_ will be generated. Open it, and you will see the result of searching.
7. You can also build and launch unit tests:
```
cmake --build ./ --target tests
cd tests
./tests
```

## Conclusion

This is one of my first serious projects written on C++. I will be glad to any feedback with wishes and corrections from you. You can write to me here or email mzhurihin@mail.ru.