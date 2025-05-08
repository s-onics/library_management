#include <iostream>
#include <string>
#include <list>    
#include <stack>   
#include <queue>   
#include <exception>
#include <map>

using namespace std;

class Books;
class Student;
class Faculty;
class Member;
class Transaction;
class LibraryException;
class Library;

class LibraryException : public exception {
public:
    string message;
    
    LibraryException(string msg) : message(msg) {}
    const char* what() const throw() {
        return message.c_str();
    }
};

class BookNotFoundException : public LibraryException {
public:
    BookNotFoundException() : LibraryException("Book not found in the library database!") {}
};

class MemberNotFoundException : public LibraryException {
public:
    MemberNotFoundException() : LibraryException("Member ID not found in the library database!") {}
};

class BookNotAvailableException : public LibraryException {
public:
    BookNotAvailableException() : LibraryException("Book is not available for issue at the moment!") {}
};

class MaxIssueLimitException : public LibraryException {
public:
    MaxIssueLimitException(int limit) : LibraryException("Maximum issue limit of " + to_string(limit) + " books reached!") {}
};

class PendingFineException : public LibraryException {
public:
    PendingFineException(int amount) : LibraryException("Member has pending fine of Rs. " + to_string(amount) + ". Please clear before issuing new books!") {}
};

class InvalidOperationException : public LibraryException {
public:
    InvalidOperationException(string operation) : LibraryException("Invalid operation: " + operation) {}
};

class InvalidCredentialsException : public LibraryException {
public:
    InvalidCredentialsException() : LibraryException("Invalid librarian credentials!") {}
};

class Library {
public:
    string book_name;
    int book_Id;
    
    Library() : book_Id(0) {}
    Library(string bName, int bId) : book_name(bName), book_Id(bId) {}
};

class Transaction {
public:
    int transaction_Id;
    int book_Id;
    string member_Id;
    string book_name;
    int issue_date;  
    int due_date;    
    int return_date; 
    bool is_returned;
    
    Transaction(int t_id, int b_id, string m_id, string b_name) { 
        transaction_Id = t_id;
        book_Id = b_id;
        member_Id = m_id;
        book_name = b_name;
        issue_date = getCurrentDay();
        is_returned = false;
        due_date = issue_date + 14;
    }
    
    int getCurrentDay() {
        int day_counter = 1;
        return day_counter++;
    }
    
    void completeReturn() {
        return_date = getCurrentDay();
        is_returned = true;
    }
    
    int calculateLateDays() const {
        if (!is_returned) return 0;
        return (return_date > due_date) ? (return_date - due_date) : 0;
    }
    
    int calculateFine() const {
        return calculateLateDays() * 10;
    }
    
    void display() {
        cout << "\n------ Transaction Details ------" << endl;
        cout << "Transaction ID: " << transaction_Id << endl;
        cout << "Book ID: " << book_Id << endl;
        cout << "Book Name: " << book_name << endl;
        cout << "Member ID: " << member_Id << endl;
        cout << "Issue Date: Day " << issue_date << endl;
        cout << "Due Date: Day " << due_date << endl;
        
        if (is_returned) {
            cout << "Return Date: Day " << return_date << endl;
            int late_days = calculateLateDays();
            if (late_days > 0) {
                cout << "Late by: " << late_days << " days" << endl;
                cout << "Fine: Rs. " << calculateFine() << endl;
            } else {
                cout << "Returned on time" << endl;
            }
        } else {
            cout << "Status: Not Returned" << endl;
        }
        cout << "---------------------------------" << endl;
    }
};

class Books : public virtual Library {
public:
    string author_name;
    string publisher;
    int ISBN_no;
    int no_of_copies;
    int no_of_copies_issued;
    bool availability;
    vector<string> reserved_by;

    Books() : ISBN_no(0), no_of_copies(0), no_of_copies_issued(0), availability(true) {}
    
    Books(string bName, int bId, string author, string pub, int isbn, int copies) { 
        book_name = bName;
        book_Id = bId;
        author_name = author;
        publisher = pub;
        ISBN_no = isbn;
        no_of_copies = copies;
        no_of_copies_issued = 0;
        availability = true;
    }

