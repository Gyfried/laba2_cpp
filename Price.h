#ifndef PRICE_H
#define PRICE_H

#include <iostream>

class PRICE {
private:
    char* productName;
    char* storeName;
    double price;

    static char* copyText(const char* text);
    static void replaceText(char*& destination, const char* source);

public:
    PRICE();
    PRICE(const char* productName, const char* storeName, double price);
    PRICE(const PRICE& other);
    ~PRICE();

    PRICE& operator=(const PRICE& other);

    const char* getProductName() const;
    const char* getStoreName() const;
    double getPrice() const;

    void setProductName(const char* value);
    void setStoreName(const char* value);
    void setPrice(double value);

    friend std::ostream& operator<<(std::ostream& out, const PRICE& item);
    friend std::istream& operator>>(std::istream& in, PRICE& item);
};

#endif
