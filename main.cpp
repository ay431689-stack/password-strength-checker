#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <random>
#include <cctype>
#include <algorithm>

using namespace std;

// Function prototypes
bool hasUppercase(const string& password);
bool hasLowercase(const string& password);
bool hasDigit(const string& password);
bool hasSpecialChar(const string& password);
int calculateStrengthScore(const string& password);
string getStrengthLevel(int score);
void displayDetailedReport(const string& password, int score, const vector<string>& suggestions);
string generateStrongPassword(int length = 16);
void saveAnalysisToFile(const string& password, int score, const string& strengthLevel);
void showMenu();
string getHiddenPassword(); // Simple hidden input simulation

int main() {
    set<string> passwordHistory;
    int choice;
    string password;

    cout << "=====================================\n";
    cout << "   PASSWORD STRENGTH CHECKER v1.0\n";
    cout << "=====================================\n\n";

    do {
        showMenu();
        cin >> choice;
        cin.ignore(); // Clear newline

        switch (choice) {
            case 1: { // Check Password
                cout << "\nEnter your password: ";
                password = getHiddenPassword();

                if (password.empty()) {
                    cout << "Error: Password cannot be empty!\n";
                    break;
                }

                // Check history
                if (passwordHistory.count(password)) {
                    cout << "Warning: This password has been analyzed in this session before.\n";
                } else {
                    passwordHistory.insert(password);
                }

                int score = calculateStrengthScore(password);
                string level = getStrengthLevel(score);

                vector<string> suggestions;
                if (score < 60) {
                    if (password.length() < 8) suggestions.push_back("- Increase length to at least 8 characters");
                    if (!hasUppercase(password)) suggestions.push_back("- Add at least one uppercase letter");
                    if (!hasLowercase(password)) suggestions.push_back("- Add at least one lowercase letter");
                    if (!hasDigit(password)) suggestions.push_back("- Add at least one number");
                    if (!hasSpecialChar(password)) suggestions.push_back("- Add at least one special character (!@#$%^&*)");
                }

                displayDetailedReport(password, score, suggestions);
                saveAnalysisToFile(password, score, level);
                break;
            }

            case 2: { // Generate Strong Password
                string strongPass = generateStrongPassword();
                cout << "\nGenerated Strong Password: " << strongPass << endl;
                cout << "Copy this password and store it safely!\n";
                break;
            }

            case 3: { // View History
                if (passwordHistory.empty()) {
                    cout << "\nNo passwords analyzed in this session yet.\n";
                } else {
                    cout << "\n=== Passwords Analyzed in this Session ===\n";
                    int index = 1;
                    for (const auto& p : passwordHistory) {
                        cout << index++ << ". " << p << endl;
                    }
                }
                break;
            }

            case 4: // Exit
                cout << "\nThank you for using Password Strength Checker!\n";
                break;

            default:
                cout << "Invalid choice! Please try again.\n";
        }

        if (choice != 4) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }

    } while (choice != 4);

    return 0;
}

// Check for uppercase
bool hasUppercase(const string& password) {
    return any_of(password.begin(), password.end(), ::isupper);
}

// Check for lowercase
bool hasLowercase(const string& password) {
    return any_of(password.begin(), password.end(), ::islower);
}

// Check for digit
bool hasDigit(const string& password) {
    return any_of(password.begin(), password.end(), ::isdigit);
}

// Check for special character
bool hasSpecialChar(const string& password) {
    const string specials = "!@#$%^&*()_+-=[]{}|;:,.<>/?";
    return any_of(password.begin(), password.end(), [&](char c) {
        return specials.find(c) != string::npos;
    });
}

