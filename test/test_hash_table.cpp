#include "gtest.h"
#include "hash_table.h"
#include <string>
#include <vector>
#include <map>      // Для сравнения и отслеживания состояния
#include <random>   // Для генерации случайных данных
#include <algorithm> // Для std::shuffle, std::sort
#include <chrono>    // Для измерения времени (опционально)

// Тестовый набор для класса Hash_table
class HashTableTest : public ::testing::Test {
protected:
    // Здесь можно определить общие объекты или настройки для тестов,
    // но для простых тестов часто достаточно создавать объекты внутри каждого TEST.
    // void SetUp() override {}
    // void TearDown() override {}

    // Используем конкретные типы для тестов
    using StringTable = Hash_table<std::string, std::string>;
    using IntTable = Hash_table<int, double>;
};

// --- Тесты Конструкторов и Начального Состояния ---

TEST_F(HashTableTest, DefaultConstructor) {
    // Тестируем конструктор по умолчанию
    StringTable table;
    EXPECT_EQ(table.size(), 0);          // Ожидаем размер 0
    EXPECT_TRUE(table.empty());         // Ожидаем, что таблица пуста
    EXPECT_EQ(table.get_capacity(), 1); // Ваш конструктор по умолчанию ставит capacity = 1
}

TEST_F(HashTableTest, ConstructorWithCapacity) {
    // Тестируем конструктор с заданной емкостью
    size_t initial_capacity = 50;
    StringTable table(initial_capacity);
    EXPECT_EQ(table.size(), 0);
    EXPECT_TRUE(table.empty());
    EXPECT_EQ(table.get_capacity(), initial_capacity); // Ожидаем заданную емкость
}

// --- Тесты Метода insert ---

TEST_F(HashTableTest, InsertNewElement) {
    // Тестируем вставку нового элемента
    StringTable table;
    table.insert("apple", "red");
    EXPECT_EQ(table.size(), 1);             // Размер должен стать 1
    EXPECT_FALSE(table.empty());            // Таблица не должна быть пустой
    EXPECT_EQ(table.find("apple"), "red"); // Проверяем, что элемент можно найти
}

TEST_F(HashTableTest, InsertMultipleElements) {
    // Тестируем вставку нескольких разных элементов
    StringTable table(10); // Задаем емкость, чтобы избежать рехеширования сразу
    table.insert("apple", "red");
    table.insert("banana", "yellow");
    table.insert("grape", "purple");

    EXPECT_EQ(table.size(), 3);
    EXPECT_EQ(table.find("apple"), "red");
    EXPECT_EQ(table.find("banana"), "yellow");
    EXPECT_EQ(table.find("grape"), "purple");
}

TEST_F(HashTableTest, InsertExistingElement) {
    // Тестируем вставку элемента с уже существующим ключом (обновление)
    StringTable table;
    table.insert("apple", "red");
    EXPECT_EQ(table.size(), 1);
    EXPECT_EQ(table.find("apple"), "red");

    table.insert("apple", "green"); // Вставляем тот же ключ с новым значением
    EXPECT_EQ(table.size(), 1);     // Размер НЕ должен увеличиться
    EXPECT_EQ(table.find("apple"), "green"); // Значение должно обновиться
}

// --- Тесты Метода find ---

TEST_F(HashTableTest, FindExistingElement) {
    // Тестируем поиск существующего элемента
    IntTable table(5);
    table.insert(10, 3.14);
    table.insert(20, 2.71);

    EXPECT_EQ(table.find(10), 3.14);
    EXPECT_EQ(table.find(20), 2.71);
}

TEST_F(HashTableTest, FindNonExistingElement) {
    // Тестируем поиск несуществующего элемента
    IntTable table(5);
    table.insert(10, 3.14);

    // Ожидаем, что поиск несуществующего ключа бросит исключение (const char*)
    // Примечание: Использование const char* для исключений - плохая практика.
    // Лучше использовать std::out_of_range или свой класс исключения.
    EXPECT_THROW(table.find(999), const char*);
}

// --- Тесты Метода erase ---
// ВАЖНО: Эти тесты предполагают, что вы ИСПРАВИЛИ ошибки в вашем методе erase,
// как описано в замечаниях выше (правильное имя переменной index, вычисление индекса
// перед циклом, ОТСУТСТВИЕ throw вне функции).

TEST_F(HashTableTest, EraseExistingElement) {
    // Тестируем удаление существующего элемента
    StringTable table;
    table.insert("one", "1");
    table.insert("two", "2");
    table.insert("three", "3");
    ASSERT_EQ(table.size(), 3); // Убедимся, что размер правильный перед удалением

    table.erase("two"); // Удаляем средний элемент

    EXPECT_EQ(table.size(), 2);            // Размер должен уменьшиться
    EXPECT_EQ(table.find("one"), "1");     // Остальные элементы должны быть доступны
    EXPECT_EQ(table.find("three"), "3");
    EXPECT_THROW(table.find("two"), const char*); // Удаленный элемент не должен находиться
}

