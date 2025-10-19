#ifndef BANK_HPP
#define BANK_HPP

#include <string>
#include <vector>
#include "transaction.h"
#include "bankTransaction.h"

struct BankAccount
{
    int accountId;
    std::string ownerName;
    double balance;

    BankAccount(int accountId, const std::string &ownerName, double initialBalance);
};

class Bank
{
private:
    std::vector<BankAccount> accounts;
    std::vector<BankTransaction> history;

    BankAccount* findAccount(int accountId);
    const BankAccount* findAccount(int accountId) const;

public:
    // Getter
    const std::vector<BankAccount>& getAccounts() const;

    void addAccount(const BankAccount& newAccount); // Menambahkan akun baru
    bool transfer(int fromAccountId, int toAccountId, double amount); // Transfer antar akun
    void printAllAccounts() const; // Mencetak semua akun
    void topUp(int accountId, double amount); // Top-up saldo akun tertentu
    bool withdraw(int accountId, double amount); // Tarik tunai dari akun tertentu
    void checkBalance(int accountId) const; // Cek saldo akun tertentu
    void listCashFlow(int accountId) const; // Lihat riwayat transaksi akun tertentu

    // Fitur Serialisasi
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void saveHistoryToFile(const std::string& filename) const;
    void loadHistoryFromFile(const std::string& filename);

    // Fitur laporan
    void listDormantAccounts(const std::vector<Transaction>& transactions) const;
    void listTopUsersToday(int n_count, const std::vector<Transaction>& transactions) const;
};

#endif