    virtual void add_book() {
        cin.ignore();
        cout << "Enter Book Name: ";
        getline(cin, book_name);
        cout << "Enter Book Id: ";
        cin >> book_Id;
        cin.ignore();
        cout << "Enter Author Name: ";
        getline(cin, author_name);
        cout << "Enter ISBN no: ";
        cin >> ISBN_no;
        cin.ignore();
        cout << "Enter Publisher: ";
        getline(cin, publisher);
        cout << "Enter No of Copies: ";
        cin >> no_of_copies;
    }

    virtual void display() {
        cout << "\n------ Book Details ------" << endl;
        cout << "Book Name: " << book_name << endl;
        cout << "Book Id: " << book_Id << endl;
        cout << "Author Name: " << author_name << endl;
        cout << "Publisher: " << publisher << endl;
        cout << "ISBN No: " << ISBN_no << endl;
        cout << "Copies Available: " << (no_of_copies - no_of_copies_issued) << "/" << no_of_copies << endl;
        cout << "Status: " << (availability ? "Available" : "Not Available") << endl;
        
        if (!reserved_by.empty()) {
            cout << "Reserved by: ";
            for (const auto& member_id : reserved_by) {
                cout << member_id << " ";
            }
            cout << endl;
        }
        cout << "--------------------------" << endl;
    }

    void update_availability() {
        if (no_of_copies_issued >= no_of_copies) {
            availability = false;
        } else {
            availability = true;
        }
    }

    bool issueBook() {
        if (no_of_copies_issued < no_of_copies) {
            no_of_copies_issued++;
            update_availability();
            return true;
        }
        return false;
    }
    
    bool returnBook() {
        if (no_of_copies_issued > 0) {
            no_of_copies_issued--;
            update_availability();
            return true;
        }
        return false;
    }
    
    bool reserveBook(string member_id) {
        if (!availability) {
            for (const auto& id : reserved_by) {
                if (id == member_id) {
                    return false;
                }
            }
            reserved_by.push_back(member_id);
            return true;
        }
        return false;
    }
    
    bool isReservedBy(string member_id) {
        for (const auto& id : reserved_by) {
            if (id == member_id) {
                return true;
            }
        }
        return false;
    }
    
    void removeReservation(string member_id) {
        for (auto it = reserved_by.begin(); it != reserved_by.end(); ++it) {
            if (*it == member_id) {
                reserved_by.erase(it);
                break;
            }
        }
    }
};

class EBook : public virtual Books {
public:
    string format;
    string download_link;

    EBook() {}
    
    EBook(string bName, int bId, string author, string pub, int isbn, int copies, string fmt, string link) {
        book_name = bName;
        book_Id = bId;
        author_name = author;
        publisher = pub;
        ISBN_no = isbn;
        no_of_copies = copies;
        no_of_copies_issued = 0;
        availability = true;
        format = fmt;
        download_link = link;
    }
    
    void addEBook() {
        add_book();
        cout << "Enter Format (PDF/EPUB/etc): ";
        cin >> format;
        cout << "Enter Download Link: ";
        cin >> download_link;
    }
    
    void display() override {
        Books::display();
        cout << "E-Book Format: " << format << endl;
        cout << "Download Link: " << download_link << endl;
    }
};

class ResearchJournal : public virtual Books {
public:
    string journal_name;
    int volume;
    int issue;
    
    ResearchJournal() : volume(0), issue(0) {}
    
    ResearchJournal(string bName, int bId, string author, string pub, int isbn, int copies, 
                   string jName, int vol, int iss) {
        book_name = bName;
        book_Id = bId;
        author_name = author;
        publisher = pub;
        ISBN_no = isbn;
        no_of_copies = copies;
        no_of_copies_issued = 0;
        availability = true;
        journal_name = jName;
        volume = vol;
        issue = iss;
    }
    
    void addJournal() {
        add_book();
        cin.ignore();
        cout << "Enter Journal Name: ";
        getline(cin, journal_name);
        cout << "Enter Volume Number: ";
        cin >> volume;
        cout << "Enter Issue Number: ";
        cin >> issue;
    }
    
    void display() override {
        Books::display();
        cout << "Journal Name: " << journal_name << endl;
        cout << "Volume: " << volume << endl;
        cout << "Issue: " << issue << endl;
    }
};

class Member {
public:
    string name;
    string member_id;
    double contact;
    string email;
    string address;
    bool fine;
    int fine_amount;
    vector<int> issued_book_ids;

    Member() : contact(0), fine(false), fine_amount(0) {}
    
