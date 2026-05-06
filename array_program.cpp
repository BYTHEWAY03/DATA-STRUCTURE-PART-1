// ============================================================
//  CT077-3-2-DSTR  |  Lab Work #1
//  Program 1       |  Array-Based Implementation
//  Topic           :  Comparative Analysis of Arrays and
//                     Singly Linked Lists for Sorting and
//                     Searching in Urban Carbon Emission Data
// ============================================================

#include "carbon_emission.hpp"

// ---- GLOBAL ARRAY & COUNTER ----
Resident records[MAX_RECORDS];
int total = 0;

// ============================================================
//  DATA LOADING
// ============================================================

// Read one CSV file and append its records to the global array
void loadData(const string& filename, const string& cityName) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "  [ERROR]  Cannot open file: " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // skip header row

    int before = total;
    while (getline(file, line) && total < MAX_RECORDS) {
        if (line.empty()) continue;
        if (line.back() == '\r') line.pop_back();

        stringstream ss(line);
        string tok;
        Resident r;

        getline(ss, tok, ','); r.id        = trim(tok);
        getline(ss, tok, ','); r.age       = stoi(trim(tok));
        getline(ss, tok, ','); r.transport = trim(tok);
        getline(ss, tok, ','); r.distance  = stod(trim(tok));
        getline(ss, tok, ',');
        string ef = trim(tok);
        r.emFactor = ef.empty() ? 0.0 : stod(ef);
        getline(ss, tok, ',');
        string dv = trim(tok);
        r.days = dv.empty() ? 0 : stoi(dv);

        r.emission    = r.distance * r.emFactor * r.days;
        r.city        = cityName;
        records[total++] = r;
    }

    file.close();
    cout << "  Loaded  " << left << setw(26) << filename
         << "->  " << (total - before) << " records" << endl;
}

// ============================================================
//  SORTING ALGORITHMS
// ============================================================

// Bubble Sort  |  Best O(n)  |  Worst O(n^2)  |  Space O(1)
// field:  0 = age   1 = distance   2 = emission
void bubbleSort(Resident arr[], int n, int field) {
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            bool doSwap = false;
            if (field == 0 && arr[j].age      > arr[j+1].age)      doSwap = true;
            if (field == 1 && arr[j].distance > arr[j+1].distance) doSwap = true;
            if (field == 2 && arr[j].emission > arr[j+1].emission) doSwap = true;
            if (doSwap) {
                Resident tmp = arr[j];
                arr[j]       = arr[j+1];
                arr[j+1]     = tmp;
                swapped = true;
            }
        }
        if (!swapped) break;  // already sorted — early exit
    }
}

// Selection Sort  |  Best & Worst O(n^2)  |  Space O(1)
void selectionSort(Resident arr[], int n, int field) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            bool isLess = false;
            if (field == 0 && arr[j].age      < arr[minIdx].age)      isLess = true;
            if (field == 1 && arr[j].distance < arr[minIdx].distance) isLess = true;
            if (field == 2 && arr[j].emission < arr[minIdx].emission) isLess = true;
            if (isLess) minIdx = j;
        }
        Resident tmp  = arr[i];
        arr[i]        = arr[minIdx];
        arr[minIdx]   = tmp;
    }
}

// ============================================================
//  SEARCHING ALGORITHMS
// ============================================================

// Linear Search by transport mode  |  O(n)  |  No sorting needed
void linearSearchMode(const string& mode) {
    cout << "\n  Searching for residents who travel by: " << mode << endl;
    printSep();

    auto t1 = chrono::high_resolution_clock::now();
    int idx[MAX_RECORDS], found = 0;
    for (int i = 0; i < total; i++)
        if (records[i].transport == mode) idx[found++] = i;
    auto t2  = chrono::high_resolution_clock::now();
    long long us = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

    cout << "  Algorithm  : Linear Search (unsorted data)" << endl;
    cout << "  Time taken : " << us    << " microseconds" << endl;
    cout << "  Found      : " << found << " residents" << endl;
    printSep();
    printTableHeader();
    int show = found < 15 ? found : 15;
    for (int i = 0; i < show; i++) printTableRow(records[idx[i]]);
    printSep();
    if (found > 15)
        cout << "  (Showing first 15 of " << found << " matching results)" << endl;
}

