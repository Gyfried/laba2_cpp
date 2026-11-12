#include "PriceList.h"

#include <iostream>
#include <cstring>
#include <stdexcept>

PriceList::PriceList()
    : items(new PRICE[2]), count(0), capacity(2) {
    std::cout << "PriceList: конструктор без параметров\n";
}

PriceList::PriceList(const PriceList& other)
    : items(new PRICE[other.capacity]),
      count(other.count),
      capacity(other.capacity) {
    for (int i = 0; i < count; ++i) {
        items[i] = other.items[i];
    }
    std::cout << "PriceList: конструктор копирования\n";
}

PriceList::~PriceList() {
    std::cout << "PriceList: деструктор\n";
    delete[] items;
}

PriceList& PriceList::operator=(const PriceList& other) {
    if (this != &other) {
        PRICE* newItems = new PRICE[other.capacity];
        try {
            for (int i = 0; i < other.count; ++i) {
                newItems[i] = other.items[i];
            }
        } catch (...) {
            delete[] newItems;
            throw;
        }

        delete[] items;
        items = newItems;
        count = other.count;
        capacity = other.capacity;
    }
    return *this;
}

void PriceList::resize(int newCapacity) {
    if (newCapacity < count) {
        throw std::invalid_argument("Новая ёмкость меньше количества товаров.");
    }

    PRICE* newItems = new PRICE[newCapacity];
    try {
        for (int i = 0; i < count; ++i) {
            newItems[i] = items[i];
        }
    } catch (...) {
        delete[] newItems;
        throw;
    }

    delete[] items;
    items = newItems;
    capacity = newCapacity;
}

void PriceList::sortByName() {
    // Сортировка вставками по названию товара.
    for (int i = 1; i < count; ++i) {
        PRICE key(items[i]);
        int j = i - 1;

        while (j >= 0 &&
               std::strcmp(items[j].getProductName(), key.getProductName()) > 0) {
            items[j + 1] = items[j];
            --j;
        }
        items[j + 1] = key;
    }
}

int PriceList::getCount() const {
    return count;
}

const PRICE& PriceList::getAt(int position) const {
    if (position < 1 || position > count) {
        throw std::out_of_range("Номер товара находится вне допустимого диапазона.");
    }
    return items[position - 1];
}

void PriceList::add(const PRICE& item, int position) {
    if (position < 1 || position > count + 1) {
        throw std::out_of_range("Позиция добавления должна быть от 1 до количества товаров + 1.");
    }

    if (count == capacity) {
        resize(capacity * 2);
    }

    for (int i = count; i >= position; --i) {
        items[i] = items[i - 1];
    }

    items[position - 1] = item;
    ++count;
    sortByName();
}

void PriceList::remove(int position) {
    if (position < 1 || position > count) {
        throw std::out_of_range("Товара с таким номером нет.");
    }

    for (int i = position - 1; i < count - 1; ++i) {
        items[i] = items[i + 1];
    }
    --count;
}

void PriceList::edit(int position, const PRICE& item) {
    if (position < 1 || position > count) {
        throw std::out_of_range("Нельзя редактировать: товара с таким номером нет.");
    }

    items[position - 1] = item;
    sortByName();
}

void PriceList::showAll() const {
    if (count == 0) {
        std::cout << "Список товаров пуст.\n";
        return;
    }

    std::cout << "\n===== СПИСОК ТОВАРОВ (ПО АЛФАВИТУ) =====\n";
    for (int i = 0; i < count; ++i) {
        std::cout << "\nТовар №" << (i + 1) << '\n' << items[i];
    }
}

bool PriceList::findByName(const char* name, int& foundPosition) const {
    if (name == nullptr || name[0] == '\0') {
        throw std::invalid_argument("Название для поиска не должно быть пустым.");
    }

    for (int i = 0; i < count; ++i) {
        if (std::strcmp(items[i].getProductName(), name) == 0) {
            foundPosition = i + 1;
            return true;
        }
    }
    return false;
}
