#include <iostream>
#include <chrono> // Untuk menangani tanggal
#include <ctime>  // Untuk konversi waktu
#include <map>   // Untuk menghitung frekuensi item
#include <algorithm> // Untuk std::sort
#include <vector> // Untuk mengurutkan hasil map
#include "onlinestore.h"


// Implementasi fitur pelaporan

// Fungsi pembantu untuk mencetak tanggal dengan format yang mudah dibaca
void printFormattedDate(const std::chrono::system_clock::time_point& time_point)
{
    std::time_t time = std::chrono::system_clock::to_time_t(time_point);
    
    std::string time_str = std::ctime(&time);
    time_str.pop_back(); // Hapus newline di akhir
    std::cout << time_str;
}

void OnlineStore::listLatestTransactions(int k_days) const
{
    std::cout << "\n[LAPORAN] Daftar Transaksi dalam " << k_days << " hari terakhir:\n";
    std::cout << "----------------------------------------\n";
    
    // Dapatkan waktu sekarang
    auto now = std::chrono::system_clock::now();
    auto cutoff_date = now - std::chrono::hours(24 * k_days); // Hitung batas waktu

    bool found = false;
    // Loop melalui semua transaksi dan tampilkan yang dalam k hari terakhir
    for (const auto& tx : this->transactions) {
        if (tx.date >= cutoff_date) {
            std::cout << " - ID: " << tx.id << ", Total: " << tx.totalPrice << ", Tanggal: ";
            printFormattedDate(tx.date);
            std::cout << "\n";
            found = true;
        }
    }

    if (!found) {
        std::cout << "Tidak ada transaksi dalam " << k_days << " hari terakhir.\n";
    }
    std::cout << "----------------------------------------\n";
}

void OnlineStore::listPaidButNotCompleted() const
{
    std::cout << "\n[LAPORAN] Daftar Transaksi yang Sudah Dibayar tapi Belum Selesai:\n";
    std::cout << "----------------------------------------\n";
    
    bool found = false;
    for (const auto& tx : this->transactions) {
        // Cek status transaksi
        if (tx.status == TransactionStatus::PAID) {
            std::cout << " - ID: " << tx.id << ", BuyerID: " << tx.buyerAccountId
                    << ", SellerID: " << tx.sellerAccountId << ", Total: " << tx.totalPrice << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "Tidak ada transaksi yang sudah dibayar tapi belum selesai.\n";
    }
    std::cout << "----------------------------------------\n";
}

void OnlineStore::listMostFrequentItems(int m_count) const
{
    std::cout << "\n[LAPORAN] " << m_count << " Item Paling Sering Dibeli:" << std::endl;
    std::cout << "----------------------------------------\n";

    if (this->transactions.empty()) {
        std::cout << "Belum ada transaksi untuk dianalisis." << std::endl;
        std::cout << "----------------------------------------\n";
        return;
    }

    // Hitung frekuensi setiap item
    std::map<int, std::pair<std::string, int>> itemFrequency;
    for (const auto& tx : this->transactions) {
        for (const auto& item : tx.items) {
            // Jika item belum ada di map, inisialisasi dengan nama dan hitung 0
        if (itemFrequency.find(item.id) == itemFrequency.end()) {
            itemFrequency[item.id] = {item.name, 0};
        }

        // Tambahkan hitungannya
        itemFrequency[item.id].second++;
        }
    }

    // Pindahkan data ke vector untuk diurutkan
    std::vector<std::pair<int, std::pair<std::string, int>>> sortedItems(itemFrequency.begin(), itemFrequency.end());

    // Urutkan berdasarkan frekuensi paling banyak dibeli ke paling sedikit
    std::sort(sortedItems.begin(), sortedItems.end(),
            [](const auto& a, const auto& b) {
                  return a.second.second > b.second.second; // Urutkan menurun berdasarkan frekuensi
            });
    
    // Tampilkan m_count item teratas
    int count = 0;
    for (const auto& entry : sortedItems) {
        if (count >= m_count) break;
        std::cout << " - Nama: " << entry.second.first 
                << ", (ID: " << entry.first << ")"
                << ", Dibeli: " << entry.second.second << " kali\n";
        count++;
    }
    std::cout << "----------------------------------------\n";
}

void OnlineStore::listMostActiveBuyers(int m_count) const
{
    std::cout << "\n[LAPORAN] " << m_count << " Buyer Paling Aktif:" << std::endl;
    std::cout << "----------------------------------------\n";

    if (this->transactions.empty()) {
        std::cout << "Belum ada transaksi untuk dianalisis." << std::endl;
        std::cout << "----------------------------------------\n";
        return;
    }

    // Hitung frekuensi setiap buyer
    std::map<int, int> buyerFrequency;
    for (const auto& tx : this->transactions) {
        buyerFrequency[tx.buyerAccountId]++;
    }

    // Pindahkan data ke vector untuk diurutkan
    std::vector<std::pair<int, int>> sortedBuyers(buyerFrequency.begin(), buyerFrequency.end());

    // Urutkan berdasarkan frekuensi paling banyak transaksi ke paling sedikit
    std::sort(sortedBuyers.begin(), sortedBuyers.end(),
            [](const auto& a, const auto& b) {
                return a.second > b.second; // Urutkan menurun berdasarkan frekuensi
            });
    
    // Tampilkan m_count buyer teratas
    int count = 0;
    for (const auto& entry : sortedBuyers) {
        if (count >= m_count) break;

        // Cari nama pembeli dari daftar users berdasarkan buyerId
        std::string buyerName = "Unknown";
        for (const auto& user : this->users) {
            if (user->getAccountId() == entry.first) {
                buyerName = user->getName();
                break;
            }
        }
        std::cout << " - Nama: " << buyerName 
                << ", (ID: " << entry.first << ")"
                << ", Jumlah Transaksi: " << entry.second << std::endl;
        count++;
    }
    std::cout << "----------------------------------------\n";
}

void OnlineStore::listMostActiveSellers(int m_count) const
{
    std::cout << "\n[LAPORAN] " << m_count << " Seller Paling Aktif:" << std::endl;
    std::cout << "----------------------------------------\n";

    if (this->transactions.empty()) {
        std::cout << "Belum ada transaksi untuk dianalisis." << std::endl;
        std::cout << "----------------------------------------\n";
        return;
    }

    // Hitung frekuensi setiap seller
    std::map<int, int> sellerFrequency;
    for (const auto& tx : this->transactions) {
        sellerFrequency[tx.sellerAccountId]++;
    }

    // Pindahkan data ke vector untuk diurutkan
    std::vector<std::pair<int, int>> sortedSellers(sellerFrequency.begin(), sellerFrequency.end());

    // Urutkan berdasarkan frekuensi paling banyak transaksi ke paling sedikit
    std::sort(sortedSellers.begin(), sortedSellers.end(),
            [](const auto& a, const auto& b) {
                return a.second > b.second; // Urutkan menurun berdasarkan frekuensi
            });

    // Tampilkan m_count seller teratas
    int count = 0;
    for (const auto& entry : sortedSellers) {
        if (count >= m_count) break;

        // Cari nama seller dari daftar users berdasarkan sellerId
        std::string sellerName = "Unknown";
        for (const auto& user : this->users) {
            if (user->getAccountId() == entry.first) {
                sellerName = user->getName();
                break;
            }
        }
        std::cout << " - Nama: " << sellerName 
                << ", (ID: " << entry.first << ")"
                << ", Jumlah Transaksi: " << entry.second << std::endl;
        count++;
    }
    std::cout << "----------------------------------------\n";
}
