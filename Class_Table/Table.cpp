#include "Table.h"
#include <iostream>

namespace table {

    Table::Table(const Element *arr_elem, int size_init) {
        for (int i = 0; i < size_init; i++) {
            if (find(arr_elem[i].m_key) != -1) { throw std::logic_error("invalid key"); }
        }

        int free_states = m_numQUOTA*QUOTA - m_curr_size;
        if (free_states > size_init){
            for (int i = 0; i < size_init; i++) { m_table[m_curr_size + i] = arr_elem[i]; }
            m_curr_size += size_init;
        }
        else {
            int num = (size_init - free_states)/QUOTA;
            auto* new_array = new Element[(m_numQUOTA + num)*QUOTA];

            for (int i = 0; i < m_curr_size; i++) { new_array[i] = m_table[i]; }
            for (int i = 0; i < size_init; i++) { new_array[m_curr_size + i] = arr_elem[i]; }

            m_numQUOTA += num;
            m_curr_size += size_init;
            delete[] m_table;
            m_table = new_array;
        }
    }

    Table::Table(const Table& tab): m_curr_size(tab.m_curr_size), m_numQUOTA(tab.m_numQUOTA) {
        m_table = new Element[m_numQUOTA*QUOTA];
        for (int i = 0; i < m_curr_size; i++) { m_table[i] = tab.m_table[i]; }
    }

    Table::Table(Table&& tab) noexcept: m_curr_size(tab.m_curr_size), m_table(tab.m_table), m_numQUOTA(tab.m_numQUOTA) {
        tab.m_table = nullptr;
    }

    Table &Table::operator=(const Table& tab) {
        if (this != &tab){
            m_curr_size = tab.m_curr_size;
            m_numQUOTA = tab.m_numQUOTA;
            delete[] m_table;
            m_table = new Element[m_numQUOTA*QUOTA];
            for (int i = 0; i < m_curr_size; i++) { m_table[i] = tab.m_table[i]; }
        }
        return *this;
    }

    Table &Table::operator=(Table&& tab) noexcept {
        std::swap(m_curr_size, tab.m_curr_size);
        std::swap(m_numQUOTA, tab.m_numQUOTA);
        std::swap(m_table, tab.m_table);
        return *this;
    }

    int Table::find(int key) const {
        const Element *arr_elem = m_table;

        for (int i = 0; i < m_curr_size; i++) {
            if (arr_elem[i].m_busy && arr_elem[i].m_key == key) { return i; }
        }
        return -1;
    }

    Element Table::findElem(int key) const {
        const Element *arr_elem = m_table;

        for (int i = 0; i < m_curr_size; i++) {
            if (arr_elem[i].m_busy && arr_elem[i].m_key == key) { return arr_elem[i]; }
        }
        const Element empty_elem(0, "");
        return empty_elem;
    }

    Table &Table::operator+=(const Element &elem) {
        if (find(elem.m_key) != -1) { throw std::logic_error("invalid key"); }

        if (m_curr_size != QUOTA*m_numQUOTA - 1) {
            m_table[m_curr_size] = elem;
            m_curr_size += 1;
        }
        else {
            auto* new_arr = new Element[QUOTA*(m_numQUOTA + 1)];
            for (int i = 0; i < m_curr_size; i++) { new_arr[i] = m_table[i]; }
            new_arr[m_curr_size] = elem;
            delete[] m_table;
            m_numQUOTA += 1;
            m_table = new_arr;
            m_curr_size += 1;
        }
        return *this;
    }

    Table &Table::operator-=(int key) {
        int index = find(key);

        if (index == -1) { throw std::logic_error("invalid key"); }
        m_table[index].m_busy = false;
        return *this;
    }

    void Table::garbageCollector() {
        int size = 0;
        for (int i = 0; i < m_curr_size; i++) {
            if (m_table[i].m_busy) {
                if (i != size) { m_table[size] = m_table[i]; }
                size += 1;
            }
        }
        for (int i = size; i < m_curr_size; i++) { m_table[i].m_busy = false; }
        m_curr_size = size;
    }

    std::istream& operator >>(std::istream& i, Table& table) {
        int key;
        i >> key;
        bool isCorrect = true;
        if (!i.good()) { isCorrect = false; }
        if (i.peek() != '\n' && i.peek() != ' ') { isCorrect = false; }

        char info[INF_MAX];
        if (!isCorrect) {
            i.ignore();
            i.getline(info, 9999, '\n');
            if (strlen(info) >= INF_MAX) { isCorrect = false; }
        }

        if (!isCorrect) { i.setstate(std::ios::failbit); }
        else {
            Element new_elem{key, info};
            table+=new_elem;
        }
        return i;
    }

    std::ostream& operator <<(std::ostream& o, const Element& elem) {
        o << elem.m_key << " ";
        o << elem.m_busy << " ";
        o << elem.m_info << " " << "\n";
        return o;
    }

    std::ostream& operator <<(std::ostream& o, Table& table){
        table.garbageCollector();

        for (int i = 0; i < table.m_curr_size; i++) {
            o << i << " ";
            o << table.m_table[i].m_key << " ";
            o << table.m_table[i].m_busy << " ";
            o << table.m_table[i].m_info << " " << "\n";
        }
        return o;
    }

    auto Table::operator[](int key) const -> const char (&)[INF_MAX] {
        int index = find(key);
        if (index == -1) { throw std::out_of_range("invalid key"); }
        else { return m_table[index].m_info; }
    }

    auto Table::operator[](int key) -> char (&)[INF_MAX] {
        int index = find(key);
        if (index == -1){ throw std::out_of_range("invalid key"); }
        else { return m_table[index].m_info; }
    }

    Table::operator bool() const { return this->m_curr_size != 0; }
}