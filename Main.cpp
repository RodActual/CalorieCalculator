// Anthony Rodriguez
// 12/05/2023

#include <iostream>
#include <iomanip>
#include <string>
#include "WeeklyProgress.h"

using namespace std;

// Function prototypes
template <typename T>
void saveProgressToFile(const string& filename, const T* progress, int weeks);

template <typename T>
void loadProgressFromFile(const string& filename, T* progress, int& weeks);

bool isValidChoice(int choice);

void displayWelcomeMessage() {
    cout << "\t\t\t\tHello, welcome to the Calorie Calculator!\n\n";
    cout << "\t\tHere we calculate daily goal calories whether you want to maintain, lose,\n";
    cout << "\t\tor gain weight.\n\n";
    cout << "\t\tPlease follow the prompts to calculate your recommended daily calorie intake.\n\n";
    cout << "\t\tPlease press enter to begin...\n\n";
}

// Function to get a valid float input from the user
float getValidFloatInput(const string& prompt) {
    float value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            break;
        }
        else {
            cout << "Invalid input. Please enter a valid numerical value.\n";
            cin.clear();  // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard invalid input
        }
    }
    return value;
}

void getUserInput(string& name, float& currentWeight, float& targetWeight, int& strictness) {
    cin.ignore();

    cout << "What is your full name? ";
    getline(cin, name);

    // Convert name to proper case using cstring functions
    for (size_t i = 0; i < name.size(); i++) {
        if (i == 0 || isspace(name[i - 1])) {
            name[i] = toupper(name[i]);
        }
        else {
            name[i] = tolower(name[i]);
        }
    }

    cout << endl;

    currentWeight = getValidFloatInput("What is your current weight, in lbs.? ");
    targetWeight = getValidFloatInput("What is your goal weight, in lbs.? ");

    cout << "How strict do you intend to be with your calories?\n";
    cout << "1. Low = 0.5 lbs per week,\n";
    cout << "2. Moderate = 1 lbs per week, or\n";
    cout << "3. High = 2 lbs per week.\n";
    strictness = static_cast<int>(getValidFloatInput("Please enter 1 for low, 2 for moderate, or 3 for high: "));
    cout << endl << endl;
}

template <typename T>
void calculateCaloriesAndWeeks(float currentWeight, float targetWeight, int strictness, T* progress) {
    int week = 0;
    float maintenance = currentWeight * 15;

    while (currentWeight != targetWeight) {
        progress[week].setWeek(week + 1);
        progress[week].setCurrentWeight(currentWeight);
        progress[week].setRecommendedCalories(maintenance);

        if (targetWeight > currentWeight) {
            currentWeight += (strictness == 1) ? 0.5 : (strictness == 2) ? 1.0 : 2.0;
        }
        else if (targetWeight < currentWeight) {
            currentWeight -= (strictness == 1) ? 0.5 : (strictness == 2) ? 1.0 : 2.0;
        }

        // Recalculate recommended calories based on the new current weight
        maintenance = currentWeight * 15;

        week++;
    }

    // Add the final week with the goal weight
    progress[week].setWeek(week + 1);
    progress[week].setCurrentWeight(currentWeight);
    progress[week].setRecommendedCalories(currentWeight * 15);
}

template <typename T>
void bubbleSortProgress(T* progress, int weeks, bool ascending) {
    for (int i = 0; i < weeks - 1; i++) {
        for (int j = 0; j < weeks - i - 1; j++) {
            if (ascending) {
                if (progress[j].getWeek() > progress[j + 1].getWeek()) {
                    // Swap progress[j] and progress[j + 1]
                    T temp = progress[j];
                    progress[j] = progress[j + 1];
                    progress[j + 1] = temp;
                }
            }
            else {
                if (progress[j].getWeek() < progress[j + 1].getWeek()) {
                    // Swap progress[j] and progress[j + 1]
                    T temp = progress[j];
                    progress[j] = progress[j + 1];
                    progress[j + 1] = temp;
                }
            }
        }
    }
}

template <typename T>
int searchWeek(const T* progress, int weeks, int targetWeek) {
    for (int week = 0; week < weeks; week++) {
        if (progress[week].getWeek() == targetWeek) {
            return week;
        }
    }
    return -1;  // Return -1 if the week is not found
}

template <typename T>
void displayResultsTable(const string& name, T* progress, int weeks, bool ascending) {
    cout << "\nWeekly Progress Table for " << name << "\n";
    cout << "----------------------------------------------\n";
    cout << "Week    Current Weight    Recommended Calories\n";
    cout << "----------------------------------------------\n";

    bubbleSortProgress(progress, weeks, ascending);

    for (int week = 0; week < weeks; week++) {
        cout << setw(4) << progress[week].getWeek() << setw(15) << fixed << setprecision(2) << progress[week].getCurrentWeight()
            << setw(20) << progress[week].getRecommendedCalories() << endl;
    }
    cout << "----------------------------------------------\n";
}

int main() {
    displayWelcomeMessage();

    while (true) {
        string name;
        float currentWeight, targetWeight;
        int strictness;

        getUserInput(name, currentWeight, targetWeight, strictness);

        const int maxWeeks = 100;  // Set maximum number of weeks

        WeeklyProgress* progress = new WeeklyProgress[maxWeeks];

        calculateCaloriesAndWeeks(currentWeight, targetWeight, strictness, progress);

        // Determine the actual number of weeks (when goal weight is met)
        int actualWeeks = 0;
        while (progress[actualWeeks].getCurrentWeight() != targetWeight) {
            actualWeeks++;
        }

        cout << "It will take " << actualWeeks << " weeks to reach your goal weight." << endl;

        int choice;
        cout << "Do you want to view a table (1) or search for a specific week (2)? Please enter 1 or 2: ";
        cin >> choice;

        if (isValidChoice(choice)) {
            bool ascending;
            cout << "Do you want to display the table in ascending order (1) or descending order (0)? ";
            cin >> ascending;

            displayResultsTable(name, progress, actualWeeks + 1, ascending);
        }
        else {
            cout << "Invalid choice. Please select 1 to view a table or 2 to search for a specific week." << endl;
        }

        delete[] progress;

        cout << "Do you want to re-enter your data (1) or end the program (0)? Enter your choice: ";
        cin >> choice;

        if (choice == 0) {
            cout << "\nThank you very much for using the Calorie Calculator!\n";
            break;
        }
    }

    return 0;
}

template <typename T>
void saveProgressToFile(const string& filename, const T* progress, int weeks) {
    ofstream file(filename, ios::binary | ios::out);
    if (!file) {
        cerr << "Error: Unable to open file for writing." << endl;
        return;
    }

    file.write(reinterpret_cast<const char*>(&weeks), sizeof(int));
    file.write(reinterpret_cast<const char*>(progress), sizeof(T) * weeks);

    file.close();
}

template <typename T>
void loadProgressFromFile(const string& filename, T* progress, int& weeks) {
    ifstream file(filename, ios::binary | ios::in);
    if (!file) {
        cerr << "Warning: No previous progress data found." << endl;
        return;
    }

    file.read(reinterpret_cast<char*>(&weeks), sizeof(int));
    file.read(reinterpret_cast<char*>(progress), sizeof(T) * weeks);

    file.close();
}

bool isValidChoice(int choice) {
    return choice == 1 || choice == 2;
}