// Linear Search by minimum daily distance  |  O(n)
void linearSearchDistance(double minKm) {
    cout << "\n  Searching for residents with daily distance > " << minKm << " km" << endl;
    printSep();

    auto t1 = chrono::high_resolution_clock::now();
    int idx[MAX_RECORDS], found = 0;
    for (int i = 0; i < total; i++)
        if (records[i].distance > minKm) idx[found++] = i;
    auto t2 = chrono::high_resolution_clock::now();
    long long us = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

    cout << "  Algorithm  : Linear Search (unsorted data)" << endl;
    cout << "  Time taken : " << us    << " microseconds" << endl;
    cout << "  Found      : " << found << " residents" << endl;
    printSep();
    printTableHeader();
    int show = found < 15 ? found : 15;
    for (int i = 0; i < show; i++) printTableRow(records[idx[i]]);
    printSep();
    if (found > 15)
        cout << "  (Showing first 15 of " << found << " matching results)" << endl;
}

// Linear Search by age group index  |  O(n)
void linearSearchGroup(int groupIdx) {
    string labels[] = {"6-17", "18-25", "26-45", "46-60", "61-100"};
    cout << "\n  Searching for residents in age group: " << labels[groupIdx] << endl;
    printSep();

    auto t1 = chrono::high_resolution_clock::now();
    int idx[MAX_RECORDS], found = 0;
    for (int i = 0; i < total; i++)
        if (getGroupIndex(records[i].age) == groupIdx) idx[found++] = i;
    auto t2 = chrono::high_resolution_clock::now();
    long long us = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

    cout << "  Algorithm  : Linear Search (unsorted data)" << endl;
    cout << "  Time taken : " << us    << " microseconds" << endl;
    cout << "  Found      : " << found << " residents" << endl;
    printSep();
    printTableHeader();
    int show = found < 15 ? found : 15;
    for (int i = 0; i < show; i++) printTableRow(records[idx[i]]);
    printSep();
    if (found > 15)
        cout << "  (Showing first 15 of " << found << " matching results)" << endl;
}

// Binary Search for an age range  |  O(log n)  |  Array must be sorted by age
void binarySearchAge(Resident arr[], int n, int loAge, int hiAge) {
    cout << "\n  Binary Search for residents aged " << loAge
         << " to " << hiAge << " (sorted array)" << endl;
    printSep();

    auto t1 = chrono::high_resolution_clock::now();

    // Find the left boundary of the range using binary search
    int lo = 0, hi = n - 1, leftIdx = -1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (arr[mid].age >= loAge) { leftIdx = mid; hi = mid - 1; }
        else lo = mid + 1;
    }

    int idx[MAX_RECORDS], found = 0;
    if (leftIdx != -1)
        for (int i = leftIdx; i < n && arr[i].age <= hiAge; i++)
            idx[found++] = i;

    auto t2 = chrono::high_resolution_clock::now();
    long long us = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

    cout << "  Algorithm  : Binary Search (sorted by age)" << endl;
    cout << "  Time taken : " << us    << " microseconds" << endl;
    cout << "  Found      : " << found << " residents" << endl;
    printSep();
    printTableHeader();
    int show = found < 15 ? found : 15;
    for (int i = 0; i < show; i++) printTableRow(arr[idx[i]]);
    printSep();
    if (found > 15)
        cout << "  (Showing first 15 of " << found << " matching results)" << endl;
}

