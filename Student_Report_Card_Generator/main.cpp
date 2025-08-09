#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

const int NUM_SUBJECTS = 6;

class Student {
public:
    string name;
    int roll;
    string s_class;
    vector<int> marks;

    void input();
    void edit();
};

void Student::input() {
    cout << "\nEnter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Roll Number: ";
    cin >> roll;
    cout << "Enter Class: ";
    cin >> s_class;

    marks.clear();
    for (int i = 0; i < NUM_SUBJECTS; ++i) {
        int m;
        cout << "Enter marks for subject " << i + 1 << ": ";
        cin >> m;
        marks.push_back(m);
    }
}

void Student::edit() {
    cout << "\nEditing student " << name << " (Roll: " << roll << ")\n";
    input();
}

class ReportCard {
public:
    float total = 0, average = 0;
    char grade;

    void calculate(const Student& s);
    void display(const Student& s);
};

void ReportCard::calculate(const Student& s) {
    total = 0;
    for (int mark : s.marks)
        total += mark;

    average = total / s.marks.size();

    if (average >= 90) grade = 'A';
    else if (average >= 75) grade = 'B';
    else if (average >= 60) grade = 'C';
    else if (average >= 40) grade = 'D';
    else grade = 'F';
}

void ReportCard::display(const Student& s) {
    cout << "\n--- Report Card ---\n";
    cout << "Name   : " << s.name << endl;
    cout << "Roll   : " << s.roll << endl;
    cout << "Class  : " << s.s_class << endl;
    cout << "Marks  : ";
    for (int m : s.marks) cout << m << " ";
    cout << "\nTotal  : " << total;
    cout << "\nAverage: " << average;
    cout << "\nGrade  : " << grade << endl;
}

void saveAll(const vector<Student>& students) {
    ofstream file("students.txt");
    for (const auto& s : students) {
        file << s.name << "," << s.roll << "," << s.s_class;
        for (int m : s.marks) file << "," << m;
        file << "\n";
    }
    file.close();
}

vector<Student> loadAll() {
    vector<Student> students;
    ifstream file("students.txt");
    string line;
    while (getline(file, line)) {
        Student s;
        s.marks.clear();
        stringstream ss(line);
        string token;
        int field = 0;
        while (getline(ss, token, ',')) {
            if (field == 0) s.name = token;
            else if (field == 1) s.roll = stoi(token);
            else if (field == 2) s.s_class = token;
            else s.marks.push_back(stoi(token));
            field++;
        }
        students.push_back(s);
    }
    file.close();
    return students;
}

void exportToCSV(const vector<Student>& students) {
    ofstream file("students.csv");
    file << "Name,Roll,Class,";

    // Only 6 subjects
    for (int i = 1; i <= NUM_SUBJECTS; ++i) {
        file << "Subject" << i;
        if (i != NUM_SUBJECTS) file << ",";
    }

    file << ",Total,Average,Grade\n";

    for (const auto& s : students) {
        ReportCard r;
        r.calculate(s);

        file << s.name << "," << s.roll << "," << s.s_class << ",";

        for (int i = 0; i < NUM_SUBJECTS; ++i) {
            if (i < s.marks.size()) file << s.marks[i];
            file << (i < NUM_SUBJECTS - 1 ? "," : "");
        }

        file << "," << r.total << "," << r.average << "," << r.grade << "\n";
    }

    file.close();
    cout << "\n Student data exported to 'students.csv' successfully.\n";
}

int main() {
    vector<Student> students = loadAll();
    int choice;

    do {
        cout << "\nStudent Report Card Menu\n";
        cout << "1. Add Student\n";
        cout << "2. View All Students\n";
        cout << "3. Search by Roll Number\n";
        cout << "4. Edit Student\n";
        cout << "5. Delete Student\n";
        cout << "6. Export to Excel (CSV)\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            Student s;
            s.input();
            students.push_back(s);
            saveAll(students);
        }
        else if (choice == 2) {
            for (Student& s : students) {
                ReportCard r;
                r.calculate(s);
                r.display(s);
                cout << "-------------------------\n";
            }
        }
        else if (choice == 3) {
            int rno;
            cout << "Enter Roll Number to search: ";
            cin >> rno;
            bool found = false;
            for (Student& s : students) {
                if (s.roll == rno) {
                    ReportCard r;
                    r.calculate(s);
                    r.display(s);
                    found = true;
                }
            }
            if (!found) cout << "Student not found.\n";
        }
        else if (choice == 4) {
            int rno;
            cout << "Enter Roll Number to edit: ";
            cin >> rno;
            bool found = false;
            for (Student& s : students) {
                if (s.roll == rno) {
                    s.edit();
                    found = true;
                    break;
                }
            }
            if (found) saveAll(students);
            else cout << "Student not found.\n";
        }
        else if (choice == 5) {
            int rno;
            cout << "Enter Roll Number to delete: ";
            cin >> rno;
            auto it = remove_if(students.begin(), students.end(), [=](Student& s) { return s.roll == rno; });
            if (it != students.end()) {
                students.erase(it, students.end());
                saveAll(students);
                cout << "Student deleted.\n";
            } else {
                cout << "Student not found.\n";
            }
        }
        else if (choice == 6) {
            exportToCSV(students);
        }

    } while (choice != 7);

    return 0;
}
