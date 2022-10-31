#include <gtest/gtest.h>
#include "../Class_Table/Table.h"
#include "string.h"

using namespace table;
TEST(DefaultElementConstructor, busy) {
	Element elem = Element();
    EXPECT_EQ(false, elem.m_busy);
}

TEST(DefaultElementConstructor, key) {
    Element elem = Element();
    EXPECT_EQ(0, elem.m_key);
}

TEST(DefaultElementConstructor, info) {
    Element elem = Element();
    ASSERT_STREQ("", elem.m_info);
}

TEST(ElementConstructor, key) {
    Element elem = Element(5, "");
    EXPECT_EQ(5, elem.m_key);
}

TEST(ElementConstructor, info) {
    Element elem = Element(5, "qqq");
    ASSERT_STREQ("qqq", elem.m_info);
}

TEST(DefaultTableConstructor, current_size) {
    Table tab = Table();
    int a = tab.getCurrSize();
    EXPECT_EQ(0, a);
}

TEST(TableConstructor, array_elems) {
    const char* s1 = "qqq";
    const char* s2 = "www";
    const char* s3 = "eee";
    Element elem1 = Element(1, s1);
    Element elem2 = Element(2, s2);
    Element elem3 = Element(3, s3);

    Element array_elem[3] = {elem1, elem2, elem3};
    Table tab = Table(array_elem, 3);
    EXPECT_EQ(elem1.m_key, tab.getArray()[0].m_key);
}

TEST(TableOperators, add) {
    Table tab = Table();
    Element elem = Element(5, "qqq");
    tab += elem;
    const Element* arr = tab.getArray();
    Element added = arr[0];

    ASSERT_STREQ("qqq", added.m_info);
}

TEST(TableMethods, find) {
    Table tab = Table();
    Element elem = Element(5, "qqq");
    tab += elem;
    const Element* arr = tab.getArray();
    int index = tab.find(5);
    EXPECT_EQ(0, index);
}

TEST(TableMethods, garbage_collector) {
    Table tab = Table();
    Element elem1 = Element(5, "qqq");
    Element elem2 = Element(6, "www");
    Element elem3 = Element(7, "eee");
    tab += elem1;
    tab += elem2;
    tab += elem3;
    tab -= 6;
    tab.garbageCollector();
    int size = tab.getCurrSize();
    EXPECT_EQ(2, size);
}

TEST(TableOperators, remove) {
    Table tab = Table();
    Element elem = Element(5, "qqq");
    tab += elem;
    tab -= 5;
    int index = tab.find(5);
    EXPECT_EQ(-1, index);
}

TEST(TableMethods, add_exception) {
    Table tab = Table();
    Element elem1 = Element(5, "qqq");
    Element elem2 = Element(5, "www");
    tab += elem1;
    ASSERT_ANY_THROW(tab += elem2);
}

TEST(TableMethods, remove_exception) {
    Table tab = Table();
    Element elem1 = Element(5, "qqq");
    Element elem2 = Element(5, "www");
    ASSERT_ANY_THROW(tab -= 10;);
}

TEST(TableOperators, const_index) {
    Table tab = Table();
    Element elem1 = Element(5, "qqq");
    tab += elem1;
    const char* message = tab[5];
    ASSERT_STREQ("qqq", message);
}

TEST(TableOperators, index) {
    Table tab = Table();
    Element elem1 = Element(5, "qqq");
    tab += elem1;
    char new_message[10] = "www";
    strcpy(tab[5], new_message);
    ASSERT_STREQ("www", tab[5]);
}

TEST(Constructor, Copy) {
    Element elem = Element(1, "qwe");
    Table tab1 = Table();
    tab1+=elem;
    Table tab2(tab1);
    const Element* arr = tab2.getArray();
    EXPECT_EQ(arr[0].m_key, 1);
    ASSERT_STREQ("qwe", arr[0].m_info);
}

TEST(Constructor, Move) {
    Element elem = Element(1, "qwe");
    Table tab1 = Table();
    tab1+=elem;
    Table tab2 = std::move(tab1);
    const Element* arr = tab2.getArray();
    EXPECT_EQ(arr[0].m_key, 1);
    ASSERT_STREQ("qwe", arr[0].m_info);
}

TEST(Operators, CopyEq) {
    Element elem = Element(1, "qwe");
    Table tab1 = Table();
    Table tab2;
    tab1+=elem;
    tab2 = tab1;
    const Element* arr = tab2.getArray();
    EXPECT_EQ(arr[0].m_key, 1);
    ASSERT_STREQ("qwe", arr[0].m_info);
}

TEST(Operators, MoveEq) {
    Element elem = Element(1, "qwe");
    Table tab1 = Table();
    Table tab2;
    tab1+=elem;
    tab2 = std::move(tab1);
    const Element* arr2 = tab2.getArray();
    const Element* arr1 = tab1.getArray();
    EXPECT_EQ(arr2[0].m_key, 1);
    ASSERT_STREQ("qwe", arr2[0].m_info);
    EXPECT_EQ(arr1[0].m_key, 0);
    ASSERT_STREQ("", arr1[0].m_info);
}


int main() {
	::testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}
