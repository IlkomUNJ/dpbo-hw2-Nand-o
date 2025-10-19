#include <iostream>
#include <string>
#include <limits>
#include <chrono>
#include "handlers.h"

using namespace std;

string statusToString(TransactionStatus status) {
    switch (status) {
        case TransactionStatus::PAID: return "DIBAYAR";
        case TransactionStatus::COMPLETED: return "SELESAI";
        case TransactionStatus::CANCELLED: return "DIBATALKAN";
        default: return "TIDAK DITEMUKAN";
    }
}

void showBuyerMenu(Buyer *buyer, OnlineStore &store, Bank &bank)
{
    bool isLoggedIn = true;
    while (isLoggedIn)
    {
        cout << "\n===== Menu Pembeli: " << buyer->getName() << " =====" << endl;
        cout << "1. Lihat Semua Barang" << endl;
        cout << "2. Beli Barang" << endl;
        cout << "3. Lihat Riwayat Order Saya" << endl;
        cout << "4. Konfirmasi Penerimaan Barang/Pesanan" << endl;
        cout << "5. Batalkan Pesanan" << endl;
        cout << "6. Lihat Arus Kas" << endl;
        cout << "7. Lihat Laporan Pengeluaran" << endl;
        cout << "8. Menu Perbankan" << endl;
        cout << "9. Logout" << endl;

        int choice = getNumericInput("Pilih opsi: ");

        switch (choice)
        {
        case 1:
        {
            cout << "\n--- Daftar Semua Barang Dijual ---" << endl;
            // Tampilkan semua barang dari semua penjual
            for (const auto *user : store.getUsers())
            {
                if (const Seller *seller = dynamic_cast<const Seller *>(user))
                {
                    cout << "\nPenjual: " << seller->getName() << " ( ID: " << seller->getAccountId() << " )" << endl;
                    seller->viewStock();
                }
            }
            break;
        }
        case 2:
        {
            cout << "\n--- Beli Barang ---" << endl;
            int sellerId = getNumericInput("Masukkan ID Penjual: ");
            int itemId = getNumericInput("Masukkan ID Item: ");
            int quantity = getNumericInput("Masukkan Jumlah: ");

            // Cari penjual berdasarkan ID
            Seller *seller = dynamic_cast<Seller *>(store.findUserById(sellerId));
            if (seller)
            {
                bool success = buyer->purchase(*seller, bank, itemId, quantity);
                if (success)
                {
                    // Cari detail item untuk catatan transaksi
                    const Item *purchasedItem = nullptr;
                    for (const auto &item : seller->getStock())
                    {
                        if (item.id == itemId)
                        {
                            purchasedItem = &item;
                            break;
                        }
                    }

                    // Catat transaksi jika pembelian berhasil
                    if (purchasedItem)
                    {
                        int newTxId = store.getTransactions().size() + 1001;
                        vector<Item> itemsInTx = {Item(purchasedItem->id, purchasedItem->name, purchasedItem->price, quantity)};
                        store.recordTransaction(
                            Transaction(newTxId, buyer->getAccountId(), seller->getAccountId(), itemsInTx));
                        cout << "Pembelian berhasil! Transaksi telah dicatat." << endl;
                    }
                }
                else
                {
                    cout << "Pembelian gagal. Silakan cek kembali detail pembelian Anda." << endl;
                }
            }
            else
            {
                cout << "Error: Penjual dengan ID " << sellerId << " tidak ditemukan." << endl;
            }
            break;
        }
        case 3:
        {
            handleViewOrderHistory(buyer, store);
            break;
        }
        case 4:
        {
            cout << "\n-- Konfirmasi Penerimaan Pesanan --" << endl;

            int txId = getNumericInput("Masukkan ID Transaksi yang ingin diselesaikan: ");

            Transaction *tx = store.findTransactionById(txId);
            // Validasi: pastikan transaksi ada & milik buyer ini
            if (tx && tx->buyerAccountId == buyer->getAccountId())
            {
                tx->setStatus(TransactionStatus::COMPLETED);
                cout << "Transaksi " << txId << " berhasil diselesaikan." << endl;
            }
            else
            {
                cout << "Error: Transaksi tidak ditemukan atau bukan milik Anda." << endl;
            }
            break;
        }
        case 5:
        {
            handleCancelOrder(buyer, store, bank);
            break;
        }
        case 6:
        {
            handleCashFlowReport(buyer, store, bank);
            break;
        }
        case 7:
        {
            handleSpendingReport(buyer, store);
            break;
        }
        case 8:
        {
            showBankMenu(buyer, bank);
            break;
        }
        // TODO: Isi logika untuk setiap pilihan
        case 9:
            isLoggedIn = false;
            cout << "Anda telah logout." << endl;
            break;
        default:
            cout << "Pilihan tidak valid." << endl;
            break;
        }
    }
}

