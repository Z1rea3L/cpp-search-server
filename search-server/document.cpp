#include "document.h"

ostream& operator<<(ostream& out, const Document& document) {
    return out << "{ "s << "document_id = "s << document.id << ", "s
        << "relevance = "s << document.relevance << ", "s
        << "rating = "s << document.rating << " }"s;
}