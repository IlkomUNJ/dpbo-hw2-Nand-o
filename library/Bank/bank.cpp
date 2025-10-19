#include <iostream>
#include <fstream> // untuk file I/O
#include <sstream> // untuk memproses string baris demi baris
#include "bank.h"

BankAccount::BankAccount(int accountId, const std::string &name, double initialBalance)
{
    this->accountId = accountId;
    this->ownerName = name;
    this->balance = initialBalance;
}

BankAccount *Bank::findAccount(int accountId)
{
    for (auto &account : accounts)
    {
        if (account.accountId == accountId)
        {
            return &account;
        }
    }
    return nullptr;
}

const BankAccount* Bank::findAccount(int accountId) const
{
    for (const auto &account : accounts)
    {
        if (account.accountId == accountId)
        {
            return &account;
        }
    }
    return nullptr;
}

// Getter
const std::vector<BankAccount>& Bank::getAccounts() const {
    return this->accounts;
}


void Bank::addAccount(const BankAccount &newAccount)
{
    accounts.push_back(newAccount);
}

bool Bank::transfer(int fromAccountId, int toAccountId, double amount)
{
    // Kita cari akun pengirim dan penerima terlebih dahulu
    BankAccount *fromAccount = findAccount(fromAccountId);
    BankAccount *toAccount = findAccount(toAccountId);

    // Validasi akun dan saldo, apakah saldo mencukupi
    if (fromAccount == nullptr)
    {
        std::cout << "Transfer gagal: Akun pengirim dengan ID " << fromAccountId << " tidak ditemukan." << std::endl;
        return false;
    }
    if (toAccount == nullptr)
    {
        std::cout << "Transfer gagal: Akun penerima dengan ID " << toAccountId << " tidak ditemukan." << std::endl;
        return false;
    }
    if (fromAccount->balance < amount)
    {
        std::cout << "Transfer gagal: Saldo akun " << fromAccountId << " tidak mencukupi." << std::endl;
        return false;
    }

    // Lakukan transaksi kalau semua validasi lolos
    fromAccount->balance -= amount;
    toAccount->balance += amount;

    // Catat riwayat transaksi
    history.push_back(BankTransaction(fromAccountId, BankTransactionType::TRANSFER_OUT, amount));
    history.push_back(BankTransaction(toAccountId, BankTransactionType::TRANSFER_IN, amount));

    std::cout << "Transfer Berhasil: " << amount << " dari akun " << fromAccountId << " ke akun " << toAccountId << "." << std::endl;
    return true;
}

void Bank::printAllAccounts() const
{
    std::cout << "--- Daftar Akun di Bank ---" << std::endl;
    for (const auto &account : accounts)
    {
        std::cout << "ID: " << account.accountId
                << ", Nama: " << account.ownerName
                << ", Saldo: " << account.balance << std::endl;
    }
    std::cout << "---------------------------" << std::endl;
}

void Bank::topUp(int accountId, double amount)
{
    BankAccount* account = findAccount(accountId);
    if (account == nullptr)
    {
        std::cout << "Akun dengan ID " << accountId << " tidak ditemukan." << std::endl;
        return;
    }
    if (amount <= 0)
    {
        std::cout << "Jumlah top-up harus lebih dari 0." << std::endl;
        return;
    }
    account->balance += amount;
    history.push_back(BankTransaction(accountId, BankTransactionType::TOP_UP, amount));
    std::cout << "Top-up berhasil! Saldo baru: " << account->balance << std::endl;
}

bool Bank::withdraw(int accountId, double amount)
{
    BankAccount* account = findAccount(accountId);
    if (account) {
        if (account->balance >= amount) {
            account->balance -= amount;
            history.push_back(BankTransaction(accountId, BankTransactionType::WITHDRAW, amount));
            std::cout << "Penarikan sebesar " << amount << " berhasil. Saldo baru: " << account->balance << std::endl;
            return true;
        } else {
            std::cout << "Penarikan gagal. Saldo tidak mencukupi." << std::endl;
            return false;
        }
    } else {
        std::cout << "Error: Akun tidak ditemukan." << std::endl;
        return false;
    }
}

void Bank::checkBalance(int accountId) const
{
    const BankAccount* account = findAccount(accountId);
    if (account == nullptr)
    {
        std::cout << "Akun dengan ID " << accountId << " tidak ditemukan." << std::endl;
        return;
    }
    std::cout << "Saldo Anda saat ini: " << account->balance << std::endl;
}

