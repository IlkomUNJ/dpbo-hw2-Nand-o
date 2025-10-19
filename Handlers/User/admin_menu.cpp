#include <iostream>
#include <string>
#include <limits>
#include "handlers.h"

using namespace std;

void showAdminMenu(OnlineStore &store, Bank &bank)
{
    bool isAdminLoggedIn = true;
    while (isAdminLoggedIn)
    {
        cout << "\n===== MENU ADMIN =====" << endl;
        cout << "1. Laporan Item Terlaris (Global)" << endl;
        cout << "2. Laporan Pembeli Teraktif (Global)" << endl;
        cout << "3. Laporan Penjual Teraktif (Global)" << endl;
        cout << "4. Laporan Akun Tidak Aktif (Dormant)" << endl;
        cout << "5. Laporan Top User Hari Ini (Bank)" << endl;
        cout << "6. Kembali ke Menu Utama" << endl;

        int choice = getNumericInput("Pilih opsi: ");

        switch (choice) {
            case 1: store.listMostFrequentItems(5); break;
            case 2: store.listMostActiveBuyers(5); break;
            case 3: store.listMostActiveSellers(5); break;
            case 4: bank.listDormantAccounts(store.getTransactions()); break;
            case 5: bank.listTopUsersToday(5, store.getTransactions()); break;
            case 6: isAdminLoggedIn = false; break;
            default: cout << "Pilihan tidak valid." << endl; break;
        }
    }
}