TEST_F(HashTableTest, EraseNonExistingElement) {
    // Тестируем удаление несуществующего элемента
    StringTable table;
    table.insert("one", "1");
    table.insert("three", "3");
    ASSERT_EQ(table.size(), 2);

    // Удаление несуществующего ключа не должно ничего менять и не должно бросать исключений
    // (исходя из исправленной логики erase, т.к. throw в вашем коде - синтаксическая ошибка)
    EXPECT_NO_THROW(table.erase("two"));

    EXPECT_EQ(table.size(), 2); // Размер не должен измениться
    EXPECT_EQ(table.find("one"), "1");
    EXPECT_EQ(table.find("three"), "3");
}

TEST_F(HashTableTest, EraseLastElement) {
    // Тестируем удаление последнего оставшегося элемента
    StringTable table;
    table.insert("only", "the one");
    ASSERT_EQ(table.size(), 1);

    table.erase("only");

    EXPECT_EQ(table.size(), 0);
    EXPECT_TRUE(table.empty());
    EXPECT_THROW(table.find("only"), const char*);
}

// --- Тесты Методов size и empty ---

TEST_F(HashTableTest, SizeAndEmptyAfterOperations) {
    // Тестируем size и empty после различных операций
    StringTable table;
    EXPECT_EQ(table.size(), 0);
    EXPECT_TRUE(table.empty());

    table.insert("a", "1");
    table.insert("b", "2");
    EXPECT_EQ(table.size(), 2);
    EXPECT_FALSE(table.empty());

    table.insert("c", "3");
    EXPECT_EQ(table.size(), 3);

    table.erase("b");
    EXPECT_EQ(table.size(), 2);
    EXPECT_FALSE(table.empty());

    table.erase("a");
    EXPECT_EQ(table.size(), 1);

    table.erase("c");
    EXPECT_EQ(table.size(), 0);
    EXPECT_TRUE(table.empty());

    // Проверка после повторного удаления (не должно менять размер)
    table.erase("a");
    EXPECT_EQ(table.size(), 0);
    EXPECT_TRUE(table.empty());
}

// --- Тесты Метода clear ---

TEST_F(HashTableTest, ClearEmptyTable) {
    // Тестируем очистку пустой таблицы
    StringTable table;
    size_t initial_capacity = table.get_capacity();
    table.clear();
    EXPECT_EQ(table.size(), 0);
    EXPECT_TRUE(table.empty());
    EXPECT_EQ(table.get_capacity(), initial_capacity); // Емкость не должна меняться
}

TEST_F(HashTableTest, ClearNonEmptyTable) {
    // Тестируем очистку непустой таблицы
    StringTable table;
    table.insert("key1", "val1");
    table.insert("key2", "val2");
    ASSERT_EQ(table.size(), 2);
    size_t initial_capacity = table.get_capacity();

    table.clear();

    EXPECT_EQ(table.size(), 0);
    EXPECT_TRUE(table.empty());
    EXPECT_EQ(table.get_capacity(), initial_capacity); // Емкость не должна меняться
    EXPECT_THROW(table.find("key1"), const char*); // Элементы должны удалиться
    EXPECT_THROW(table.find("key2"), const char*);
}

// --- Тесты Рехеширования (неявные) ---

TEST_F(HashTableTest, RehashingOnInsert) {
    // Тестируем, что таблица корректно работает после рехеширования
    // Используем IntTable для простоты генерации ключей
    IntTable table(2); // Начинаем с маленькой емкости (capacity = 2)
    // Порог 0.75. Рехеш должен произойти при вставке 2-го элемента (2/2 > 0.75 неверно, при вставке 3-го)
    // (curr_size + 1) / capacity > DEFAULT_MAX_LOAD_FACTOR
    // Вставка 1: size=1, (1+1)/2=1 > 0.75 -> rehash до capacity=4
    // Вставка 2: size=2, (2+1)/4=0.75 <= 0.75
    // Вставка 3: size=3, (3+1)/4=1 > 0.75 -> rehash до capacity=8
    // Вставка 4: size=4, (4+1)/8=0.625 <= 0.75
    // ...

    size_t capacity_before = table.get_capacity();
    EXPECT_EQ(capacity_before, 2);

    // Вставляем элементы, чтобы точно вызвать рехеширование несколько раз
    const int num_elements = 20;
    std::vector<std::pair<int, double>> elements;
    for (int i = 0; i < num_elements; ++i) {
        elements.push_back({ i, static_cast<double>(i * i) });
        table.insert(elements.back().first, elements.back().second);
    }

    EXPECT_EQ(table.size(), num_elements); // Проверяем размер после всех вставок
    size_t capacity_after = table.get_capacity();
    EXPECT_GT(capacity_after, capacity_before); // Емкость должна была увеличиться

    // Проверяем, что все элементы все еще доступны после рехеширования
    for (const auto& pair : elements) {
        EXPECT_EQ(table.find(pair.first), pair.second);
    }

    // Проверяем еще раз несуществующий элемент
    EXPECT_THROW(table.find(-1), const char*);
}

