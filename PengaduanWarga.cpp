    #include <iostream>
    #include <unordered_map>
    #include <functional>
    #include <fstream>
    #include <sstream>
    #include "MyString.h"
    #include "MyVector.h"

    using namespace std;

    // ====================== DATA STRUCTURE ======================
    struct Pengaduan {
        size_t id;
        MyString nama;
        MyString namaAsli;
        MyString nik;
        MyString alamat;
        MyString noTelepon;
        MyString isiPengaduan;
        MyString status;
        int prioritas;
        MyString tanggal; 
    };


    // ====================== ENKRIPSI SEDERHANA ======================
    MyString enkripsiNama(MyString nama) {
        for (size_t i = 0; i < nama.length(); ++i) {
        nama[i] += 3;
    }
    return nama;
    }

    // ====================== HASHING ID PENGADUAN ======================
    size_t generateHashID(const MyString& nama, const MyString& isi) {
        hash<string> hasher;
        return hasher(nama.toStdString() + isi.toStdString());
    }

    // ====================== SORTING (BUBBLE SORT) ======================
    void sortByPrioritas(MyVector<Pengaduan>& data) {
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < data.size() - i - 1; ++j) {
                if (data[j].prioritas > data[j + 1].prioritas) {
                    swap(data[j], data[j + 1]);
                }
            }
        }
    }

    // ====================== QUEUE IMPLEMENTATION ======================
    class QueuePengaduan {
    private:
        MyVector<Pengaduan> data;

    public:
        void push(const Pengaduan& p) {
            data.push_back(p);
        }

        void pop() {
            if (!empty()) {
                data.erase(0);
            }
        }

        Pengaduan front() const {
            if (!empty()) return data [0];
            throw runtime_error("Queue kosong");
        }

        bool empty() const {
            return data.empty();
        }

        size_t size() const {
            return data.size();
        }

        MyVector<Pengaduan> getAll() const {
            return data;
        }

        void removeByID(size_t id) {
            MyVector<Pengaduan> temp;
            for (size_t i = 0; i < data.size(); ++i) {
                if (data[i].id != id) temp.push_back(data[i]);
            }
            data = temp;
        }
    };

    // ====================== STACK IMPLEMENTATION ======================
    class StackPengaduan {
    private:
        MyVector<Pengaduan> data;

    public:
        void push(const Pengaduan& p) {
            data.push_back(p);
        }

        Pengaduan pop() {
            if (data.empty()) throw runtime_error("Stack undo kosong");
            Pengaduan topItem = data.back();
            data.pop_back();
            return topItem;
        }

        bool empty() const {
            return data.empty();
        }
    };

    // ====================== COMPLAINT APPLICATION ======================
    class AplikasiPengaduan {
    private:
        QueuePengaduan laporanQueue;
        StackPengaduan undoSystem;
        unordered_map<size_t, Pengaduan> laporanMap;
        unordered_map<size_t, bool> finalPengaduan;

    public:
        void tambahPengaduan(Pengaduan pengaduanBaru) {
            pengaduanBaru.namaAsli = pengaduanBaru.nama;
            pengaduanBaru.nama = enkripsiNama(pengaduanBaru.nama);
            pengaduanBaru.status = MyString("Terselesaikan");
            pengaduanBaru.id = generateHashID(pengaduanBaru.nama, pengaduanBaru.isiPengaduan);

            laporanQueue.push(pengaduanBaru);
            laporanMap[pengaduanBaru.id] = pengaduanBaru;
            finalPengaduan[pengaduanBaru.id] = false;

            cout << "Pengaduan berhasil ditambahkan! ID : " << pengaduanBaru.id << endl;

            ofstream file("data_pengaduan.txt", ios::app); 
            ifstream check("data_pengaduan.txt");
            check.seekg(0, ios::end);
            if (check.tellg() == 0) {
            file << "Tanggal|ID|Nama|Nama Enkripsi|NIK|Alamat|Telepon|Isi Pengaduan|Status|Prioritas\n";

            }
            check.close();
            
            if (file.is_open()) {
                file << pengaduanBaru.tanggal << "|" 
                    << pengaduanBaru.id << "|"
                    << pengaduanBaru.namaAsli << "|"
                    << pengaduanBaru.nama << "|"
                    << pengaduanBaru.nik << "|"
                    << pengaduanBaru.alamat << "|"
                    << pengaduanBaru.noTelepon << "|"
                    << pengaduanBaru.isiPengaduan << "|"
                    << pengaduanBaru.status << "|"
                    << pengaduanBaru.prioritas << endl;
                file.close();
            }
        }

        void muatDariFile() {
        ifstream file("data_pengaduan.txt");
        string baris;
        while (getline(file, baris)) {
            stringstream ss(baris);
            Pengaduan p;
            string idStr;
            string prioritasStr;
            string temp;

            getline(ss, temp, '|'); 
            p.tanggal = MyString(temp);
            getline(ss, idStr, '|'); p.id = stoull(idStr);
            getline(ss, temp, '|');
            p.namaAsli = MyString(temp);
            getline(ss, temp, '|');
            p.nama = MyString(temp);
            getline(ss, temp, '|');
            p.nik = MyString(temp);
            getline(ss, temp, '|');
            p.alamat = MyString(temp);
            getline(ss, temp, '|');
            p.noTelepon = MyString(temp);
            getline(ss, temp, '|');
            p.isiPengaduan = MyString(temp);
            getline(ss, temp, '|');
            p.status = MyString(temp);

            getline(ss, prioritasStr);
            p.prioritas = stoi(prioritasStr);

            laporanQueue.push(p);
            laporanMap[p.id] = p;
            finalPengaduan[p.id] = (p.status == "Terselesaikan");
        }
        file.close();
        }

        void simpanKeFile() {
            ofstream file("data_pengaduan.txt");
            file << "Tanggal|ID|Nama|Nama Enkripsi|NIK|Alamat|Telepon|Isi Pengaduan|Status|Prioritas\n";

            for (const auto& [id, p] : laporanMap) {
                file << p.tanggal << "|"
                    << p.id << "|"
                    << p.namaAsli << "|"
                    << p.nama << "|"
                    << p.nik << "|"
                    << p.alamat << "|"
                    << p.noTelepon << "|"
                    << p.isiPengaduan << "|"
                    << p.status << "|"
                    << p.prioritas << "\n";
            }

            file.close();
        }

        void tampilkanPengaduan() {
            if (laporanQueue.empty()) {
                cout << "Tidak ada pengaduan saat ini." << endl;
                return;
            }

            MyVector<Pengaduan> sortedPengaduan = laporanQueue.getAll();
            sortByPrioritas(sortedPengaduan);

            for (size_t i = 0; i < sortedPengaduan.size(); ++i) {
                const Pengaduan& p = sortedPengaduan[i];
                cout << "Tanggal: " << p.tanggal << endl;
                cout << "ID: " << p.id << endl;
                cout << "Nama: " << p.namaAsli << endl;
                cout << "NIK: " << p.nik << endl;
                cout << "Alamat: " << p.alamat << endl;
                cout << "Telepon: " << p.noTelepon << endl;
                cout << "Isi Pengaduan: " << p.isiPengaduan << endl;
                cout << "Status: " << p.status << endl;
                cout << "Prioritas: " << p.prioritas << endl;
                cout << "----------------------------" << endl;
                cout << endl;
            }
        }

        void selesaikanPengaduan(size_t id) {
            if (laporanMap.find(id) != laporanMap.end()) {
                if (finalPengaduan[id]) {
                    cout << "Pengaduan ini sudah final dan tidak dapat diubah." << endl;
                    return;
                }
                laporanMap[id].status = MyString("Terselesaikan");
                undoSystem.push(laporanMap[id]);
                finalPengaduan[id] = true;
                cout << "Pengaduan ID " << id << " telah diselesaikan dan dikunci." << endl;
            } else {
                cout << "Pengaduan dengan ID " << id << " tidak ditemukan." << endl;
            }

            simpanKeFile();
        }

        void editIsiPengaduan(size_t id, string isiBaru) {
            if (laporanMap.find(id) != laporanMap.end()) {
                if (finalPengaduan[id]) {
                    cout << "Pengaduan ini sudah final dan tidak dapat diedit." << endl;
                    return;
                }
                undoSystem.push(laporanMap[id]); 
                laporanMap[id].isiPengaduan = isiBaru;

                // Sinkronkan dengan queue
                laporanQueue.removeByID(id);
                laporanQueue.push(laporanMap[id]);

                cout << "Isi pengaduan ID " << id << " berhasil diperbarui." << endl;
            } else {
                cout << "Pengaduan dengan ID " << id << " tidak ditemukan." << endl;
            }

            simpanKeFile();
        }

        void undo() {
        if (undoSystem.empty()) {
            cout << "Tidak ada aksi untuk di-undo." << endl;
            return;
        }

        Pengaduan terakhir = undoSystem.pop();

        if (finalPengaduan[terakhir.id]) {
            cout << "Pengaduan ini sudah final dan tidak bisa di-undo." << endl;
            return;
        }

        // Kembalikan data di map
        laporanMap[terakhir.id] = terakhir;

        // Sinkronkan ke queue
        laporanQueue.removeByID(terakhir.id);
        laporanQueue.push(terakhir);

        cout << "Perubahan pada pengaduan ID " << terakhir.id << " dibatalkan." << endl;

        simpanKeFile();
        }

        void hapusPengaduan(size_t id) {
            if (laporanMap.find(id) != laporanMap.end()) {
                if (finalPengaduan[id]) {
                    cout << "Pengaduan ini sudah final dan tidak dapat dihapus." << endl;
                    return;
                }
                laporanQueue.removeByID(id);
                laporanMap.erase(id);
                finalPengaduan.erase(id);
                cout << "Pengaduan ID " << id << " telah dihapus." << endl;
            } else {
                cout << "Pengaduan dengan ID " << id << " tidak ditemukan." << endl;
            }

            simpanKeFile();
        }

        bool isFinal(size_t id) const {
            auto it = finalPengaduan.find(id);
            return it != finalPengaduan.end() && it->second;
        }
    };

    // ====================== VALIDASI ======================
    bool validasiNIK(const string& nik) {
        if (nik.length() != 16) return false;
        for (char c : nik) {
            if (!isdigit(c)) return false;
        }
        return true;
    }

    bool validasiTelepon(const string& telp) {
        if (telp.length() < 10 || telp.length() > 13) return false;
        for (char c : telp) {
            if (!isdigit(c)) return false;
        }
        return true;
    }

    bool validasiTanggal(const string& tanggal) {
        if (tanggal.length() != 8) return false; 

        // Cek karakter ke-3 dan ke-6 adalah '/'
        if (tanggal[2] != '/' || tanggal[5] != '/') return false;

        // Cek apakah karakter selain '/' adalah digit
        for (size_t i = 0; i < tanggal.length(); ++i) {
            if (i == 2 || i == 5) continue;
            if (!isdigit(tanggal[i])) return false;
        }

        return true;
    }


    // ====================== MAIN ======================
    int main() {
        AplikasiPengaduan app;
        app.muatDariFile();
        int pilihan;
        string nama, nik, alamat, telepon, isi, tanggal;
        size_t id;
        int prioritas;

        while (true) {
            cout << "=== Aplikasi Pengaduan Warga ===" << endl;
            cout << "1. Tambah Pengaduan" << endl;
            cout << "2. Tampilkan Pengaduan" << endl;
            cout << "3. Selesaikan Pengaduan" << endl;
            cout << "4. Edit Isi Pengaduan" << endl;
            cout << "5. Undo Perubahan" << endl;
            cout << "6. Hapus Pengaduan" << endl;
            cout << "7. Keluar" << endl;
            cout << "Pilih menu: ";
            cin >> pilihan;
            cin.ignore();

            switch (pilihan) {
                case 1: {
                    Pengaduan p;
                    cout << "Masukkan nama pelapor: ";
                    getline(cin, nama);
                    p.nama = nama;

                    while (true) {
        cout << "Masukkan tanggal pengaduan (dd/mm/yy): ";
        getline(cin, tanggal);
        if (validasiTanggal(tanggal)) break;
        cout << "Format tanggal tidak valid. Contoh yang benar: 25/05/25" << endl;
    }
    p.tanggal = tanggal;

                    while (true) {
                        cout << "Masukkan NIK (16 digit): ";
                        getline(cin, nik);
                        if (validasiNIK(nik)) break;
                        cout << "NIK tidak valid. Harus 16 digit angka." << endl;
                    }
                    p.nik = nik;

                    while (true) {
                        cout << "Masukkan nomor telepon (10-13 digit): ";
                        getline(cin, telepon);
                        if (validasiTelepon(telepon)) break;
                        cout << "Nomor telepon tidak valid. Harus 10-13 digit angka dan tanpa spasi." << endl;
                    }
                    p.noTelepon = telepon;

                    cout << "Masukkan alamat: ";
                    getline(cin, alamat);
                    p.alamat = alamat;

                    cout << "Masukkan isi pengaduan: ";
                    getline(cin, isi);
                    p.isiPengaduan = isi;

                    cout << "Masukkan prioritas (1-5): ";
                    cin >> prioritas;
                    p.prioritas = prioritas;

                    app.tambahPengaduan(p);
                    break;
                }

                case 2:
                    app.tampilkanPengaduan();
                    break;

                case 3:
                    cout << "Masukkan ID pengaduan yang ingin diselesaikan: ";
                    cin >> id;
                    app.selesaikanPengaduan(id);
                    break;

                case 4:
                    cout << "Masukkan ID pengaduan yang ingin diedit: ";
                    cin >> id;
                    cin.ignore();
                    if (app.isFinal(id)) {
                        cout << "Pengaduan ini sudah final dan tidak dapat diedit." << endl;
                        break;
                    }
                    cout << "Masukkan isi pengaduan baru: ";
                    getline(cin, isi);
                    app.editIsiPengaduan(id, isi);
                    break;

                case 5:
                    app.undo();
                    break;

                case 6:
                    cout << "Masukkan ID pengaduan yang ingin dihapus: ";
                    cin >> id;
                    app.hapusPengaduan(id);
                    break;

                case 7:
                    cout << "Terima kasih telah menggunakan aplikasi ini!" << endl;
                    return 0;

                default:
                    cout << "Pilihan tidak valid! Silakan coba lagi." << endl;
            }
        }

        return 0;
    }