// Calculate strength score (out of 100)
int calculateStrengthScore(const string& password) {
    int score = 0;
    int length = password.length();

    // Length contribution (max 30)
    if (length >= 16) score += 30;
    else if (length >= 12) score += 25;
    else if (length >= 8) score += 20;
    else if (length >= 6) score += 10;

    // Character variety (max 70)
    if (hasUppercase(password)) score += 15;
    if (hasLowercase(password)) score += 15;
    if (hasDigit(password)) score += 15;
    if (hasSpecialChar(password)) score += 15;

    // Bonus for extra variety
    int varietyCount = 0;
    if (hasUppercase(password)) varietyCount++;
    if (hasLowercase(password)) varietyCount++;
    if (hasDigit(password)) varietyCount++;
    if (hasSpecialChar(password)) varietyCount++;
    
    if (varietyCount == 4) score += 10;

    return min(score, 100);
}

// Get strength level
string getStrengthLevel(int score) {
    if (score >= 90) return "Very Strong";
    if (score >= 70) return "Strong";
    if (score >= 50) return "Medium";
    return "Weak";
}

// Display detailed report
void displayDetailedReport(const string& password, int score, const vector<string>& suggestions) {
    string level = getStrengthLevel(score);

    cout << "\n" << string(50, '=') << endl;
    cout << "              PASSWORD ANALYSIS REPORT\n";
    cout << string(50, '=') << endl;

    cout << "Password      : " << password << endl;
    cout << "Length        : " << password.length() << " characters\n";
    cout << "Strength      : " << level << " (" << score << "/100)\n";
    cout << "Uppercase     : " << (hasUppercase(password) ? "Yes" : "No") << endl;
    cout << "Lowercase     : " << (hasLowercase(password) ? "Yes" : "No") << endl;
    cout << "Numbers       : " << (hasDigit(password) ? "Yes" : "No") << endl;
    cout << "Special Chars : " << (hasSpecialChar(password) ? "Yes" : "No") << endl;

    cout << "\n--- Suggestions ---\n";
    if (suggestions.empty()) {
        cout << "Excellent! Your password meets all strong password criteria.\n";
    } else {
        for (const auto& sug : suggestions) {
            cout << sug << endl;
        }
    }
    cout << string(50, '=') << endl;
}

// Generate strong password
string generateStrongPassword(int length) {
    const string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const string lower = "abcdefghijklmnopqrstuvwxyz";
    const string digits = "0123456789";
    const string special = "!@#$%^&*()_+-=";
    
    string allChars = upper + lower + digits + special;
    
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> dist(0, allChars.length() - 1);
    
    string password;
    password += upper[uniform_int_distribution<>(0, upper.length()-1)(generator)];
    password += lower[uniform_int_distribution<>(0, lower.length()-1)(generator)];
    password += digits[uniform_int_distribution<>(0, digits.length()-1)(generator)];
    password += special[uniform_int_distribution<>(0, special.length()-1)(generator)];
    
    for (int i = 4; i < length; ++i) {
        password += allChars[dist(generator)];
    }
    
    // Shuffle the password
    shuffle(password.begin(), password.end(), generator);
    
    return password;
}

// Save analysis to file
void saveAnalysisToFile(const string& password, int score, const string& strengthLevel) {
    ofstream file("password_analysis.log", ios::app);
    if (!file.is_open()) {
        cout << "Warning: Could not save analysis to file.\n";
        return;
    }

    time_t now = time(nullptr);
    char timestamp[26];
    ctime_s(timestamp, sizeof(timestamp), &now);

    file << "=== Analysis @ " << timestamp;
    file << "Password: " << password << "\n";
    file << "Score: " << score << "/100 (" << strengthLevel << ")\n";
    file << "----------------------------------------\n";
    file.close();
}

// Show menu
void showMenu() {
    cout << "\n--- Main Menu ---\n";
    cout << "1. Check Password Strength\n";
    cout << "2. Generate Strong Password\n";
    cout << "3. View Analysis History (Session)\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
}

// Simple hidden password input (cross-platform simulation)
string getHiddenPassword() {
    string password;
    char ch;
    
    // For better UX in educational projects, we use visible input
    // Real hidden input requires platform-specific code (conio.h on Windows)
    getline(cin, password);
    
    return password;
}