#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

/* ---------- STRUCTURES ---------- */

struct Student {
    int roll;
    string name;
    int marks;
    string course;
};

struct Attendance {
    int roll;
    string date;
    bool present;
};

struct Book {
    int id;
    string title;
    string author;
    bool issued;
};

struct Record {
    int roll;
    string name;
    int marks;
    int bookId;
    string bookTitle;
    string issueDate;
    string returnDate;
};

/* ---------- GLOBAL DATA ---------- */

vector<Student> students;
vector<Attendance> attendanceList;
vector<Book> books;
vector<Record> history;

/* ---------- FILE NAMES ---------- */

const string STUDENT_FILE = "students.txt";
const string ATTEND_FILE  = "attendance.txt";
const string BOOK_FILE    = "books.txt";
const string RECORD_FILE  = "records.txt";

/* ---------- GRADE FUNCTION ---------- */

string calculateGrade(int marks) {
    if (marks >= 85) return "A";
    else if (marks >= 70) return "B";
    else if (marks >= 55) return "C";
    else if (marks >= 40) return "D";
    else return "F";
}

/* ---------- FILE HANDLING ---------- */

void loadStudents() {
    ifstream file(STUDENT_FILE);
    Student s;
    while (file >> s.roll) {
        file.ignore();
        getline(file, s.name);
        file >> s.marks;
        file.ignore();
        getline(file, s.course);
        students.push_back(s);
    }
}

void saveStudents() {
    ofstream file(STUDENT_FILE);
    for (auto s : students)
        file << s.roll << endl
             << s.name << endl
             << s.marks << endl
             << s.course << endl;
}

void loadAttendance() {
    ifstream file(ATTEND_FILE);
    Attendance a;
    while (file >> a.roll >> a.date >> a.present)
        attendanceList.push_back(a);
}

void saveAttendance() {
    ofstream file(ATTEND_FILE);
    for (auto a : attendanceList)
        file << a.roll << " " << a.date << " " << a.present << endl;
}

void loadBooks() {
    ifstream file(BOOK_FILE);
    Book b;
    while (file >> b.id) {
        file.ignore();
        getline(file, b.title);
        getline(file, b.author);
        file >> b.issued;
        books.push_back(b);
    }
}

void saveBooks() {
    ofstream file(BOOK_FILE);
    for (auto b : books)
        file << b.id << endl
             << b.title << endl
             << b.author << endl
             << b.issued << endl;
}

void loadRecords() {
    ifstream file(RECORD_FILE);
    Record r;
    while (file >> r.roll) {
        file.ignore();
        getline(file, r.name);
        file >> r.marks;
        file >> r.bookId;
        file.ignore();
        getline(file, r.bookTitle);
        getline(file, r.issueDate);
        getline(file, r.returnDate);
        history.push_back(r);
    }
}

void saveRecords() {
    ofstream file(RECORD_FILE);
    for (auto r : history)
        file << r.roll << endl
             << r.name << endl
             << r.marks << endl
             << r.bookId << endl
             << r.bookTitle << endl
             << r.issueDate << endl
             << r.returnDate << endl;
}

/* ---------- STUDENT ---------- */

void addStudent() {
    Student s;
    cout << "Roll No: ";
    cin >> s.roll;
    cin.ignore();

    cout << "Name: ";
    getline(cin, s.name);

    cout << "Marks: ";
    cin >> s.marks;
    cin.ignore();

    cout << "Course: ";
    getline(cin, s.course);

    students.push_back(s);
    saveStudents();
    cout << "Student added successfully.\n";
}

void showStudents() {
    cout << left << setw(8) << "ROLL"
         << setw(20) << "NAME"
         << setw(8) << "MARKS"
         << setw(8) << "GRADE"
         << "COURSE\n";
    cout << string(60, '-') << endl;

    for (auto s : students)
        cout << setw(8) << s.roll
             << setw(20) << s.name
             << setw(8) << s.marks
             << setw(8) << calculateGrade(s.marks)
             << s.course << endl;
}

/* ---------- ATTENDANCE ---------- */

void markAttendance() {
    Attendance a;
    cout << "Roll No: ";
    cin >> a.roll;
    cout << "Date (DD/MM/YYYY): ";
    cin >> a.date;
    cout << "Present (1=yes, 0=no): ";
    cin >> a.present;

    attendanceList.push_back(a);
    saveAttendance();
    cout << "Attendance saved.\n";
}

