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
struct Bill {
    int id;
    int patientId;
    double amount;
    bool isPaid;
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

PatientNode* patientHead = nullptr;
DoctorNode* doctorHead = nullptr;

// ########################################################## DEKLARASI FUNGSI
void header();
void mainMenu();

void addPatient(PatientNode*& head, const Patient& newData);
void loadPatients(PatientNode*& head);
void savePatients(PatientNode* head);
void addDoctor(DoctorNode*& head, const Doctor& newData);
void loadDoctors(DoctorNode*& head);
void saveDoctors(DoctorNode* head);

void patientManagement();
void registPatient(PatientNode*& head);
void viewPatients(PatientNode* head);
void searchPatients(PatientNode* head);
void editPatient(PatientNode* head);
void deletePatient(PatientNode*& head);

void doctorManagement();
void registDoctor(DoctorNode*& head);
void viewDoctors(DoctorNode* head);
void searchDoctors(DoctorNode* head);
void editDoctor(DoctorNode* head);
void deleteDoctor(DoctorNode*& head);

// ########################################################## MAIN PROGRAM
int main() {

    loadPatients(patientHead);
    loadDoctors(doctorHead);

    mainMenu();

    savePatients(patientHead);
    saveDoctors(doctorHead);
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
                // Penjadwalan Janji Temu; 
                break;
            case 4: 
                // Pemeriksaan dan pengobatan; 
                break;
            case 5: 
                // Billing dan pembayaran; 
                break;
            case 6: 
                // laporan; 
                break;
            case 0: 
                savePatients(patientHead);
                saveDoctors(doctorHead);
                exit(0);
                break;
            default: cout << "Pilihan tidak valid.\n";
        }
    } while (choice != 0);
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

            DoctorNode* temp = head;
            bool found = false;
            while (temp != nullptr) {
                if (temp->data.id == id) {
                    cout << "Data Dokter Ditemukan:\n";
                    cout << "ID         : " << temp->data.id << endl;
                    cout << "Nama       : " << temp->data.name << endl;
                    cout << "Spesialis  : " << temp->data.specialization << endl;
                    found = true;
                    break;
                }
                temp = temp->next;
            }
            if (!found) {
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