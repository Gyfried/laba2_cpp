#include "Price.h"
#include "PriceList.h"
#include "TextProcessor.h"

#include <iostream>
#include <limits>
#include <stdexcept>

int readInt(const char* prompt) {
    int value;

    while (true) {
        std::cout << prompt;

        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        if (std::cin.eof()) {
            throw std::runtime_error("Ввод завершён пользователем.");
        }

        std::cout << "Ошибка: необходимо ввести целое число.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void addProduct(PriceList& list) {
    PRICE item;

    std::cout << "\nВведите данные нового товара:\n"
              << "Название товара:\n"
              << "Название магазина:\n"
              << "Стоимость в рублях:\n";

    std::cin >> item;

    if (!std::cin) {
        throw std::runtime_error("Не удалось прочитать данные товара.");
    }

    list.add(item);
    std::cout << "Товар добавлен. Список автоматически отсортирован по алфавиту.\n";
}

void editProduct(PriceList& list) {
    if (list.getCount() == 0) {
        std::cout << "Список товаров пуст.\n";
        return;
    }

    list.showAll();
    int position = readInt("\nВведите номер товара для редактирования: ");

    PRICE edited;
    std::cout << "Введите новые данные товара:\n"
              << "Название товара:\n"
              << "Название магазина:\n"
              << "Стоимость в рублях:\n";

    std::cin >> edited;

    if (!std::cin) {
        throw std::runtime_error("Не удалось прочитать новые данные товара.");
    }

    list.edit(position, edited);
    std::cout << "Запись изменена. Список повторно отсортирован.\n";
}

void searchProduct(const PriceList& list) {
    char name[1024];
    int position = 0;

    std::cout << "Введите название товара для поиска: ";
    std::cin >> std::ws;
    std::cin.getline(name, sizeof(name));

    if (!std::cin) {
        throw std::runtime_error("Не удалось прочитать название товара.");
    }

    if (list.findByName(name, position)) {
        std::cout << "\nНайден товар (позиция " << position << "):\n"
                  << list.getAt(position);
    } else {
        std::cout << "Товар с названием \"" << name << "\" не найден.\n";
    }
}

void processTextFile() {
    char fileName[1024];

    std::cout << "Введите имя или путь к текстовому файлу: ";
    std::cin.getline(fileName, sizeof(fileName));

    if (!std::cin) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::runtime_error("Слишком длинное имя файла.");
    }

    TextProcessor::printSentencesInReverse(fileName);
}

int main() {
    try {
        PriceList products;
        bool running = true;

        while (running) {
            std::cout << "\n========== МЕНЮ УПРАВЛЕНИЯ ==========\n"
                      << "1. Добавить товар\n"
                      << "2. Показать все товары\n"
                      << "3. Удалить товар\n"
                      << "4. Редактировать товар\n"
                      << "5. Найти товар по названию\n"
                      << "6. Показать количество товаров\n"
                      << "7. Вывести предложения из файла в обратном порядке\n"
                      << "0. Выход\n";

            int choice = readInt("Выберите действие: ");

            try {
                switch (choice) {
                case 1:
                    addProduct(products);
                    break;
                case 2:
                    products.showAll();
                    break;
                case 3: {
                    products.showAll();
                    int position = readInt("Введите номер товара для удаления: ");
                    products.remove(position);
                    std::cout << "Товар удалён.\n";
                    break;
                }
                case 4:
                    editProduct(products);
                    break;
                case 5:
                    searchProduct(products);
                    break;
                case 6:
                    std::cout << "Количество товаров: " << products.getCount() << '\n';
                    break;
                case 7:
                    processTextFile();
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    std::cout << "Ошибка: такого пункта меню нет.\n";
                }
            } catch (const std::exception& error) {
                std::cout << "Ошибка операции: " << error.what() << '\n';
            }
        }

        std::cout << "\nРабота программы завершена.\n";
    } catch (const std::exception& error) {
        std::cerr << "Критическая ошибка: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
