#include <iostream>
#include <string>
#include <limits>
#include "handlers.h"

using namespace std;

void handleRegister(OnlineStore &store, Bank &bank)
{
    cout << "\n--- Registrasi Pengguna Baru ---" << endl;

    // Menanyakan tipe user yang ingin dibuat
    int userType = getNumericInput("Daftar sebagai (1: Pembeli, 2: Penjual): ");

    // Meminta nama pengguna
    string name;
    while (true)
    {
        cout << "Masukkan nama Anda: ";
        getline(cin, name);

        // Validasi nama apakah sudah ada di sistem
        bool isValidName = checkingValidName(store, name);
        if (!isValidName)
        {
            cout << "\nError: Nama pengguna '" << name << "' sudah digunakan." << endl;
            char tryAgain;
            cout << "Apakah Anda ingin mencoba nama lain? (y/n): ";
            cin >> tryAgain;

            if (tryAgain == 'y' || tryAgain == 'Y')
            {
                continue; // Ulangi loop untuk memasukkan nama baru
            }
            else
            {
                cout << "\nRegistrasi dibatalkan...." << endl;
                return; // Batalkan registrasi
            }
        }
        else
        {
            break; // Nama valid, keluar dari loop
        }
    }

    // Jika valid, buat user baru dan akun bank
    // Buat ID unik untuk user baru
    int newUserId = store.getUsers().size() + 1;
    int newAccountId = bank.getAccounts().size() + 1;

    // Buat akun bank baru untuk user
    bank.addAccount(BankAccount(newAccountId, name, 10000.0)); // Saldo awal 10.000

    // Buat user baru berdasarkan tipe yang dipilih
    if (userType == 1)
    {
        store.registerUser(new Buyer(newUserId, name, newAccountId));
        cout << "Selamat! Akun Pembeli '" << name << "' berhasil dibuat." << endl;
    }
    else if (userType == 2)
    {
        store.registerUser(new Seller(newUserId, name, newAccountId));
        cout << "Selamat! Akun Penjual '" << name << "' berhasil dibuat." << endl;
    }
    else
    {
        cout << "Tipe user tidak valid. Registrasi gagal." << endl;
    }
}
