#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// ########################################################## DATA STRUKTUR

// Struct untuk menyimpan informasi pasien
struct Patient {
    int id;
    string name;
    int age;
    string address;
};

// Struct untuk menyimpan informasi dokter
struct Doctor {
    int id;
    string name;
    string specialization;
};

// Struct untuk menyimpan informasi janji temu
struct Appointment {
    int id;
    int patientId;
    int doctorId;
    string patientName;
    string doctorName;
    string date;
    string time;
};

// Struct untuk menyimpan hasil pemeriksaan
struct Examination {
    int id;
    int patientId;
    string diagnosis;
    string treatment;
};

// Struct untuk menyimpan tagihan
struct Tagihan {
    int id;
    string namaPasien;
    string layanan;
    long long jumlah;
    string tanggal;
    bool sudahDibayar;  
};

struct Node {
    Tagihan data;
    Node* next;
};

// Struct untuk laporan
struct Report {
    string type;
    string content;
};

// Struct untuk Node Pasien
struct PatientNode {
    Patient data;
    PatientNode* next;
};

// Struct untuk Node Dokter
struct DoctorNode {
    Doctor data;
    DoctorNode* next;
};

// Struct untuk Node Janji Temu (Appointment)
struct AppointmentNode {
    Appointment data;
    AppointmentNode* next;
};

// Ukuran hash table
const int TABLE_SIZE = 100;

// Hash Table
struct HashTable {
    DoctorNode* table[TABLE_SIZE];
};

PatientNode* patientHead = nullptr;
DoctorNode* doctorHead = nullptr;
AppointmentNode* appointmentHead = nullptr;
AppointmentNode* appointmentTail = nullptr;
Node* head = nullptr; 
HashTable doctorHashTable;

int lastAppointmentId = 0;

// ########################################################## DEKLARASI FUNGSI
void header();
void mainMenu();

int hashFunction(int id);
void addDoctorToHashTable(HashTable& hashTable, const Doctor& newDoctor);
void removeDoctorFromHashTable(HashTable& hashTable, int id);
void addPatient(PatientNode*& head, const Patient& newData);
void loadPatients(PatientNode*& head);
void savePatients(PatientNode* head);
void addDoctor(DoctorNode*& head, const Doctor& newData);
void loadDoctors(DoctorNode*& head);
void saveDoctors(DoctorNode* head);
void addAppointment(AppointmentNode*& head, AppointmentNode*& tail, const Appointment& newData);
void loadAppointments(AppointmentNode*& head, AppointmentNode*& tail);
void saveAppointments(AppointmentNode* head);

void patientManagement();
void registPatient(PatientNode*& head);
void viewPatients(PatientNode* head);
void searchPatients(PatientNode* head);
void editPatient(PatientNode* head);
void deletePatient(PatientNode*& head);

void doctorManagement();
void registDoctor(DoctorNode*& head);
void viewDoctors(DoctorNode* head);
DoctorNode* searchDoctorById(HashTable& hashTable, int id);
void searchDoctors(DoctorNode* head);
void editDoctor(DoctorNode* head);
void deleteDoctor(DoctorNode*& head);

void appointmentScheduling();
void createAppointment(AppointmentNode*& head, AppointmentNode*& tail);
void viewAppointments(AppointmentNode* head);
void editAppointment(AppointmentNode* head);
void cancelAppointment(AppointmentNode*& head, AppointmentNode*& tail);

void billingandPayment();
void buatTagihanBaru();
void lihatDaftarTagihan();
bool cariTagihanRekursif(Node* temp, int idCari, string namaCari);
void cariTagihan();
void editTagihan();
void bayarTagihan();


// ########################################################## MAIN PROGRAM
int main() {

    // Inisialisasi hash table dokter
    for (int i = 0; i < TABLE_SIZE; ++i) {
        doctorHashTable.table[i] = nullptr;
    }


    loadPatients(patientHead);
    loadDoctors(doctorHead);
    loadAppointments(appointmentHead, appointmentTail);

    mainMenu();

    savePatients(patientHead);
    saveDoctors(doctorHead);
    saveAppointments(appointmentHead);
    return 0;
}

// ########################################################## FUNGSI-FUNGSI
void header(){
    system("cls");
	cout <<"#=================================================================#"<<endl;
	cout <<"#--------------------- RUMAH SAKIT SEJAHTERA ---------------------#"<<endl;
	cout <<"#=================================================================#"<<endl;
}

void mainMenu() {
    header();
    cout <<"#-------------------------- MENU UTAMA ---------------------------#"<<endl;
    cout << endl;

    int choice;
    do {
        cout << "1. Manajemen Pasien\n";
        cout << "2. Manajemen Dokter\n";
        cout << "3. Penjadwalan Janji Temu\n";
        cout << "4. Pemeriksaan dan Pengobatan\n";
        cout << "5. Billing dan Pembayaran\n";
        cout << "6. Laporan dan Analisis\n";
        cout << "0. Keluar\n";

        cout << endl;
        cout << "Pilih menu: ";
        cin >> choice;

        switch (choice) {
            case 1: 
                patientManagement(); 
                break;
            case 2: 
                doctorManagement();
                break;
            case 3: 
                appointmentScheduling();
                break;
            case 4: 
                // Pemeriksaan dan pengobatan; 
                break;
            case 5: 
                billingandPayment(); 
                break;
            case 6: 
                // laporan; 
                break;
            case 0: 
                savePatients(patientHead);
                saveDoctors(doctorHead);
                saveAppointments(appointmentHead);
                exit(0);
                break;
            default: cout << "Pilihan tidak valid.\n";
        }
    } while (choice != 0);
}

