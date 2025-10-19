#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
#include <vector>
#include <chrono> // Library for handling date and time
#include "item.h"

// Menggunakan enum class untuk mendefinisikan status transaksi
enum class TransactionStatus {
    PAID,
    COMPLETED,
    CANCELLED
};

struct Transaction {
    int id;
    int buyerAccountId;
    int sellerAccountId;
    std::vector<Item> items; // Menggunakan vector untuk menyimpan daftar item
    double totalPrice;
    TransactionStatus status;
    std::chrono::system_clock::time_point date;

    // Konstruktor untuk membuat transaksi baru
    Transaction(int id, int buyerAccountId, int sellerAccountId, const std::vector<Item>& items);

    // Konstruktor untuk memuat transaksi dari file (termasuk status dan tanggal)
    Transaction(int id, int buyerAccountId, int sellerAccountId, const std::vector<Item>& items,
                double totalPrice, TransactionStatus status,
                const std::chrono::system_clock::time_point& date);

    void setStatus(TransactionStatus newStatus);
};

#endif 