// Тестовый набор для стресс-тестов Hash_table
class HashTableStressTest : public ::testing::Test {
protected:
    // Здесь можно определить общие объекты или настройки для тестов
    // void SetUp() override {}
    // void TearDown() override {}

    // Генератор случайных чисел для всех тестов в этом наборе
    std::mt19937 rng{ std::random_device{}() };

    // Функция для генерации случайной строки
    std::string generate_random_string(size_t length) {
        const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::uniform_int_distribution<size_t> dist(0, characters.length() - 1);
        std::string random_string;
        random_string.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            random_string += characters[dist(rng)];
        }
        return random_string;
    }
};

// --- Стресс-тест: Большое количество вставок и поисков ---
TEST_F(HashTableStressTest, MassiveInsertsAndFinds) {
    Hash_table<int, int> ht;
    std::map<int, int> reference_map; // Для проверки корректности

    const int num_elements = 100000; // Увеличим количество элементов

    // 1. Вставка большого количества элементов
    for (int i = 0; i < num_elements; ++i) {
        int key = i;
        int value = i * 3 + 7; // Некоторое преобразование значения
        ht.insert(key, value);
        reference_map[key] = value;
        // Проверяем размер на каждом шаге (может быть накладно для очень больших num_elements)
        // Для чистого стресс-теста на производительность можно убрать
        ASSERT_EQ(ht.size(), reference_map.size());
    }
    ASSERT_EQ(ht.size(), num_elements);
    size_t capacity_after_inserts = ht.get_capacity();
    std::cout << "[MassiveInsertsAndFinds] Capacity after " << num_elements << " inserts: " << capacity_after_inserts << std::endl;
    EXPECT_GT(capacity_after_inserts, 1); // Емкость должна была вырасти

    // 2. Поиск всех вставленных элементов
    for (int i = 0; i < num_elements; ++i) {
        ASSERT_NO_THROW({
            ASSERT_EQ(ht.find(i), reference_map.at(i)) << "Mismatch for key " << i;
            });
    }

    // 3. Попытка найти несуществующие элементы (ключи за пределами вставленных)
    for (int i = num_elements; i < num_elements + 1000; ++i) {
        ASSERT_THROW(ht.find(i), const char*);
    }
}

// --- Стресс-тест: Случайные операции (вставка, обновление, удаление, поиск) ---
TEST_F(HashTableStressTest, RandomOperations) {
    Hash_table<std::string, int> ht;
    std::map<std::string, int> reference_map;

    const int num_operations = 200000; // Общее количество операций
    const int max_key_value = 10000;    // Ограничение для генерации ключей (чтобы были коллизии и обновления)
    const int string_key_length = 10;   // Длина строковых ключей

    std::uniform_int_distribution<int> op_dist(0, 3); // 0: insert/update, 1: erase, 2: find_existing, 3: find_non_existing
    std::uniform_int_distribution<int> key_num_dist(0, max_key_value - 1);
    std::uniform_int_distribution<int> val_dist(0, 1000000);

    std::vector<std::string> existing_keys_cache; // Кэш для выбора существующих ключей для erase/find

    for (int i = 0; i < num_operations; ++i) {
        int operation_type = op_dist(rng);
        std::string key;

        if (operation_type == 0) { // Insert/Update
            key = "key_" + std::to_string(key_num_dist(rng));
            // или можно использовать generate_random_string(string_key_length);
            int value = val_dist(rng);
            ht.insert(key, value);
            reference_map[key] = value;
            if (std::find(existing_keys_cache.begin(), existing_keys_cache.end(), key) == existing_keys_cache.end()) {
                existing_keys_cache.push_back(key);
            }
        }
        else if (operation_type == 1 && !existing_keys_cache.empty()) { // Erase existing
            std::uniform_int_distribution<size_t> existing_key_idx_dist(0, existing_keys_cache.size() - 1);
            size_t key_idx_to_erase = existing_key_idx_dist(rng);
            key = existing_keys_cache[key_idx_to_erase];

            ht.erase(key);
            reference_map.erase(key);
            existing_keys_cache.erase(existing_keys_cache.begin() + key_idx_to_erase);
        }
        else if (operation_type == 2 && !existing_keys_cache.empty()) { // Find existing
            std::uniform_int_distribution<size_t> existing_key_idx_dist(0, existing_keys_cache.size() - 1);
            key = existing_keys_cache[existing_key_idx_dist(rng)];

            auto it_ref = reference_map.find(key);
            ASSERT_NE(it_ref, reference_map.end()) << "Key " << key << " should be in reference_map but isn't.";
            ASSERT_NO_THROW({
                ASSERT_EQ(ht.find(key), it_ref->second) << "Mismatch for existing key " << key;
                });
        }
        else { // Find non-existing (or operation_type == 1/2 and cache is empty)
            key = "non_existent_key_" + std::to_string(i); // Гарантированно не существующий
            ASSERT_THROW(ht.find(key), const char*) << "Should throw for non-existing key " << key;
        }

        if (i % (num_operations / 20) == 0 && i > 0) { // Периодическая проверка размера
            ASSERT_EQ(ht.size(), reference_map.size()) << "Size mismatch at operation " << i;
            std::cout << "[RandomOperations] Progress: " << (static_cast<double>(i) / num_operations * 100.0) << "%, "
                << "Size: " << ht.size() << ", Capacity: " << ht.get_capacity() << std::endl;
        }
    }

    // Финальная проверка всех элементов
    ASSERT_EQ(ht.size(), reference_map.size());
    for (const auto& pair : reference_map) {
        ASSERT_NO_THROW({
            ASSERT_EQ(ht.find(pair.first), pair.second) << "Final check mismatch for key " << pair.first;
            });
    }
    std::cout << "[RandomOperations] Final size: " << ht.size() << ", Final capacity: " << ht.get_capacity() << std::endl;
}

