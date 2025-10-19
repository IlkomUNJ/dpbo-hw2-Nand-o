#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include "handlers.h"

using namespace std;

void handleLogin(OnlineStore &store, Bank &bank)
{
    cout << "\n--- Login Pengguna ---" << endl;

    int userId = getNumericInput("Masukkan ID Pengguna Anda: ");

    // Cari user berdasarkan ID
    User *user = store.findUserById(userId);

    // Validasi apakah user ditemukan
    if (user == nullptr)
    {
        cout << "Error: Pengguna dengan ID " << userId << " tidak ditemukan." << endl;
        return;
    }

    // Tampilkan menu sesuai tipe user
    if (Seller *seller = dynamic_cast<Seller *>(user))
    {
        showSellerMenu(seller, store, bank);
    }
    else if (Buyer *buyer = dynamic_cast<Buyer *>(user))
    {
        showBuyerMenu(buyer, store, bank);
    }
    else
    {
        cout << "Error: Tipe pengguna tidak dikenali." << endl;
    }
}

void handleAdminLogin(OnlineStore &store, Bank &bank) {
    string stored_user, stored_pass;
    ifstream adminFile("data/conf/admin.conf");
    if (adminFile.is_open()) {
        string line;
        // Baca baris username
        if (getline(adminFile, line) && line.find("username=") == 0) {
            stored_user = line.substr(9); // Panjang "username=" adalah 9
        }
        if (getline(adminFile, line) && line.find("password=") == 0) {
            stored_pass = line.substr(9); // Panjang "password=" adalah 9
        }
    } else {
        cout << "Error: Tidak dapat membuka file konfigurasi admin." << endl;
        return;
    }

    if (stored_user.empty() || stored_pass.empty()) {
        cout << "Error: Format file konfigurasi admin tidak valid." << endl;
        return;
    }

    // Minta input username dan password
    cout << "\n--- Login Admin ---" << endl;
    string input_user, input_pass;
    cout << "Masukkan username admin: ";
    cin >> input_user;
    cout << "Masukkan password admin: ";
    cin >> input_pass;

    // Validasi
    if (input_user == stored_user && input_pass == stored_pass) {
        cout << "\nLogin admin berhasil. Selamat datang!" << endl;
        showAdminMenu(store, bank);
    } else {
        cout << "Error: Username atau password admin salah." << endl;
    }
}
