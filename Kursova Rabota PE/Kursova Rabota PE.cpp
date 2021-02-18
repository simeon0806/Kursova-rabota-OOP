#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <sstream>
#pragma warning(disable:4996) //for Visual Studio
#define MAX_ESTATES 5
using namespace std;
class Person {
private:
    string name;
    string EGN;
    string address;
    string city;
    string country;
public:
    Person(string* personalInfo);
    string getName();
    string getEGN();
    string getAddress();
    string getCity();
    string getCountry();
};

class Estate : public Person {
private:
    char* adr[MAX_ESTATES];
    int size;
public:
    Estate(string* personalInfo, string* estatesAdr, int size);
    friend ostream& operator<<(ostream& os, Estate& estate);
    friend ofstream& operator<<(ofstream& ofs, Estate& estate);
    void printEstates();
    bool livesOnSameAdr();
    static void loadDataFromFile(const char* fname, vector<Estate>& estateList);
    static void listOwnersByAddress(vector<Estate>& estateList);
    static void listEstatesByOwnerEGN(const char* egn, vector<Estate>& estateList);
    static void saveDataInFile(vector<Estate>& estateList);
    string getEstatesToString();
};

//Person конструктор
Person::Person(string* personalInfo) {
    this->name = personalInfo[0];
    this->EGN = personalInfo[1];
    this->address = personalInfo[2];
    this->city = personalInfo[3];
    this->country = personalInfo[4];
}
// Getters на класа Person 
string Person::getName() {
    return this->name;
}

string Person::getEGN() {
    return this->EGN;
}

string Person::getAddress() {
    return this->address;
}

string Person::getCity() {
    return this->city;
}

string Person::getCountry() {
    return this->country;
}

//Estate конструктор
Estate::Estate(string* personalInfo, string* estatesAdr, int size) : Person(personalInfo) {
    this->size = size;
    for (int i = 0; i < size; i++) {
        this->adr[i] = new char[estatesAdr[i].length() + 1];
        strcpy(this->adr[i], estatesAdr[i].c_str());
    }
}

// Извежда на конзолата всички недвижни имоте за личноста
void Estate::printEstates() {
    for (int i = 0; i < this->size; i++) {
        cout << " \t" << i + 1 << ". " << this->adr[i] << "\n";
    }
}

// Проверява дали личноста живее на неговия недвижен имот
bool Estate::livesOnSameAdr() {
    string personAdr = this->getAddress();
    for (auto adr : this->adr)
        if (strcmp(adr, personAdr.c_str()) == 0)
            return true;
    return false;
}

void Estate::loadDataFromFile(const char* fname, vector<Estate>& estateList) {
    ifstream fin(fname);
    if (!fin.is_open()) {
        cout << "Error opeing file!\n";
        exit(2);
    }
    string personalInfo[5], addresses[MAX_ESTATES];
    string line, word;
    int index;
    while (getline(fin, line)) {
        index = 0;
        stringstream spliter(line);
        while (getline(spliter, word, '|')) {
            if (index < 5) {
                personalInfo[index] = word;
            }
            else {
                addresses[index - 5] = word;
            }
            index++;
        }
        index -= 5;
        if (MAX_ESTATES < index)
            index = MAX_ESTATES;
        estateList.push_back(Estate(personalInfo, addresses, index));
    }
    fin.close();
}

void Estate::listOwnersByAddress(vector<Estate>& estateList) {
    for (Estate estate : estateList)
        if (estate.livesOnSameAdr())
            cout << estate;
}

void Estate::listEstatesByOwnerEGN(const char* egn, vector<Estate>& estateList) {
    bool found = false;
    for (Estate estate : estateList) {
        string selectedEGN = estate.getEGN();
        if (strcmp(egn, selectedEGN.c_str()) == 0) {
            cout << estate;
            found = true;
            break;
        }
    }
    if (!found)
        cout << "No results found for person with EGN: " << egn << "\n";
}

// Връща недвижните имоти като низа отедела с ,
string Estate::getEstatesToString() {
    string addresses;
    for (int i = 0; i < this->size; i++) {
        addresses.append(this->adr[i]);
        if (this->size - 1 != i)
            addresses.append(", ");
    }
    return addresses;
}

void Estate::saveDataInFile(vector<Estate>& estateList) {
    ofstream fout("output.txt");
    if (!fout.is_open()) {
        cout << "Error opeing file!\n";
        exit(3);
    }

    for (Estate var : estateList)
        fout << var;

    cout << "\nInformation is successfully write in file output.txt" << endl;

    fout.close();
}

// Предефиниране на << при извеждане на конзола от класа Estate
ostream& operator<<(ostream& os, Estate& estate) {
    os << "\n-------------\n"
        << "Name: " << estate.getName() << "\nEGN: " << estate.getEGN() << "\nAddress: " <<
        estate.getAddress() << ", " << estate.getCity() << ", " << estate.getCountry() << "\nEstate:\n";
    estate.printEstates();
    os << "-------------\n";
    return os;
}

// Предефиниране на << при записване в файл от класа Estate
ofstream& operator<<(ofstream& ofs, Estate& estate) {
    ofs << estate.getName() << " | " << estate.getEGN() << " | " << estate.getAddress() << " | " << estate.getCity() << " | "
        << estate.getCountry() << " | " << estate.getEstatesToString() << "\n";
    return ofs;
}

void clearScreen() {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("reset");
#endif
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}

int main() {
    vector<Estate> owners;
    int choice = 0, k;
    const char* fname = "data.txt";
    string egn;
    // Записва информацията за хората и тяхните имоти от data.txt
    Estate::loadDataFromFile(fname, owners);
    do
    {
        cout << " -------- MENU --------\n";
        cout << " 1. Display of information for persons whose address coincides with any of the addresses of the owned properties.\n";
        cout << " 2. Output of the owned properties by a person assigned with a EGN.\n";
        cout << " 3. Clear the screen.\n";
        cout << " 0. Exit\n";
        cout << "\nChoose\n";
        cout << "(0..3): ";
        do
        {
            k = scanf("%d", &choice);
            while (getchar() != '\n');
        } while (k == 0);
        if (k == 1)
        {
            switch (choice)
            {
            case 1:
                // Извежда информация за хората които живеят на сопствения недвижен имот
                Estate::listOwnersByAddress(owners);
                break;
            case 2:
                cout << "\nInput EGN: ";
                cin >> egn;
                //По зададено ЕГН, намира лицето и извежда информация за него
                Estate::listEstatesByOwnerEGN(egn.c_str(), owners);
                break;
            case 3:
                // Изтрижане на податоците в командната линия
                clearScreen();
                break;
            case 0:
                // При завършаване на приложението запазва информацията в output.txt 
                Estate::saveDataInFile(owners);
                owners.clear();
                break;
            default:
                cout << "\nInvalid option!\n";
            }
        }
    } while (choice != 0);
    return 0;
}