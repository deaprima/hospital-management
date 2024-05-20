#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// ########################################################## STRUKTUR

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


// ########################################################## DEKLARASI FUNGSI
void header();
void mainMenu();

// ########################################################## MAIN PROGRAM
int main() {
    mainMenu();
    return 0;
}

// ########################################################## FUNGSI-FUNGSI
void header(){
    system("cls");
	cout<<"#=================================================================#"<<endl;
	cout<<"#--------------------- RUMAH SAKIT SEJAHTERA ---------------------#"<<endl;
	cout<<"#=================================================================#"<<endl;
    cout<<endl;
}

void mainMenu() {
    header();

    int choice;
    do {
        cout << "Main Menu:\n";
        cout << "1. Pendaftaran Pasien\n";
        cout << "2. Manajemen Dokter\n";
        cout << "3. Penjadwalan Janji Temu\n";
        cout << "4. Pemeriksaan dan Pengobatan\n";
        cout << "5. Billing dan Pembayaran\n";
        cout << "6. Laporan dan Analisis\n";
        cout << "7. Keluar\n";
        cout << "Pilih menu: ";
        cin >> choice;

        switch (choice) {
            case 1: /*Pendaftaran Pasien*/; break;
            case 2: /*Manajemen Dokter*/; break;
            case 3: /*Penjadwalan Janji Temu*/; break;
            case 4: /*Pemeriksaan dan pengobatan*/; break;
            case 5: /*Billing dan pembayaran*/; break;
            case 6: /*laporan*/; break;
            case 7: cout << "Keluar dari program.\n"; break;
            default: cout << "Pilihan tidak valid.\n";
        }
    } while (choice != 7);
}