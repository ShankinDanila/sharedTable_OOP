// I decided to make a table without duplicate keys
#pragma once
#include <iostream>
#include <cstring>


namespace table{
    const int INF_MAX = 10;

    struct Element {
        bool m_busy;
        int m_key;
        char m_info[INF_MAX];
        Element(): m_busy(false), m_key(0) { std::strcpy(m_info, ""); }
        Element(int key_init, const char* info_init): m_key(key_init), m_busy(true) {
            if (strlen(info_init) >= INF_MAX) { throw std::logic_error("string overflow"); }
            std::strcpy(m_info, info_init);
        }
        friend std::ostream& operator <<(std::ostream&, const Element&);
    };

    class Table {
    private:
        static const int QUOTA = 10;
        Element* m_table;
        int m_curr_size;
        int m_numQUOTA;
    public:
        //constructors/////////////////////////////
        Table(): m_curr_size(0), m_table(new Element[QUOTA]), m_numQUOTA(1) {};
        Table(const Element* arr_elem, int size_init);
        Table(const Table&);
        Table (Table&&) noexcept ;
        ~Table() { delete[] m_table; }

        //overloaded operators of assignment/////
        Table& operator =(const Table&);
        Table& operator =(Table &&) noexcept;

        //setters/////////////////////////////////
        Table& operator +=(const Element& elem);
        Table& operator -=(int key);
        void garbageCollector();

        //work with streams//////////////////////
        friend std::istream& operator >>(std::istream&, Table&);
        friend std::ostream& operator <<(std::ostream&, Table&);

        //getters////////////////////////////////
        [[nodiscard]] int find(int key) const;
        [[nodiscard]] Element findElem(int key) const;
        [[nodiscard]] const Element* getArray() const { return m_table; }
        [[nodiscard]] int getCurrSize() const { return m_curr_size; }
        auto operator [](int key) -> char(&)[INF_MAX];
        auto operator [](int key) const -> const char(&)[INF_MAX];
        explicit operator bool() const;
    };
}

namespace Tools {
    template <typename T>
    int getNum(T &elem) {
        std::cin >> elem;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            return 0;
        }
        return 1;
    }
}
