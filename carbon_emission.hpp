// ============================================================
//  CT077-3-2-DSTR  |  Lab Work #1
//  Shared Header   |  Common types, constants, and utilities
//                     used by both Array and Linked List programs
// ============================================================

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <chrono>
#include <cmath>

using namespace std;

// ---- CONSTANTS ----
const int MAX_RECORDS = 600;
const int NUM_MODES   = 6;
const int NUM_GROUPS  = 5;
const int LINE_WIDTH  = 80;

// ---- SHARED DATA STRUCTURE ----
// Stores one resident's commuting record
struct Resident {
    string id;
    int    age;
    string transport;
    double distance;    // km per day
    double emFactor;    // kg CO2 per km
    int    days;        // average days per month
    double emission;    // total = distance * emFactor * days
    string city;
};

// ---- LINKED LIST NODE ----
struct Node {
    Resident data;
    Node*    next;
};

// ---- LOOKUP TABLES ----
static string MODES[NUM_MODES] = {
    "Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool"
};
static string CITIES[3] = {"CityA", "CityB", "CityC"};
static string CITY_NAMES[3] = {
    "City A  (Metropolitan City)",
    "City B  (University Town)",
    "City C  (Suburban/Rural Area)"
};
static string GROUP_LABELS[NUM_GROUPS] = {
    "6-17   Children & Teenagers",
    "18-25  University Students / Young Adults",
    "26-45  Working Adults (Early Career)",
    "46-60  Working Adults (Late Career)",
    "61-100 Senior Citizens / Retirees"
};

// ============================================================
//  SHARED UTILITY FUNCTIONS
// ============================================================

// Strip leading/trailing whitespace and carriage returns
inline string trim(string s) {
    int lo = 0, hi = (int)s.size() - 1;
    while (lo <= hi && (s[lo] == ' ' || s[lo] == '\r' || s[lo] == '\t')) lo++;
    while (hi >= lo && (s[hi] == ' ' || s[hi] == '\r' || s[hi] == '\t')) hi--;
    return (lo > hi) ? "" : s.substr(lo, hi - lo + 1);
}

// Map a resident's age to an age-group index (0-4), or -1 if out of range
inline int getGroupIndex(int age) {
    if (age >= 6   && age <= 17 ) return 0;
    if (age >= 18  && age <= 25 ) return 1;
    if (age >= 26  && age <= 45 ) return 2;
    if (age >= 46  && age <= 60 ) return 3;
    if (age >= 61  && age <= 100) return 4;
    return -1;
}

// Print a full-width horizontal rule
inline void printSep(char c = '-') {
    cout << string(LINE_WIDTH, c) << endl;
}

// Print a section banner: double rule, centred title, double rule
inline void printBanner(const string& title) {
    printSep('=');
    int pad = (LINE_WIDTH - (int)title.size()) / 2;
    if (pad < 0) pad = 0;
    cout << string(pad, ' ') << title << endl;
    printSep('=');
}

// Pause execution and wait for the user to press Enter
inline void waitForEnter() {
    cout << "\n  [ Press ENTER to return to the Main Menu... ]";
    cin.get();
    cout << endl;
}

// ---- TABLE FORMATTING ----
inline void printTableHeader() {
    cout << left
         << setw(8)  << "ID"
         << setw(5)  << "Age"
         << setw(13) << "Transport"
         << setw(10) << "Dist(km)"
         << setw(10) << "Factor"
         << setw(7)  << "Days"
         << setw(14) << "Emission(kg)"
         << "City" << endl;
    printSep();
}

inline void printTableRow(const Resident& r) {
    cout << left
         << setw(8)  << r.id
         << setw(5)  << r.age
         << setw(13) << r.transport
         << setw(10) << fixed << setprecision(1) << r.distance
         << setw(10) << fixed << setprecision(3) << r.emFactor
         << setw(7)  << r.days
         << setw(14) << fixed << setprecision(2) << r.emission
         << r.city << endl;
}
