#ifndef ONLINESTORE_H
#define ONLINESTORE_H

#include <string>
#include <vector>
#include "user.h"
#include "transaction.h"
#include "bank.h"

class OnlineStore
{
private:
    std::vector<User *> users;             // Menyimpan pointer ke User (Seller/Buyer)
    std::vector<Transaction> transactions; // Menyimpan riwayat transaksi
    Bank &bank;                            // Referensi ke objek Bank

public:
    OnlineStore(Bank &bank);
    ~OnlineStore(); // Destruktor untuk membersihkan memori

    User* findUserById(int userId);
    Transaction* findTransactionById(int transactionId);

    // Getter
    const std::vector<Transaction>& getTransactions() const;
    const std::vector<User*>& getUsers() const;

    // Fitur Utama
    void registerUser(User *newUser);
    void recordTransaction(const Transaction &transaction);

    // Fitur Pelaporan
    void listLatestTransactions(int k_days) const;
    void listPaidButNotCompleted() const;
    void listMostFrequentItems(int m_count) const;
    void listMostActiveBuyers(int m_count) const;
    void listMostActiveSellers(int m_count) const;

    // Fitur Serialisasi
    void saveUsersToFile(const std::string& filename) const;
    void loadUsersFromFile(const std::string& filename);
    void saveTransactionsToFile(const std::string& filename) const;
    void loadTransactionsFromFile(const std::string& filename);
};

#endif