// ============================================================
//  SECTION 1 — CARBON EMISSION ANALYSIS
// ============================================================
void carbonAnalysis() {
    printBanner("SECTION 1  |  CARBON EMISSION ANALYSIS");

    // ---- Total Emission Per City ----
    cout << "\n  Total Carbon Emissions per Dataset (City)" << endl;
    printSep();
    cout << left
         << setw(32) << "  City"
         << setw(10) << "Records"
         << "Total Emission (kg CO2)" << endl;
    printSep();

    double grandTotal = 0;
    for (int c = 0; c < 3; c++) {
        double cityTotal = 0;
        int    cnt       = 0;
        for (int i = 0; i < total; i++) {
            if (records[i].city == CITIES[c]) {
                cityTotal += records[i].emission;
                cnt++;
            }
        }
        cout << "  " << left << setw(30) << CITY_NAMES[c]
             << setw(10) << cnt
             << fixed << setprecision(2) << cityTotal << " kg CO2" << endl;
        grandTotal += cityTotal;
    }
    printSep();
    cout << "  " << left << setw(30) << "ALL CITIES COMBINED"
         << setw(10) << total
         << fixed << setprecision(2) << grandTotal << " kg CO2" << endl;
    printSep();

    // ---- Emission Per Transport Mode ----
    cout << "\n  Carbon Emissions by Mode of Transport" << endl;
    printSep();
    cout << "  " << left
         << setw(14) << "Mode"
         << setw(9)  << "Count"
         << setw(24) << "Total (kg CO2)"
         << "Average per Resident" << endl;
    printSep();

    for (int m = 0; m < NUM_MODES; m++) {
        double mTotal = 0;
        int    mCnt   = 0;
        for (int i = 0; i < total; i++) {
            if (records[i].transport == MODES[m]) {
                mTotal += records[i].emission;
                mCnt++;
            }
        }
        if (mCnt == 0) continue;
        cout << "  " << left
             << setw(14) << MODES[m]
             << setw(9)  << mCnt
             << setw(24) << fixed << setprecision(2) << mTotal
             << fixed << setprecision(2) << mTotal / mCnt << " kg CO2" << endl;
    }
    printSep();

    // ---- Emission Per Age Group ----
    cout << "\n  Carbon Emissions Breakdown by Age Group" << endl;

    for (int g = 0; g < NUM_GROUPS; g++) {
        double modeEm[NUM_MODES]  = {};
        int    modeCnt[NUM_MODES] = {};
        double gTotal = 0;
        int    gCount = 0;

        for (int i = 0; i < total; i++) {
            if (getGroupIndex(records[i].age) == g) {
                gTotal += records[i].emission;
                gCount++;
                for (int m = 0; m < NUM_MODES; m++)
                    if (records[i].transport == MODES[m]) {
                        modeEm[m]  += records[i].emission;
                        modeCnt[m] ++;
                    }
            }
        }
        if (gCount == 0) continue;

        int best = 0;
        for (int m = 1; m < NUM_MODES; m++)
            if (modeCnt[m] > modeCnt[best]) best = m;

        cout << "\n  Age Group : " << GROUP_LABELS[g] << endl;
        printSep('-');
        cout << "  " << left
             << setw(14) << "Mode"
             << setw(9)  << "Count"
             << setw(24) << "Total (kg CO2)"
             << "Average per Resident" << endl;
        printSep('-');
        for (int m = 0; m < NUM_MODES; m++) {
            if (modeCnt[m] == 0) continue;
            cout << "  " << left
                 << setw(14) << MODES[m]
                 << setw(9)  << modeCnt[m]
                 << setw(24) << fixed << setprecision(2) << modeEm[m]
                 << fixed << setprecision(2) << modeEm[m] / modeCnt[m] << " kg CO2" << endl;
        }
        printSep('-');
        cout << "  Total Residents  : " << gCount << endl;
        cout << "  Total Emission   : " << fixed << setprecision(2) << gTotal << " kg CO2" << endl;
        cout << "  Average/Resident : " << fixed << setprecision(2) << gTotal / gCount << " kg CO2" << endl;
        cout << "  Most Preferred   : " << MODES[best] << " (" << modeCnt[best] << " residents)" << endl;
    }

    waitForEnter();
}