void handleViewOrderHistory(Buyer *buyer, OnlineStore &store)
{
    cout << "\n-- Riwayat Order Saya --" << endl;

    int statusChoice = getNumericInput("Filter berdasarkan status (1: PAID, 2: COMPLETED, 3: CANCELLED, 4: Tampilkan Semua): ");

    bool found = false;
    for (const auto &tx : store.getTransactions())
    {
        if (tx.buyerAccountId == buyer->getAccountId())
        {
            bool shouldPrint = false;
            switch (statusChoice)
            {
            case 1:
                if (tx.status == TransactionStatus::PAID)
                    shouldPrint = true;
                break;
            case 2:
                if (tx.status == TransactionStatus::COMPLETED)
                    shouldPrint = true;
                break;
            case 3:
                if (tx.status == TransactionStatus::CANCELLED)
                    shouldPrint = true;
                break;
            case 4:
                shouldPrint = true;
                break;
            default:
                break;
            }

            if (shouldPrint)
            {
                cout << "------------------------------------------" << endl;
                cout << "  ID Transaksi: " << tx.id << " | Total: " << tx.totalPrice
                        << " | Status: " << statusToString(tx.status) << endl;
                
                // 2. TAMBAHAN: Loop melalui item di dalam transaksi dan tampilkan detailnya
                cout << "    Detail Barang:" << endl;
                for (const auto& item : tx.items) {
                    // Di dalam objek Transaction, 'item.stock' kita gunakan untuk menyimpan kuantitas yang dibeli
                    cout << "    - " << item.name << " (Jumlah: " << item.stock << ")" << endl;
                }
                found = true;
            }
        }
    }

    if (!found)
    {
        cout << "Tidak ada riwayat transaksi yang sesuai dengan filter." << endl;
    }
}

void handleSpendingReport(Buyer *buyer, OnlineStore &store)
{
    cout << "\n-- Laporan Pengeluaran --" << endl;

    int k_days = getNumericInput("Masukkan periode hari (misal: 30 untuk 30 hari terakhir): ");

    using days = chrono::duration<int, ratio<86400>>;
    const auto now = chrono::system_clock::now();
    const auto cutoff_date = now - days(k_days);

    double totalSpending = 0.0;
    for (const auto &tx : store.getTransactions())
    {
        // Filter transaksi: hanya untuk buyer ini DAN dalam periode waktu yang ditentukan
        if (tx.buyerAccountId == buyer->getAccountId() && tx.date >= cutoff_date)
        {
            totalSpending += tx.totalPrice;
        }
    }

    cout << "Total pengeluaran Anda dalam " << k_days << " hari terakhir adalah: " << totalSpending << endl;
}

void handleCancelOrder(Buyer *buyer, OnlineStore &store, Bank &bank)
{
    cout << "\n-- Batalkan Pesanan --" << endl;

    int txId = getNumericInput("Masukkan ID Transaksi yang ingin dibatalkan: ");

    Transaction* tx = store.findTransactionById(txId);

    // 1. Validasi
    if (!tx || tx->buyerAccountId != buyer->getAccountId()) {
        cout << "Error: Transaksi tidak ditemukan atau bukan milik Anda." << endl;
        return;
    }
    if (tx->status != TransactionStatus::PAID) {
        cout << "Error: Hanya transaksi dengan status 'DIBAYAR' yang bisa dibatalkan." << endl;
        return;
    }

    // 2. Proses Refund
    bool refundSuccess = bank.transfer(tx->sellerAccountId, tx->buyerAccountId, tx->totalPrice);

    if (refundSuccess) {
        // 3. Proses Restock
        Seller* seller = nullptr;
        for (auto* user : store.getUsers()) {
            if (Seller* s = dynamic_cast<Seller*>(user)) {
                if (s->getAccountId() == tx->sellerAccountId) {
                    seller = s;
                    break;
                }
            }
        }

        if (seller) {
            for (const auto& item : tx->items) {
                // Kita gunakan -quantity untuk menambah stok (stock -= (-qty) -> stock += qty)
                seller->updateStock(item.id, item.stock);
            }
        }

        // 4. Ubah Status
        tx->setStatus(TransactionStatus::CANCELLED);
        cout << "Transaksi " << txId << " berhasil dibatalkan dan dana telah dikembalikan." << endl;
    } else {
        cout << "Pembatalan gagal karena proses refund tidak berhasil." << endl;
    }
}