    Member(string n, string id, double c, string e, string addr) {
        name = n;
        member_id = id;
        contact = c;
        email = e;
        address = addr;
        fine = false;
        fine_amount = 0;
    }
    
    virtual void inputDetails() {
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Member ID: ";
        cin >> member_id;
        cout << "Enter Phone no: ";
        cin >> contact;
        cin.ignore();
        cout << "Enter Email Id: ";
        getline(cin, email);
        cout << "Enter Address: ";
        getline(cin, address);
    }

    virtual void display() {
        cout << "\n------ Member Details ------" << endl;
        cout << "Name: " << name << endl;
        cout << "Member ID: " << member_id << endl;
        cout << "Contact: " << contact << endl;
        cout << "Email: " << email << endl;
        cout << "Address: " << address << endl;
        if (fine) {
            cout << "Fine Amount: $" << fine_amount << endl;
        }
        
        if (!issued_book_ids.empty()) {
            cout << "Issued Books: ";
            for (const auto& book_id : issued_book_ids) {
                cout << book_id << " ";
            }
            cout << endl;
        }
        cout << "--------------------------" << endl;
    }
    
    virtual int getMaxBooks() const = 0;
    
    void addIssuedBook(int book_id) {
        issued_book_ids.push_back(book_id);
    }
    
    void removeIssuedBook(int book_id) {
        for (auto it = issued_book_ids.begin(); it != issued_book_ids.end(); ++it) {
            if (*it == book_id) {
                issued_book_ids.erase(it);
                break;
            }
        }
    }
    
    bool hasIssuedBook(int book_id) {
        for (const auto& id : issued_book_ids) {
            if (id == book_id) {
                return true;
            }
        }
        return false;
    }
    
    void addFine(int amount) {
        fine = true;
        fine_amount += amount;
    }
    
    void clearFine() {
        fine = false;
        fine_amount = 0;
    }
};

class Student : public virtual Member {
public:
    string department;
    string year;
    
    Student() {}
    
    Student(string n, string id, double c, string e, string a, string d, string y) {
        name = n;
        member_id = id;
        contact = c;
        email = e;
        address = a;
        department = d;
        year = y;
        fine = false;
        fine_amount = 0;
    }
    
    void inputDetails() override {
        Member::inputDetails();
        cin.ignore();
        cout << "Enter Department: ";
        getline(cin, department);
        cout << "Enter Year: ";
        getline(cin, year);
    }

    void display() override {
        Member::display();
        cout << "Department: " << department << endl;
        cout << "Year: " << year << endl;
    }
    
    int getMaxBooks() const override {
        return 3;
    }
};

class Faculty : public virtual Member {
public:
    string department;
    string designation;
    
    Faculty() {}
    
    Faculty(string n, string id, double c, string e, string a, string d, string des) {
        name = n;
        member_id = id;
        contact = c;
        email = e;
        address = a;
        department = d;
        designation = des;
        fine = false;
        fine_amount = 0;
    }
    
    void inputDetails() override {
        Member::inputDetails();
        cin.ignore();
        cout << "Enter Department: ";
        getline(cin, department);
        cout << "Enter Designation: ";
        getline(cin, designation);
    }

    void display() override {
        Member::display();
        cout << "Department: " << department << endl;
        cout << "Designation: " << designation << endl;
    }
    
    int getMaxBooks() const override {
        return 5;
    }
};

class Librarian {
public:
    string name;
    int staff_id;
    string email;
    string password;
    
    Librarian() : staff_id(0) {}
    
    Librarian(string n, int id, string e, string p) {
        name = n;
        staff_id = id;
        email = e;
        password = p;
    }
    
    void display() {
        cout << "\n------ Librarian Details ------" << endl;
        cout << "Name: " << name << endl;
        cout << "Staff ID: " << staff_id << endl;
        cout << "Email: " << email << endl;
        cout << "--------------------------" << endl;
    }
    
    bool authenticate(string inputPassword) {
        return password == inputPassword;
    }
};

class LibrarySystem {
public:
    list<Books*> book_collection;
    list<Member*> members;
    list<Librarian*> librarians;
    list<Transaction*> transactions;
    stack<Transaction*> recent_transactions;
    queue<pair<int, string>> reservation_queue;
    int transaction_counter;
    bool isLibrarianLoggedIn;
    string librarian_password;
    
    LibrarySystem() {
        transaction_counter = 1000;
        isLibrarianLoggedIn = false;
        librarian_password = "library123";
    }
    
