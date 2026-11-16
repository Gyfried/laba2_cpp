#include "TextProcessor.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <stdexcept>
#include <cctype>

namespace {
    bool isSentenceEnd(char ch) {
        return ch == '.' || ch == '!' || ch == '?';
    }

    void trimSentence(char* sentence) {
        int length = static_cast<int>(std::strlen(sentence));
        int begin = 0;

        while (begin < length &&
               std::isspace(static_cast<unsigned char>(sentence[begin]))) {
            ++begin;
        }

        while (length > begin &&
               std::isspace(static_cast<unsigned char>(sentence[length - 1]))) {
            --length;
        }

        int newLength = length - begin;
        if (begin > 0 && newLength > 0) {
            std::memmove(sentence, sentence + begin, newLength);
        }
        sentence[newLength] = '\0';
    }
}

void TextProcessor::printSentencesInReverse(const char* fileName) {
    if (fileName == nullptr || fileName[0] == '\0') {
        throw std::invalid_argument("Имя файла не должно быть пустым.");
    }

    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть указанный файл.");
    }

    // Файловый поток читает файл, строковый поток собирает весь текст.
    std::ostringstream textBuffer;
    textBuffer << file.rdbuf();

    if (file.bad()) {
        throw std::runtime_error("Ошибка при чтении файла.");
    }
    file.close();

    const std::string text = textBuffer.str();
    if (text.empty()) {
        std::cout << "Файл пуст.\n";
        return;
    }

    // Строковый поток используется для последовательного разбора текста.
    std::istringstream input(text);

    int capacity = 8;
    int count = 0;
    char** sentences = new char*[capacity];

    char* current = new char[text.size() + 1];
    int currentLength = 0;
    char ch;

    try {
        while (input.get(ch)) {
            current[currentLength++] = ch;

            if (isSentenceEnd(ch)) {
                // Включаем подряд идущие знаки конца предложения (?!, ...).
                while (input.peek() != EOF) {
                    char next = static_cast<char>(input.peek());
                    if (isSentenceEnd(next)) {
                        input.get(next);
                        current[currentLength++] = next;
                    } else {
                        break;
                    }
                }

                // Пробелы после знака остаются в тексте текущего предложения.
                while (input.peek() != EOF) {
                    char next = static_cast<char>(input.peek());
                    if (std::isspace(static_cast<unsigned char>(next))) {
                        input.get(next);
                        current[currentLength++] = next;
                    } else {
                        break;
                    }
                }

                current[currentLength] = '\0';
                trimSentence(current);

                if (current[0] != '\0') {
                    if (count == capacity) {
                        int newCapacity = capacity * 2;
                        char** newSentences = new char*[newCapacity];

                        for (int i = 0; i < count; ++i) {
                            newSentences[i] = sentences[i];
                        }

                        delete[] sentences;
                        sentences = newSentences;
                        capacity = newCapacity;
                    }

                    sentences[count] = new char[std::strlen(current) + 1];
                    std::strcpy(sentences[count], current);
                    ++count;
                }

                currentLength = 0;
            }
        }

        // Добавляем последний фрагмент, если файл заканчивается без . ! или ?
        if (currentLength > 0) {
            current[currentLength] = '\0';
            trimSentence(current);

            if (current[0] != '\0') {
                if (count == capacity) {
                    int newCapacity = capacity * 2;
                    char** newSentences = new char*[newCapacity];

                    for (int i = 0; i < count; ++i) {
                        newSentences[i] = sentences[i];
                    }

                    delete[] sentences;
                    sentences = newSentences;
                    capacity = newCapacity;
                }

                sentences[count] = new char[std::strlen(current) + 1];
                std::strcpy(sentences[count], current);
                ++count;
            }
        }

        if (count == 0) {
            std::cout << "В файле не найдено предложений.\n";
        } else {
            std::cout << "\n===== ПРЕДЛОЖЕНИЯ В ОБРАТНОМ ПОРЯДКЕ =====\n";
            for (int i = count - 1; i >= 0; --i) {
                std::cout << sentences[i] << '\n';
            }
        }
    } catch (...) {
        for (int i = 0; i < count; ++i) {
            delete[] sentences[i];
        }
        delete[] sentences;
        delete[] current;
        throw;
    }

    for (int i = 0; i < count; ++i) {
        delete[] sentences[i];
    }
    delete[] sentences;
    delete[] current;
}
