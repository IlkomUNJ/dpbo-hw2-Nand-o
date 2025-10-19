#ifndef HANDLERS_H
#define HANDLERS_H

#include <string>
#include "onlinestore.h"
#include "bank.h"

bool checkingValidName(OnlineStore &store, std::string &name);
int getNumericInput(const std::string &prompt);

void handleRegister(OnlineStore &store, Bank &bank);
void handleLogin(OnlineStore &store, Bank &bank);
void handleAdminLogin(OnlineStore &store, Bank &bank);
void handleShowAllUsers(OnlineStore &store);

// Fungsi handler spesifik untuk setelah login berhasil
void showBuyerMenu(Buyer *buyer, OnlineStore &store, Bank &bank);
void showSellerMenu(Seller *seller, OnlineStore &store, Bank &bank);
void showAdminMenu(OnlineStore &store, Bank &bank);
void showBankMenu(User *user, Bank &bank);

// Fungsi spesifik untuk keduanya (Buyer & Seller)
void handleCashFlowReport(User *user, OnlineStore &store, Bank &bank);

// Fungsi spesifik untuk Buyer
void handleViewOrderHistory(Buyer *buyer, OnlineStore &store);
void handleSpendingReport(Buyer *buyer, OnlineStore &store);
void handleCancelOrder(Buyer *buyer, OnlineStore &store, Bank &bank);


#endif