    bool librarianLogin() {
        if (isLibrarianLoggedIn) {
            cout << "A librarian is already logged in!" << endl;
            return true;
        }
        
        string password;
        cout << "Enter librarian password: ";
        cin >> password;
        
        if (password == librarian_password) {
            isLibrarianLoggedIn = true;
            cout << "Login successful!" << endl;
            return true;
        } else {
            cout << "Invalid password!" << endl;
            return false;
        }
    }
    
    void librarianLogout() {
        if (isLibrarianLoggedIn) {
            isLibrarianLoggedIn = false;
            cout << "Librarian logged out successfully!" << endl;
        } else {
            cout << "No librarian is currently logged in!" << endl;
        }
    }
    
    bool checkLibrarianAccess() {
        return isLibrarianLoggedIn;
    }
    
    void addBook(Books* book) {
        for (auto existing_book : book_collection) {
            if (existing_book->book_Id == book->book_Id) {
                throw InvalidOperationException("Book with ID " + to_string(book->book_Id) + " already exists!");
            }
        }
        book_collection.push_back(book);
        cout << "Book added successfully!" << endl;
    }
    
    void addMember(Member* member) {
        members.push_back(member);
        cout << "Member added successfully!" << endl;
    }
    
    void addLibrarian(Librarian* librarian) {
        librarians.push_back(librarian);
        cout << "Librarian added successfully!" << endl;
    }
    
    void displayBooks() {
        cout << "\n======= BOOKS CATALOG =======" << endl;
        if (book_collection.empty()) {
            cout << "No books available in the library." << endl;
            return;
        }
        
        for (const auto& book : book_collection) {
            book->display();
        }
    }
    
    void displayMembers() {
        cout << "\n======= LIBRARY MEMBERS =======" << endl;
        if (members.empty()) {
            cout << "No members registered in the library." << endl;
            return;
        }
        
        for (const auto& member : members) {
            member->display();
        }
    }
    
    void displayTransactions() {
        cout << "\n======= TRANSACTION HISTORY =======" << endl;
        if (transactions.empty()) {
            cout << "No transactions recorded." << endl;
            return;
        }
        
        for (const auto& transaction : transactions) {
            transaction->display();
        }
    }
    
    Books* findBook(int book_id) {
        for (auto& book : book_collection) {
            if (book->book_Id == book_id) {
                return book;
            }
        }
        return nullptr;
    }
    
    Member* findMember(string member_id) {
        for (auto& member : members) {
            if (member->member_id == member_id) {
                return member;
            }
        }
        return nullptr;
    }
    
    void sortBooksById() {
        book_collection.sort([](Books* a, Books* b) { return a->book_Id < b->book_Id; });
    }
    
    void sortBooksByTitle() {
        book_collection.sort([](Books* a, Books* b) { return a->book_name < b->book_name; });
    }
    
    void sortMembersById() {
        members.sort([](Member* a, Member* b) { return a->member_id < b->member_id; });
    }
    
    void issueBook(int book_id, string member_id) {
        Books* book = findBook(book_id);
        Member* member = findMember(member_id);
        
        try {
            if (!book) {
                throw BookNotFoundException();
            }
            
            if (!member) {
                throw MemberNotFoundException();
            }
            
            if (!book->availability) {
                if (book->isReservedBy(member_id)) {
                    book->removeReservation(member_id);
                    
                    queue<pair<int, string>> temp_queue;
                    while (!reservation_queue.empty()) {
                        auto reservation = reservation_queue.front();
                        reservation_queue.pop();
                        
                        if (!(reservation.first == book_id && reservation.second == member_id)) {
                            temp_queue.push(reservation);
                        }
                    }
                    
                    reservation_queue = temp_queue;
                } else {
                    throw BookNotAvailableException();
                }
            }
            
            if (member->issued_book_ids.size() >= member->getMaxBooks()) {
                throw MaxIssueLimitException(member->getMaxBooks());
            }
            
            if (member->fine) {
                throw PendingFineException(member->fine_amount);
            }
            
            if (book->issueBook()) {
                member->addIssuedBook(book_id);
                
                Transaction* transaction = new Transaction(transaction_counter++, book_id, member_id, book->book_name);
                transactions.push_back(transaction);
                
                recent_transactions.push(transaction);
                
                cout << "Book issued successfully! Transaction ID: " << transaction->transaction_Id << endl;
                cout << "Due Date: Day " << transaction->due_date << endl;
            } else {
                throw InvalidOperationException("Failed to issue book due to unknown error!");
            }
        }
        catch (const LibraryException& e) {
            cout << "Exception during book issue process: " << e.what() << endl;
            throw;
        }
    }
    
