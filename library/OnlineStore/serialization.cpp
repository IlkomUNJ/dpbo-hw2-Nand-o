#include <iostream>
#include <fstream> // Untuk file I/O
#include <sstream> // Untuk memproses string baris demi baris
#include <chrono> // Untuk menangani tanggal
#include <ctime>  // Untuk konversi waktu
#include "onlinestore.h"


// Implementasi fitur serialisasi
void OnlineStore::saveUsersToFile(const std::string& base_filename) const
{
    // Membuat dua file terpisah untuk Seller dan Buyer
    std::ofstream sellerFile(base_filename + "_sellers.csv");
    std::ofstream buyerFile(base_filename + "_buyers.csv");

    // Validasi pembukaan file
    if (!sellerFile.is_open() || !buyerFile.is_open())
    {
        std::cerr << "Error: Tidak dapat membuka file untuk menyimpan data pengguna." << std::endl;
        return;
    }

    // Tulis header CSV
    sellerFile << "ID,Name,AccountID\n";
    buyerFile << "ID,Name,AccountID\n";

    // Tulis data pengguna ke file yang sesuai
    for (User *user : this->users)
    {
        // Coba cast pointer User ke pointer Seller
        Seller *seller = dynamic_cast<Seller *>(user);
        if (seller != nullptr)
        {
            // Jika berhasil, berarti user adalah Seller
            sellerFile << seller->getId() << "," << seller->getName() << "," << seller->getAccountId() << "\n";

            // Simpan stock item Seller
            std::string stockFilename = "data/stock_" + std::to_string(seller->getId()) + ".csv";
            std::ofstream stockFile(stockFilename);
            if (stockFile.is_open())
            {
                stockFile << "id,name,price,stock\n"; // Header
                for (const Item &item : seller->getStock())
                {
                    stockFile << item.id << "," << item.name << "," << item.price << "," << item.stock << "\n";
                }
            }

            continue; // Lanjut ke user berikutnya
        }

        Buyer *buyer = dynamic_cast<Buyer *>(user);
        if (buyer != nullptr)
        {
            // Jika berhasil, berarti user adalah Buyer
            buyerFile << buyer->getId() << "," << buyer->getName() << "," << buyer->getAccountId() << "\n";
        }
    }
}

void OnlineStore::loadUsersFromFile(const std::string& base_filename)
{
    // Muat data Seller dari file sellers.csv
    std::ifstream sellerFile(base_filename + "_sellers.csv");
    if (sellerFile.is_open())
    {
        std::string line;
        std::getline(sellerFile, line); // Lewati header

        while (std::getline(sellerFile, line))
        {
            std::stringstream ss(line);
            std::string segment;
            int id, accountId;
            std::string name;

            std::getline(ss, segment, ','); id = std::stoi(segment);
            std::getline(ss, segment, ','); name = segment;
            std::getline(ss, segment, ','); accountId = std::stoi(segment);

            Seller *newSeller = new Seller(id, name, accountId);

            // Muat stock item Seller dari file terpisah
            std::string stockFilename = "data/stock_" + std::to_string(id) + ".csv";
            std::ifstream stockFile(stockFilename);
            if (stockFile.is_open())
            {
                std::string itemLine;
                std::getline(stockFile, itemLine); // Lewati header
                while (std::getline(stockFile, itemLine))
                {
                    std::stringstream item_ss(itemLine);
                    std::string item_segment;
                    int itemId, itemStock;
                    std::string itemName;
                    double itemPrice;

                    std::getline(item_ss, item_segment, ',');
                    itemId = std::stoi(item_segment);
                    std::getline(item_ss, item_segment, ',');
                    itemName = item_segment;
                    std::getline(item_ss, item_segment, ',');
                    itemPrice = std::stod(item_segment);
                    std::getline(item_ss, item_segment, ',');
                    itemStock = std::stoi(item_segment);

                    newSeller->_reAddItem(Item(itemId, itemName, itemPrice, itemStock));
                }
            }
            this->registerUser(newSeller);
        }
        std::cout << "Data Seller berhasil dimuat dari sellers.csv\n";
    }

    // Muat data Buyer dari file buyers.csv
    std::ifstream buyerFile(base_filename + "_buyers.csv");
    if (buyerFile.is_open())
    {
        std::string line;
        std::getline(buyerFile, line); // Lewati header

        while (std::getline(buyerFile, line))
        {
            std::stringstream ss(line);
            std::string segment;
            int id, accountId;
            std::string name;

            std::getline(ss, segment, ',');
            id = std::stoi(segment);
            std::getline(ss, segment, ',');
            name = segment;
            std::getline(ss, segment, ',');
            accountId = std::stoi(segment);

            // Buat objek Buyer dan daftarkan
            this->registerUser(new Buyer(id, name, accountId));
        }
        std::cout << "Data Buyer berhasil dimuat dari buyers.csv\n";
    }
}

