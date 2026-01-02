#include <iostream>
#include <string>
#include <sstream>
using namespace std;

struct Student {
    string name;
    string id;
    float mark;s
};

float readFloat(const string& prompt) {
    string line;
    float value;
    while (true) {
        cout << prompt;
        getline(cin, line);
        stringstream ss(line);
        if (ss >> value && ss.eof()) return value;
        cout << "Invalid number. Try again.\n";
    }
}

int main() {
    Student s;

    cout << "Enter student name: ";
    getline(cin, s.name);

    cout << "Enter student ID: ";
    getline(cin, s.id);

    s.mark = readFloat("Enter student mark: ");

    cout << "\n--- Student Information ---\n";
    cout << "Name: " << s.name << endl;
    cout << "ID: " << s.id << endl;
    cout << "Mark: " << s.mark << endl;

    return 0;
}

