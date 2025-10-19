#include "bankTransaction.h"

BankTransaction::BankTransaction(int accountId, BankTransactionType type, double amount) {
    this->accountId = accountId;
    this->type = type;
    this->amount = amount;
    this->date = std::chrono::system_clock::now(); // Set tanggal ke waktu saat ini
}
