#ifndef BANK_TRANSACTION_H
#define BANK_TRANSACTION_H

#include <chrono>

enum class BankTransactionType {
    TOP_UP,
    WITHDRAW,
    TRANSFER_IN,
    TRANSFER_OUT
};

struct BankTransaction {
    int accountId;
    BankTransactionType type;
    double amount;
    std::chrono::system_clock::time_point date;

    BankTransaction(int accountId, BankTransactionType type, double amount);
};

#endif