void OnlineStore::saveTransactionsToFile(const std::string& filename) const {
    std::ofstream transFile(filename);

    if (!transFile.is_open()) {
        std::cerr << "Error: Tidak dapat membuka " << filename << " untuk menyimpan data transaksi." << std::endl;
        return;
    }

    // Tulis header CSV
    transFile << "ID,BuyerID,SellerID,TotalPrice,Status,Date\n";

    // Tulis data transaksi
    for (const auto& tx : this->transactions) {
        transFile << tx.id << ","
                    << tx.buyerAccountId << ","
                    << tx.sellerAccountId << ","
                    << tx.totalPrice << ","
                    << static_cast<int>(tx.status) << "," // Simpan status sebagai integer
                    << tx.date.time_since_epoch().count() << "\n"; // Simpan tanggal sebagai epoch time

        // Buat file terpisah untuk item dalam transaksi
        std::string itemsFilename = "data/transaction_items_" + std::to_string(tx.id) + ".csv";
        std::ofstream itemsFile(itemsFilename);
        if (itemsFile.is_open()) {
            itemsFile << "id,name,price,stock\n"; // Header
            for (const auto& item : tx.items) {
                itemsFile << item.id << "," << item.name << "," << item.price << "," << item.stock << "\n";
            }
        }
    }
    std::cout << "Data transaksi berhasil disimpan ke " << filename << std::endl;
}

void OnlineStore::loadTransactionsFromFile(const std::string& filename) {
    std::ifstream transFile(filename);

    if (!transFile.is_open()) {
        return; // File tidak ada, anggap tidak ada transaksi
    }

    this->transactions.clear(); // Bersihkan transaksi yang ada

    std::string line;
    std::getline(transFile, line); // Lewati header

    while (std::getline(transFile, line)) {
        std::stringstream ss(line);
        std::string segment;

        int id, buyerAccountId, sellerAccountId, statusInt;
        double totalPrice;
        long long dateCount;

        // Baca data utama dari transactions.csv
        std::getline(ss, segment, ','); id = std::stoi(segment);
        std::getline(ss, segment, ','); buyerAccountId = std::stoi(segment);
        std::getline(ss, segment, ','); sellerAccountId = std::stoi(segment);
        std::getline(ss, segment, ','); totalPrice = std::stod(segment);
        std::getline(ss, segment, ','); statusInt = std::stoi(segment);
        std::getline(ss, segment, ','); dateCount = std::stoll(segment);

        // Baca item-item dari file terpisahnya
        std::vector<Item> loadedItems;
        std::string itemsFilename = "data/transaction_items_" + std::to_string(id) + ".csv";
        std::ifstream itemsFile(itemsFilename);
        if (itemsFile.is_open()) {
            std::string itemLine;
            std::getline(itemsFile, itemLine); // Lewati header
            while (std::getline(itemsFile, itemLine)) {
                std::stringstream item_ss(itemLine);
                std::string item_segment;
                int itemId, itemStock;
                std::string itemName;
                double itemPrice;

                std::getline(item_ss, item_segment, ','); itemId = std::stoi(item_segment);
                std::getline(item_ss, item_segment, ','); itemName = item_segment;
                std::getline(item_ss, item_segment, ','); itemPrice = std::stod(item_segment);
                std::getline(item_ss, item_segment, ','); itemStock = std::stoi(item_segment);

                loadedItems.push_back(Item(itemId, itemName, itemPrice, itemStock));
            }
        }

        // Buat ulang objek status dan tanggal
        TransactionStatus status = static_cast<TransactionStatus>(statusInt);
        auto date = std::chrono::system_clock::time_point(std::chrono::nanoseconds(dateCount));

        // Gunakan konstruktor lengkap untuk memuat transaksi
        this->transactions.push_back(
            Transaction(id, buyerAccountId, sellerAccountId, loadedItems, totalPrice, status, date)
        );
    }
    std::cout << "Data transaksi berhasil dimuat dari " << filename << std::endl;
}