int hashFunction(int id) {
    return id % TABLE_SIZE;
}

void addDoctorToHashTable(HashTable& hashTable, const Doctor& newDoctor) {
    int index = hashFunction(newDoctor.id);
    DoctorNode* newNode = new DoctorNode{newDoctor, hashTable.table[index]};
    hashTable.table[index] = newNode;
}

void removeDoctorFromHashTable(HashTable& hashTable, int id) {
    int index = hashFunction(id);
    DoctorNode* temp = hashTable.table[index];
    DoctorNode* prev = nullptr;
    while (temp != nullptr) {
        if (temp->data.id == id) {
            if (prev == nullptr) {
                hashTable.table[index] = temp->next;
            } else {
                prev->next = temp->next;
            }
            delete temp;
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

void addPatient(PatientNode*& head, const Patient& newData) {
    PatientNode* newNode = new PatientNode{newData, nullptr};
    if (head == nullptr) {
        head = newNode;
    } else {
        PatientNode* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void loadPatients(PatientNode*& head) {
    ifstream file("data_pasien.txt");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        Patient dataPatient;

        getline(ss, line, ',');
        dataPatient.id = stoi(line);

        getline(ss, dataPatient.name, ',');

        getline(ss, line, ',');
        dataPatient.age = stoi(line);

        getline(ss, dataPatient.address, ',');

        addPatient(head, dataPatient);
    }
    file.close();
}

void savePatients(PatientNode* head) {
    ofstream file("data_pasien.txt");

    PatientNode* current = head;
    while (current != nullptr) {
        Patient& dataPatient = current->data;

        file << dataPatient.id << "," 
            << dataPatient.name << "," 
            << dataPatient.age << "," 
            << dataPatient.address << "\n";

        current = current->next;
    }
    file.close();
}

void addDoctor(DoctorNode*& head, const Doctor& newData) {
    DoctorNode* newNode = new DoctorNode{newData, nullptr};
    if (head == nullptr) {
        head = newNode;
    } else {
        DoctorNode* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    addDoctorToHashTable(doctorHashTable, newData);
}

void loadDoctors(DoctorNode*& head) {
    ifstream file("data_dokter.txt");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        Doctor dataDoctor;

        getline(ss, line, ',');
        dataDoctor.id = stoi(line);

        getline(ss, dataDoctor.name, ',');

        getline(ss, dataDoctor.specialization, ',');

        addDoctor(head, dataDoctor);
        addDoctorToHashTable(doctorHashTable, dataDoctor);
    }
    file.close();
}

void saveDoctors(DoctorNode* head) {
    ofstream file("data_dokter.txt");

    DoctorNode* current = head;
    while (current != nullptr) {
        Doctor& dataDoctor = current->data;

        file << dataDoctor.id << "," 
            << dataDoctor.name << "," 
            << dataDoctor.specialization << "\n";

        current = current->next;
    }
    file.close();
}

void addAppointment(AppointmentNode*& head, AppointmentNode*& tail, const Appointment& newData) {
    AppointmentNode* newNode = new AppointmentNode{newData, nullptr};
    if (tail == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
}

void loadAppointments(AppointmentNode*& head, AppointmentNode*& tail) {
    ifstream file("data_janji_temu.txt");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        Appointment dataAppointment;

        getline(ss, line, ',');
        dataAppointment.id = stoi(line);

        getline(ss, line, ',');
        dataAppointment.patientId = stoi(line);

        getline(ss, dataAppointment.patientName, ',');

        getline(ss, line, ',');
        dataAppointment.doctorId = stoi(line);

        getline(ss, dataAppointment.doctorName, ',');

        getline(ss, dataAppointment.date, ',');

        getline(ss, dataAppointment.time, ',');

        lastAppointmentId = max(lastAppointmentId, dataAppointment.id);

        addAppointment(head, tail, dataAppointment);
    }
    file.close();
}

void saveAppointments(AppointmentNode* head) {
    ofstream file("data_janji_temu.txt");

    AppointmentNode* current = head;
    while (current != nullptr) {
        Appointment& dataAppointment = current->data;

        file << dataAppointment.id << "," 
             << dataAppointment.patientId << "," 
             << dataAppointment.patientName << "," 
             << dataAppointment.doctorId << "," 
             << dataAppointment.doctorName << ","
             << dataAppointment.date << "," 
             << dataAppointment.time << "\n";

        current = current->next;
    }
    file.close();
}

string getPatientNameById(PatientNode* head, int patientId) {
    PatientNode* current = head;
    while (current != nullptr) {
        if (current->data.id == patientId) {
            return current->data.name;
        }
        current = current->next;
    }
    return "Unknown";
}

string getDoctorNameById(DoctorNode* head, int doctorId) {
    DoctorNode* current = head;
    while (current != nullptr) {
        if (current->data.id == doctorId) {
            return current->data.name;
        }
        current = current->next;
    }
    return "Unknown";
}

void patientManagement(){
    header();
    cout <<"#----------------------- REGISTRASI PASIEN -----------------------#"<<endl;
    cout << endl;

    int choice;
                do {
                    cout << "1. Tambah Pasien Baru\n";
                    cout << "2. Lihat Daftar Pasien\n";
                    cout << "3. Cari Pasien\n";
                    cout << "4. Edit Data Pasien\n";
                    cout << "5. Hapus Pasien\n";
                    cout << "0. Kembali\n";

                    cout << endl;
                    cout << "Pilih sub-menu: ";
                    cin >> choice;

                    switch (choice) {
                        case 1: 
                            registPatient(patientHead); 
                            break;
                        case 2: 
                            viewPatients(patientHead); 
                            break;
                        case 3: 
                            searchPatients(patientHead);
                            break;
                        case 4: 
                            editPatient(patientHead); 
                            break;
                        case 5: 
                            deletePatient(patientHead);
                            break;
                        case 0: 
                            mainMenu();
                            break;
                        default: 
                            cout << "Pilihan tidak valid." << endl;
                    }
                } while (choice != 6);
}

void registPatient(PatientNode*& head) {
    header();
    cout <<"#------------------------- TAMBAH PASIEN -------------------------#"<<endl;
    cout << endl;

    Patient p;
    cout << "Masukkan ID Pasien: ";
    cin >> p.id;

    cout << "Masukkan Nama Pasien: ";
    cin.ignore(); 
    getline(cin, p.name); 

    cout << "Masukkan Umur Pasien: ";
    cin >> p.age;

    cout << "Masukkan Alamat Pasien: ";
    cin.ignore(); 
    getline(cin, p.address);

    addPatient(head, p);

    cout << "Pasien berhasil ditambahkan." << endl;
    savePatients(head);

    int back;
    do{
        cout << endl;
        cout << "0. Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back != 0);

    system("cls");
    patientManagement();
}

void viewPatients(PatientNode* head) {
    header();
    cout <<"#-------------------------- DATA PASIEN --------------------------#"<<endl;
    cout << endl;

    PatientNode* temp = head;
    while (temp) {
        cout << "ID         : " << temp->data.id << endl;
        cout << "Nama       : " << temp->data.name << endl;
        cout << "Umur       : " << temp->data.age << endl;
        cout << "Alamat     : " << temp->data.address << endl;
        cout << "--------------------------------------\n";
        temp = temp->next;
    }

    int back;
    do{
        cout << endl;
        cout << "0.Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back !=0);

    system("cls");
    patientManagement();
}

void searchPatients(PatientNode* head) {
    header();
    cout <<"#------------------------ CARI DATA PASIEN -----------------------#"<<endl;
    cout << endl;

    int choice;
    cout << "1. Cari berdasarkan ID\n";
    cout << "2. Cari berdasarkan Nama\n";
    cout << "0. Kembali\n";

    cout << endl ;
    cout << "Pilih opsi: ";
    cin >> choice;

    if (choice == 0) {
        patientManagement();
        return;
    }

    switch (choice) {
        case 1: {
            header();
            cout <<"#---------------------- CARI ID DATA PASIEN ----------------------#"<<endl;
            cout << endl;

            int id;
            cout << "Masukkan ID Pasien: ";
            cin >> id;

            PatientNode* temp = head;
            bool found = false;
            while (temp != nullptr) {
                if (temp->data.id == id) {
                    cout << "Data Pasien Ditemukan:\n";
                    cout << "ID         : " << temp->data.id << endl;
                    cout << "Nama       : " << temp->data.name << endl;
                    cout << "Umur       : " << temp->data.age << endl;
                    cout << "Alamat     : " << temp->data.address << endl;
                    found = true;
                    break;
                }
                temp = temp->next;
            }
            if (!found) {
                cout << "Data Pasien dengan ID " << id << " tidak ditemukan.\n";
            }
            break;
        }
        case 2: {
            header();
            cout <<"#--------------------- CARI NAMA DATA PASIEN ---------------------#"<<endl;
            cout << endl;

            string name;
            cout << "Masukkan Nama Pasien: ";
            cin.ignore();
            getline(cin, name);

            PatientNode* temp = head;
            bool found = false;
            while (temp != nullptr) {
                if (temp->data.name == name) {
                    cout << "Data Pasien Ditemukan:\n";
                    cout << "ID         : " << temp->data.id << endl;
                    cout << "Nama       : " << temp->data.name << endl;
                    cout << "Umur       : " << temp->data.age << endl;
                    cout << "Alamat     : " << temp->data.address << endl;
                    found = true;
                }
                temp = temp->next;
            }
            if (!found) {
                cout << "Data Pasien dengan nama " << name << " tidak ditemukan.\n";
            }
            break;
        }
        default:
            cout << "Pilihan tidak valid." << endl;
            break;
    }

    int back;
    do {
        cout << endl;
        cout << "0. Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back != 0);

    system("cls");
    patientManagement();
}

void editPatient(PatientNode* head) {
    header();
    cout <<"#----------------------- EDIT DATA PASIEN -----------------------#"<<endl;
    cout << endl;

    int choice;
    cout << "1. Edit Berdasarkan ID\n";
    cout << "2. Edit Berdasarkan Nama\n";
    cout << "0. Kembali\n";

    cout << endl;
    cout << "Masukkan pilihan: ";
    cin >> choice;

    if (choice == 0) {
        patientManagement();
        return;
    }

    if (choice == 1) {
        int id;
        cout << "Masukkan ID Pasien yang Ingin Diedit: ";
        cin >> id;

        PatientNode* temp = head;
        bool found = false;
        while (temp != nullptr) {
            if (temp->data.id == id) {
                cout << "Data Pasien Ditemukan:\n";
                cout << "ID         : " << temp->data.id << endl;
                cout << "Nama       : " << temp->data.name << endl;
                cout << "Umur       : " << temp->data.age << endl;
                cout << "Alamat     : " << temp->data.address << endl;

                cout << endl;
                cout << "Masukkan Data Baru:\n";
                cout << "Nama Pasien: ";
                cin.ignore();
                getline(cin, temp->data.name);
                cout << "Umur       : ";
                cin >> temp->data.age;
                cout << "Alamat     : ";
                cin.ignore();
                getline(cin, temp->data.address);

                savePatients(patientHead);
                cout << "Data Pasien Berhasil Diperbarui.\n";
                found = true;
                break;
            }
            temp = temp->next;
        }
        if (!found) {
            cout << "Data Pasien dengan ID " << id << " tidak ditemukan.\n";
        }
    } else if (choice == 2) {
        string name;
        cout << "Masukkan Nama Pasien yang Ingin Diedit: ";
        cin.ignore();
        getline(cin, name);

        PatientNode* temp = head;
        bool found = false;
        while (temp != nullptr) {
            if (temp->data.name == name) {
                cout << "Data Pasien Ditemukan:\n";
                cout << "ID         : " << temp->data.id << endl;
                cout << "Nama       : " << temp->data.name << endl;
                cout << "Umur       : " << temp->data.age << endl;
                cout << "Alamat     : " << temp->data.address << endl;

                cout << endl;
                cout << "Masukkan Data Baru:\n";
                cout << "Nama Pasien: ";
                getline(cin, temp->data.name);
                cout << "Umur       : ";
                cin >> temp->data.age;
                cout << "Alamat     : ";
                cin.ignore();
                getline(cin, temp->data.address);

                savePatients(patientHead);
                cout << "Data Pasien Berhasil Diperbarui.\n";
                found = true;
                break;
            }
            temp = temp->next;
        }
        if (!found) {
            cout << "Data Pasien dengan Nama " << name << " tidak ditemukan.\n";
        }
    } else {
        cout << "Pilihan tidak valid.\n";
    }

    int back;
    do {
        cout << endl;
        cout << "0. Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back != 0);

    system("cls");
    patientManagement();
}

void deletePatient(PatientNode*& head) {
    header();
    cout <<"#----------------------- HAPUS DATA PASIEN -----------------------#"<<endl;
    cout << endl;

    int choice;
    cout << "1. Hapus Berdasarkan ID\n";
    cout << "2. Hapus Berdasarkan Nama\n";
    cout << "0. Kembali\n";

    cout << endl;
    cout << "Masukkan pilihan: ";
    cin >> choice;

    if (choice == 0) {
        patientManagement();
        return;
    }

    if (choice == 1) {
        int id;
        cout << "Masukkan ID Pasien yang Ingin Dihapus: ";
        cin >> id;

        PatientNode* temp = head;
        PatientNode* prev = nullptr;
        bool found = false;
        while (temp != nullptr) {
            if (temp->data.id == id) {
                cout << "Data Pasien yang akan dihapus:\n";
                cout << "ID         : " << temp->data.id << endl;
                cout << "Nama       : " << temp->data.name << endl;
                cout << "Umur       : " << temp->data.age << endl;
                cout << "Alamat     : " << temp->data.address << endl;

                char confirm;
                cout << "Apakah Anda yakin ingin menghapus data ini? (y/n): ";
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    if (prev == nullptr) {
                        head = temp->next;
                    } else {
                        prev->next = temp->next;
                    }
                    delete temp;
                    savePatients(patientHead);
                    cout << "Data Pasien dengan ID " << id << " berhasil dihapus.\n";
                } else {
                    cout << "Penghapusan data dibatalkan.\n";
                }
                found = true;
                break;
            }
            prev = temp;
            temp = temp->next;
        }
        if (!found) {
            cout << "Data Pasien dengan ID " << id << " tidak ditemukan.\n";
        }
    } else if (choice == 2) {
        string name;
        cout << "Masukkan Nama Pasien yang Ingin Dihapus: ";
        cin.ignore();
        getline(cin, name);

        PatientNode* temp = head;
        PatientNode* prev = nullptr;
        bool found = false;
        while (temp != nullptr) {
            if (temp->data.name == name) {
                // Menampilkan data pasien yang akan dihapus
                cout << "Data Pasien yang akan dihapus:\n";
                cout << "ID         : " << temp->data.id << endl;
                cout << "Nama       : " << temp->data.name << endl;
                cout << "Umur       : " << temp->data.age << endl;
                cout << "Alamat     : " << temp->data.address << endl;

                // Meminta konfirmasi pengguna
                char confirm;
                cout << "Apakah Anda yakin ingin menghapus data ini? (y/n): ";
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    if (prev == nullptr) {
                        head = temp->next;
                    } else {
                        prev->next = temp->next;
                    }
                    delete temp;
                    savePatients(patientHead);
                    cout << "Data Pasien dengan Nama " << name << " berhasil dihapus.\n";
                } else {
                    cout << "Penghapusan data dibatalkan.\n";
                }
                found = true;
                break;
            }
            prev = temp;
            temp = temp->next;
        }
        if (!found) {
            cout << "Data Pasien dengan Nama " << name << " tidak ditemukan.\n";
        }
    } else {
        cout << "Pilihan tidak valid.\n";
    }

    int back;
    do {
        cout << endl;
        cout << "0. Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back != 0);

    system("cls");
    patientManagement();
}

void doctorManagement() {
    header();
    cout <<"#----------------------- MANAJEMEN DOKTER ------------------------#"<<endl;
    cout << endl;

    int choice;
    do {
        cout << "1. Tambah Dokter Baru\n";
        cout << "2. Lihat Daftar Dokter\n";
        cout << "3. Cari Dokter\n";
        cout << "4. Edit Data Dokter\n";
        cout << "5. Hapus Dokter\n";
        cout << "0. Kembali\n";

        cout << endl;
        cout << "Pilih sub-menu: ";
        cin >> choice;

        switch (choice) {
            case 1: 
                registDoctor(doctorHead);
                break;
            case 2: 
                viewDoctors(doctorHead);
                break;
            case 3: 
                searchDoctors(doctorHead);
                break;
            case 4: 
                editDoctor(doctorHead);
                break;
            case 5: 
                deleteDoctor(doctorHead);
                break;
            case 0: 
                mainMenu();
                break;
            default: 
                cout << "Pilihan tidak valid." << endl;
        }
    } while (choice != 6);
}

void registDoctor(DoctorNode*& head) {
    header();
    cout <<"#------------------------- TAMBAH DOKTER -------------------------#"<<endl;
    cout << endl;

    Doctor d;
    cout << "Masukkan ID Dokter: ";
    cin >> d.id;

    cout << "Masukkan Nama Dokter: ";
    cin.ignore(); 
    getline(cin, d.name); 

    cout << "Masukkan Spesialisasi Dokter: ";
    getline(cin, d.specialization);

    addDoctor(head, d);

    cout << "Dokter berhasil ditambahkan." << endl;
    saveDoctors(head);

    int back;
    do{
        cout << endl;
        cout << "0.Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back !=0);

    system("cls");
    doctorManagement();
}

void viewDoctors(DoctorNode* head) {
    header();
    cout <<"#-------------------------- DATA DOKTER --------------------------#"<<endl;
    cout << endl;

    DoctorNode* temp = head;
    while (temp) {
        cout << "ID         : " << temp->data.id << endl;
        cout << "Nama       : " << temp->data.name << endl;
        cout << "Spesialis  : " << temp->data.specialization << endl;
        cout << "--------------------------------------\n";
        temp = temp->next;
    }

    int back;
    do{
        cout << endl;
        cout << "0.Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back !=0);

    system("cls");
    doctorManagement();
}

DoctorNode* searchDoctorById(HashTable& hashTable, int id) {
    int index = hashFunction(id);
    DoctorNode* temp = hashTable.table[index];
    while (temp != nullptr) {
        if (temp->data.id == id) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}

void searchDoctors(DoctorNode* head) {
    header();
    cout <<"#------------------------ CARI DATA DOKTER -----------------------#"<<endl;
    cout << endl;

    int choice;
    cout << "1. Cari berdasarkan ID\n";
    cout << "2. Cari berdasarkan Nama\n";
    cout << "0. Kembali\n";

    cout << endl ;
    cout << "Pilih opsi: ";
    cin >> choice;

    if (choice == 0) {
        doctorManagement();
        return;
    }

    switch (choice) {
        case 1: {
            header();
            cout <<"#---------------------- CARI ID DATA DOKTER ----------------------#"<<endl;
            cout << endl;

            int id;
            cout << "Masukkan ID Dokter: ";
            cin >> id;

            DoctorNode* temp = searchDoctorById(doctorHashTable, id);
            if (temp != nullptr) {
                cout << "Data Dokter Ditemukan:\n";
                cout << "ID         : " << temp->data.id << endl;
                cout << "Nama       : " << temp->data.name << endl;
                cout << "Spesialis  : " << temp->data.specialization << endl;
            } else {
                cout << "Data Dokter dengan ID " << id << " tidak ditemukan.\n";
            }
            break;
        }
        case 2: {
            header();
            cout <<"#--------------------- CARI NAMA DATA DOKTER ---------------------#"<<endl;
            cout << endl;

            string name;
            cout << "Masukkan Nama Dokter: ";
            cin.ignore();
            getline(cin, name);

            DoctorNode* temp = head;
            bool found = false;
            while (temp != nullptr) {
                if (temp->data.name == name) {
                    cout << "Data Dokter Ditemukan:\n";
                    cout << "ID         : " << temp->data.id << endl;
                    cout << "Nama       : " << temp->data.name << endl;
                    cout << "Spesialis  : " << temp->data.specialization << endl;
                    found = true;
                }
                temp = temp->next;
            }
            if (!found) {
                cout << "Data Dokter dengan nama " << name << " tidak ditemukan.\n";
            }
            break;
        }
        default:
            cout << "Pilihan tidak valid." << endl;
            break;
    }

    int back;
    do {
        cout << endl;
        cout << "0. Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back != 0);

    system("cls");
    doctorManagement();
}

void editDoctor(DoctorNode* head) {
    header();
    cout <<"#----------------------- EDIT DATA DOKTER -----------------------#"<<endl;
    cout << endl;

    int choice;
    cout << "1. Edit Berdasarkan ID\n";
    cout << "2. Edit Berdasarkan Nama\n";
    cout << "0. Kembali\n";

    cout << endl;
    cout << "Masukkan pilihan: ";
    cin >> choice;

    if (choice == 0) {
        doctorManagement();
        return;
    }

    if (choice == 1) {
        int id;
        cout << "Masukkan ID Dokter yang Ingin Diedit: ";
        cin >> id;

        DoctorNode* temp = head;
        bool found = false;
        while (temp != nullptr) {
            if (temp->data.id == id) {
                cout << "Data Dokter Ditemukan:\n";
                cout << "ID         : " << temp->data.id << endl;
                cout << "Nama       : " << temp->data.name << endl;
                cout << "Spesialis  : " << temp->data.specialization << endl;

                cout << endl;
                cout << "Masukkan Nama Dokter Baru: ";
                cin.ignore(); 
                getline(cin, temp->data.name); 

                cout << "Masukkan Spesialisasi Dokter Baru: ";
                getline(cin, temp->data.specialization);

                removeDoctorFromHashTable(doctorHashTable, id);
                addDoctorToHashTable(doctorHashTable, temp->data);
                saveDoctors(doctorHead);

                cout << "Data Dokter Berhasil Diperbarui.\n";
                found = true;
                break;
            }
            temp = temp->next;
        }
        if (!found) {
            cout << "Data Dokter dengan ID " << id << " tidak ditemukan.\n";
        }
    } else if (choice == 2) {
        string name;
        cout << "Masukkan Nama Dokter yang Ingin Diedit: ";
        cin.ignore();
        getline(cin, name);

        DoctorNode* temp = head;
        bool found = false;
        while (temp != nullptr) {
            if (temp->data.name == name) {
                cout << "Data Dokter Ditemukan:\n";
                cout << "ID         : " << temp->data.id << endl;
                cout << "Nama       : " << temp->data.name << endl;
                cout << "Spesialis  : " << temp->data.specialization << endl;

                cout << endl;
                cout << "Masukkan Nama Dokter Baru: ";
                cin.ignore(); 
                getline(cin, temp->data.name); 

                cout << "Masukkan Spesialisasi Dokter Baru: ";
                getline(cin, temp->data.specialization);

                removeDoctorFromHashTable(doctorHashTable, temp->data.id);
                addDoctorToHashTable(doctorHashTable, temp->data);
                saveDoctors(doctorHead);
                cout << "Data Dokter Berhasil Diperbarui.\n";
                found = true;
                break;
            }
            temp = temp->next;
        }
        if (!found) {
            cout << "Data Dokter dengan Nama " << name << " tidak ditemukan.\n";
        }
    } else {
        cout << "Pilihan tidak valid.\n";
    }

    int back;
    do {
        cout << endl;
        cout << "0. Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back != 0);

    system("cls");
    doctorManagement();
}

void deleteDoctor(DoctorNode*& head) {
    header();
    cout <<"#----------------------- HAPUS DATA DOKTER -----------------------#"<<endl;
    cout << endl;

    int choice;
    cout << "1. Hapus Berdasarkan ID\n";
    cout << "2. Hapus Berdasarkan Nama\n";
    cout << "0. Kembali\n";

    cout << endl;
    cout << "Masukkan pilihan: ";
    cin >> choice;

    if (choice == 0) {
        doctorManagement();
        return;
    }

    if (choice == 1) {
        int id;
        cout << "Masukkan ID Dokter yang Ingin Dihapus: ";
        cin >> id;

        DoctorNode* temp = head;
        DoctorNode* prev = nullptr;
        bool found = false;
        while (temp != nullptr) {
            if (temp->data.id == id) {
                cout << "Data Dokter yang akan dihapus:\n";
                cout << "ID         : " << temp->data.id << endl;
                cout << "Nama       : " << temp->data.name << endl;
                cout << "Spesialis  : " << temp->data.specialization << endl;

                char confirm;
                cout << "Apakah Anda yakin ingin menghapus data ini? (y/n): ";
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    if (prev == nullptr) {
                        head = temp->next;
                    } else {
                        prev->next = temp->next;
                    }
                    removeDoctorFromHashTable(doctorHashTable, id);
                    delete temp;
                    saveDoctors(doctorHead);
                    cout << "Data Dokter dengan ID " << id << " berhasil dihapus.\n";
                } else {
                    cout << "Penghapusan data dibatalkan.\n";
                }
                found = true;
                break;
            }
            prev = temp;
            temp = temp->next;
        }
        if (!found) {
            cout << "Data Dokter dengan ID " << id << " tidak ditemukan.\n";
        }
    } else if (choice == 2) {
        string name;
        cout << "Masukkan Nama Dokter yang Ingin Dihapus: ";
        cin.ignore();
        getline(cin, name);

        DoctorNode* temp = head;
        DoctorNode* prev = nullptr;
        bool found = false;
        while (temp != nullptr) {
            if (temp->data.name == name) {
                cout << "Data Dokter yang akan dihapus:\n";
                cout << "ID         : " << temp->data.id << endl;
                cout << "Nama       : " << temp->data.name << endl;
                cout << "Spesialis  : " << temp->data.specialization << endl;

                char confirm;
                cout << "Apakah Anda yakin ingin menghapus data ini? (y/n): ";
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y') {
                    if (prev == nullptr) {
                        head = temp->next;
                    } else {
                        prev->next = temp->next;
                    }
                    removeDoctorFromHashTable(doctorHashTable, temp->data.id);
                    delete temp;
                    saveDoctors(doctorHead);
                    cout << "Data Dokter dengan Nama " << name << " berhasil dihapus.\n";
                } else {
                    cout << "Penghapusan data dibatalkan.\n";
                }
                found = true;
                break;
            }
            prev = temp;
            temp = temp->next;
        }
        if (!found) {
            cout << "Data Dokter dengan Nama " << name << " tidak ditemukan.\n";
        }
    } else {
        cout << "Pilihan tidak valid.\n";
    }

    int back;
    do {
        cout << endl;
        cout << "0. Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back != 0);

    system("cls");
    doctorManagement();
}


void appointmentScheduling() {
    header();
    cout <<"#--------------------- PENJADWALAN JANJI TEMU --------------------#"<<endl;
    cout << endl;

    int choice;
    do {
        cout << "1. Buat Janji Temu Baru\n";
        cout << "2. Lihat Daftar Janji Temu\n";
        cout << "3. Edit Janji Temu\n";
        cout << "4. Batalkan Janji Temu\n";
        cout << "0. Kembali\n";

        cout << endl;
        cout << "Pilih sub-menu: ";
        cin >> choice;

        switch (choice) {
            case 1: 
                createAppointment(appointmentHead, appointmentTail); 
                break;
            case 2: 
                viewAppointments(appointmentHead); 
                break;
            case 3: 
                editAppointment(appointmentHead); 
                break;
            case 4: 
                cancelAppointment(appointmentHead, appointmentTail); 
                break;
            case 0: 
                mainMenu(); 
                break;
            default: cout << "Pilihan tidak valid.\n";
        }
    } while (choice != 0);
}

void createAppointment(AppointmentNode*& head, AppointmentNode*& tail) {
    Appointment newAppointment;

    newAppointment.id = ++lastAppointmentId;

    cout << "Masukkan ID pasien: ";
    cin >> newAppointment.patientId;

    newAppointment.patientName = getPatientNameById(patientHead, newAppointment.patientId);
    cout << "Nama Pasien: " << newAppointment.patientName << endl;
    
    cout << "Masukkan ID dokter: ";
    cin >> newAppointment.doctorId;

    newAppointment.doctorName = getDoctorNameById(doctorHead, newAppointment.doctorId);
    cout << "Nama Dokter: " << newAppointment.doctorName << endl;
    
    cout << "Masukkan tanggal janji temu (DD-MM-YYYY): ";
    cin >> newAppointment.date;

    cout << "Masukkan waktu janji temu (HH:MM): ";
    cin >> newAppointment.time;

    addAppointment(head, tail, newAppointment);

    cout << "Janji temu berhasil dibuat.\n";
    saveAppointments(head);

    int back;
    do{
        cout << endl;
        cout << "0.Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back !=0);

    system("cls");
    appointmentScheduling();
}

void viewAppointments(AppointmentNode* head) {
    header();
    cout <<"#----------------------- DAFTAR JANJI TEMU -----------------------#"<<endl;
    cout << endl;

    AppointmentNode* current = head;
    while (current != nullptr) {
        Appointment& appointment = current->data;
        cout << "ID Janji Temu: " << appointment.id << endl;
        cout << "ID Pasien: " << appointment.patientId << endl;
        cout << "Nama Pasien: " << appointment.patientName << endl;
        cout << "ID Dokter: " << appointment.doctorId << endl;
        cout << "Nama Doktor: " << appointment.doctorName << endl;
        cout << "Tanggal: " << appointment.date << endl;
        cout << "Waktu: " << appointment.time << endl;
        cout << "-----------------------------\n";
        current = current->next;
    }

    int back;
    do{
        cout << endl;
        cout << "0.Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back !=0);

    system("cls");
    appointmentScheduling();

}

void editAppointment(AppointmentNode* head) {
    int id;
    cout << "Masukkan ID janji temu yang ingin diedit: ";
    cin >> id;

    AppointmentNode* current = head;
    while (current != nullptr) {
        if (current->data.id == id) {
            cout << "ID Pasien: " << current->data.patientId << "\n";
            cout << "Nama Pasien: " << current->data.patientName << "\n";
            cout << "ID Dokter: " << current->data.doctorId << "\n";
            cout << "Nama Pasien: " << current->data.doctorName << "\n";
            cout << "Tanggal (saat ini): " << current->data.date << "\n";
            cout << "Waktu (saat ini): " << current->data.time << "\n";

            cout << "Masukkan Tanggal baru (YYYY-MM-DD): ";
            cin >> current->data.date;

            cout << "Masukkan Waktu baru (HH:MM): ";
            cin >> current->data.time;

            cout << "Janji temu berhasil diupdate.\n";
            saveAppointments(head);
            return;
        }
        current = current->next;
    }
    cout << "Janji temu tidak ditemukan.\n";

    int back;
    do{
        cout << endl;
        cout << "0.Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back !=0);

    system("cls");
    appointmentScheduling();
}

void cancelAppointment(AppointmentNode*& head, AppointmentNode*& tail) {
    int id;
    cout << "Masukkan ID janji temu yang ingin dibatalkan: ";
    cin >> id;

    AppointmentNode* current = head;
    AppointmentNode* previous = nullptr;

    while (current != nullptr) {
        if (current->data.id == id) {
            if (previous == nullptr) {
                head = current->next;
            } else {
                previous->next = current->next;
            }
            if (current == tail) {
                tail = previous;
            }
            delete current;

            cout << "Janji temu berhasil dibatalkan.\n";
            saveAppointments(head);
            return;
        }
        previous = current;
        current = current->next;
    }

    cout << "Janji temu dengan ID tersebut tidak ditemukan.\n";

    int back;
    do {
        cout << endl;
        cout << "0. Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back != 0);

    system("cls");
    appointmentScheduling();
}

void billingandPayment() {
    int pilihan;
    do {
        cout << "\nMenu Billing dan Pembayaran:" << endl;
        cout << "1. Buat Tagihan Baru" << endl;
        cout << "2. Lihat Daftar Tagihan" << endl;
        cout << "3. Cari Tagihan" << endl;
        cout << "4. Edit Tagihan" << endl;
        cout << "5. Bayar Tagihan" << endl;
        cout << "6. Keluar" << endl;
        cout << "Masukkan pilihan: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                buatTagihanBaru();
                break;
            case 2:
                lihatDaftarTagihan();
                break;
            case 3:
                cariTagihan();
                break;
            case 4:
                editTagihan();
                break;
            case 5:
                bayarTagihan();
                break;
            case 6:
                break;
            default:
                cout << "\nPilihan tidak valid. Silakan coba lagi." << endl;
        }
    } while (pilihan != 6);
}

void buatTagihanBaru() {
    Tagihan tagihanBaru;
    
    cout << "\nMasukkan ID tagihan\t\t: ";
    cin >> tagihanBaru.id;
    cin.ignore(); 

    Node* temp = head;
    while (temp != nullptr) {
        if (temp->data.id == tagihanBaru.id) {
            cout << "ID tagihan sudah ada. Silakan masukkan ID lain.\n";
            return; 
        }
        temp = temp->next;
    }

    cout << "Masukkan nama pasien \t\t: ";
    getline(cin, tagihanBaru.namaPasien);
    cout << "Masukkan layanan\t\t: ";
    getline(cin, tagihanBaru.layanan);
    cout << "Masukkan jumlah tagihan\t\t: ";
    cin >> tagihanBaru.jumlah;
    cin.ignore();
    cout << "Masukkan tanggal (YYYY-MM-DD)\t: ";
    getline(cin, tagihanBaru.tanggal);

    Node* newNode = new Node;
    newNode->data = tagihanBaru;
    newNode->next = nullptr;

    if (head == nullptr) {
        head = newNode;
    } else { 
        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    cout << "\nTagihan berhasil dibuat!" << endl;
}

void lihatDaftarTagihan() {
    if (head == nullptr) {
        cout << "\nTidak ada tagihan yang tersimpan." << endl;
    return;
    }
    Node* temp = head;
    if (temp == nullptr) {
        cout << "\nTidak ada tagihan yang ditemukan." << endl;
        return;
    }
    cout << "\nDaftar Tagihan" << endl;
    while (temp != nullptr) {
        Tagihan tagihanSaatIni = temp->data;
        cout << "ID\t\t: " << tagihanSaatIni.id << "\nNama Pasien\t: " << tagihanSaatIni.namaPasien
             << " \nLayanan\t\t: " << tagihanSaatIni.layanan << " \nJumlah\t\t: Rp" << tagihanSaatIni.jumlah
             << " \nTanggal\t\t: " << tagihanSaatIni.tanggal << endl;
        temp = temp->next;
    }
}

bool cariTagihanRekursif(Node* temp, int idCari, string namaCari) {
  if (temp == nullptr) {
    return false; 
  }

  if (temp->data.id == idCari || temp->data.namaPasien == namaCari) {
    cout << "\nTagihan ditemukan\t\t:" << endl;
    cout << "\nID\t\t: " << temp->data.id << "\nNama Pasien\t: " << temp->data.namaPasien
         << " \nLayanan\t\t: " << temp->data.layanan << " \nJumlah\t\t: Rp" << temp->data.jumlah
         << " \nTanggal\t\t: " << temp->data.tanggal << endl;
    return true; 
  }

  return cariTagihanRekursif(temp->next, idCari, namaCari);
}

void cariTagihan() {
    if (head == nullptr) {
        cout << "\nTidak ada tagihan yang tersimpan untuk dicari." << endl;
        return;
    }
    int idCari;
    string namaCari;
    cout << "\nMasukkan ID tagihan yang ingin dicari\t: ";
    cin >> idCari;
    cin.ignore(); 

   bool ditemukan = cariTagihanRekursif(head, idCari, namaCari);
  if (!ditemukan) {
    cout << "Tagihan tidak ditemukan." << endl;
  }
}

void editTagihan() {
    if (head == nullptr) {
        cout << "\nTidak ada tagihan yang tersimpan untuk diedit." << endl;
        return;
    }

    int idEdit;
    cout << "\nMasukkan ID tagihan yang ingin diedit: ";
    cin >> idEdit;

    Node* temp = head;
    bool ditemukan = false;
    while (temp != nullptr) {
        Tagihan& tagihanSaatIni = temp->data;
        if (tagihanSaatIni.id == idEdit) {
            ditemukan = true;
            cout << "Masukkan nama pasien baru\t\t: ";
            cin.ignore(); 
            getline(cin, tagihanSaatIni.namaPasien);
            cout << "Masukkan layanan baru\t\t\t: ";
            cin.ignore(); 
            getline(cin, tagihanSaatIni.layanan);
            cout << "Masukkan jumlah baru\t\t\t: ";
            cin >> tagihanSaatIni.jumlah;
            cin.ignore();
            cout << "Masukkan tanggal baru (YYYY-MM-DD)\t: ";
            getline(cin, tagihanSaatIni.tanggal);
            cout << "Tagihan berhasil diperbarui!" << endl;
            break;
        }
        temp = temp->next;
    }

    if (!ditemukan) {
        cout << "Tagihan tidak ditemukan." << endl;
    }
}

void bayarTagihan() {
    if (head == nullptr) {
        cout << "Tidak ada tagihan yang tersimpan." << endl;
        return;
    }

    int id;
    cout << "Masukkan ID Tagihan yang ingin dibayar: ";
    cin >> id;

    Node* temp = head;
    Node* prev = nullptr;
    bool found = false;
    while (temp != nullptr) {
        if (temp->data.id == id) {
            found = true;
            cout << "Biaya Tagihan\t: Rp" << temp->data.jumlah << endl;
            cout << "Nama \t\t: " << temp->data.namaPasien << endl;

            char konfirmasi;
            cout << "Apakah Anda ingin membayar tagihan ini? (y/n): ";
            cin >> konfirmasi;

            if (konfirmasi == 'y' || konfirmasi == 'Y') {
                temp->data.sudahDibayar = true; 
                cout << "\nTagihan berhasil dibayar." << endl;
                if (prev == nullptr) { 
                    head = temp->next;
                } else {
                    prev->next = temp->next;
                }
                delete temp;
                return;
            } else {
                cout << "Pembayaran dibatalkan." << endl;
                return;
            }
        }
        prev = temp;
        temp = temp->next;
    }

    if (!found) {
        cout << "Tagihan tidak ditemukan." << endl;
        return;
    }
}