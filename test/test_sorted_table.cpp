#include "sorted_table.h"
#include <gtest.h>
#include "string.h"



TEST(SortedTable, CanCreateEmptyTable) {
    SortedTable<int, std::string> table;
    ASSERT_EQ(table.begin(), table.end());
}

TEST(SortedTable, InsertIncreasesSize) {
    SortedTable<int, int> table;
    table.insert(1, 10);
    ASSERT_NE(table.begin(), table.end());
}

TEST(SortedTable, FindExistingElement) {
    SortedTable<int, std::string> table;
    table.insert(1, "one");
    auto it = table.find(1);
    ASSERT_NE(it, table.end());
    ASSERT_EQ(it->second, "one");
}

TEST(SortedTable, InsertThrowsWhenDuplicate) {
    SortedTable<std::string, int> table;
    table.insert("key", 10);
    ASSERT_THROW(table.insert("key", 20), const char*);
}

TEST(SortedTable, EraseRemovesElement) {
    SortedTable<int, double> table;
    table.insert(1, 3.14);
    table.erase(1);
    ASSERT_EQ(table.find(1), table.end());
}

TEST(SortedTable, EraseNonExistingThrows) {
    SortedTable<int, int> table;
    ASSERT_THROW(table.erase(2), const char*);
}

TEST(SortedTable, ElementsAreSortedAfterInsert) {
    SortedTable<int, char> table;
    table.insert(3, 'C');
    table.insert(1, 'A');
    table.insert(2, 'B');

    auto it = table.begin();
    ASSERT_EQ(it->first, 1);
    ASSERT_EQ(it->second, 'A');

    ++it;
    ASSERT_EQ(it->first, 2);
    ASSERT_EQ(it->second, 'B');

    ++it;
    ASSERT_EQ(it->first, 3);
    ASSERT_EQ(it->second, 'C');
}

TEST(SortedTable, IteratorIncrement) {
    SortedTable<int, int> table;
    table.insert(1, 10);
    table.insert(2, 20);

    auto it = table.begin();
    ASSERT_EQ(it->second, 10);

    auto copy = it++;
    ASSERT_EQ(copy->second, 10);
    ASSERT_EQ(it->second, 20);

    auto& ref = ++it;
    ASSERT_EQ(ref, table.end());
}

TEST(SortedTable, MaintainsSortOrderAfterComplexInserts) {
    SortedTable<int, int> table;
    table.insert(5, 50);
    table.insert(3, 30);
    table.insert(7, 70);
    table.insert(1, 10);
    table.insert(9, 90);

    auto it = table.begin();
    ASSERT_EQ(it++->first, 1);
    ASSERT_EQ(it++->first, 3);
    ASSERT_EQ(it++->first, 5);
    ASSERT_EQ(it++->first, 7);
    ASSERT_EQ(it++->first, 9);
    ASSERT_EQ(it, table.end());
}

TEST(SortedTable, CanUseRangeBasedForLoop) {
    SortedTable<int, int> table;
    table.insert(2, 20);
    table.insert(1, 10);
    table.insert(3, 30);

    int expected_key = 1;
    for (auto& pair : table) {
        ASSERT_EQ(pair.first, expected_key++);
    }
}

TEST(SortedTable, InsertReturnsCorrectIteratorPosition) {
    SortedTable<int, std::string> table;
    auto it1 = table.insert(3, "three");
    ASSERT_EQ(it1->first, 3);

    auto it2 = table.insert(1, "one");
    ASSERT_EQ(it2->first, 1);

    auto it3 = table.insert(5, "five");
    ASSERT_EQ(it3->first, 5);
}