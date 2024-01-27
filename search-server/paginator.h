#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end, size_t page_size)
        : begin_(begin), end_(end), size_(page_size) {
    }
 
    Iterator begin() const {
        return begin_;
    }
 
    Iterator end() const {
        return end_;
    }
 
    size_t size() const {
        return size_;
    }
 
private:
    Iterator begin_;
    Iterator end_;
    size_t size_;
};

template <typename Iterator>
class Paginator {
public:
    explicit Paginator(Iterator begin, Iterator end, size_t page_size) {
        for(auto it = begin; it!=end; it = next(it,page_size)){
            if(distance(it,end)<page_size){
                pages_.push_back({it, end,page_size});
                break;
            }
            pages_.push_back({it, next(it,page_size),page_size});
        }
    }
 
    auto begin() const {
        return pages_.begin();
    }
 
    auto end() const {
        return pages_.end();
    }
 
    size_t size() const {
        return pages_.size();
    }
 
private:
    vector<IteratorRange<Iterator>> pages_;
};



template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

template <typename Iterator>
ostream& operator<<(ostream& out, const IteratorRange<Iterator>& range) {
    for (Iterator it = range.begin(); it != range.end(); ++it) {
        out << *it;
    }
    return out;
}