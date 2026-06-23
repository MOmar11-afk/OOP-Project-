
#include <iostream>
#include <string>
using namespace std;

template <typename T>
void showValue(T value) {
    cout << value << endl;
}

class Transaction {
    string type;
    double amount;

public:
    Transaction(string t = "None", double a = 0) {
        type = t;
        amount = a;
    }

    void show() {
        cout << type << " | Amount: $" << amount << endl;
    }
};

class Security {
public:
    static bool verifyPIN(int entered, int actual) {
        return entered == actual;
    }
};

class Account {
protected:
    int accountNumber;
    double balance;
    Transaction transactions[10];
    int transCount;
    bool frozen;

public:
    Account(int acc, double bal) {
        accountNumber = acc;
        balance = bal;
        transCount = 0;
        frozen = false;
    }

    virtual ~Account() {}

    virtual void calculateInterest() = 0;
    virtual string getAccountType() = 0;

    void deposit(double amt) {
        if (frozen) {
            cout << "Account is frozen." << endl;
            return;
        }
        if (transCount >= 10) {
            cout << "Transaction limit reached." << endl;
            return;
        }
        balance += amt;
        transactions[transCount++] = Transaction("Deposit", amt);
        cout << "Deposit successful." << endl;
    }

    void withdraw(double amt) {
        if (frozen) {
            cout << "Account is frozen." << endl;
            return;
        }
        if (amt > balance) {
            cout << "Insufficient balance." << endl;
            return;
        }
        if (transCount >= 10) {
            cout << "Transaction limit reached." << endl;
            return;
        }
        balance -= amt;
        transactions[transCount++] = Transaction("Withdraw", amt);
        cout << "Withdrawal successful." << endl;
    }

    void showAccount() {
        cout << endl << " Account Details " << endl;
        cout << "Account No: " << accountNumber << endl;
        cout << "Account Type: " << getAccountType() << endl;
        cout << "Balance: ";
        showValue(balance);
        cout << "Status: " << (frozen ? "Frozen" : "Active") << endl;
    }

    void showTransactions() {
        cout << endl << " Transactions " << endl;
        for (int i = 0; i < transCount; i++)
            transactions[i].show();
    }

    void setFrozen(bool status) {
        frozen = status;
    }

    double getBalance() {
        return balance;
    }
};

class SavingsAccount : public Account {
public:
    SavingsAccount(int acc, double bal) : Account(acc, bal) {}

    void calculateInterest() {
        balance += balance * 0.04;
        cout << "Interest added." << endl;
    }

    string getAccountType() {
        return "Savings Account";
    }
};

class ATM {
    int* pin;

public:
    ATM(int* p) {
        pin = p;
    }

    void withdraw(Account* acc) {
        int entered;
        double amt;

        cout << "Enter PIN: ";
        cin >> entered;

        if (!Security::verifyPIN(entered, *pin)) {
            cout << "Incorrect PIN." << endl;
            return;
        }

        cout << "Enter withdrawal amount: ";
        cin >> amt;

        acc->withdraw(amt);
    }

    void setPIN(int newPin) {
        *pin = newPin;
    }
};

class Loan {
protected:
    double amount;
    int years;

public:
    Loan(double amt, int y) {
        amount = amt;
        years = y;
    }

    virtual ~Loan() {}
    virtual double calculateEMI() = 0;
    virtual string getLoanType() = 0;
};

class HomeLoan : public Loan {
public:
    HomeLoan(double amt, int y) : Loan(amt, y) {}

    double calculateEMI() {
        return (amount + amount * 0.10) / (years * 12);
    }

    string getLoanType() {
        return "Home Loan";
    }
};

class User {
public:
    virtual ~User() {}
    virtual void menu() = 0;
};

class Customer : public User {
    Account* account;
    Loan* loan;
    ATM atm;

public:
    Customer(Account* acc, Loan* ln, int* pin) : atm(pin) {
        account = acc;
        loan = ln;
    }

