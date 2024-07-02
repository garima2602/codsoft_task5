#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

class Book {
public:
    std::string title, author, ISBN;
    bool available;
    Book(std::string t, std::string a, std::string i) : title(t), author(a), ISBN(i), available(true) {}
};

class Borrower {
public:
    std::string name;
    std::vector<Book*> borrowedBooks;
    Borrower(std::string n) : name(n) {}
};

class Library {
private:
    std::vector<Book> books;
    std::vector<Borrower> borrowers;

    int findBook(std::string searchTerm) {
        for (size_t i = 0; i < books.size(); i++) {
            if (books[i].title == searchTerm || books[i].author == searchTerm || books[i].ISBN == searchTerm) {
                return i;
            }
        }
        return -1;
    }

public:
    void addBook(std::string title, std::string author, std::string ISBN) {
        books.push_back(Book(title, author, ISBN));
    }

    void searchBook() {
        std::string searchTerm;
        std::cout << "Enter title, author, or ISBN to search: ";
        std::cin.ignore();
        std::getline(std::cin, searchTerm);

        int index = findBook(searchTerm);
        if (index != -1) {
            std::cout << "Book found: " << books[index].title << " by " << books[index].author << "\n";
            std::cout << "ISBN: " << books[index].ISBN << "\n";
            std::cout << "Status: " << (books[index].available ? "Available" : "Borrowed") << "\n";
        } else {
            std::cout << "Book not found.\n";
        }
    }

    void checkoutBook() {
        std::string bookTitle, borrowerName;
        std::cout << "Enter book title: ";
        std::cin.ignore();
        std::getline(std::cin, bookTitle);

        int index = findBook(bookTitle);
        if (index != -1 && books[index].available) {
            std::cout << "Enter borrower name: ";
            std::getline(std::cin, borrowerName);

            auto it = std::find_if(borrowers.begin(), borrowers.end(),
                [&borrowerName](const Borrower& b) { return b.name == borrowerName; });

            if (it == borrowers.end()) {
                borrowers.push_back(Borrower(borrowerName));
                it = borrowers.end() - 1;
            }

            it->borrowedBooks.push_back(&books[index]);
            books[index].available = false;
            std::cout << "Book checked out successfully.\n";
        } else {
            std::cout << "Book not available for checkout.\n";
        }
    }

    void returnBook() {
        std::string bookTitle;
        std::cout << "Enter book title to return: ";
        std::cin.ignore();
        std::getline(std::cin, bookTitle);

        int index = findBook(bookTitle);
        if (index != -1 && !books[index].available) {
            books[index].available = true;
            for (auto& borrower : borrowers) {
                auto it = std::find_if(borrower.borrowedBooks.begin(), borrower.borrowedBooks.end(),
                    [&](Book* b) { return b->title == bookTitle; });
                if (it != borrower.borrowedBooks.end()) {
                    borrower.borrowedBooks.erase(it);
                    break;
                }
            }
            std::cout << "Book returned successfully.\n";
        } else {
            std::cout << "Book not found or already returned.\n";
        }
    }

    void calculateFine() {
        std::string borrowerName;
        std::cout << "Enter borrower name: ";
        std::cin.ignore();
        std::getline(std::cin, borrowerName);

        auto it = std::find_if(borrowers.begin(), borrowers.end(),
            [&borrowerName](const Borrower& b) { return b.name == borrowerName; });

        if (it != borrowers.end()) {
            int daysOverdue = 5;  // Assuming 5 days overdue for this example
            double finePerDay = 0.5;  // $0.50 per day
            double totalFine = daysOverdue * finePerDay * it->borrowedBooks.size();
            std::cout << "Fine for " << borrowerName << ": $" << totalFine << "\n";
        } else {
            std::cout << "Borrower not found.\n";
        }
    }
};

int main() {
    Library library;
    library.addBook("The Great Gatsby", "F. Scott Fitzgerald", "9780743273565");
    library.addBook("To Kill a Mockingbird", "Harper Lee", "9780446310789");
    library.addBook("1984", "George Orwell", "9780451524935");

    int choice;
    do {
        std::cout << "\n1. Search for a book\n2. Check out a book\n3. Return a book\n";
        std::cout << "4. Calculate fine\n5. Exit\nEnter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: library.searchBook(); break;
            case 2: library.checkoutBook(); break;
            case 3: library.returnBook(); break;
            case 4: library.calculateFine(); break;
            case 5: std::cout << "Exiting...\n"; break;
            default: std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}