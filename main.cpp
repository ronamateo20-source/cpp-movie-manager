#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>

using namespace std;

// Структура для зберігання даних про фільм
struct Movie {
    string title;
    string genre;
    int year;
    string status; // наприклад: "Переглянуто", "Планую", "У процесі"
    int rating;    // Оцінка від 1 до 10 (0 - без оцінки)
};

const string FILE_NAME = "movies.txt";

// Функція для збереження даних у файл
void saveToFile(const vector<Movie>& catalog) {
    ofstream file(FILE_NAME);
    if (!file.is_open()) return;
    for (const auto& m : catalog) {
        file << m.title << "|" << m.genre << "|" << m.year << "|" 
             << m.status << "|" << m.rating << "\n";
    }
    file.close();
}

// Функція для завантаження даних з файлу
void loadFromFile(vector<Movie>& catalog) {
    ifstream file(FILE_NAME);
    if (!file.is_open()) return; // Файлу ще немає, це нормально
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Movie m;
        string yearStr, ratingStr;
        
        getline(ss, m.title, '|');
        getline(ss, m.genre, '|');
        getline(ss, yearStr, '|');
        getline(ss, m.status, '|');
        getline(ss, ratingStr);
        
        if (!m.title.empty()) {
            try {
                m.year = stoi(yearStr);
                m.rating = stoi(ratingStr);
                catalog.push_back(m);
            } catch (...) {
                // Ігноруємо пошкоджені рядки
            }
        }
    }
    file.close();
}

// Безпечне введення чисел (захист від крашу, якщо ввели букви)
int getValidInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << "Помилка: некоректне введення! Будь ласка, введіть число.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Функція додавання фільму
void addMovie(vector<Movie>& catalog) {
    Movie m;
    cout << "\n--- Додавання фільму ---\n";
    cout << "Назва: ";
    getline(cin, m.title);
    cout << "Жанр: ";
    getline(cin, m.genre);
    m.year = getValidInt("Рік випуску: ");
    cout << "Статус перегляду: ";
    getline(cin, m.status);
    m.rating = 0; // Оцінка ставиться окремо, або можна запитати тут
    
    catalog.push_back(m);
    saveToFile(catalog);
    cout << "Фільм успішно додано!\n";
}

// Функція виведення списку фільмів
void displayMovies(const vector<Movie>& catalog) {
    if (catalog.empty()) {
        cout << "Каталог порожній.\n";
        return;
    }
    cout << "\n--- Каталог фільмів ---\n";
    for (size_t i = 0; i < catalog.size(); ++i) {
        const auto& m = catalog[i];
        cout << "[" << i + 1 << "] " << m.title << " (" << m.year << ") | Жанр: " << m.genre 
             << " | Статус: " << m.status 
             << " | Оцінка: " << (m.rating > 0 ? to_string(m.rating) + "/10" : "Немає") << "\n";
    }
}

// Фільтрація
void filterMovies(const vector<Movie>& catalog) {
    cout << "\nФільтрувати за:\n1. Жанром\n2. Статусом\nВибір: ";
    int choice = getValidInt("");
    string keyword;
    
    if (choice == 1) {
        cout << "Введіть жанр для пошуку: ";
        getline(cin, keyword);
    } else if (choice == 2) {
        cout << "Введіть статус для пошуку: ";
        getline(cin, keyword);
    } else {
        cout << "Невірний вибір.\n";
        return;
    }

    bool found = false;
    cout << "\n--- Результати пошуку ---\n";
    for (const auto& m : catalog) {
        if ((choice == 1 && m.genre == keyword) || (choice == 2 && m.status == keyword)) {
            cout << "- " << m.title << " (" << m.year << ") [" << m.status << "]\n";
            found = true;
        }
    }
    if (!found) cout << "Фільмів не знайдено.\n";
}

// Оцінювання фільму
void rateMovie(vector<Movie>& catalog) {
    displayMovies(catalog);
    if (catalog.empty()) return;
    
    int index = getValidInt("\nВведіть номер фільму для оцінки: ") - 1;
    if (index >= 0 && index < catalog.size()) {
        int rating = getValidInt("Введіть оцінку (1-10): ");
        if (rating >= 1 && rating <= 10) {
            catalog[index].rating = rating;
            saveToFile(catalog);
            cout << "Оцінку збережено!\n";
        } else {
            cout << "Оцінка має бути від 1 до 10.\n";
        }
    } else {
        cout << "Невірний номер фільму.\n";
    }
}

int main() {
    vector<Movie> catalog;
    loadFromFile(catalog);
    
    int choice;
    do {
        cout << "\n=== МЕНЮ КАТАЛОГУ ФІЛЬМІВ ===\n";
        cout << "1. Переглянути всі фільми\n";
        cout << "2. Додати фільм\n";
        cout << "3. Фільтрувати фільми\n";
        cout << "4. Оцінити фільм\n";
        cout << "0. Вийти\n";
        choice = getValidInt("Оберіть дію: ");
        
        switch (choice) {
            case 1: displayMovies(catalog); break;
            case 2: addMovie(catalog); break;
            case 3: filterMovies(catalog); break;
            case 4: rateMovie(catalog); break;
            case 0: cout << "Завершення роботи...\n"; break;
            default: cout << "Невідома команда.\n";
        }
    } while (choice != 0);
    
    return 0;
}