    void returnBook(int book_id, string member_id) {
        try {
            Books* book = findBook(book_id);
            Member* member = findMember(member_id);
            
            if (!book) {
                throw BookNotFoundException();
            }
            
            if (!member) {
                throw MemberNotFoundException();
            }
            
            if (!member->hasIssuedBook(book_id)) {
                throw InvalidOperationException("This book was not issued to this member!");
            }
            
            if (book->returnBook()) {
                member->removeIssuedBook(book_id);
                
                Transaction* transaction = nullptr;
                for (auto& t : transactions) {
                    if (t->book_Id == book_id && t->member_Id == member_id && !t->is_returned) {
                        transaction = t;
                        break;
                    }
                }
                
                if (transaction) {
                    transaction->completeReturn();
                    
                    recent_transactions.push(transaction);
                    
                    int late_days = transaction->calculateLateDays();
                    if (late_days > 0) {
                        int fine_amount = transaction->calculateFine();
                        member->addFine(fine_amount);
                        
                        cout << "Book returned late by " << late_days << " days!" << endl;
                        cout << "Fine imposed: Rs. " << fine_amount << endl;
                    } else {
                        cout << "Book returned successfully and on time!" << endl;
                    }
                    
                    bool reservation_processed = false;
                    queue<pair<int, string>> temp_queue;
                    
                    while (!reservation_queue.empty() && !reservation_processed) {
                        auto reservation = reservation_queue.front();
                        reservation_queue.pop();
                        
                        if (reservation.first == book_id) {
                            cout << "Notification: Book is now available for member " << reservation.second << " who had reserved it." << endl;
                            reservation_processed = true;
                        } else {
                            temp_queue.push(reservation);
                        }
                    }
                    
                    while (!reservation_queue.empty()) {
                        temp_queue.push(reservation_queue.front());
                        reservation_queue.pop();
                    }
                    reservation_queue = temp_queue;
                    
                } else {
                    cout << "Book returned successfully, but transaction record not found!" << endl;
                }
            } else {
                throw InvalidOperationException("Failed to return book due to system error!");
            }
        } 
        catch (const LibraryException& e) {
            cout << "Exception during book return process: " << e.what() << endl;
            throw;
        }
    }
    
    void reserveBook(int book_id) {
        try {
            string member_id;
            cout << "Enter Member ID who wants to reserve: ";
            cin >> member_id;
            
            Books* book = findBook(book_id);
            Member* member = findMember(member_id);
            
            if (!book) {
                throw BookNotFoundException();
            }
            
            if (!member) {
                throw MemberNotFoundException();
            }
            
            if (book->availability) {
                cout << "Book is currently available! No need to reserve." << endl;
                return;
            }
            
            if (book->reserveBook(member_id)) {
                reservation_queue.push(make_pair(book_id, member_id));
                cout << "Book reserved successfully!" << endl;
                cout << "You are position " << book->reserved_by.size() << " in the queue." << endl;
            } else {
                cout << "Book already reserved by this member or cannot be reserved!" << endl;
            }
        }
        catch (const LibraryException& e) {
            cout << "Exception during book reservation process: " << e.what() << endl;
        }
    }
    
    void clearFine(string member_id) {
        try {
            Member* member = findMember(member_id);
            
            if (!member) {
                throw MemberNotFoundException();
            }
            
            if (!member->fine) {
                cout << "No fines pending for this member!" << endl;
                return;
            }
            
            cout << "Fine amount to be paid: Rs. " << member->fine_amount << endl;
            cout << "Confirm payment (Y/N): ";
            char choice;
            cin >> choice;
            
            if (toupper(choice) == 'Y') {
                member->clearFine();
                cout << "Fine cleared successfully!" << endl;
            } else {
                cout << "Payment canceled!" << endl;
            }
        }
        catch (const LibraryException& e) {
            cout << "Exception during fine clearance process: " << e.what() << endl;
        }
    }
    
