#include "Price.h"

#include <iostream>
#include <exception>
#include <limits>

int main() {
    try {
        std::cout << "=== Проверка класса PRICE  ===\n";
        std::cout << "Введите данные товара в следующем порядке:\n"
                  << "1. Название товара\n"
                  << "2. Название магазина\n"
                  << "3. Стоимость в рублях\n\n";

        PRICE item;
        std::cin >> item;

        if (!std::cin)
            throw std::runtime_error("Не удалось считать данные товара.");

        std::cout << "\nВведённый товар:\n" << item;

        PRICE copied(item);
        std::cout << "\nКопия товара:\n" << copied;

        PRICE assigned;
        assigned = item;
        std::cout << "\nПосле оператора присваивания:\n" << assigned;
    } catch (const std::exception& error) {
        std::cerr << "Ошибка: " << error.what() << '\n';
        return 1;
    }

    std::cout << "\n=== Проверка завершена ===\n";
    return 0;
}