// --- Стресс-тест: Вставка элементов с предсказуемыми коллизиями ---
// Для этого теста нужна хэш-функция, которая легко вызывает коллизии,
// или мы должны подобрать ключи, которые будут хэшироваться в один бакет.
// Поскольку мы используем std::hash, предсказать коллизии сложно без специализации.
// Поэтому этот тест будет больше сосредоточен на проверке корректности длинных цепочек.
// Мы можем попытаться вызвать это, используя много ключей, которые могут быть близки.
// Или, если бы у нас была возможность передать свою хэш-функцию, это было бы проще.
// Для текущей реализации сделаем тест на большое количество вставок с проверкой целостности.
// Адаптируем тест RehashingIntegrity из предыдущего ответа, но с большим количеством элементов.
TEST_F(HashTableStressTest, IntegrityDuringManyRehashes) {
    Hash_table<int, int> ht(2); // Начнем с очень маленькой capacity
    std::map<int, int> reference_map;
    const int num_elements = 25000; // Увеличим количество

    size_t current_capacity_marker = ht.get_capacity();
    ASSERT_EQ(current_capacity_marker, 2);

    for (int i = 0; i < num_elements; ++i) {
        int key = i;
        // Используем разные значения, чтобы убедиться, что не только ключи, но и значения корректны
        int value = rng() % 10000; // Случайное значение
        ht.insert(key, value);
        reference_map[key] = value;

        ASSERT_EQ(ht.size(), reference_map.size()) << "Size mismatch at i=" << i;
        ASSERT_NO_THROW({
            ASSERT_EQ(ht.find(key), value) << "Value mismatch for current key " << key << " at i=" << i;
            });

        // Если произошло рехеширование, проверяем ВСЕ ранее вставленные элементы
        if (ht.get_capacity() > current_capacity_marker) {
            std::cout << "[IntegrityDuringManyRehashes] Rehash occurred! Old capacity: " << current_capacity_marker
                << ", New capacity: " << ht.get_capacity() << " at element count: " << ht.size() << std::endl;
            current_capacity_marker = ht.get_capacity();
            for (const auto& pair : reference_map) {
                ASSERT_NO_THROW({
                   ASSERT_EQ(ht.find(pair.first), pair.second)
                       << "Mismatch for key " << pair.first << " after rehash at i=" << i;
                    });
            }
        }
    }

    EXPECT_GT(ht.get_capacity(), 2); // Убедимся, что rehash точно произошел много раз
    ASSERT_EQ(ht.size(), num_elements);

    // Финальная проверка всех элементов
    for (const auto& pair : reference_map) {
        ASSERT_NO_THROW({
            ASSERT_EQ(ht.find(pair.first), pair.second) << "Final check mismatch for key " << pair.first;
            });
    }
    std::cout << "[IntegrityDuringManyRehashes] Final size: " << ht.size() << ", Final capacity: " << ht.get_capacity() << std::endl;
}


