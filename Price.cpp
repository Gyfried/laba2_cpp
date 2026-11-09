#include "Price.h"

#include <cstring>
#include <stdexcept>
#include <cmath>

char* PRICE::copyText(const char* text) {
    if (text == nullptr) text = "";
    char* result = new char[std::strlen(text) + 1];
    std::strcpy(result, text);
    return result;
}

void PRICE::replaceText(char*& destination, const char* source) {
    char* newText = copyText(source);
    delete[] destination;
    destination = newText;
}

PRICE::PRICE()
    : productName(copyText("Не указан")),
      storeName(copyText("Не указан")),
      price(0.0) {
    std::cout << "PRICE: конструктор без параметров\n";
}

PRICE::PRICE(const char* productName, const char* storeName, double price)
    : productName(copyText(productName)),
      storeName(copyText(storeName)),
      price(0.0) {
    setPrice(price);
    std::cout << "PRICE: конструктор с параметрами\n";
}

PRICE::PRICE(const PRICE& other)
    : productName(copyText(other.productName)),
      storeName(copyText(other.storeName)),
      price(other.price) {
    std::cout << "PRICE: конструктор копирования\n";
}

PRICE::~PRICE() {
    std::cout << "PRICE: деструктор объекта \""
              << (productName != nullptr ? productName : "без названия")
              << "\"\n";
    delete[] productName;
    delete[] storeName;
}

PRICE& PRICE::operator=(const PRICE& other) {
    if (this != &other) {
        char* newProductName = copyText(other.productName);
        char* newStoreName = nullptr;
        try {
            newStoreName = copyText(other.storeName);
        } catch (...) {
            delete[] newProductName;
            throw;
        }

        delete[] productName;
        delete[] storeName;
        productName = newProductName;
        storeName = newStoreName;
        price = other.price;
    }
    return *this;
}

const char* PRICE::getProductName() const { return productName; }
const char* PRICE::getStoreName() const { return storeName; }
double PRICE::getPrice() const { return price; }

void PRICE::setProductName(const char* value) {
    if (value == nullptr || value[0] == '\0')
        throw std::invalid_argument("Название товара не может быть пустым.");
    replaceText(productName, value);
}

void PRICE::setStoreName(const char* value) {
    if (value == nullptr || value[0] == '\0')
        throw std::invalid_argument("Название магазина не может быть пустым.");
    replaceText(storeName, value);
}

void PRICE::setPrice(double value) {
    if (!std::isfinite(value) || value < 0.0)
        throw std::invalid_argument("Стоимость должна быть неотрицательным числом.");
    price = value;
}

std::ostream& operator<<(std::ostream& out, const PRICE& item) {
    out << "Название товара: " << item.productName << '\n'
        << "Магазин: " << item.storeName << '\n'
        << "Стоимость: " << item.price << " руб.\n";
    return out;
}

std::istream& operator>>(std::istream& in, PRICE& item) {
    char productBuffer[1024];
    char storeBuffer[1024];
    double enteredPrice = 0.0;

    in >> std::ws;
    in.getline(productBuffer, sizeof(productBuffer));
    if (!in) return in;

    in.getline(storeBuffer, sizeof(storeBuffer));
    if (!in) return in;

    in >> enteredPrice;
    if (!in) return in;

    item.setProductName(productBuffer);
    item.setStoreName(storeBuffer);
    item.setPrice(enteredPrice);
    return in;
}