// ============================================================
//  SECTION 2 — SORTING EXPERIMENTS
// ============================================================
void sortingExperiments() {
    printBanner("SECTION 2  |  SORTING EXPERIMENTS");

    string fields[3] = {"Age", "Daily Distance (km)", "Monthly Emission (kg CO2)"};

    // ---- Performance Comparison ----
    cout << "\n  Measuring Sort Performance: Bubble Sort vs Selection Sort" << endl;
    printSep();
    cout << left
         << setw(20) << "  Algorithm"
         << setw(28) << "Sort By"
         << setw(10) << "Records"
         << "Time (microseconds)" << endl;
    printSep();

    Resident copy1[MAX_RECORDS], copy2[MAX_RECORDS];

    for (int f = 0; f < 3; f++) {
        for (int i = 0; i < total; i++) copy1[i] = copy2[i] = records[i];

        auto t1 = chrono::high_resolution_clock::now();
        bubbleSort(copy1, total, f);
        auto t2 = chrono::high_resolution_clock::now();
        long long us1 = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

        auto t3 = chrono::high_resolution_clock::now();
        selectionSort(copy2, total, f);
        auto t4 = chrono::high_resolution_clock::now();
        long long us2 = chrono::duration_cast<chrono::microseconds>(t4 - t3).count();

        cout << left
             << setw(20) << "  Bubble Sort"
             << setw(28) << fields[f]
             << setw(10) << total << us1 << " us" << endl;
        cout << left
             << setw(20) << "  Selection Sort"
             << setw(28) << fields[f]
             << setw(10) << total << us2 << " us" << endl;
        printSep('-');
    }
    printSep();

    // ---- Top 10 Highest Monthly Emitters ----
    Resident byEmission[MAX_RECORDS];
    for (int i = 0; i < total; i++) byEmission[i] = records[i];
    bubbleSort(byEmission, total, 2);

    cout << "\n  Top 10 Highest Monthly Emitters" << endl;
    cout << "  (Sorted by Monthly Emission using Bubble Sort — descending)" << endl;
    printSep();
    printTableHeader();
    for (int i = total - 1; i >= total - 10 && i >= 0; i--)
        printTableRow(byEmission[i]);
    printSep();

    // ---- 10 Youngest Residents ----
    Resident byAge[MAX_RECORDS];
    for (int i = 0; i < total; i++) byAge[i] = records[i];
    selectionSort(byAge, total, 0);

    cout << "\n  10 Youngest Residents in the Dataset" << endl;
    cout << "  (Sorted by Age using Selection Sort — ascending)" << endl;
    printSep();
    printTableHeader();
    for (int i = 0; i < 10; i++) printTableRow(byAge[i]);
    printSep();

    // ---- Complexity Reference Table ----
    cout << "\n  Algorithm Complexity Reference" << endl;
    printSep();
    cout << left
         << setw(20) << "  Algorithm"
         << setw(16) << "Best Case"
         << setw(16) << "Worst Case"
         << "Space" << endl;
    printSep();
    cout << left
         << setw(20) << "  Bubble Sort"
         << setw(16) << "O(n)"
         << setw(16) << "O(n^2)"
         << "O(1)" << endl;
    cout << left
         << setw(20) << "  Selection Sort"
         << setw(16) << "O(n^2)"
         << setw(16) << "O(n^2)"
         << "O(1)" << endl;
    printSep();

    waitForEnter();
}

