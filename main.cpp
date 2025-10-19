#include <iostream>
#include <string>
#include <limits>
#include <filesystem> // Untuk mengecek keberadaan file (direktori)
#include "onlinestore.h"
#include "handlers.h"

using namespace std;

// Enum untuk pilihan menu
enum MainMenuOption {
    REGISTER = 1,
    SHOW_ALL_USERS = 2,
    LOGIN = 3,
    ADMIN = 4,
    EXIT = 5
};

int main() {
    // Setup Awal Program
    const string dataDir = "data";
    filesystem::create_directory(dataDir); // Buat direktori data jika belum ada
    const string bankFile = dataDir + "/accounts.csv";
    const string bankHistoryFile = dataDir + "/bank_history.csv";
    const string userFile = dataDir + "/users";
    const string transactionFile = dataDir + "/transactions.csv";

    Bank bankUtama;
    OnlineStore nandoStore(bankUtama);

    // Load data yang sudah ada saat program dijalankan
    cout << "Memuat data dari sesi sebelumnya..." << endl;
    bankUtama.loadFromFile(bankFile);
    bankUtama.loadHistoryFromFile(bankHistoryFile);
    nandoStore.loadUsersFromFile(userFile);
    nandoStore.loadTransactionsFromFile(transactionFile);
    cout << "\nData berhasil dimuat. Selamat datang di Simulasi E-Commerce!" << endl;

    bool running = true;
    while (running) {
        cout << "\n ===== MENU UTAMA =====" << endl;
        cout << "1. Daftar (Register)" << endl;
        cout << "2. Tampilkan Semua Pengguna (Show All Users)" << endl;
        cout << "3. Masuk (Login)" << endl;
        cout << "4. Masuk Sebagai Admin (Admin Login)" << endl;
        cout << "5. Keluar (Exit)" << endl;
        cout << "Pilih opsi: ";

        int choice = getNumericInput("Pilih opsi: ");

        MainMenuOption option = static_cast<MainMenuOption>(choice);

        switch (option) {
            case REGISTER: {
                handleRegister(nandoStore, bankUtama);
                break;
            }
            case SHOW_ALL_USERS: {
                handleShowAllUsers(nandoStore);
                break;
            }
            case LOGIN: {
                handleLogin(nandoStore, bankUtama);
                break;
            }
            case ADMIN: {
                handleAdminLogin(nandoStore, bankUtama);
                break;
            }
            case EXIT: {
                cout << "\nMenyimpan data sebelum keluar..." << endl;
                // Panggil semua fungsi simpan data
                bankUtama.saveToFile(bankFile);
                bankUtama.saveHistoryToFile(bankHistoryFile);
                nandoStore.saveUsersToFile(userFile);
                nandoStore.saveTransactionsToFile(transactionFile);
                cout << "Data berhasil disimpan. Terima kasih telah menggunakan aplikasi ini!" << endl;
                running = false;
                break;
            }
            default:
                cout << "Opsi tidak valid. Silakan coba lagi!" << endl;
                break;
        }
    }
    
    return 0;
}