    void menu() {
        int ch;

        while (true) {
            cout << endl << "Customer Menu" << endl;
            cout << "1. Deposit" << endl;
            cout << "2. Withdraw" << endl;
            cout << "3. ATM" << endl;
            cout << "4. Account" << endl;
            cout << "5. Transactions" << endl;
            cout << "6. Interest" << endl;
            cout << "7. EMI" << endl;
            cout << "0. Logout" << endl;
            cout << "Select option: ";
            cin >> ch;

            if (ch == 0) return;

            if (ch == 1) {
                double a;
                cout << "Enter deposit amount: ";
                cin >> a;
                account->deposit(a);
            }
            else if (ch == 2) {
                double a;
                cout << "Enter withdrawal amount: ";
                cin >> a;
                account->withdraw(a);
            }
            else if (ch == 3) {
                atm.withdraw(account);
            }
            else if (ch == 4) {
                account->showAccount();
            }
            else if (ch == 5) {
                account->showTransactions();
            }
            else if (ch == 6) {
                account->calculateInterest();
            }
            else if (ch == 7) {
                cout << loan->getLoanType() << " EMI: ";
                showValue(loan->calculateEMI());
            }
        }
    }
};

class Admin : public User {
    string username;
    string password;
    Account* account;
    Loan* loan;
    int* pin;

public:
    Admin(Account* acc, Loan* ln, int* p, string u, string pw) {
        account = acc;
        loan = ln;
        pin = p;
        username = u;
        password = pw;
    }

    bool login() {
        string u, p;
        cout << "Enter admin username: ";
        cin >> u;
        cout << "Enter admin password: ";
        cin >> p;

        if (u != username || p != password) {
            cout << "Invalid credentials." << endl;
            return false;
        }
        cout << "Login successful." << endl;
        return true;
    }

    void menu() {
        int ch;

        while (true) {
            cout << endl << "Admin Menu" << endl;
            cout << "1. View Account" << endl;
            cout << "2. Report" << endl;
            cout << "3. View Transactions" << endl;
            cout << "4. Reset PIN" << endl;
            cout << "5. Freeze/Unfreeze Account" << endl;
            cout << "0. Logout" << endl;
            cout << "Select option: ";
            cin >> ch;

            if (ch == 0) return;

            if (ch == 1) {
                account->showAccount();
                cout << "Loan Type: " << loan->getLoanType() << endl;
            }
            else if (ch == 2) {
                cout << "Balance: " << account->getBalance() << endl;
                cout << "Loan: " << loan->getLoanType() << endl;
            }
            else if (ch == 3) {
                account->showTransactions();
            }
            else if (ch == 4) {
                int newPin;
                cout << "Enter new PIN: ";
                cin >> newPin;
                *pin = newPin;
                cout << "PIN updated successfully." << endl;
            }
            else if (ch == 5) {
                int opt;
                cout << "1. Freeze" << endl << "2. Unfreeze" << endl;
                cout << "Select: ";
                cin >> opt;
                account->setFrozen(opt == 1);
                cout << "Account status updated." << endl;
            }
        }
    }
};

int main() {
    int customerPIN = 1234;

    SavingsAccount account(1001, 5000);
    HomeLoan loan(200000, 15);

    int choice;
    do {
        cout << endl << "Bank System" << endl;
        cout << "1. Customer" << endl;
        cout << "2. Admin" << endl;
        cout << "0. Exit" << endl;
        cout << "Select option: ";
        cin >> choice;

        if (choice == 1) {
            Customer customer(&account, &loan, &customerPIN);
            customer.menu();
        }
        else if (choice == 2) {
            Admin admin(&account, &loan, &customerPIN, "admin", "admin123");
            if (admin.login())
                admin.menu();
        }

    } while (choice != 0);

    cout << "System exited." << endl;
    return 0;
}