// ============================================================
//  SECTION 3 — SEARCHING EXPERIMENTS
// ============================================================
void searchingExperiments() {
    printBanner("SECTION 3  |  SEARCHING EXPERIMENTS");

    // ---- Linear Search on Unsorted Data ----
    cout << "\n  Part A: Linear Search on Unsorted Data" << endl;
    printSep('=');

    linearSearchMode("Car");
    linearSearchMode("Bicycle");
    linearSearchGroup(2);        // age group 26-45
    linearSearchDistance(15.0);

    // ---- Binary Search on Sorted Data ----
    cout << "\n  Part B: Binary Search on Sorted Data (sorted by age)" << endl;
    printSep('=');

    Resident sortedByAge[MAX_RECORDS];
    for (int i = 0; i < total; i++) sortedByAge[i] = records[i];
    bubbleSort(sortedByAge, total, 0);  // sort by age first

    binarySearchAge(sortedByAge, total, 26, 45);
    binarySearchAge(sortedByAge, total, 18, 25);

    // ---- Complexity Summary ----
    cout << "\n  Search Algorithm Complexity Summary" << endl;
    printSep();
    cout << left
         << setw(20) << "  Algorithm"
         << setw(16) << "Best Case"
         << setw(16) << "Worst Case"
         << "Requirement" << endl;
    printSep();
    cout << left
         << setw(20) << "  Linear Search"
         << setw(16) << "O(1)"
         << setw(16) << "O(n)"
         << "No pre-sorting required" << endl;
    cout << left
         << setw(20) << "  Binary Search"
         << setw(16) << "O(1)"
         << setw(16) << "O(log n)"
         << "Array must be sorted first" << endl;
    printSep();

    waitForEnter();
}

// ============================================================
//  SECTION 4 — PERFORMANCE ANALYSIS
// ============================================================
void performanceAnalysis() {
    printBanner("SECTION 4  |  PERFORMANCE ANALYSIS");

    cout << "\n  Comparing Arrays vs Singly Linked Lists for this dataset\n" << endl;

    // ---- Operation Complexity Table ----
    printSep();
    cout << left
         << setw(27) << "  Operation"
         << setw(18) << "Array"
         << "Singly Linked List" << endl;
    printSep();
    cout << left << setw(27) << "  Access by index"   << setw(18) << "O(1)"       << "O(n)"                    << endl;
    cout << left << setw(27) << "  Linear Search"     << setw(18) << "O(n)"       << "O(n)"                    << endl;
    cout << left << setw(27) << "  Binary Search"     << setw(18) << "O(log n)"   << "Not directly applicable" << endl;
    cout << left << setw(27) << "  Jump Search"       << setw(18) << "N/A"        << "O(sqrt n)"               << endl;
    cout << left << setw(27) << "  Bubble Sort"       << setw(18) << "O(n^2)"     << "O(n^2)"                  << endl;
    cout << left << setw(27) << "  Selection Sort"    << setw(18) << "O(n^2)"     << "O(n^2)"                  << endl;
    cout << left << setw(27) << "  Merge Sort"        << setw(18) << "O(n log n)" << "O(n log n)"              << endl;
    cout << left << setw(27) << "  Insert at front"   << setw(18) << "O(n)"       << "O(1)"                    << endl;
    cout << left << setw(27) << "  Insert at end"     << setw(18) << "O(1)"       << "O(1) with tail pointer"  << endl;
    cout << left << setw(27) << "  Delete element"    << setw(18) << "O(n)"       << "O(n)"                    << endl;
    cout << left << setw(27) << "  Memory per record" << setw(18) << "Minimal"    << "+8 bytes/node (pointer)" << endl;
    printSep();

    // ---- Arrays ----
    cout << "\n  Array Advantages:" << endl;
    cout << "    - O(1) random index access makes Binary Search effective" << endl;
    cout << "    - Contiguous memory layout gives better CPU cache performance" << endl;
    cout << "    - Simpler index-based sorting logic" << endl;
    cout << "    - Lower memory overhead — no extra pointer per element" << endl;

    cout << "\n  Array Disadvantages:" << endl;
    cout << "    - Fixed size must be declared in advance" << endl;
    cout << "    - Inserting at front requires shifting all elements: O(n)" << endl;

    // ---- Linked Lists ----
    cout << "\n  Singly Linked List Disadvantages:" << endl;
    cout << "    - No random access — must traverse from head to reach element i" << endl;
    cout << "    - Binary Search cannot be applied directly" << endl;
    cout << "    - Each node carries an extra pointer (8 bytes on 64-bit systems)" << endl;

    // ---- Conclusion ----
    cout << "\n  Conclusion for this Project:" << endl;
    cout << "    Arrays perform better here because the dominant operations are" << endl;
    cout << "    reading, searching, and sorting fixed data — all of which benefit" << endl;
    cout << "    from O(1) index access and better cache locality." << endl;
    cout << "    Linked lists would be preferred if records were frequently" << endl;
    cout << "    inserted or deleted at arbitrary positions during runtime." << endl;
    printSep();

    waitForEnter();
}