    void generateReport() {
        int total_books = book_collection.size();
        int books_issued = 0;
        int total_members = members.size();
        int total_transactions = transactions.size();
        int active_transactions = 0;
        int completed_transactions = 0;
        int total_fines = 0;
        int total_reservations = reservation_queue.size();
        
        for (const auto& book : book_collection) {
            books_issued += book->no_of_copies_issued;
        }
        
        for (const auto& transaction : transactions) {
            if (transaction->is_returned) {
                completed_transactions++;
            } else {
                active_transactions++;
            }
        }
        
        for (const auto& member : members) {
            if (member->fine) {
                total_fines += member->fine_amount;
            }
        }
        
        cout << "\n======= LIBRARY SYSTEM REPORT =======" << endl;
        cout << "Total Books in Library: " << total_books << endl;
        cout << "Books Currently Issued: " << books_issued << endl;
        cout << "Total Members: " << total_members << endl;
        cout << "Total Transactions: " << total_transactions << endl;
        cout << "Active Issues: " << active_transactions << endl;
        cout << "Completed Returns: " << completed_transactions << endl;
        cout << "Total Pending Fines: Rs. " << total_fines << endl;
        cout << "Current Reservations: " << total_reservations << endl;
        cout << "=======================================\n" << endl;
    }
            
    void addSampleData() {
        Books* book1 = new Books();
        book1->book_name = "Introduction to C++";
        book1->book_Id = 101;
        book1->author_name = "Bjarne Stroustrup";
        book1->publisher = "Addison-Wesley";
        book1->ISBN_no = 1234567890;
        book1->no_of_copies = 5;
        book1->no_of_copies_issued = 0;
        book1->availability = true;
        addBook(book1);
        
        Books* book2 = new Books();
        book2->book_name = "Data Structures and Algorithms";
        book2->book_Id = 102;
        book2->author_name = "Thomas Cormen";
        book2->publisher = "MIT Press";
        book2->ISBN_no = 2345678901;
        book2->no_of_copies = 3;
        book2->no_of_copies_issued = 0;
        book2->availability = true;
        addBook(book2);
        
        EBook* ebook1 = new EBook();
        ebook1->book_name = "Python for Data Science";
        ebook1->book_Id = 103;
        ebook1->author_name = "Jake VanderPlas";
        ebook1->publisher = "O'Reilly";
        ebook1->ISBN_no = 3456789012;
        ebook1->no_of_copies = 10;
        ebook1->no_of_copies_issued = 0;
        ebook1->availability = true;
        ebook1->format = "PDF";
        ebook1->download_link = "library.com/ebooks/python_ds.pdf";
        addBook(ebook1);
        
        ResearchJournal* journal1 = new ResearchJournal();
        journal1->book_name = "Journal of Computer Science";
        journal1->book_Id = 104;
        journal1->author_name = "Various";
        journal1->publisher = "IEEE";
        journal1->ISBN_no = 1067890123;
        journal1->no_of_copies = 2;
        journal1->no_of_copies_issued = 0;
        journal1->availability = true;
        journal1->journal_name = "IEEE Transactions on Software Engineering";
        journal1->volume = 45;
        journal1->issue = 3;
        addBook(journal1);
        
        Student* student1 = new Student();
        student1->name = "Prabuddha Saxena";
        student1->member_id = "S001";
        student1->contact = 9479979748;
        student1->email = "jprabuddha02.04@gmail.com";
        student1->address = "123 Student Ave";
        student1->department = "Computer Science";
        student1->year = "1st";
        student1->fine = false;
        student1->fine_amount = 0;
        addMember(student1);
        
        Faculty* faculty1 = new Faculty();
        faculty1->name = "Dr. Sarah Johnson";
        faculty1->member_id = "F001";
        faculty1->contact = 8765432109;
        faculty1->email = "sarah@example.com";
        faculty1->address = "456 Faculty Blvd";
        faculty1->department = "Computer Science";
        faculty1->designation = "Professor";
        faculty1->fine = false;
        faculty1->fine_amount = 0;
        addMember(faculty1);
        
        Librarian* librarian1 = new Librarian();
        librarian1->name = "Mark Wilson";
        librarian1->staff_id = 3001;
        librarian1->email = "mark@library.com";
        librarian1->password = "library123";
        addLibrarian(librarian1);
    }
    