void Bank::listCashFlow(int accountId) const {
    std::cout << "\n[LAPORAN BANK] Arus Kas Lengkap untuk Akun " << accountId << ":" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    bool found = false;
    for (const auto& tx : this->history) {
        if (tx.accountId == accountId) {
            found = true;
            switch (tx.type) {
                case BankTransactionType::TOP_UP:
                    std::cout << "  - [KREDIT] Top Up: +" << tx.amount << std::endl;
                    break;
                case BankTransactionType::WITHDRAW:
                    std::cout << "  - [DEBIT] Penarikan: -" << tx.amount << std::endl;
                    break;
                case BankTransactionType::TRANSFER_IN:
                    std::cout << "  - [KREDIT] Dana Masuk: +" << tx.amount << std::endl;
                    break;
                case BankTransactionType::TRANSFER_OUT:
                    std::cout << "  - [DEBIT] Dana Keluar: -" << tx.amount << std::endl;
                    break;
            }
        }
    }
    if (!found) {
        std::cout << "Tidak ada riwayat arus kas yang ditemukan." << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}

// Impelmentasi fitur serialisasi
void Bank::saveToFile(const std::string& filename) const {
    // Buka file untuk ditulis
    std::ofstream outFile(filename);
    
    // Validasi pembukaan file
    if (!outFile) {
        std::cerr << "Error: Tidak dapat membuka file " << filename << " untuk ditulis." << std::endl;
        return;
    }

    // Tulis header untuk file CSV
    outFile << "accountId,ownerName,balance\n";

    // Tulis data setiap akun ke file
    for (const auto& account : this->accounts) {
        outFile << account.accountId << ","
                << account.ownerName << ","
                << account.balance << "\n";
    }

    // Tutup file setelah selesai
    std::cout << "Data bank berhasil disimpan ke " << filename << std::endl;
}

void Bank::loadFromFile(const std::string& filename) {
    // Buka file untuk dibaca
    std::ifstream inFile(filename);

    // Validasi pembukaan file
    if (!inFile) {
        std::cerr << "Info: File " << filename << " tidak ditemukan. Memulai dengan data kosong." << std::endl;
        return;
    }

    this->accounts.clear(); // Bersihkan data akun yang ada

    std::string line;
    std::getline(inFile, line); // Lewati header

    // Baca setiap baris data akun
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string segment;

        // Variabel untuk menyimpan data sementara
        int accountId;
        std::string ownerName;
        double balance;

        // Baca accountId
        std::getline(ss, segment, ',');
        accountId = std::stoi(segment); // Konversi string ke int

        // Baca ownerName
        std::getline(ss, segment, ',');
        ownerName = segment;

        // Baca balance
        std::getline(ss, segment, ',');
        balance = std::stod(segment); // Konversi string ke double

        // Buat objek BankAccount dan tambahkan ke daftar akun
        this->accounts.push_back(BankAccount(accountId, ownerName, balance));
    }

    std::cout << "Data bank berhasil dimuat dari " << filename << std::endl;
}

void Bank::saveHistoryToFile(const std::string& filename) const {
    std::ofstream historyFile(filename);
    if (!historyFile.is_open()) {
        std::cerr << "Error: Tidak dapat membuka file " << filename << " untuk menulis riwayat." << std::endl;
        return;
    }

    historyFile << "accountId,type,amount,date\n"; // Header CSV
    for (const auto& tx : this->history) {
        historyFile << tx.accountId << ","
                    << static_cast<int>(tx.type) << ","
                    << tx.amount << ","
                    << tx.date.time_since_epoch().count() << "\n"; // Simpan tanggal sebagai epoch time
    }
}

void Bank::loadHistoryFromFile(const std::string& filename) {
    std::ifstream historyFile(filename);
    if (!historyFile.is_open()) {
        return; // Tidak ada file history, tidak apa-apa
    }

    this->history.clear();
    std::string line;
    std::getline(historyFile, line); // Lewati header

    while (std::getline(historyFile, line)) {
        std::stringstream ss(line);
        std::string segment;

        int accountId, typeInt;
        double amount;
        long long dateCount;

        std::getline(ss, segment, ','); accountId = std::stoi(segment);
        std::getline(ss, segment, ','); typeInt = std::stoi(segment);
        std::getline(ss, segment, ','); amount = std::stod(segment);
        std::getline(ss, segment, ','); dateCount = std::stoll(segment);

        // Buat ulang objek BankTransaction
        BankTransaction tx(accountId, static_cast<BankTransactionType>(typeInt), amount);
        // Setel ulang tanggal sesuai data yang disimpan
        tx.date = std::chrono::system_clock::time_point(std::chrono::nanoseconds(dateCount));
        
        this->history.push_back(tx);
    }
}
