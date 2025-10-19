#include "transaction.h"
#include <numeric>

Transaction::Transaction(int id, int buyerAccountId, int sellerAccountId, const std::vector<Item>& items) {
    this->id = id;
    this->buyerAccountId = buyerAccountId;
    this->sellerAccountId = sellerAccountId;
    this->items = items;

    // Hitung total harga dari semua item dalam transaksi
    // Menggunakan std::accumulate untuk menjumlahkan harga item
    this->totalPrice = std::accumulate(items.begin(), items.end(), 0.0, 
        [](double sum, const Item& currentItem) {
            return sum + (currentItem.price * currentItem.stock);
        });

    this->status = TransactionStatus::PAID; // Default status saat transaksi dibuat

    // Rekam tanggal dan waktu saat transaksi dibuat
    this->date = std::chrono::system_clock::now();
}

// Konstruktor untuk memuat transaksi dari file (termasuk status dan tanggal)
Transaction::Transaction(int id, int buyerAccountId, int sellerAccountId, const std::vector<Item>& items,
                        double totalPrice, TransactionStatus status,
                        const std::chrono::system_clock::time_point& date) {
    this->id = id;
    this->buyerAccountId = buyerAccountId;
    this->sellerAccountId = sellerAccountId;
    this->items = items;
    this->totalPrice = totalPrice; // Total harga sudah diberikan
    this->status = status;
    this->date = date; // Tanggal sudah diberikan
}

void Transaction::setStatus(TransactionStatus newStatus) {
    this->status = newStatus;
}
