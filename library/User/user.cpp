#include <iostream>
#include <algorithm>
#include "user.h"

// Implementasi untuk kelas User
User::User(int id, const std::string &name, int accountId)
{
    this->id = id;
    this->name = name;
    this->accountId = accountId;
}

// Implementasi getter
int User::getId() const
{
    return id;
}

const std::string &User::getName() const
{
    return name;
}

int User::getAccountId() const
{
    return accountId;
}

// Implementasi untuk kelas Seller
Seller::Seller(int id, const std::string &name, int accountId) : User(id, name, accountId) {}

// Implementasi getter
const std::vector<Item> &Seller::getStock() const
{
    return this->stock;
}

// Method Seller
void Seller::addItem(const std::string& name, double price, int stock)
{
    this->stock.push_back(Item(this->nextItemId, name, price, stock));
    this->nextItemId++;
}

void Seller::_reAddItem(const Item& item)
{
    this->stock.push_back(item);

    this->nextItemId = std::max(this->nextItemId, item.id + 1);
    
}

void Seller::viewStock() const
{
    std::cout << "\n--- Stock Milik Penjual: " << this->name << " ---" << std::endl;
    for (const auto& item : this->stock) {
        std::cout << "ID: " << item.id << ", Nama: " << item.name 
        << ", Harga: " << item.price << ", Stok: " << item.stock << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;
}

void Seller::updateStock(int itemId, int newStock)
{
    for (auto& item : this->stock) {
        if (item.id == itemId) {
            item.stock += newStock;
            break;
        }
    }
}

void Seller::replenishStock(int itemId, int additionalQuantity) {
    for (auto& item : stock) {
        if (item.id == itemId) {
            if (additionalQuantity > 0) {
                item.stock += additionalQuantity;
                std::cout << "Stok untuk " << item.name << " berhasil ditambah sebanyak " 
                        << additionalQuantity << " unit. Stok baru: " << item.stock << std::endl;
            } else {
                std::cout << "Jumlah tambahan harus positif." << std::endl;
            }
        }
        return;
    }
    std::cout << "Error: Item dengan ID " << itemId << " tidak ditemukan." << std::endl;
}

void Seller::discardItem(int itemId) {
    size_t initial_size = stock.size();
    
    // Hapus item dengan ID yang sesuai dengan memindahkan item yang cocok ke akhir dan menghapusnya
    auto new_end = std::remove_if(stock.begin(), stock.end(),
                                [itemId](const Item& item) { 
        return item.id == itemId; 
    });

    stock.erase(new_end, stock.end());

    if (stock.size() < initial_size) {
        std::cout << "Item dengan ID " << itemId << " berhasil dihapus dari stok." << std::endl;
    } else {
        std::cout << "Error: Item dengan ID " << itemId << " tidak ditemukan." << std::endl;
    }
}

// Implementasi untuk kelas Buyer
Buyer::Buyer(int id, const std::string& name, int accountId) : User(id, name, accountId) {}

// Method Buyer
bool Buyer::purchase(Seller& seller, Bank& bank, int itemId, int quantity)
{
    // Cari item di stock seller
    const Item* itemToBuy = nullptr;
    for (const auto& item : seller.getStock()) {
        if (item.id == itemId) {
            itemToBuy = &item;
            break;
        }
    }

    // Validasi item dan stock
    if (itemToBuy == nullptr) {
        std::cout << "Pembelian Gagal: Item dengan ID " << itemId << " tidak ditemukan di stock penjual." << std::endl;
        return false;
    }
    if (itemToBuy->stock < quantity) {
        std::cout << "Pembelian Gagal: Stock item ID " << itemId << " tidak mencukupi." << std::endl;
        return false;
    }

    // Proses pembelian
    double totalPrice = itemToBuy->price * quantity;
    bool transferSuccess = bank.transfer(this->getAccountId(), seller.getAccountId(), totalPrice); // Placeholder untuk hasil transfer

    if (transferSuccess) {
        seller.updateStock(itemId, -quantity); // Kurangi stock penjual
        std::cout << "Pembelian Berhasil! " << this->name << " membeli " << quantity 
                << " unit " << itemToBuy->name << " seharga " << totalPrice << "." << std::endl;
        return true;
    } else {
        std::cout << "Pembelian Gagal karena transfer tidak berhasil." << std::endl;
        return false;
    }
}
