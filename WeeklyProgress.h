#pragma once
#ifndef WEEKLY_PROGRESS_H
#define WEEKLY_PROGRESS_H

#include <iostream>
#include <iomanip>
#include <fstream>

class WeeklyProgress {
private:
    int week;
    float currentWeight;
    float recommendedCalories;

public:
    WeeklyProgress();  // Default constructor
    WeeklyProgress(int week, float currentWeight, float recommendedCalories);

    // Accessor functions
    int getWeek() const;
    float getCurrentWeight() const;
    float getRecommendedCalories() const;

    // Mutator functions
    void setWeek(int week);
    void setCurrentWeight(float currentWeight);
    void setRecommendedCalories(float recommendedCalories);

    // Display function
    void display() const;

    // Overloaded operators
    bool operator==(const WeeklyProgress& other) const;
};

#endif