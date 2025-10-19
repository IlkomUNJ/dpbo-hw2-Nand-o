#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "item.h"
#include "bank.h"

// Deklarasi terlebih dahulu untuk Seller, karena dibutuhkan oleh method di Buyer
class Seller;

class User
{
protected:
    int id;
    std::string name;
    int accountId;

public:
    User(int id, const std::string &name, int accountId);

    virtual ~User() = default;

    // Getter
    int getId() const;
    const std::string &getName() const;
    int getAccountId() const;
};

// Kelas Seller
class Seller : public User
{
private:
    std::vector<Item> stock; // Stock item yang dimiliki seller
    int nextItemId = 1; // ID untuk item berikutnya yang ditambahkan
public:
    Seller(int id, const std::string &name, int accoundId);

    // Getter
    const std::vector<Item> &getStock() const;

    // Method Seller
    void addItem(const std::string& name, double price, int stock);
    void _reAddItem(const Item& item); // Untuk sistem/pemuatan data
    void viewStock() const;
    void updateStock(int itemId, int newStock);
    void replenishStock(int itemId, int additionalQuantity);
    void discardItem(int itemId);

    // Fitur laporan
    void discoverTopKItems(int k_count, const std::vector<Transaction>& all_transactions) const;
    void discoverLoyalCustomers(int k_count, const std::vector<Transaction>& all_transactions) const;
};

// Kelas Buyer
class Buyer : public User
{
public:
    Buyer(int id, const std::string& name, int accountId);

    // Method Buyer
    bool purchase(Seller& seller, Bank& bank, int itemId, int quantity);
};

#endif
