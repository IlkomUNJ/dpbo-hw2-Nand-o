#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include "user.h"

// Implementasi method laporan di Seller
void Seller::discoverTopKItems(int k_count, const std::vector<Transaction> &all_transactions) const
{
    std::cout << "\n[LAPORAN SELLER] " << k_count << " Item Terpopuler Milik '" << this->name << "' Sebulan Terakhir:" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    using days = std::chrono::duration<int, std::ratio<86400>>;
    const auto now = std::chrono::system_clock::now();
    const auto one_month_ago = now - days(30);

    std::map<int, std::pair<std::string, int>> itemFrequency;

    for (const auto &tx : all_transactions)
    {
        // Filter transaksi: hanya untuk seller ini DAN dalam sebulan terakhir
        if (tx.sellerAccountId == this->getAccountId() && tx.date >= one_month_ago)
        {
            for (const auto &item : tx.items)
            {
                if (itemFrequency.find(item.id) == itemFrequency.end())
                {
                    itemFrequency[item.id] = {item.name, 0};
                }
                itemFrequency[item.id].second += 1; // Hitung per item dalam transaksi
            }
        }
    }

    if (itemFrequency.empty())
    {
        std::cout << "Tidak ada penjualan dalam sebulan terakhir." << std::endl;
    }
    else
    {
        std::vector<std::pair<int, std::pair<std::string, int>>> sortedItems(itemFrequency.begin(), itemFrequency.end());
        std::sort(sortedItems.begin(), sortedItems.end(), 
                [](const auto &a, const auto &b)
                {
                    return a.second.second > b.second.second;
                });

        int count = 0;
        for (const auto &entry : sortedItems)
        {
            if (count >= k_count)
                break;
            std::cout << "  - Nama: " << entry.second.first << " (ID: " << entry.first << "), Terjual: " << entry.second.second << " unit" << std::endl;
            count++;
        }
    }
    std::cout << "------------------------------------------------------------" << std::endl;
}

void Seller::discoverLoyalCustomers(int k_count, const std::vector<Transaction> &all_transactions) const
{
    std::cout << "\n[LAPORAN SELLER] " << k_count << " Pelanggan Paling Setia untuk '" << this->name << "' Sebulan Terakhir:" << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    using days = std::chrono::duration<int, std::ratio<86400>>;
    const auto now = std::chrono::system_clock::now();
    const auto one_month_ago = now - days(30);

    std::map<int, int> buyerFrequency;

    for (const auto& tx : all_transactions) {
        // Filter transaksi: hanya untuk seller ini DAN dalam sebulan terakhir
        if (tx.sellerAccountId == this->getAccountId() && tx.date >= one_month_ago) {
            buyerFrequency[tx.buyerAccountId]++;
        }
    }

    if (buyerFrequency.empty()) {
        std::cout << "Tidak ada penjualan dalam sebulan terakhir." << std::endl;
    } else {
        std::vector<std::pair<int, int>> sortedBuyers(buyerFrequency.begin(), buyerFrequency.end());
        std::sort(sortedBuyers.begin(), sortedBuyers.end(), 
            [](const auto& a, const auto& b) {
                return a.second > b.second;
            });

        int count = 0;
        for (const auto& entry : sortedBuyers) {
            if (count >= k_count) break;
            std::cout << "  - ID Pelanggan (Akun Bank): " << entry.first << ", Jumlah Transaksi: " << entry.second << std::endl;
            count++;
        }
    }
    std::cout << "------------------------------------------------------------" << std::endl;
}
