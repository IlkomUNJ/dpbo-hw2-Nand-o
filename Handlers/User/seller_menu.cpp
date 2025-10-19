#include <iostream>
#include <string>
#include <limits>
#include "handlers.h"

using namespace std;


void showSellerMenu(Seller *seller, OnlineStore &store, Bank &bank)
{
    bool isLoggedIn = true;
    while (isLoggedIn)
    {
        cout << "\n===== Menu Penjual: " << seller->getName() << " =====" << endl;
        cout << "1. Lihat Stok Barang Saya" << endl;
        cout << "2. Tambah Item Baru" << endl;
        cout << "3. Isi Ulang Stok Item" << endl;
        cout << "4. Hapus Item dari Etalase Penjualan" << endl;
        cout << "5. Lihat Arus Kas" << endl;
        cout << "6. Lihat Laporan Penjualan" << endl;
        cout << "7. Menu Perbankan" << endl;
        cout << "8. Logout" << endl;

        int choice = getNumericInput("Pilih opsi: ");

        switch (choice)
        {
        case 1:
        {
            seller->viewStock();
            break;
        }
        case 2:
        {
            // Menambahkan item baru
            cout << "\n--- Tambah Item Baru ---" << endl;
            int stock;
            string name;
            double price;

            cout << "Masukkan Nama Item: ";
            getline(cin, name);
            
            price = getNumericInput("Masukkan Harga Item: ");
            stock = getNumericInput("Masukkan Jumlah Stok Item: ");

            // Tambahkan item ke stok penjual
            seller->addItem(name, price, stock);
            cout << "Item '" << name << "' berhasil ditambahkan ke stok Anda." << endl;
            break;
        }
        case 3:
        {
            // Isi ulang stok item
            cout << "\n--- Isi Ulang Stok Item ---" << endl;

            int itemId = getNumericInput("Masukkan ID Item yang akan diisi ulang: ");
            int quantity = getNumericInput("Masukkan jumlah tambahan: ");

            seller->replenishStock(itemId, quantity);
            break;
        }
        case 4:
        {
            // Hapus item dari etalase penjualan
            cout << "\n--- Hapus Item dari Etalase Penjualan ---" << endl;
            
            int itemId = getNumericInput("Masukkan ID Item yang akan dihapus: ");

            seller->discardItem(itemId);
            break;
        }
        case 5:
        {
            handleCashFlowReport(seller, store, bank);
            break;
        }
        case 6:
        {
            // Tinggal kita panggil method laporan yang sudah ada di class Seller
            cout << "\n--- Laporan Penjualan ---" << endl;
            seller->discoverTopKItems(5, store.getTransactions());      // Contoh: Top 5 item
            seller->discoverLoyalCustomers(5, store.getTransactions()); // Contoh: Top 5 pelanggan
            break;
        }
        case 7:
        {
            showBankMenu(seller, bank);
            break;
        }
        case 8:
            isLoggedIn = false;
            cout << "Anda telah logout..." << endl;
            break;
        default:
            cout << "Pilihan tidak valid." << endl;
            break;
        }
    }
}