    void run() {
        int choice = 0;
        
        addSampleData();
        
        while (true) {
            if (!isLibrarianLoggedIn) {
                cout << "\n===== LIBRARY MANAGEMENT SYSTEM LOGIN =====" << endl;
                cout << "1. Login as Librarian" << endl;
                cout << "2. Exit" << endl;
                cout << "Enter your choice: ";
                cin >> choice;
                
                switch (choice) {
                    case 1:
                        if (librarianLogin()) {
                            break;
                        } else {
                            continue;
                        }
                    case 2:
                        cout << "Thank you for using Library Management System!" << endl;
                        return;
                    default:
                        cout << "Invalid choice! Please try again." << endl;
                        continue;
                }
            }
            
            cout << "\n===== SMART LIBRARY MANAGEMENT SYSTEM =====" << endl;
            cout << "1. Display Books" << endl;
            cout << "2. Display Members" << endl;
            cout << "3. Issue Book" << endl;
            cout << "4. Return Book" << endl;
            cout << "5. Reserve Book" << endl;
            cout << "6. Clear Fine" << endl;
            cout << "7. Display Transactions" << endl;
            cout << "8. Generate Report" << endl;
            cout << "9. Add New Book/Member" << endl;
            cout << "10. Logout" << endl;
            cout << "11. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            
            switch (choice) {
                case 1:
                    displayBooks();
                    break;
                case 2:
                    displayMembers();
                    break;
                case 3: {
                    int book_id;
                    string member_id;
                    cout << "Enter Book ID: ";
                    cin >> book_id;
                    cout << "Enter Member ID: ";
                    cin >> member_id;
                    try {
                        issueBook(book_id, member_id);
                    } catch (const LibraryException& e) {
                        cout << "Error: " << e.what() << endl;
                    }
                    break;
                }
                case 4: {
                    int book_id;
                    string member_id;
                    cout << "Enter Book ID: ";
                    cin >> book_id;
                    cout << "Enter Member ID: ";
                    cin >> member_id;
                    try {
                        returnBook(book_id, member_id);
                    } catch (const LibraryException& e) {
                        cout << "Error: " << e.what() << endl;
                    }
                    break;
                }
                case 5: {
                    int book_id;
                    cout << "Enter Book ID to reserve: ";
                    cin >> book_id;
                    reserveBook(book_id);
                    break;
                }
                case 6: {
                    string member_id;
                    cout << "Enter Member ID: ";
                    cin >> member_id;
                    clearFine(member_id);
                    break;
                }
                case 7:
                    displayTransactions();
                    break;
                case 8:
                    generateReport();
                    break;
                case 9: {
                    int sub_choice;
                    cout << "\n1. Add Book" << endl;
                    cout << "2. Add E-Book" << endl;
                    cout << "3. Add Research Journal" << endl;
                    cout << "4. Add Student" << endl;
                    cout << "5. Add Faculty" << endl;
                    cout << "Enter choice: ";
                    cin >> sub_choice;
                    
                    switch (sub_choice) {
                        case 1: {
                            Books* new_book = new Books();
                            new_book->add_book();
                            try {
                                addBook(new_book);
                            } catch (const LibraryException& e) {
                                cout << "Error: " << e.what() << endl;
                                delete new_book;
                            }
                            break;
                        }
                        case 2: {
                            EBook* new_ebook = new EBook();
                            new_ebook->addEBook();
                            try {
                                addBook(new_ebook);
                            } catch (const LibraryException& e) {
                                cout << "Error: " << e.what() << endl;
                                delete new_ebook;
                            }
                            break;
                        }
                        case 3: {
                            ResearchJournal* new_journal = new ResearchJournal();
                            new_journal->addJournal();
                            try {
                                addBook(new_journal);
                            } catch (const LibraryException& e) {
                                cout << "Error: " << e.what() << endl;
                                delete new_journal;
                            }
                            break;
                        }
                        case 4: {
                            Student* new_student = new Student();
                            new_student->inputDetails();
                            addMember(new_student);
                            break;
                        }
                        case 5: {
                            Faculty* new_faculty = new Faculty();
                            new_faculty->inputDetails();
                            addMember(new_faculty);
                            break;
                        }
                        default:
                            cout << "Invalid choice!" << endl;
                    }
                    break;
                }
                case 10:
                    librarianLogout();
                    break;
                case 11:
                    cout << "Thank you for using Smart Library Management System!" << endl;
                    return;
                default:
                    cout << "Invalid choice! Please try again." << endl;
            }
        }
    }
};

int main() {
    LibrarySystem library;
    library.run();
    return 0;
}