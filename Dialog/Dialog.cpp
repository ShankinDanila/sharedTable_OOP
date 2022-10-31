//
// Created by danila on 07.10.2021.
//

#include "Dialog.h"
#include "Table.h"
#include <iostream>

using namespace table;
namespace Dialog{
    const char* MSGS[] = { "0. Quit", "1. Add element", "2. Find element with key",
                           "3. Delete element","4. Output"};

    const int MSGS_SIZE = sizeof(MSGS) / sizeof(MSGS[0]);


    void D_add(Table& tab) {
        std::cout << "Enter integer key + char[10] info ----> ";
        try { std::cin >> tab; }
        catch (const std::bad_alloc& ba) {std::cout << "------ Error: " << ba.what() << std::endl; return;}
        if (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(32767, '\n');
            std::cout << "Something went wrong" << std::endl;
        }
    }

    void D_find(Table& tab) {
        std::cout << "Enter integer key, which you want to find ---->  ";
        int key;
        const char* pr = "";
        do{
            std::cout << pr << std::endl;
            std::cout << "Enter key: --> " << std::endl;
            if (Tools::getNum(key) < 0) { pr = "You are wrong; repeat please!"; }
            std::cout << std::endl;
        } while(key < 0);

        int index = tab.find(key);
        if (index == -1) { std::cout << "There is no elements in table with such key" << std::endl; return; }

        Element elem = tab.getArray()[index];

        std::cout << elem;
    }

    void D_delete(Table& tab) {
        std::cout << "Enter integer key, which you want to delete ---->  ";
        int key;
        const char* pr = "";
        do{
            std::cout << pr << std::endl;
            std::cout << "Enter key: --> " << std::endl;
            if (Tools::getNum(key) < 0) { pr = "You are wrong; repeat please!"; }
            std::cout << std::endl;
        } while(key < 0);

        try { tab -= key; }
        catch (const std::logic_error& ba) { std::cout << "------ Error: " << ba.what() << std::endl; }
    }

    void D_output(Table& tab) {
        std::cout << std::endl;
        std::cout << "----------------------------------------------------" << std::endl;
        std::cout << "                    The Table :                     " << std::endl;
        std::cout << "----------------------------------------------------" << std::endl;
        std::cout << "----------------------------------------------------" << std::endl;
        std::cout << "|  #  |    KEY1    |   BUSY   |       STRING       |" << std::endl;
        std::cout << "----------------------------------------------------" << std::endl;
        std::cout << tab << std::endl;
    }


    void Choice(const char* msgs[], int n, int& choice) {
        const char* pr = "";
        do {
            std::cout << "Make your choice: ";
            std::cout << pr << std::endl;
            for (int i = 0; i < n; ++i) { std::cout << msgs[i] << std::endl; }
            if (Tools::getNum(choice) < 0) { std::cout << "You are wrong; repeat please!"; }
            if (std::cin.eof()) { choice = 0; return; }
        } while ((choice < 0) || (choice > n - 1));
    }

    void Dialog() {
        int choice;
        Table tab{};
        do {
            Choice(MSGS, MSGS_SIZE, choice);
            switch (choice) {
                case 0:
                    break;
                case 1:
                    D_add(tab);
                    break;
                case 2:
                    D_find(tab);
                    break;
                case 3:
                    D_delete(tab);
                    break;
                case 4:
                    D_output(tab);
                    break;
            }
        } while (choice != 0);
    }
}
