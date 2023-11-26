#include <iostream>
#include <string>

class Account {
public:
    Account(const std::string accountNumber, const std::string accountHolder, double balance)
        : accountNumber(accountNumber), accountHolder(accountHolder), balance(balance) {}

    virtual void withdraw(double amount) = 0;
    virtual void deposit(double amount) = 0;

    std::string accountNumber;
    std::string accountHolder;
    double balance;
    virtual std::string accountType() const = 0;

    virtual Account& operator+(Account& rhs) {
        performTransfer(rhs);
        return rhs;
    }

    friend std::ostream& operator<<(std::ostream& os, const Account& account);

protected:
    void performTransfer(Account& rhs) {
        double transferAmount = 300.0;
        withdraw(transferAmount);
        rhs.deposit(transferAmount);
    }
};

class SavingsAccount : public Account {
public:
    SavingsAccount(const std::string& accountNumber, const std::string& accountHolder, double balance, double interestRate)
        : Account(accountNumber, accountHolder, balance), interestRate(interestRate) {}

    std::string accountType() const override {
        return "Savings Account";
    }

    void withdraw(double amount) override {
        const double minimumBalance = 100.0;

        if (amount > 0 && (balance - amount) >= minimumBalance) {
            balance -= amount;
        } else {
            std::cout << "Minimum Balance Reached!\n";
        }
    }


    void deposit(double amount) override {
        if (amount > 0) {
            balance += amount;
        } else {
            std::cout << "Invalid deposit amount.\n";
        }
    }

    SavingsAccount& operator+(Account& rhs) override {
        SavingsAccount& savingsRHS = dynamic_cast<SavingsAccount&>(rhs);
        performTransfer(savingsRHS);
        return *this; 
    }

    friend std::ostream& operator<<(std::ostream& os, const SavingsAccount& account);

private:
    double interestRate;
};

class CurrentAccount : public Account {
public:
    CurrentAccount(const std::string& accountNumber, const std::string& accountHolder, double balance, double overdraftLimit)
        : Account(accountNumber, accountHolder, balance), overdraftLimit(overdraftLimit) {}

    std::string accountType() const override {
        return "Current Account";
    }

    void withdraw(double amount) override {
        if (amount > 0 && (balance - amount) >= -overdraftLimit) {
            balance -= amount;
        } else {
            std::cout << "Overdraft Limit!\n";
        }
    }

    void deposit(double amount) override {
        if (amount > 0) {
            balance += amount;
        } else {
            std::cout << "Invalid deposit amount.\n";
        }
    }

    CurrentAccount& operator+(Account& rhs) override {
        performTransfer(rhs);
        return dynamic_cast<CurrentAccount&>(rhs);
    }

    friend std::ostream& operator<<(std::ostream& os, const CurrentAccount& account);

private:
    double overdraftLimit;
};

std::ostream& operator<<(std::ostream& os, const SavingsAccount& account) {
    os << "Savings Account Details (ID: " << account.accountNumber << "):\n";
    os << "   Holder: " << account.accountHolder << "\n";
    os << "   Balance: $" << account.balance << ".00\n";
    os << "   Interest Rate: " << (account.interestRate * 100) << "%\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const CurrentAccount& account) {
    os << "Current Account Details (ID: " << account.accountNumber << "):\n";
    os << "   Holder: " << account.accountHolder << "\n";
    os << "   Balance: $" << account.balance << ".00\n";
    os << "   Overdraft Limit: $" << account.overdraftLimit << ".00\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Account& account) {
    os << "Account Details for " << account.accountType() << " (ID: " << account.accountNumber << "):\n";
    os << "   Holder: " << account.accountHolder << "\n";
    os << "   Balance: $" << account.balance << ".00\n";
    return os;
}

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    std::cout << savings << "\n";
    std::cout << current << "\n";
    std::cout << "\n";

    savings.deposit(500);
    current.withdraw(1000);
    std::cout << "Account Details after deposit and withdrawal: \n";
    std::cout << savings << "\n";
    std::cout << current << "\n";
    std::cout << "\n";

    current = current + savings;

    std::cout << "Account Details After Transfer: \n";
    std::cout << savings << "\n";
    std::cout << current << "\n";

    return 0;
}
