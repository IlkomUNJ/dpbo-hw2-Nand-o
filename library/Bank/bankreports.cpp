#include <iostream>
#include <chrono>
#include <map>
#include <algorithm>
#include <vector>
#include "bank.h"

// Implementasi fitur laporan
void Bank::listDormantAccounts(const std::vector<Transaction>& transactions) const {
    std::cout << "\n[LAPORAN BANK] Daftar Akun Dormant (Tidak Aktif) dalam 30 Hari Terakhir:" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;

    // Dapatkan waktu saat ini dan hitung batas waktu 30 hari yang lalu
    auto now = std::chrono::system_clock::now();
    auto one_month_ago = now - std::chrono::hours(24 * 30);

    // Loop melalui setiap akun di bank
    for (const auto& account : this->accounts) {
        bool isActive = false;

        for (const auto& tx : transactions) {
            // Cek apakah akun ini terlibat dalam transaksi sebulan terakhir
            if (tx.date >= one_month_ago) {
                if (tx.buyerAccountId == account.accountId || tx.sellerAccountId == account.accountId) {
                    isActive = true;
                    break; // Tidak perlu cek transaksi lainnya
                }
            }
        }

        // Jika akun tidak aktif, tampilkan informasinya
        if (!isActive) {
            std::cout << " - Akun Dormant: ID " << account.accountId
                    << ", Nama Pemilik: " << account.ownerName
                    << ", Saldo: " << account.balance << std::endl;
        }
    }
    std::cout << "--------------------------------------------------------" << std::endl;
}

void Bank::listTopUsersToday(int n_count, const std::vector<Transaction>& transactions) const {
    std::cout << "\n[LAPORAN BANK] " << n_count << " Pengguna Teratas Hari Ini berdasarkan Total Transaksi:" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;

    // Dapatkan tanggal hari ini
    using days = std::chrono::duration<int, std::ratio<86400>>;
    const auto now = std::chrono::system_clock::now();
    const auto today = std::chrono::time_point_cast<days>(now);

    // Hitung frekuensi transaksi per akun hari ini
    std::map<int, int> userFrequency;
    for (const auto& tx : transactions) {
        // Konversi tanggal transaksi ke format hari
        auto tx_day = std::chrono::time_point_cast<days>(tx.date);

        // Cek apakah transaksi terjadi hari ini
        if (tx_day == today) {
            userFrequency[tx.buyerAccountId]++;
            userFrequency[tx.sellerAccountId]++;
        }
    }

    if (userFrequency.empty()) {
        std::cout << "Tidak ada transaksi hari ini." << std::endl;
        std::cout << "--------------------------------------------------------" << std::endl;
        return;
    }

    // Salin data ke vector untuk diurutkan
    std::vector<std::pair<int, int>> sortedUsers(userFrequency.begin(), userFrequency.end());

    // Urutkan berdasarkan frekuensi transaksi (nilai kedua dari pair)
    std::sort(sortedUsers.begin(), sortedUsers.end(),
            [](const auto& a, const auto& b) {
                return a.second > b.second; // Urutkan menurun
            });

    // Tampilkan n_count pengguna teratas
    int count = 0;
    for (const auto& user : sortedUsers) {
        if (count >= n_count) break;

        // Cari detail akun berdasarkan accountId
        const BankAccount* account = findAccount(user.first);
        std::string ownerName = account ? account->ownerName : "Tidak Diketahui";

        std::cout << " - Nama: " << ownerName
                << ", (ID Akun: " << user.first << ")"
                << ", Jumlah Transaksi Hari Ini: " << user.second << std::endl;
    }
    std::cout << "--------------------------------------------------------" << std::endl;
}