// ============================================================
//  SECTION 5 — INSIGHTS & RECOMMENDATIONS
// ============================================================
void insightsAndRecommendations() {
    printBanner("SECTION 5  |  INSIGHTS & RECOMMENDATIONS");

    double grandTotal = 0;
    for (int i = 0; i < total; i++) grandTotal += records[i].emission;

    // ---- Cross-City Comparison ----
    cout << "\n  Cross-City Emission Comparison" << endl;
    printSep();
    cout << "  " << left
         << setw(32) << "City"
         << setw(16) << "Total (kg CO2)"
         << setw(16) << "Avg/Resident"
         << "Most Used Mode" << endl;
    printSep();

    for (int c = 0; c < 3; c++) {
        double tot = 0;
        int    cnt = 0;
        int    modeCnt[NUM_MODES] = {};

        for (int i = 0; i < total; i++) {
            if (records[i].city == CITIES[c]) {
                tot += records[i].emission;
                cnt++;
                for (int m = 0; m < NUM_MODES; m++)
                    if (records[i].transport == MODES[m]) modeCnt[m]++;
            }
        }

        int top = 0;
        for (int m = 1; m < NUM_MODES; m++)
            if (modeCnt[m] > modeCnt[top]) top = m;

        cout << "  " << left
             << setw(32) << CITY_NAMES[c]
             << setw(16) << fixed << setprecision(2) << tot
             << setw(16) << fixed << setprecision(2) << (cnt > 0 ? tot / cnt : 0.0)
             << MODES[top] << endl;
    }
    printSep();

    // ---- Age Group Emission Share ----
    cout << "\n  Age Group Emission Share" << endl;
    cout << "  (Grand Total: " << fixed << setprecision(2) << grandTotal << " kg CO2)" << endl;
    printSep();
    cout << "  " << left
         << setw(40) << "Age Group"
         << setw(9)  << "Count"
         << setw(18) << "Total (kg CO2)"
         << "Share (%)" << endl;
    printSep();

    for (int g = 0; g < NUM_GROUPS; g++) {
        double gTotal = 0;
        int    gCnt   = 0;
        for (int i = 0; i < total; i++) {
            if (getGroupIndex(records[i].age) == g) {
                gTotal += records[i].emission;
                gCnt++;
            }
        }
        if (gCnt == 0) continue;
        double pct = grandTotal > 0 ? gTotal / grandTotal * 100.0 : 0.0;
        cout << "  " << left
             << setw(40) << GROUP_LABELS[g]
             << setw(9)  << gCnt
             << setw(18) << fixed << setprecision(2) << gTotal
             << fixed << setprecision(1) << pct << "%" << endl;
    }
    printSep();

    // ---- Policy Recommendations ----
    cout << "\n  Policy Recommendations for Cityville Planners" << endl;
    printSep();
    cout << "  1.  Promote Cycling & Walking for Ages 6-25" << endl;
    cout << "      These modes produce zero carbon emissions. Young residents" << endl;
    cout << "      already favour them — invest in safe cycling lanes and" << endl;
    cout << "      well-lit pedestrian paths to sustain this habit." << endl;
    cout << endl;
    cout << "  2.  Expand Public Bus Networks in City A & City C" << endl;
    cout << "      Cars average ~90.70 kg CO2/month; buses only ~17.04 kg." << endl;
    cout << "      Shifting even 20% of car users to buses could reduce" << endl;
    cout << "      citywide emissions significantly." << endl;
    cout << endl;
    cout << "  3.  Incentivise Carpooling for Working Adults (Ages 26-60)" << endl;
    cout << "      This group produces the largest share of total emissions." << endl;
    cout << "      Parking subsidies and tax credits for carpool participants" << endl;
    cout << "      can make shared commuting economically attractive." << endl;
    cout << endl;
    cout << "  4.  Electrify School Buses in City B" << endl;
    cout << "      Replacing fossil-fuel school buses with electric alternatives" << endl;
    cout << "      immediately reduces emissions for the youngest commuters." << endl;
    cout << endl;
    cout << "  5.  Subsidise Electric Shuttles for Senior Citizens (Ages 61+)" << endl;
    cout << "      Elderly residents may lack accessible public-transport options." << endl;
    cout << "      On-demand electric mini-shuttles can reduce private car use" << endl;
    cout << "      in this age group without sacrificing mobility." << endl;
    printSep();

    waitForEnter();
}

