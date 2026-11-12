#ifndef PRICELIST_H
#define PRICELIST_H

#include "Price.h"

class PriceList {
private:
    PRICE* items;
    int count;
    int capacity;

    void resize(int newCapacity);
    void sortByName();

public:
    PriceList();
    PriceList(const PriceList& other);
    ~PriceList();

    PriceList& operator=(const PriceList& other);

    int getCount() const;
    const PRICE& getAt(int position) const;
    void add(const PRICE& item, int position);
    void remove(int position);
    void edit(int position, const PRICE& item);
    void showAll() const;
    bool findByName(const char* name, int& foundPosition) const;
};

#endif
