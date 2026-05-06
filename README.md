# CT077-3-2-DSTR | Lab Work #1
## Comparative Analysis of Arrays and Singly Linked Lists
### Urban Carbon Emission Data - Cityville

---

## Overview

This project implements and compares two fundamental data structures - **Arrays** and **Singly Linked Lists** - applied to a real-world dataset of urban carbon emissions from resident commuting patterns across three cities.

Two separate programs are provided, each performing identical analyses using a different underlying data structure.

---

## Files

| File | Description |
|------|-------------|
| `carbon_emission.hpp` | Shared header - structs, constants, lookup tables, utility functions |
| `array_program.cpp` | Program 1 - Array-based implementation |
| `linkedlist_program.cpp` | Program 2 - Singly Linked List implementation |
| `dataset1-cityA.csv` | City A (Metropolitan City) - 200 records |
| `dataset2-cityB.csv` | City B (University Town) - 178 records |
| `dataset3-cityC.csv` | City C (Suburban/Rural Area) - 122 records |

---

## Data Structure

Each resident record contains:

| Field | Type | Description |
|-------|------|-------------|
| `id` | string | Resident ID |
| `age` | int | Age of resident |
| `transport` | string | Mode of transport |
| `distance` | double | Daily commute distance (km) |
| `emFactor` | double | Emission factor (kg CO2/km) |
| `days` | int | Average commuting days per month |
| `emission` | double | Monthly emission = distance × emFactor × days |
| `city` | string | City identifier |

---

## How to Compile

Requires **g++** with C++17 support.

```bash
# Program 1 — Array
g++ -std=c++17 -o array_program.exe array_program.cpp

# Program 2 — Linked List
g++ -std=c++17 -o linkedlist_program.exe linkedlist_program.cpp
```

---

## How to Run

```bash
./array_program.exe
./linkedlist_program.exe
```

Both programs present an interactive menu:

```
1. Carbon Emission Analysis
2. Sorting Experiments
3. Searching Experiments
4. Performance Analysis
5. Insights & Recommendations
0. Exit
```

---

## Features

### 1. Carbon Emission Analysis
- Total emissions per city
- Emissions broken down by transport mode
- Emissions broken down by age group with most preferred transport mode

### 2. Sorting Experiments

| Program | Algorithms Used |
|---------|----------------|
| Array | Bubble Sort, Selection Sort |
| Linked List | Bubble Sort, Merge Sort |

- Performance timing (microseconds) for each algorithm
- Top 10 highest monthly emitters
- 10 youngest residents

### 3. Searching Experiments

| Program | Algorithms Used |
|---------|----------------|
| Array | Linear Search, Binary Search |
| Linked List | Linear Search, Jump Search |

- Search by transport mode
- Search by age group
- Search by minimum daily distance

### 4. Performance Analysis
- Time and space complexity comparison table
- Memory usage estimate (Array vs Linked List)
- Pros and cons of each structure for this dataset

### 5. Insights & Recommendations
- Cross-city emission comparison
- Age group emission share (% of total)
- Policy recommendations for sustainable urban transport

---

## Algorithm Complexity Summary

| Algorithm | Best | Worst | Space |
|-----------|------|-------|-------|
| Bubble Sort | O(n) | O(n²) | O(1) |
| Selection Sort | O(n²) | O(n²) | O(1) |
| Merge Sort | O(n log n) | O(n log n) | O(log n) |
| Linear Search | O(1) | O(n) | O(1) |
| Binary Search | O(1) | O(log n) | O(1) — requires sorted array |
| Jump Search | O(1) | O(√n) | O(1) — requires sorted list |

---

## Transport Modes Covered

- Car
- Bus
- Bicycle
- Walking
- School Bus
- Carpool

---

## Age Groups

| Group | Range |
|-------|-------|
| Children & Teenagers | 6 – 17 |
| University Students / Young Adults | 18 – 25 |
| Working Adults (Early Career) | 26 – 45 |
| Working Adults (Late Career) | 46 – 60 |
| Senior Citizens / Retirees | 61 – 100 |

---

## Subject
**CT077-3-2 Data Structures & Algorithms**
