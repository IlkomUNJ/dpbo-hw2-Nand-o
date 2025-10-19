[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/SCVt0OYF)

Name: Ernando Febrian

NIM : 1313624021

# Instruction
1. Fill in your details
2. Clone this repo
3. Move your source to this repo
4. Fill in the required section in the Readme
5. This assignment is due October 20th, 23:55 AM

# Features Implemented (list all completed)
1.  **Struktur OOP Inti:** Implementasi kelas-kelas dasar (`Item`, `Transaction`, `BankAccount`, `BankTransaction`, `Bank`, `User`, `Seller`, `Buyer`, `OnlineStore`) dengan enkapsulasi yang tepat.
2.  **Pewarisan (Inheritance):** Kelas `Seller` dan `Buyer` mewarisi dari kelas dasar `User`.
3.  **Polimorfisme:** Pengelolaan pengguna (`Buyer` & `Seller`) menggunakan pointer kelas dasar (`User*`) di `OnlineStore`, dan penggunaan `dynamic_cast` untuk menentukan peran spesifik pengguna.
4.  **Persistensi Data (Serialisasi):** Implementasi fungsi simpan (`saveToFile`) dan muat (`loadFromFile`) untuk semua data penting (akun bank, riwayat bank, pengguna, stok penjual, transaksi) menggunakan format file CSV ke dalam folder `data` terpisah.
5.  **Manajemen Waktu:** Penggunaan library `<chrono>` untuk mencatat tanggal transaksi dan memfilter laporan berdasarkan periode waktu (hari ini, sebulan terakhir, k hari terakhir).
6.  **Struktur Proyek Modular:** Pemisahan deklarasi (`.hpp`/`.h`) dan implementasi (`.cpp`) ke dalam folder-folder logis (`library`, `handlers`, `data`). Implementasi kelas besar dipecah ke beberapa file (`onlinestore_*.cpp`).
7.  **Sistem Build:** Konfigurasi `meson.build` untuk otomatisasi proses kompilasi proyek multi-file.
8.  **Antarmuka Terminal Interaktif:** Implementasi menu berbasis *loop* (`while`) dengan `enum` untuk navigasi yang jelas (Menu Utama, Menu Pembeli, Menu Penjual, Menu Admin, Menu Perbankan).
9.  **Validasi Input:** Fungsi *helper* `getNumericInput` untuk memastikan input numerik yang aman dan penanganan *error* input dasar.
10. **Fungsionalitas Bank:** Implementasi `Top Up`, `Withdraw`, `Transfer`, `Cek Saldo`, serta pencatatan otomatis semua aktivitas ke dalam riwayat bank (`BankTransaction`).
11. **Fungsionalitas Pembeli:** Lihat semua barang, Beli barang (dengan validasi stok & saldo), Lihat riwayat order (dengan filter status `PAID`/`COMPLETED`/`CANCELLED` dan detail item), Konfirmasi penerimaan pesanan (mengubah status ke `COMPLETED`), Batalkan pesanan (dengan proses *refund* & *restock*, hanya jika status `PAID`), Laporan pengeluaran `k` hari terakhir, Laporan Arus Kas detail.
12. **Fungsionalitas Penjual:** Semua fitur pembeli (termasuk perbankan & arus kas), Manajemen Stok (Tambah item dengan ID otomatis, Isi Ulang Stok, Hapus Item), Laporan Penjualan (Item terpopuler per bulan, Pelanggan setia per bulan).
13. **Fungsionalitas Admin:** Akses menu admin melalui "login" sederhana (username/password dari file `data/conf/admin.conf`), Laporan Global (Item terlaris, Pembeli/Penjual teraktif, Akun *dormant*, Top user bank harian).
14. **Fitur Tambahan:** Tampilkan semua pengguna terdaftar (urut ID) di menu utama.

# Collaborators (The person you learned from)
1. Nandana Ammar T.

# Students (the person you taught to complete the assignments)
1. Nandana Ammar T.
2. Leonard Dwi C.

# AI Assistance
AI Tools: Gemini, Github Copilot

Paid AI Plugin (state none, if not subscribed): none

Total Prompts used (int estimate): 75

Dominant prompt used for (seek solution / implement ideas) : implement ideas