// ============================================================
//  MAIN ENTRY POINT
// ============================================================
int main() {

    // ---- Startup Banner ----
    cout << endl;
    printSep('=');
    cout << "  CT077-3-2  Data Structures & Algorithms  |  Lab Work #1" << endl;
    cout << "  Program 1  -  Array-Based Implementation" << endl;
    cout << "  Carbon Emission Analysis for Cityville" << endl;
    printSep('=');

    // ---- Load Data ----
    cout << "\n  Loading datasets from CSV files..." << endl;
    printSep();
    loadData("dataset1-cityA.csv", "CityA");
    loadData("dataset2-cityB.csv", "CityB");
    loadData("dataset3-cityC.csv", "CityC");
    printSep();
    cout << "  Total records loaded into array: " << total << endl;
    printSep('=');

    // ---- Sample Preview ----
    cout << "\n  Data Preview  (first 10 records from the combined dataset)" << endl;
    printSep();
    printTableHeader();
    for (int i = 0; i < 10 && i < total; i++) printTableRow(records[i]);
    printSep();
    cout << "  Note: " << total << " records are stored. Showing first 10 only." << endl;

    // ---- Main Menu Loop ----
    int choice = -1;
    while (choice != 0) {
        cout << endl;
        printSep('=');
        cout << "  MAIN MENU  —  Array Implementation" << endl;
        printSep('=');
        cout << "  1.  Carbon Emission Analysis" << endl;
        cout << "      Breakdown by city, transport mode, and age group" << endl;
        cout << endl;
        cout << "  2.  Sorting Experiments" << endl;
        cout << "      Bubble Sort vs Selection Sort with timing results" << endl;
        cout << endl;
        cout << "  3.  Searching Experiments" << endl;
        cout << "      Linear Search (unsorted) vs Binary Search (sorted)" << endl;
        cout << endl;
        cout << "  4.  Performance Analysis" << endl;
        cout << "      Time & space complexity — Arrays vs Linked Lists" << endl;
        cout << endl;
        cout << "  5.  Insights & Recommendations" << endl;
        cout << "      Age-group trends and sustainable transport policies" << endl;
        cout << endl;
        cout << "  0.  Exit Program" << endl;
        printSep();
        cout << "  Enter your choice (0-5): ";
        cin >> choice;
        cin.ignore(1000, '\n');  // clear newline from input buffer
        cout << endl;

        if      (choice == 1) carbonAnalysis();
        else if (choice == 2) sortingExperiments();
        else if (choice == 3) searchingExperiments();
        else if (choice == 4) performanceAnalysis();
        else if (choice == 5) insightsAndRecommendations();
        else if (choice != 0) {
            cout << "  [!] Invalid input. Please enter a number between 0 and 5." << endl;
        }
    }

    // ---- Exit ----
    cout << endl;
    printSep('=');
    cout << "  Thank you for using the Cityville Carbon Emission Analyser." << endl;
    cout << "  Program 1 (Array Implementation) terminated." << endl;
    printSep('=');
    cout << endl;

    return 0;
}
