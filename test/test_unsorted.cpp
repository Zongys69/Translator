#include "gtest.h"
#include "unsorted.h"

// Тесты для insert
TEST(UnsortedTableTest, Insert_SingleElement) {
    Unsorted_table<int, std::string> table;
    Unsorted_table<int, std::string>::Iterator it = table.insert(1, "value1");
    ASSERT_NE(it, table.end());
    ASSERT_EQ(it->first, 1);
    ASSERT_EQ(it->second, "value1");
}

TEST(UnsortedTableTest, Insert_MultipleElements) {
    Unsorted_table<int, std::string> table;
    table.insert(1, "value1");
    table.insert(2, "value2");
    table.insert(3, "value3");

    Unsorted_table<int, std::string>::Iterator it_find = table.find(2);
    ASSERT_NE(it_find, table.end());
    ASSERT_EQ(it_find->first, 2);
    ASSERT_EQ(it_find->second, "value2");
}

// Тесты для find
TEST(UnsortedTableTest, Find_ExistingElement) {
    Unsorted_table<int, std::string> table;
    table.insert(1, "value1");
    table.insert(2, "value2");

    Unsorted_table<int, std::string>::Iterator it_find = table.find(2);
    ASSERT_NE(it_find, table.end());
    ASSERT_EQ(it_find->first, 2);
    ASSERT_EQ(it_find->second, "value2");
}

TEST(UnsortedTableTest, Find_NonExistingElement) {
    Unsorted_table<int, std::string> table;
    table.insert(1, "value1");

    Unsorted_table<int, std::string>::Iterator it_find = table.find(2);
    ASSERT_EQ(it_find, table.end());
}

// Тесты для erase
TEST(UnsortedTableTest, Erase_ExistingElement) {
    Unsorted_table<int, std::string> table;
    table.insert(1, "value1");
    table.insert(2, "value2");

    Unsorted_table<int, std::string>::Iterator it_erase = table.erase(2);
    Unsorted_table<int, std::string>::Iterator it_find = table.find(2);
    ASSERT_EQ(it_find, table.end());
}

TEST(UnsortedTableTest, Erase_NonExistingElement) {
    Unsorted_table<int, std::string> table;
    table.insert(1, "value1");

    Unsorted_table<int, std::string>::Iterator it_erase = table.erase(2);
    ASSERT_EQ(it_erase, table.end());
}

TEST(UnsortedTableTest, Erase_FirstElement) {
    Unsorted_table<int, std::string> table;
    table.insert(1, "value1");
    table.insert(2, "value2");
    Unsorted_table<int, std::string>::Iterator it_erase = table.erase(1);
    Unsorted_table<int, std::string>::Iterator it_find = table.find(1);
    ASSERT_EQ(it_find, table.end());

    it_find = table.find(2);
    ASSERT_NE(it_find, table.end());
}