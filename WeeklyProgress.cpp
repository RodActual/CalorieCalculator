#include "WeeklyProgress.h"

using namespace std;

WeeklyProgress::WeeklyProgress() : week(0), currentWeight(0.0), recommendedCalories(0.0) {}

WeeklyProgress::WeeklyProgress(int week, float currentWeight, float recommendedCalories)
    : week(week), currentWeight(currentWeight), recommendedCalories(recommendedCalories) {}

int WeeklyProgress::getWeek() const {
    return week;
}

float WeeklyProgress::getCurrentWeight() const {
    return currentWeight;
}

float WeeklyProgress::getRecommendedCalories() const {
    return recommendedCalories;
}

void WeeklyProgress::setWeek(int week) {
    this->week = week;
}

void WeeklyProgress::setCurrentWeight(float currentWeight) {
    this->currentWeight = currentWeight;
}

void WeeklyProgress::setRecommendedCalories(float recommendedCalories) {
    this->recommendedCalories = recommendedCalories;
}

void WeeklyProgress::display() const {
    cout << "Week " << week << ": "
        << "Current Weight: " << fixed << setprecision(2) << currentWeight << " lbs, "
        << "Recommended Calories: " << recommendedCalories << " calories\n";
}

bool WeeklyProgress::operator==(const WeeklyProgress& other) const {
    return (week == other.week) && (currentWeight == other.currentWeight) &&
        (recommendedCalories == other.recommendedCalories);
}