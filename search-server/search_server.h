#pragma once
#include "document.h"
#include "string_processing.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

class SearchServer {
public:
    
    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words)
    : stop_words_(MakeUniqueNonEmptyStrings(stop_words))
    {
        if (!all_of(stop_words_.begin(), stop_words_.end(), IsValidWord)) {
            throw invalid_argument("Some of stop words are invalid"s);
        }
    }

    explicit SearchServer(const string& stop_words_text)
        : SearchServer(
            SplitIntoWords(stop_words_text))
        {
        }

    void AddDocument(int document_id, const string& document, DocumentStatus status,
                     const vector<int>& ratings);
    
template <typename DocumentPredicate>
vector<Document> FindTopDocuments(const string& raw_query,
                                                    DocumentPredicate document_predicate) const {
    const auto query = ParseQuery(raw_query);

    auto matched_documents = FindAllDocuments(query, document_predicate);

    sort(matched_documents.begin(), matched_documents.end(),
         [](const Document& lhs, const Document& rhs) {
             if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
                 return lhs.rating > rhs.rating;
             } else {
                 return lhs.relevance > rhs.relevance;
             }
         });
    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }

    return matched_documents;
}

    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const;

    vector<Document> FindTopDocuments(const string& raw_query) const;

    int GetDocumentCount() const;

    int GetDocumentId(int index) const;

    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query,
                                                        int document_id) const;

private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };
    const set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;
    vector<int> document_ids_;

    bool IsStopWord(const string& word) const;

    static bool IsValidWord(const string& word);

    vector<string> SplitIntoWordsNoStop(const string& text) const;

    static int ComputeAverageRating(const vector<int>& ratings);

    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(const string& text) const;

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    Query ParseQuery(const string& text) const;

    double ComputeWordInverseDocumentFreq(const string& word) const;

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query,
                                                    DocumentPredicate document_predicate) const;
};
