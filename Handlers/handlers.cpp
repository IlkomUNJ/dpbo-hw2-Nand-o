#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include <vector>
#include "handlers.h"

using namespace std;

bool checkingValidName(OnlineStore &store, string &name)
{
    for (const auto *user : store.getUsers())
    {
        if (user->getName() == name)
        {
            return false; // Nama sudah ada
        }
    }
    return true; // Nama valid
}

int getNumericInput(const string &prompt)
{
    int choice;
    while (true)
    {
        cout << prompt;
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();                                         // Bersihkan flag error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Buang input yang salah
            cout << "Error: Input tidak valid. Silakan masukkan angka." << endl;
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Buang newline dari input sebelumnya
            return choice;
        }
    }
}

void showBankMenu(User *user, Bank &bank)
{
    bool inBankMenu = true;
    while (inBankMenu)
    {
        cout << "\n===== Menu Perbankan: " << user->getName() << " =====" << endl;
        cout << "1. Setor Tunai (Top Up)" << endl;
        cout << "2. Tarik Tunai (Withdraw)" << endl;
        cout << "3. Cek Saldo" << endl;
        cout << "4. Kembali ke Menu Utama" << endl;

        int choice = getNumericInput("Pilih opsi: ");

        switch (choice)
        {
        case 1:
        {
            double amount;
            cout << "Masukkan jumlah top up: ";
            cin >> amount;
            bank.topUp(user->getAccountId(), amount);
            break;
        }
        case 2:
        {
            double amount;
            cout << "Masukkan jumlah penarikan: ";
            cin >> amount;
            bank.withdraw(user->getAccountId(), amount);
            break;
        }
        case 3:
        {
            bank.checkBalance(user->getAccountId());
            break;
        }
        case 4:
        {
            inBankMenu = false;
            break;
        }
        default:
            cout << "Pilihan tidak valid." << endl;
            break;
        }
    }
}

void handleCashFlowReport(User *user, OnlineStore &store, Bank &bank)
{
    bank.listCashFlow(user->getAccountId());
}

void handleShowAllUsers(OnlineStore& store) {
    std::cout << "\n===== Daftar Semua Pengguna Terdaftar (Urut ID) =====" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    
    // 1. Buat salinan dari daftar pengguna
    std::vector<User*> sortedUsers = store.getUsers();

    if (sortedUsers.empty()) {
        std::cout << "Belum ada pengguna yang terdaftar." << std::endl;
    } else {
        // 2. Urutkan salinan tersebut menggunakan std::sort
        std::sort(sortedUsers.begin(), sortedUsers.end(), 
            [](const User* a, const User* b) {
                // Aturan pengurutan: bandingkan ID dari kedua user
                return a->getId() < b->getId();
            });

        // 3. Loop melalui daftar yang SUDAH TERURUT
        for (const auto* user : sortedUsers) {
            std::string userType = "Tidak Diketahui";
            if (dynamic_cast<const Seller*>(user)) {
                userType = "Penjual";
            } else if (dynamic_cast<const Buyer*>(user)) {
                userType = "Pembeli";
            }
            std::cout << "  - ID: " << user->getId()
                    << ", Nama: " << user->getName()
                    << ", Tipe: " << userType << std::endl;
        }
    }
    std::cout << "-------------------------------------------" << std::endl;
}