void showAttendance() {
    int roll;
    cout << "Enter Roll No: ";
    cin >> roll;

    cout << left << setw(12) << "DATE" << "STATUS\n";
    cout << string(25, '-') << endl;

    for (auto a : attendanceList)
        if (a.roll == roll)
            cout << setw(12) << a.date
                 << (a.present ? "Present" : "Absent") << endl;
}

/* ---------- LIBRARY ---------- */

void addBook() {
    Book b;
    b.id = books.empty() ? 1001 : books.back().id + 1;
    cin.ignore();

    cout << "Book Title: ";
    getline(cin, b.title);
    cout << "Author: ";
    getline(cin, b.author);

    b.issued = false;
    books.push_back(b);
    saveBooks();
    cout << "Book added successfully.\n";
}

void showBooks() {
    cout << left << setw(8) << "ID"
         << setw(25) << "TITLE"
         << setw(20) << "AUTHOR"
         << "STATUS\n";
    cout << string(65, '-') << endl;

    for (auto b : books)
        cout << setw(8) << b.id
             << setw(25) << b.title
             << setw(20) << b.author
             << (b.issued ? "Issued" : "Available") << endl;
}

void issueBook() {
    int id, roll;
    cout << "Enter Book ID: ";
    cin >> id;

    for (auto &b : books) {
        if (b.id == id && !b.issued) {
            Record r;
            cin.ignore();

            cout << "Student Roll No: ";
            cin >> roll;

            for (auto s : students) {
                if (s.roll == roll) {
                    r.roll = s.roll;
                    r.name = s.name;
                    r.marks = s.marks;
                    break;
                }
            }

            cout << "Issue Date: ";
            cin >> r.issueDate;

            r.bookId = b.id;
            r.bookTitle = b.title;
            r.returnDate = "Not Returned";

            history.push_back(r);
            b.issued = true;

            saveBooks();
            saveRecords();
            cout << "Book issued successfully.\n";
            return;
        }
    }
    cout << "Book not available.\n";
}

void returnBook() {
    int id;
    cout << "Enter Book ID: ";
    cin >> id;

    for (auto &b : books) {
        if (b.id == id && b.issued) {
            for (auto &r : history) {
                if (r.bookId == id && r.returnDate == "Not Returned") {
                    cout << "Return Date: ";
                    cin >> r.returnDate;
                    break;
                }
            }
            b.issued = false;
            saveBooks();
            saveRecords();
            cout << "Book returned successfully.\n";
            return;
        }
    }
    cout << "Invalid Book ID.\n";
}

void showIssuedBooksWithStudents() {
    cout << left << setw(8) << "BID"
         << setw(20) << "BOOK"
         << setw(18) << "STUDENT"
         << setw(8) << "ROLL"
         << setw(8) << "GRADE"
         << setw(12) << "ISSUED"
         << "STATUS\n";
    cout << string(85, '-') << endl;

    for (auto r : history)
        cout << setw(8) << r.bookId
             << setw(20) << r.bookTitle
             << setw(18) << r.name
             << setw(8) << r.roll
             << setw(8) << calculateGrade(r.marks)
             << setw(12) << r.issueDate
             << r.returnDate << endl;
}

/* ---------- MAIN ---------- */

int main() {
    loadStudents();
    loadAttendance();
    loadBooks();
    loadRecords();

    int choice;

    while (true) {
        cout << "\n===== STUDENT + ATTENDANCE + LIBRARY SYSTEM =====\n";
        cout << "1. Add Student\n2. Show Students\n3. Mark Attendance\n";
        cout << "4. View Attendance\n5. Add Book\n6. Show Books\n";
        cout << "7. Issue Book\n8. Return Book\n";
        cout << "9. Show Issued Books with Students\n10. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 10) break;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: showStudents(); break;
            case 3: markAttendance(); break;
            case 4: showAttendance(); break;
            case 5: addBook(); break;
            case 6: showBooks(); break;
            case 7: issueBook(); break;
            case 8: returnBook(); break;
            case 9: showIssuedBooksWithStudents(); break;
            default: cout << "Invalid choice.\n";
        }
    }

    cout << "\nThank you for using the system!\n";
    return 0;
}
