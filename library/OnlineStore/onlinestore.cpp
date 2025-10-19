#include <iostream>
#include "onlinestore.h"

OnlineStore::OnlineStore(Bank &bank) : bank(bank) {}

// Destruktor untuk membersihkan memori
OnlineStore::~OnlineStore()
{
    for (User *user : users)
    {
        delete user; // Menghapus setiap User yang terdaftar
    }
}

User* OnlineStore::findUserById(int userId) {
    for (auto* user : users) {
        if (user->getId() == userId) {
            return user;
        }
    }
    return nullptr; // Jika tidak ditemukan
}

Transaction* OnlineStore::findTransactionById(int transactionId) {
    for (auto& tx : transactions) {
        if (tx.id == transactionId) {
            return &tx;
        }
    }
    return nullptr; // Jika tidak ditemukan
}

// Getter
const std::vector<Transaction>& OnlineStore::getTransactions() const {
    return this->transactions;
}

const std::vector<User*>& OnlineStore::getUsers() const {
    return this->users;
}

// Implementasi fitur utama
void OnlineStore::registerUser(User *newUser)
{
    this->users.push_back(newUser);
}

void OnlineStore::recordTransaction(const Transaction &transaction)
{
    this->transactions.push_back(transaction);
}
