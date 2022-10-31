//
// Created by danila on 07.10.2021.
//
#pragma ones
#include "../Class_Table/Table.h"

using namespace table;
namespace Dialog{
    void D_add(Table& tab);
    void D_find(Table& tab);
    void D_delete(Table& tab);
    void D_output(Table& tab);
    void Choice(const char* msgs[], int n, int& choice);
    void Dialog();
}