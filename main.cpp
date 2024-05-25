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
    int doctorId;
    string patientName;
    string doctorName;
    string doctorspecialization;
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
    bool sudahDibayar = false;
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

// Struct untuk tree Pasien
struct PatientTreeNode {
    Patient data;
    PatientTreeNode* left;
    PatientTreeNode* right;
};

// Struct untuk Node Janji Temu (Appointment)
struct AppointmentNode {
    Appointment data;
    AppointmentNode* next;
};

// Struct untuk Node Pemeriksaan
struct ExaminationNode {
    Examination data;
    ExaminationNode* next;
};

// Struct untuk operasi stack (Undo)
struct Operation {
    string type;
    Examination data;
};

struct StackNode {
    Operation data;
    StackNode* next;
};

struct SpecializationNode {
    string specialization;
    ExaminationNode* examinationList;
    SpecializationNode* next;
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
PatientTreeNode* patientRoot = nullptr;
Node* head = nullptr; 
HashTable doctorHashTable;
ExaminationNode* examinationHead = nullptr;
SpecializationNode* specializationHead = nullptr;
StackNode* operationStack = nullptr;

int lastAppointmentId = 0;
int lastExaminationId = 0;

// ########################################################## DEKLARASI FUNGSI
void header();
void mainMenu();

int hashFunction(int id);
void addDoctorToHashTable(HashTable& hashTable, const Doctor& newDoctor);
void removeDoctorFromHashTable(HashTable& hashTable, int id);
void insertPatientToTree(PatientTreeNode*& root, const Patient& newData);
void deletePatientTreeNode(PatientTreeNode*& root);
void addPatient(PatientNode*& head, const Patient& newData);
void loadPatients(PatientNode*& head);
void savePatients(PatientNode* head);
void addDoctor(DoctorNode*& head, const Doctor& newData);
void loadDoctors(DoctorNode*& head);
void saveDoctors(DoctorNode* head);
void addAppointment(AppointmentNode*& head, AppointmentNode*& tail, const Appointment& newData);
void loadAppointments(AppointmentNode*& head, AppointmentNode*& tail);
void saveAppointments(AppointmentNode* head);
void loadExaminations(ExaminationNode*& head);
void saveExaminations(ExaminationNode* head);
void addExamination(ExaminationNode*& head, const Examination& newData);
void push(StackNode*& top, const Operation& newData);
Operation pop(StackNode*& top);
bool isEmpty(StackNode* top);

void patientManagement();
void registPatient(PatientNode*& head);
void viewPatients(PatientNode* head);
PatientTreeNode* searchPatientById(PatientTreeNode* root, int id);
void searchPatients(PatientNode* head);
void editPatient(PatientNode* head);
void deletePatientById(PatientTreeNode*& root, int id);
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

void examinationAndTreatment();
void addExaminationResult();
void viewMedicalHistory();
void editExaminationResult();
void deleteExaminationResult();
void undoLastOperation();
void buildSpecializationGraph();
void displaySpecializationGraph();
void displayAllExaminations();

void loadTagihanDariBill();
void billingandPayment();
void simpanKeBill();
void buatTagihanBaru();
void lihatDaftarTagihan();
bool cariTagihanRekursif(Node* temp, int idCari, string namaCari);
void cariTagihan();
void editTagihan();
void bayarTagihan();


// ########################################################## MAIN PROGRAM
int main() {

    // Inisialisasi untuk hash table dokter
    for (int i = 0; i < TABLE_SIZE; ++i) {
        doctorHashTable.table[i] = nullptr;
    }


    loadPatients(patientHead);
    loadDoctors(doctorHead);
    loadAppointments(appointmentHead, appointmentTail);
    loadExaminations(examinationHead);
    loadTagihanDariBill();

    mainMenu();

    savePatients(patientHead);
    saveDoctors(doctorHead);
    saveAppointments(appointmentHead);
    saveExaminations(examinationHead);
    simpanKeBill();

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
                examinationAndTreatment();
                break;
            case 5: 
                billingandPayment(); 
                break;
            case 0: 
                savePatients(patientHead);
                saveDoctors(doctorHead);
                saveAppointments(appointmentHead);
                saveExaminations(examinationHead);
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

void insertPatientToTree(PatientTreeNode*& root, const Patient& newData) {
    if (root == nullptr) {
        root = new PatientTreeNode{newData, nullptr, nullptr};
    } else {
        if (newData.id < root->data.id) {
            insertPatientToTree(root->left, newData);
        } else if (newData.id > root->data.id) {
            insertPatientToTree(root->right, newData);
        }
    }
}

void deletePatientTreeNode(PatientTreeNode*& root) {
    if (root == nullptr) {
        return;
    }
    deletePatientTreeNode(root->left);
    deletePatientTreeNode(root->right);
    delete root;
    root = nullptr;
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
        insertPatientToTree(patientRoot, dataPatient);
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

string getDoctorSpecializationById(DoctorNode* head, int doctorId) {
    DoctorNode* current = head;
    while (current != nullptr) {
        if (current->data.id == doctorId) {
            return current->data.specialization;
        }
        current = current->next;
    }
    return "Unknown";
}

void addExamination(ExaminationNode*& head, const Examination& newData) {
    ExaminationNode* newNode = new ExaminationNode{newData, nullptr};
    if (head == nullptr) {
        head = newNode;
    } else {
        ExaminationNode* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void loadExaminations(ExaminationNode*& head) {
    ifstream file("data_pemeriksaan.txt");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        Examination dataExamination;

        getline(ss, line, ',');
        dataExamination.id = stoi(line);

        getline(ss, line, ',');
        dataExamination.patientId = stoi(line);

        getline(ss, dataExamination.patientName, ',');

        getline(ss, line, ',');
        dataExamination.doctorId = stoi(line);

        getline(ss, dataExamination.doctorName, ',');

        getline(ss, dataExamination.doctorspecialization, ',');

        getline(ss, dataExamination.diagnosis, ',');

        getline(ss, dataExamination.treatment, ',');

        addExamination(head, dataExamination);
    }
    file.close();
}

void saveExaminations(ExaminationNode* head) {
    ofstream file("data_pemeriksaan.txt");

    ExaminationNode* current = head;
    while (current != nullptr) {
        Examination& dataExamination = current->data;

        file << dataExamination.id << "," 
            << dataExamination.patientId << "," 
            << dataExamination.patientName << "," 
            << dataExamination.doctorId << "," 
            << dataExamination.doctorName << ","
            << dataExamination.doctorspecialization << ","
            << dataExamination.diagnosis << "," 
            << dataExamination.treatment << "\n";

        current = current->next;
    }
    file.close();
}

void push(StackNode*& top, const Operation& newData) {
    StackNode* newNode = new StackNode{newData, nullptr};
    newNode->next = top;
    top = newNode;
}

Operation pop(StackNode*& top) {
    if (isEmpty(top)) {
        throw runtime_error("Stack is empty");
    }
    StackNode* temp = top;
    Operation data = temp->data;
    top = top->next;
    delete temp;
    return data;
}

bool isEmpty(StackNode* top) {
    return top == nullptr;
}

void buildSpecializationGraph() {
    // Reset graph
    while (specializationHead != nullptr) {
        SpecializationNode* temp = specializationHead;
        specializationHead = specializationHead->next;
        delete temp;
    }

    // Buat graph
    ExaminationNode* current = examinationHead;
    while (current != nullptr) {
        SpecializationNode* specNode = specializationHead;
        SpecializationNode* prev = nullptr;

        // Cek jika node spesialis sudah ada
        while (specNode != nullptr && specNode->specialization != current->data.doctorspecialization) {
            prev = specNode;
            specNode = specNode->next;
        }

        // jika belum ada
        if (specNode == nullptr) {
            SpecializationNode* newSpecNode = new SpecializationNode{current->data.doctorspecialization, nullptr, nullptr};
            if (prev == nullptr) {
                specializationHead = newSpecNode;
            } else {
                prev->next = newSpecNode;
            }
            specNode = newSpecNode;
        }

        // Tambahkan pemeriksaan ke daftar graf spesialisasi
        ExaminationNode* newExaminationNode = new ExaminationNode{current->data, specNode->examinationList};
        specNode->examinationList = newExaminationNode;

        current = current->next;
    }
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
    cout << "Masukkan ID Pasien     : ";
    cin >> p.id;

    cout << "Masukkan Nama Pasien   : ";
    cin.ignore(); 
    getline(cin, p.name); 

    cout << "Masukkan Umur Pasien   : ";
    cin >> p.age;

    cout << "Masukkan Alamat Pasien : ";
    cin.ignore(); 
    getline(cin, p.address);

    addPatient(head, p);

    cout << "Pasien berhasil ditambahkan." << endl;
    savePatients(head);
    insertPatientToTree(patientRoot, p);

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

PatientTreeNode* searchPatientById(PatientTreeNode* root, int id) {
    if (root == nullptr || root->data.id == id) {
        return root;
    }
    if (id < root->data.id) {
        return searchPatientById(root->left, id);
    }
    return searchPatientById(root->right, id);
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

            bool found = false;
            PatientTreeNode* patient = searchPatientById(patientRoot, id);
            if (patient != nullptr) {
                found = true;
                cout << "Data Pasien Ditemukan:\n";
                cout << "ID         : " << patient->data.id << endl;
                cout << "Nama       : " << patient->data.name << endl;
                cout << "Umur       : " << patient->data.age << endl;
                cout << "Alamat     : " << patient->data.address << endl;
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
                    found = true;
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
                deletePatientById(patientRoot, id);
                insertPatientToTree(patientRoot, temp->data);
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
                found = true;
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
                deletePatientById(patientRoot, temp->data.id);
                insertPatientToTree(patientRoot, temp->data);
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

void deletePatientById(PatientTreeNode*& root, int id) {
    if (root == nullptr) {
        return;
    }
    if (id < root->data.id) {
        deletePatientById(root->left, id);
    } else if (id > root->data.id) {
        deletePatientById(root->right, id);
    } else {
        if (root->left == nullptr) {
            PatientTreeNode* temp = root->right;
            delete root;
            root = temp;
        } else if (root->right == nullptr) {
            PatientTreeNode* temp = root->left;
            delete root;
            root = temp;
        } else {
            PatientTreeNode* temp = root->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            root->data = temp->data;
            deletePatientById(root->right, temp->data.id);
        }
    }
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
                    deletePatientById(patientRoot, id);
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
    cout << "Masukkan ID Dokter             : ";
    cin >> d.id;

    cout << "Masukkan Nama Dokter           : ";
    cin.ignore(); 
    getline(cin, d.name); 

    cout << "Masukkan Spesialisasi Dokter   : ";
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

    cout << "Masukkan ID pasien     : ";
    cin >> newAppointment.patientId;

    newAppointment.patientName = getPatientNameById(patientHead, newAppointment.patientId);
    cout << "Nama Pasien            : " << newAppointment.patientName << endl;
    
    cout << "Masukkan ID dokter     : ";
    cin >> newAppointment.doctorId;

    newAppointment.doctorName = getDoctorNameById(doctorHead, newAppointment.doctorId);
    cout << "Nama Dokter            : " << newAppointment.doctorName << endl;
    
    cout << "Masukkan tanggal janji temu (DD-MM-YYYY)   : ";
    cin >> newAppointment.date;

    cout << "Masukkan waktu janji temu (HH:MM)          : ";
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
        cout << "ID Janji Temu      : " << appointment.id << endl;
        cout << "ID Pasien          : " << appointment.patientId << endl;
        cout << "Nama Pasien        : " << appointment.patientName << endl;
        cout << "ID Dokter          : " << appointment.doctorId << endl;
        cout << "Nama Doktor        : " << appointment.doctorName << endl;
        cout << "Tanggal            : " << appointment.date << endl;
        cout << "Waktu              : " << appointment.time << endl;
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
    header();
    cout <<"#------------------------ EDIT JANJI TEMU ------------------------#"<<endl;
    cout << endl;

    int id;
    cout << "Masukkan ID janji temu yang ingin diedit: ";
    cin >> id;

    AppointmentNode* current = head;
    while (current != nullptr) {
        if (current->data.id == id) {
            cout << "ID Pasien          : " << current->data.patientId << "\n";
            cout << "Nama Pasien        : " << current->data.patientName << "\n";
            cout << "ID Dokter          : " << current->data.doctorId << "\n";
            cout << "Nama Dokter        : " << current->data.doctorName << "\n";
            cout << "Tanggal (saat ini) : " << current->data.date << "\n";
            cout << "Waktu (saat ini)   : " << current->data.time << "\n";

            cout << endl;
            cout << "Masukkan Tanggal baru (DD-MM-YYYY) : ";
            cin >> current->data.date;

            cout << "Masukkan Waktu baru (HH:MM)        : ";
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
    header();
    cout <<"#----------------------- BATAL JANJI TEMU -----------------------#"<<endl;
    cout << endl;
    
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

void examinationAndTreatment() {
    header();
    cout <<"#------------------- PEMERIKSAAN DAN PENGOBATAN ------------------#"<<endl;
    cout << endl;

    int choice;
    do {
        cout << "1. Tambah Hasil Pemeriksaan\n";
        cout << "2. Lihat Riwayat Pemeriksaan\n";
        cout << "3. Edit Hasil Pemeriksaan\n";
        cout << "4. Hapus Hasil Pemeriksaan\n";
        cout << "5. Undo Operasi Terakhir\n";
        cout << "0. Kembali\n";

        cout << endl;
        cout << "Pilih sub-menu: ";
        cin >> choice;

        switch (choice) {
            case 1: 
                addExaminationResult(); 
                break;
            case 2: 
                viewMedicalHistory(); 
                break;
            case 3: 
                editExaminationResult(); 
                break;
            case 4: 
                deleteExaminationResult(); 
                break;
            case 5:
                undoLastOperation();
                break;
            case 0: 
                mainMenu();
                break;
            default: 
                cout << "Pilihan tidak valid." << endl;
        }
    } while (choice != 6);
}

void addExaminationResult() {
    header();
    cout <<"#------------------ TAMBAH HASIL PEMERIKSAAN ------------------#"<<endl;
    cout << endl;

    Examination newExamination;

    newExamination.id = ++lastExaminationId;

    cout << "Masukkan ID Pasien     : ";
    cin >> newExamination.patientId;

    newExamination.patientName = getPatientNameById(patientHead, newExamination.patientId);
    cout << "Nama Pasien            : " << newExamination.patientName << endl;

    cout << "Masukkan ID dokter     : ";
    cin >> newExamination.doctorId;

    newExamination.doctorName = getDoctorNameById(doctorHead, newExamination.doctorId);
    cout << "Nama Dokter            : " << newExamination.doctorName << endl;

    newExamination.doctorspecialization = getDoctorSpecializationById(doctorHead, newExamination.doctorId);
    cout << "specialization         : " << newExamination.doctorspecialization << endl;

    cout << "Masukkan Diagnosis     : ";
    cin.ignore(); 
    getline(cin, newExamination.diagnosis); 

    cout << "Masukkan Pengobatan    : ";
    getline(cin, newExamination.treatment);

    addExamination(examinationHead, newExamination);
    saveExaminations(examinationHead);

    Operation op = {"add", newExamination};
    push(operationStack, op);

    cout << "Hasil pemeriksaan berhasil ditambahkan." << endl;

    int back;
    do {
        cout << endl;
        cout << "0.Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back != 0);

    system("cls");
    examinationAndTreatment();
}

void viewMedicalHistory() {
    header();
    cout <<"#----------------- RIWAYAT HASIL PEMERIKSAAN ------------------#"<<endl;
    cout << endl;

    int choice;
    cout << "1. Tampilkan Semua Pemeriksaan\n";
    cout << "2. Tampilkan Pemeriksaan Berdasarkan Spesialisasi\n";
    cout << "Pilih opsi: ";
    cin >> choice;

    switch (choice) {
        case 1:
            displayAllExaminations();
            break;
        case 2:
            buildSpecializationGraph();
            displaySpecializationGraph();
            break;
        default:
            cout << "Pilihan tidak valid." << endl;
    }

    int back;
    do {
        cout << endl;
        cout << "0. Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back != 0);

    system("cls");
    examinationAndTreatment();
}

void displayAllExaminations() {
    ExaminationNode* current = examinationHead;
    while (current != nullptr) {
        cout << "ID Pemeriksaan         : " << current->data.id << endl;
        cout << "ID Pasien              : " << current->data.patientId << endl;
        cout << "Nama Pasien            : " << current->data.patientName << endl;
        cout << "ID Dokter              : " << current->data.doctorId << endl;
        cout << "Nama Dokter            : " << current->data.doctorName << endl;
        cout << "Spesialisasi Dokter    : " << current->data.doctorspecialization << endl;
        cout << "Diagnosis              : " << current->data.diagnosis << endl;
        cout << "Pengobatan             : " << current->data.treatment << endl;
        cout << "-----------------------------" << endl;
        current = current->next;
    }
}

void displaySpecializationGraph() {
    SpecializationNode* current = specializationHead;
    int choice;
    int index = 1;

    // Display available specializations
    cout << "Daftar Spesialisasi Dokter:\n";
    while (current != nullptr) {
        cout << index++ << ". " << current->specialization << endl;
        current = current->next;
    }

    cout << "Pilih spesialisasi (nomor): ";
    cin >> choice;

    current = specializationHead;
    index = 1;

    // Find the chosen specialization
    while (current != nullptr && index < choice) {
        current = current->next;
        index++;
    }

    if (current == nullptr) {
        cout << "Pilihan tidak valid." << endl;
        return;
    }

    // Display examinations for the chosen specialization
    ExaminationNode* examNode = current->examinationList;
    while (examNode != nullptr) {
        cout << "ID Pemeriksaan         : " << examNode->data.id << endl;
        cout << "ID Pasien              : " << examNode->data.patientId << endl;
        cout << "Nama Pasien            : " << examNode->data.patientName << endl;
        cout << "ID Dokter              : " << examNode->data.doctorId << endl;
        cout << "Nama Dokter            : " << examNode->data.doctorName << endl;
        cout << "Spesialisasi Dokter    : " << examNode->data.doctorspecialization << endl;
        cout << "Diagnosis              : " << examNode->data.diagnosis << endl;
        cout << "Pengobatan             : " << examNode->data.treatment << endl;
        cout << "-----------------------------" << endl;
        examNode = examNode->next;
    }
}

void editExaminationResult() {
    header();
    cout <<"#----------------- EDIT HASIL PEMERIKSAAN ------------------#"<<endl;
    cout << endl;

    int id;
    cout << "Masukkan ID Pemeriksaan yang ingin diedit: ";
    cin >> id;

    ExaminationNode* current = examinationHead;
    while (current != nullptr) {
        if (current->data.id == id) {
            cout << "ID Pasien              : " << current->data.patientId << "\n";
            cout << "Nama Pasien            : " << current->data.patientName << "\n";
            cout << "ID Dokter              : " << current->data.doctorId << "\n";
            cout << "Nama Dokter            : " << current->data.doctorName << "\n";
            cout << "specialization Dokter  : " << current->data.doctorspecialization << endl;

            cout << endl;
            cout << "Masukkan Diagnosis Baru    : ";
            cin.ignore();
            getline(cin, current->data.diagnosis);

            cout << "Masukkan Pengobatan Baru   : ";
            getline(cin, current->data.treatment);

            cout << "Hasil pemeriksaan berhasil diperbarui." << endl;

            saveExaminations(examinationHead);
            return;
        }
        current = current->next;
    }
    cout << "Hasil pemeriksaan dengan ID " << id << " tidak ditemukan." << endl;

    int back;
    do {
        cout << endl;
        cout << "0.Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back != 0);

    system("cls");
    examinationAndTreatment();
}

void deleteExaminationResult() {
    header();
    cout <<"#---------------- HAPUS HASIL PEMERIKSAAN ----------------#"<<endl;
    cout << endl;

    int id;
    cout << "Masukkan ID Pemeriksaan yang ingin dihapus: ";
    cin >> id;

    ExaminationNode* current = examinationHead;
    ExaminationNode* prev = nullptr;

    while (current != nullptr && current->data.id != id) {
        prev = current;
        current = current->next;
    }

    if (current == nullptr) {
        cout << "Hasil pemeriksaan dengan ID " << id << " tidak ditemukan." << endl;
        return;
    }

    if (prev == nullptr) {
        examinationHead = current->next;
    } else {
        prev->next = current->next;
    }

    Operation op = {"delete", current->data};
    push(operationStack, op);

    delete current;
    cout << "Hasil pemeriksaan berhasil dihapus." << endl;

    saveExaminations(examinationHead);

    int back;
    do {
        cout << endl;
        cout << "0.Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back != 0);

    system("cls");
    examinationAndTreatment();
}

void undoLastOperation() {
    header();
    cout <<"#---------------------- UNDO OPERASI TERAKHIR ----------------------#"<<endl;
    cout << endl;

    try {
        Operation lastOp = pop(operationStack);

        if (lastOp.type == "add") {
            ExaminationNode* current = examinationHead;
            ExaminationNode* prev = nullptr;

            while (current != nullptr && current->data.id != lastOp.data.id) {
                prev = current;
                current = current->next;
            }

            if (current != nullptr) {
                if (prev == nullptr) {
                    examinationHead = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                cout << "Operasi 'Tambah Pemeriksaan' berhasil di-undo." << endl;
            }
        } else if (lastOp.type == "delete") {
            addExamination(examinationHead, lastOp.data);
            cout << "Operasi 'Hapus Pemeriksaan' berhasil di-undo." << endl;
        }
    } catch (const runtime_error& newExamination) {
        cout << "Tidak ada operasi yang dapat di-undo." << endl;
    }

    saveExaminations(examinationHead);

    int back;
    do {
        cout << endl;
        cout << "0.Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back != 0);

    system("cls");
    examinationAndTreatment();
}

void billingandPayment() {
    header();
    cout <<"#--------------------- BILLING & PEMBAYARAN ----------------------#"<<endl;
    cout << endl;
    
    int pilihan;
    do {
        cout << "1. Buat Tagihan Baru" << endl;
        cout << "2. Lihat Daftar Tagihan" << endl;
        cout << "3. Cari Tagihan" << endl;
        cout << "4. Edit Tagihan" << endl;
        cout << "5. Bayar Tagihan" << endl;
        cout << "0. Kembali" << endl;

        cout << endl;
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
            case 0:
                mainMenu();
                break;
            default:
                cout << "\nPilihan tidak valid. Silakan coba lagi." << endl;
        }
    } while (pilihan != 0);
}

void loadTagihanDariBill() {
    ifstream file("bill.txt");
    if (file.is_open()) {
        while (!file.eof()) {
            Tagihan tagihanBaru;
            file >> tagihanBaru.id;
            file.ignore();
            getline(file, tagihanBaru.namaPasien, ',');
            getline(file, tagihanBaru.layanan, ',');
            file >> tagihanBaru.jumlah;
            file.ignore();
            getline(file, tagihanBaru.tanggal);
            file >> tagihanBaru.sudahDibayar;

            if (!tagihanBaru.sudahDibayar) { 
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
            }
        }
        file.close();
    } else {
        cout << "Gagal membuka file untuk memuat data tagihan." << endl;
    }
}

void simpanKeBill() {
    ofstream file("bill.txt");
    if (file.is_open()) {
        Node* temp = head;
        while (temp != nullptr) {
            file << temp->data.id << ","
                 << temp->data.namaPasien << ","
                 << temp->data.layanan << ","
                 << temp->data.jumlah << ","
                 << temp->data.tanggal << endl;
            temp = temp->next;
        }
        file.close();
    } else {
        cout << "Gagal membuka file untuk menyimpan data." << endl;
    }
}

void buatTagihanBaru() {
    header();
    cout <<"#----------------------- BUAT TAGIHAN BARU -----------------------#"<<endl;
    cout << endl;

    Tagihan tagihanBaru;    
    cout << "Masukkan ID tagihan            : ";
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

    cout << "Masukkan nama pasien           : ";
    getline(cin, tagihanBaru.namaPasien);
    cout << "Masukkan layanan               : ";
    getline(cin, tagihanBaru.layanan);
    cout << "Masukkan jumlah tagihan        : ";
    cin >> tagihanBaru.jumlah;
    cin.ignore();
    cout << "Masukkan tanggal (YYYY-MM-DD)  : ";
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
    simpanKeBill();

    int back;
    do {
        cout << endl;
        cout << "0.Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back != 0);

    system("cls");
    billingandPayment();
}

void lihatDaftarTagihan() {
    header();
    cout <<"#---------------------- LIHAT DAFTAR TAGIHAN ---------------------#"<<endl;
    cout << endl;

    if (head == nullptr) {
        cout << "\nTidak ada tagihan yang tersimpan." << endl;
        
        int back;
        do {
            cout << endl;
            cout << "0. Kembali\n";
            cout << "Pilih opsi: ";
            cin >> back;
        } while (back != 0);

        system("cls");
        billingandPayment();    
    }

    Node* temp = head;
    if (temp == nullptr) {
        cout << "\nTidak ada tagihan yang ditemukan." << endl;
        return;
    }
    
    while (temp != nullptr) {
        Tagihan tagihanSaatIni = temp->data;
        cout << "ID             : " << tagihanSaatIni.id 
            << "\nNama Pasien   : " << tagihanSaatIni.namaPasien
            << "\nLayanan       : " << tagihanSaatIni.layanan 
            << "\nJumlah        : Rp" << tagihanSaatIni.jumlah
            << "\nTanggal       : " << tagihanSaatIni.tanggal << endl;
        cout << "----------------------------"<< endl;
        temp = temp->next;
    }

    int back;
    do {
        cout << endl;
        cout << "0.Kembali\n";
        cout << "Pilih opsi: ";
        cin >> back;
    } while (back != 0);

    system("cls");
    billingandPayment();
}

bool cariTagihanRekursif(Node* temp, int idCari, string namaCari) {
    if (temp == nullptr) {
        return false; 
    }

    if (temp->data.id == idCari || temp->data.namaPasien == namaCari) {
        cout << "\nTagihan ditemukan\t\t:" << endl;
        cout<< "\nID            : " << temp->data.id 
            << "\nNama Pasien   : " << temp->data.namaPasien
            << "\nLayanan       : " << temp->data.layanan 
            << "\nJumlah        : Rp" << temp->data.jumlah
            << "\nTanggal       : " << temp->data.tanggal << endl;
        return true; 
    }

    return cariTagihanRekursif(temp->next, idCari, namaCari);
}

void cariTagihan() {
    header();
    cout <<"#---------------- CARI TAGIHAN ----------------#"<<endl;
    cout << endl;

    if (head == nullptr) {
        cout << "\nTidak ada tagihan yang tersimpan untuk dicari." << endl;      
        
        int back;
        do {
            cout << endl;
            cout << "0. Kembali\n";
            cout << "Pilih opsi: ";
            cin >> back;
        } while (back != 0);

        system("cls");
        billingandPayment();    
    }

    int idCari;
    string namaCari;
    cout << "\nMasukkan ID tagihan yang ingin dicari\t: ";
    cin >> idCari;
    cin.ignore(); 

    bool ditemukan = cariTagihanRekursif(head, idCari, namaCari);
    if (!ditemukan) {
        cout << "Tagihan tidak ditemukan." << endl;
        
        int back;
        do {
            cout << endl;
            cout << "0. Kembali\n";
            cout << "Pilih opsi: ";
            cin >> back;
        } while (back != 0);

        system("cls");
        billingandPayment();  

    }

}

void editTagihan() {
    header();
    cout <<"#---------------- EDIT TAGIHAN  ----------------#"<<endl;
    cout << endl;

    if (head == nullptr) {
        cout << "\nTidak ada tagihan yang tersimpan untuk diedit." << endl;
        
        int back;
        do {
            cout << endl;
            cout << "0. Kembali\n";
            cout << "Pilih opsi: ";
            cin >> back;
        } while (back != 0);

        system("cls");
        billingandPayment();    
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
            cout << "Masukkan nama pasien baru  : ";
            cin.ignore(); 
            getline(cin, tagihanSaatIni.namaPasien);
            cout << "Masukkan layanan baru      : ";
            getline(cin, tagihanSaatIni.layanan);
            cout << "Masukkan jumlah baru       : ";
            cin >> tagihanSaatIni.jumlah;
            cin.ignore();
            cout << "Masukkan tanggal baru (YYYY-MM-DD) : ";
            getline(cin, tagihanSaatIni.tanggal);
            cout << "Tagihan berhasil diperbarui!" << endl;
            simpanKeBill();
            break;
        }
        temp = temp->next;
    }

    if (!ditemukan) {
        cout << "Tagihan tidak ditemukan." << endl;

        int back;
        do {
            cout << endl;
            cout << "0. Kembali\n";
            cout << "Pilih opsi: ";
            cin >> back;
        } while (back != 0);

        system("cls");
        billingandPayment();  
    }
}

void bayarTagihan() {
    header();
    cout <<"#-------------------- PEMBAYARAN --------------------#"<<endl;
    cout << endl;

    if (head == nullptr) {
        cout << "Tidak ada tagihan yang tersimpan." << endl;
        
        int back;
        do {
            cout << endl;
            cout << "0. Kembali\n";
            cout << "Pilih opsi: ";
            cin >> back;
        } while (back != 0);

        system("cls");
        billingandPayment();    
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
            cout << "Biaya Tagihan  : Rp" << temp->data.jumlah << endl;
            cout << "Nama           : " << temp->data.namaPasien << endl;

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
                simpanKeBill();
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
        int back;
        do {
            cout << endl;
            cout << "0. Kembali\n";
            cout << "Pilih opsi: ";
            cin >> back;
        } while (back != 0);

        system("cls");
        billingandPayment();  
    }
}
