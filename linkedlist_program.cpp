// CT077-3-2-DSTR Lab Work #1
// Program 2 - Singly Linked List Implementation
// Carbon Emission Analysis for Cityville

#include "carbon_emission.hpp"

// linked list variables
Node* head = nullptr;
Node* tail = nullptr;
int listSize = 0;

// add a new node at the end of the list
void appendNode(Resident d) {
    Node* newNode = new Node();
    newNode->data = d;
    newNode->next = nullptr;

    if (head == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    listSize++;
}

// free all nodes in list
void clearList(Node*& h) {
    Node* curr = h;
    while (curr != nullptr) {
        Node* temp = curr;
        curr = curr->next;
        delete temp;
    }
    h = nullptr;
}

// load csv file into linked list
void loadData(string filename, string cityName) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: cannot open " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line.back() == '\r') line.pop_back();

        stringstream ss(line);
        string token;
        Resident r;

        getline(ss, token, ','); r.id        = trim(token);
        getline(ss, token, ','); r.age       = stoi(trim(token));
        getline(ss, token, ','); r.transport = trim(token);
        getline(ss, token, ','); r.distance  = stod(trim(token));
        getline(ss, token, ',');
        string ef = trim(token);
        r.emFactor = ef.empty() ? 0.0 : stod(ef);
        getline(ss, token, ',');
        string d = trim(token);
        r.days = d.empty() ? 0 : stoi(d);

        r.emission = r.distance * r.emFactor * r.days;
        r.city = cityName;

        appendNode(r);
    }
    file.close();
    cout << "Loaded " << filename << endl;
}


// ---- CARBON EMISSION ANALYSIS ----
void carbonAnalysis() {
    cout << "\n========== CARBON EMISSION ANALYSIS (LINKED LIST) ==========" << endl;

    string cities[]    = {"CityA", "CityB", "CityC"};
    string cityNames[] = {"City A (Metropolitan)", "City B (University Town)", "City C (Suburban/Rural)"};
    string modes[]     = {"Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool"};
    string groupNames[] = {
        "6-17  (Children & Teens)",
        "18-25 (Young Adults)",
        "26-45 (Working Early)",
        "46-60 (Working Late)",
        "61-100 (Seniors)"
    };

    // total per city - traverse the list
    cout << "\n-- Total Emission Per Dataset --" << endl;
    printSep();
    cout << left << setw(30) << "City" << setw(10) << "Nodes" << "Total Emission (kg CO2)" << endl;
    printSep();

    double grandTotal = 0;
    for (int c = 0; c < 3; c++) {
        double cityTotal = 0;
        int count = 0;
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->data.city == cities[c]) {
                cityTotal += curr->data.emission;
                count++;
            }
            curr = curr->next;
        }
        cout << left << setw(30) << cityNames[c] << setw(10) << count
             << fixed << setprecision(2) << cityTotal << " kg" << endl;
        grandTotal += cityTotal;
    }
    printSep();
    cout << left << setw(30) << "GRAND TOTAL" << setw(10) << listSize
         << fixed << setprecision(2) << grandTotal << " kg" << endl;
    printSep();

    // per mode
    cout << "\n-- Emission Per Transport Mode --" << endl;
    printSep();
    cout << left << setw(14) << "Mode" << setw(9) << "Count"
         << setw(22) << "Total (kg CO2)" << "Avg Per Resident" << endl;
    printSep();

    for (int m = 0; m < 6; m++) {
        double mTotal = 0;
        int mCount = 0;
        Node* curr = head;
        while (curr != nullptr) {
            if (curr->data.transport == modes[m]) {
                mTotal += curr->data.emission;
                mCount++;
            }
            curr = curr->next;
        }
        if (mCount == 0) continue;
        cout << left << setw(14) << modes[m] << setw(9) << mCount
             << setw(22) << fixed << setprecision(2) << mTotal
             << fixed << setprecision(2) << mTotal / mCount << " kg" << endl;
    }
    printSep();

    // per age group
    cout << "\n-- Emission Per Age Group --" << endl;

    for (int g = 0; g < 5; g++) {
        double modeEm[6]  = {0};
        int    modeCnt[6] = {0};
        double gTotal = 0;
        int    gCount = 0;

        Node* curr = head;
        while (curr != nullptr) {
            if (getGroupIndex(curr->data.age) == g) {
                gTotal += curr->data.emission;
                gCount++;
                for (int m = 0; m < 6; m++)
                    if (curr->data.transport == modes[m]) {
                        modeEm[m]  += curr->data.emission;
                        modeCnt[m] ++;
                    }
            }
            curr = curr->next;
        }

        if (gCount == 0) continue;

        int bestMode = 0;
        for (int m = 1; m < 6; m++)
            if (modeCnt[m] > modeCnt[bestMode]) bestMode = m;

        cout << "\nAge Group: " << groupNames[g] << endl;
        printSep();
        cout << left << setw(14) << "Mode" << setw(9) << "Count"
             << setw(22) << "Total (kg CO2)" << "Avg Per Resident" << endl;
        printSep();

        for (int m = 0; m < 6; m++) {
            if (modeCnt[m] == 0) continue;
            cout << left << setw(14) << modes[m] << setw(9) << modeCnt[m]
                 << setw(22) << fixed << setprecision(2) << modeEm[m]
                 << fixed << setprecision(2) << modeEm[m] / modeCnt[m] << " kg" << endl;
        }
        printSep();
        cout << "Total Residents : " << gCount << endl;
        cout << "Total Emission  : " << fixed << setprecision(2) << gTotal << " kg CO2" << endl;
        cout << "Avg per Resident: " << fixed << setprecision(2) << gTotal / gCount << " kg CO2" << endl;
        cout << "Most Preferred  : " << modes[bestMode] << " (" << modeCnt[bestMode] << " people)" << endl;
    }
}

// ---- BUBBLE SORT ON LINKED LIST ----
// swaps data between nodes (not pointers)
// field: 0 = age, 1 = distance, 2 = emission
void bubbleSortList(Node* h, int field) {
    if (h == nullptr) return;

    bool swapped;
    do {
        swapped = false;
        Node* curr = h;
        while (curr != nullptr && curr->next != nullptr) {
            bool doSwap = false;
            if (field == 0 && curr->data.age      > curr->next->data.age)      doSwap = true;
            if (field == 1 && curr->data.distance > curr->next->data.distance) doSwap = true;
            if (field == 2 && curr->data.emission > curr->next->data.emission) doSwap = true;

            if (doSwap) {
                Resident temp = curr->data;
                curr->data        = curr->next->data;
                curr->next->data  = temp;
                swapped = true;
            }
            curr = curr->next;
        }
    } while (swapped);
}

// ---- MERGE SORT ON LINKED LIST ----
// split list into two halves using slow/fast pointer
void splitHalf(Node* source, Node** front, Node** back) {
    Node* fast = source->next;
    Node* slow = source;

    while (fast != nullptr) {
        fast = fast->next;
        if (fast != nullptr) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *front = source;
    *back  = slow->next;
    slow->next = nullptr;
}

// merge two sorted lists
Node* mergeSorted(Node* a, Node* b, int field) {
    if (a == nullptr) return b;
    if (b == nullptr) return a;

    bool aIsSmaller = false;
    if (field == 0 && a->data.age      <= b->data.age)      aIsSmaller = true;
    if (field == 1 && a->data.distance <= b->data.distance) aIsSmaller = true;
    if (field == 2 && a->data.emission <= b->data.emission) aIsSmaller = true;

    if (aIsSmaller) {
        a->next = mergeSorted(a->next, b, field);
        return a;
    } else {
        b->next = mergeSorted(a, b->next, field);
        return b;
    }
}

void mergeSort(Node** headRef, int field) {
    Node* h = *headRef;
    if (h == nullptr || h->next == nullptr) return;

    Node* front = nullptr;
    Node* back  = nullptr;
    splitHalf(h, &front, &back);

    mergeSort(&front, field);
    mergeSort(&back,  field);

    *headRef = mergeSorted(front, back, field);
}

// make a copy of the linked list
Node* copyList(Node* src) {
    if (src == nullptr) return nullptr;

    Node* newHead = nullptr;
    Node* newTail = nullptr;
    Node* curr = src;

    while (curr != nullptr) {
        Node* newNode = new Node();
        newNode->data = curr->data;
        newNode->next = nullptr;

        if (newHead == nullptr) {
            newHead = newTail = newNode;
        } else {
            newTail->next = newNode;
            newTail = newNode;
        }
        curr = curr->next;
    }
    return newHead;
}

void sortingExperiments() {
    cout << "\n========== SORTING EXPERIMENTS (LINKED LIST) ==========" << endl;

    string fields[] = {"Age", "Daily Distance", "Monthly Emission"};

    cout << left << setw(20) << "Algorithm" << setw(22) << "Sort By"
         << setw(10) << "Nodes" << "Time (us)" << endl;
    printSep();

    for (int f = 0; f < 3; f++) {
        // bubble sort copy
        Node* copy1 = copyList(head);
        auto t1 = chrono::high_resolution_clock::now();
        bubbleSortList(copy1, f);
        auto t2 = chrono::high_resolution_clock::now();
        long long us1 = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        clearList(copy1);

        // merge sort copy
        Node* copy2 = copyList(head);
        auto t3 = chrono::high_resolution_clock::now();
        mergeSort(&copy2, f);
        auto t4 = chrono::high_resolution_clock::now();
        long long us2 = chrono::duration_cast<chrono::microseconds>(t4 - t3).count();
        clearList(copy2);

        cout << left << setw(20) << "Bubble Sort (LL)"  << setw(22) << fields[f] << setw(10) << listSize << us1 << " us" << endl;
        cout << left << setw(20) << "Merge Sort (LL)"   << setw(22) << fields[f] << setw(10) << listSize << us2 << " us" << endl;
    }
    printSep();

    // top 10 highest emitters using merge sort
    Node* sorted = copyList(head);
    mergeSort(&sorted, 2); // sort by emission ascending

    // count nodes and go to last 10
    int count = 0;
    Node* curr = sorted;
    while (curr != nullptr) { count++; curr = curr->next; }

    // collect last 10 into array to print in reverse
    Resident top10[10];
    int idx = 0;
    curr = sorted;
    int skip = count - 10;
    for (int i = 0; i < skip && curr != nullptr; i++) curr = curr->next;
    while (curr != nullptr && idx < 10) { top10[idx++] = curr->data; curr = curr->next; }

    cout << "\nTop 10 Highest Emitters (Merge Sort):" << endl;
    printSep();
    printTableHeader();
    for (int i = idx - 1; i >= 0; i--) printTableRow(top10[i]);
    printSep();
    clearList(sorted);

    // first 10 youngest using bubble sort
    Node* sorted2 = copyList(head);
    bubbleSortList(sorted2, 0); // sort by age

    cout << "\nFirst 10 Youngest Residents (Bubble Sort):" << endl;
    printSep();
    printTableHeader();
    curr = sorted2;
    for (int i = 0; i < 10 && curr != nullptr; i++) {
        printTableRow(curr->data);
        curr = curr->next;
    }
    printSep();
    clearList(sorted2);

    // complexity reference
    cout << "\n-- Sorting Complexity --" << endl;
    printSep();
    cout << left << setw(20) << "Algorithm" << setw(14) << "Best Case"
         << setw(14) << "Worst Case" << "Space" << endl;
    printSep();
    cout << left << setw(20) << "Bubble Sort (LL)" << setw(14) << "O(n)"
         << setw(14) << "O(n^2)" << "O(1)" << endl;
    cout << left << setw(20) << "Merge Sort (LL)"  << setw(14) << "O(n log n)"
         << setw(14) << "O(n log n)" << "O(log n)" << endl;
    printSep();
}

// ---- LINEAR SEARCH ----
void linearSearchMode(string mode) {
    cout << "\nLinear Search - Mode = '" << mode << "'" << endl;

    auto t1 = chrono::high_resolution_clock::now();
    int found = 0;
    Node* results[600];
    Node* curr = head;
    while (curr != nullptr) {
        if (curr->data.transport == mode) results[found++] = curr;
        curr = curr->next;
    }
    auto t2 = chrono::high_resolution_clock::now();
    long long us = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

    cout << "Time: " << us << " us | Found: " << found << " records" << endl;
    printSep();
    printTableHeader();
    int show = min(found, 15);
    for (int i = 0; i < show; i++) printTableRow(results[i]->data);
    printSep();
    if (found > 15) cout << "(showing 15 of " << found << ")" << endl;
}

void linearSearchGroup(int groupIdx) {
    string groupNames[] = {"6-17", "18-25", "26-45", "46-60", "61-100"};
    cout << "\nLinear Search - Age Group " << groupNames[groupIdx] << endl;

    auto t1 = chrono::high_resolution_clock::now();
    int found = 0;
    Node* results[600];
    Node* curr = head;
    while (curr != nullptr) {
        if (getGroupIndex(curr->data.age) == groupIdx) results[found++] = curr;
        curr = curr->next;
    }
    auto t2 = chrono::high_resolution_clock::now();
    long long us = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

    cout << "Time: " << us << " us | Found: " << found << " records" << endl;
    printSep();
    printTableHeader();
    int show = min(found, 15);
    for (int i = 0; i < show; i++) printTableRow(results[i]->data);
    printSep();
    if (found > 15) cout << "(showing 15 of " << found << ")" << endl;
}

void linearSearchDistance(double minDist) {
    cout << "\nLinear Search - Distance > " << minDist << " km" << endl;

    auto t1 = chrono::high_resolution_clock::now();
    int found = 0;
    Node* results[600];
    Node* curr = head;
    while (curr != nullptr) {
        if (curr->data.distance > minDist) results[found++] = curr;
        curr = curr->next;
    }
    auto t2 = chrono::high_resolution_clock::now();
    long long us = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

    cout << "Time: " << us << " us | Found: " << found << " records" << endl;
    printSep();
    printTableHeader();
    int show = min(found, 15);
    for (int i = 0; i < show; i++) printTableRow(results[i]->data);
    printSep();
    if (found > 15) cout << "(showing 15 of " << found << ")" << endl;
}

// ---- JUMP SEARCH ON LINKED LIST ----
// list must be sorted by age first
// step size = sqrt(n)
void jumpSearchAge(Node* sortedHead, int n, int targetAge) {
    cout << "\nJump Search - Age = " << targetAge << " (on sorted list)" << endl;

    auto t1 = chrono::high_resolution_clock::now();

    int step = (int)sqrt((double)n);
    Node* prev = sortedHead;
    Node* curr = sortedHead;
    int pos = 0;

    // jump through list until we pass target or reach end
    while (curr != nullptr && curr->data.age < targetAge) {
        prev = curr;
        for (int i = 0; i < step && curr != nullptr; i++) {
            curr = curr->next;
            pos++;
        }
    }

    // linear scan from prev onwards
    Node* results[600];
    int found = 0;
    Node* scan = prev;
    while (scan != nullptr && scan->data.age <= targetAge) {
        if (scan->data.age == targetAge) results[found++] = scan;
        scan = scan->next;
    }

    auto t2 = chrono::high_resolution_clock::now();
    long long us = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

    cout << "Time: " << us << " us | Found: " << found << " records" << endl;
    printSep();
    printTableHeader();
    for (int i = 0; i < found; i++) printTableRow(results[i]->data);
    printSep();
}

void searchingExperiments() {
    cout << "\n========== SEARCHING EXPERIMENTS (LINKED LIST) ==========" << endl;

    // linear searches on unsorted list
    linearSearchMode("Car");
    linearSearchMode("Bicycle");
    linearSearchGroup(2);     // 26-45
    linearSearchDistance(15.0);

    // jump search needs sorted list
    Node* sortedByAge = copyList(head);
    mergeSort(&sortedByAge, 0); // sort by age

    jumpSearchAge(sortedByAge, listSize, 22);
    jumpSearchAge(sortedByAge, listSize, 30);
    clearList(sortedByAge);

    // comparison
    cout << "\n-- Search Algorithm Comparison --" << endl;
    printSep();
    cout << left << setw(5) << "#" << setw(20) << "Algorithm" << setw(15) << "Time Complexity"
         << "Notes" << endl;
    printSep();
    cout << left << setw(5) << "1" << setw(20) << "Linear Search" << setw(15) << "O(n)"
         << "Works on any list, no sorting needed" << endl;
    cout << left << setw(5) << "2" << setw(20) << "Jump Search"   << setw(15) << "O(sqrt n)"
         << "Needs sorted list, faster than linear" << endl;
    printSep();
}

// ---- PERFORMANCE ANALYSIS ----
void performanceAnalysis() {
    cout << "\n========== PERFORMANCE ANALYSIS ==========" << endl;
    cout << "Comparing Arrays vs Singly Linked Lists\n" << endl;

    // memory estimation
    int nodeSize  = sizeof(Resident) + sizeof(Node*);
    int arraySize = sizeof(Resident);
    cout << "Memory estimate for " << listSize << " records:" << endl;
    cout << " Array:       " << listSize * arraySize << " bytes" << endl;
    cout << " Linked List: " << listSize * nodeSize  << " bytes" << endl;
    cout << " Extra cost:  " << listSize * (nodeSize - arraySize) << " bytes (pointers)" << endl;
    cout << endl;

    printSep();
    cout << left << setw(25) << "Operation" << setw(18) << "Array" << "Singly Linked List" << endl;
    printSep();
    cout << left << setw(25) << "Access by index"  << setw(18) << "O(1)"     << "O(n)" << endl;
    cout << left << setw(25) << "Linear Search"    << setw(18) << "O(n)"     << "O(n)" << endl;
    cout << left << setw(25) << "Jump Search"      << setw(18) << "N/A"      << "O(sqrt n)" << endl;
    cout << left << setw(25) << "Binary Search"    << setw(18) << "O(log n)" << "Not possible directly" << endl;
    cout << left << setw(25) << "Bubble Sort"      << setw(18) << "O(n^2)"   << "O(n^2)" << endl;
    cout << left << setw(25) << "Merge Sort"       << setw(18) << "O(n logn)"<< "O(n log n)" << endl;
    cout << left << setw(25) << "Insert at front"  << setw(18) << "O(n)"     << "O(1)" << endl;
    cout << left << setw(25) << "Insert at end"    << setw(18) << "O(1)"     << "O(1) with tail pointer" << endl;
    cout << left << setw(25) << "Delete element"   << setw(18) << "O(n)"     << "O(n)" << endl;
    printSep();

    cout << "\nSingly Linked List Disadvantages:" << endl;
    cout << "  - No random access, must traverse from head each time" << endl;
    cout << "  - Binary Search cannot be used directly (needs index)" << endl;
    cout << "  - Each node carries an extra 8-byte pointer" << endl;
    cout << "  - Poor cache locality due to scattered memory allocation" << endl;
    cout << endl;
    cout << "Array Advantages:" << endl;
    cout << "  - O(1) random index access makes Binary Search effective" << endl;
    cout << "  - Contiguous memory gives better CPU cache performance" << endl;
    cout << "  - Lower memory overhead, no extra pointer per element" << endl;
    cout << endl;
    cout << "Array Disadvantages:" << endl;
    cout << "  - Fixed capacity must be declared at compile time" << endl;
    cout << "  - Inserting at front requires O(n) element shifting" << endl;
    cout << endl;
    cout << "Conclusion for this Project:" << endl;
    cout << "  Linked lists are a natural fit when data arrives incrementally" << endl;
    cout << "  and records are inserted or removed often. However, for this" << endl;
    cout << "  dataset (fixed 600 records, mostly read-only), arrays offer" << endl;
    cout << "  better search performance thanks to O(1) index access." << endl;
    printSep();
}

// ---- INSIGHTS AND RECOMMENDATIONS ----
void insightsAndRecommendations() {
    cout << "\n========== INSIGHTS AND RECOMMENDATIONS ==========" << endl;

    string modes[]     = {"Car", "Bus", "Bicycle", "Walking", "School Bus", "Carpool"};
    string cities[]    = {"CityA", "CityB", "CityC"};
    string cityNames[] = {"City A (Metropolitan)", "City B (University Town)", "City C (Suburban)"};
    string groupNames[] = {
        "6-17  (Children & Teens)",
        "18-25 (Young Adults)",
        "26-45 (Working Early)",
        "46-60 (Working Late)",
        "61-100 (Seniors)"
    };

    double grandTotal = 0;
    Node* curr = head;
    while (curr != nullptr) { grandTotal += curr->data.emission; curr = curr->next; }

    // per city
    cout << "\n-- Cross City Comparison --" << endl;
    printSep();
    cout << left << setw(28) << "City" << setw(14) << "Total (kg)"
         << setw(14) << "Avg/Person" << "Top Mode" << endl;
    printSep();

    for (int c = 0; c < 3; c++) {
        double tot = 0;
        int cnt = 0, modeCnt[6] = {0};

        curr = head;
        while (curr != nullptr) {
            if (curr->data.city == cities[c]) {
                tot += curr->data.emission;
                cnt++;
                for (int m = 0; m < 6; m++)
                    if (curr->data.transport == modes[m]) modeCnt[m]++;
            }
            curr = curr->next;
        }

        int topM = 0;
        for (int m = 1; m < 6; m++)
            if (modeCnt[m] > modeCnt[topM]) topM = m;

        cout << left << setw(28) << cityNames[c]
             << setw(14) << fixed << setprecision(2) << tot
             << setw(14) << fixed << setprecision(2) << (cnt > 0 ? tot / cnt : 0)
             << modes[topM] << endl;
    }
    printSep();

    // per age group
    cout << "\n-- Age Group Emission Share --" << endl;
    printSep();
    cout << left << setw(32) << "Age Group" << setw(10) << "Count"
         << setw(16) << "Total (kg CO2)" << "% of Total" << endl;
    printSep();

    for (int g = 0; g < 5; g++) {
        double gTotal = 0;
        int gCount = 0;
        curr = head;
        while (curr != nullptr) {
            if (getGroupIndex(curr->data.age) == g) {
                gTotal += curr->data.emission;
                gCount++;
            }
            curr = curr->next;
        }
        if (gCount == 0) continue;
        double pct = grandTotal > 0 ? gTotal / grandTotal * 100.0 : 0;
        cout << left << setw(32) << groupNames[g]
             << setw(10) << gCount
             << setw(16) << fixed << setprecision(2) << gTotal
             << fixed << setprecision(1) << pct << "%" << endl;
    }
    printSep();

    cout << "\n-- Recommendations --" << endl;
    printSep();
    cout << "1. Promote cycling and walking - these modes produce 0 kg CO2" << endl;
    cout << "   Target ages 6-25 who already prefer these modes" << endl;
    cout << endl;
    cout << "2. Expand bus networks in City A and City C" << endl;
    cout << "   Car = avg 90.70 kg/month, Bus = avg 17.04 kg/month" << endl;
    cout << "   Switching from car to bus cuts emissions by ~80%" << endl;
    cout << endl;
    cout << "3. Encourage carpooling for working adults (26-60)" << endl;
    cout << "   This group produces over 80% of total emissions" << endl;
    cout << endl;
    cout << "4. Electrify school buses in City B for younger age groups" << endl;
    cout << endl;
    cout << "5. Provide electric shuttles for seniors (61+)" << endl;
    cout << "   Reduce private car use among elderly residents" << endl;
    printSep();
}

int main() {
    cout << "==========================================" << endl;
    cout << " CT077-3-2-DSTR | Lab Work #1" << endl;
    cout << " Program 2 - Singly Linked List Implementation" << endl;
    cout << "==========================================" << endl;

    // load all 3 datasets into linked list
    cout << "\nLoading datasets into linked list..." << endl;
    loadData("dataset1-cityA.csv", "CityA");
    loadData("dataset2-cityB.csv", "CityB");
    loadData("dataset3-cityC.csv", "CityC");
    cout << "Total nodes in list: " << listSize << "\n" << endl;

    // show first 10 nodes
    cout << "-- Sample (first 10 nodes) --" << endl;
    printSep();
    printTableHeader();
    Node* curr = head;
    for (int i = 0; i < 10 && curr != nullptr; i++) {
        printTableRow(curr->data);
        curr = curr->next;
    }
    printSep();

    int choice = -1;
    while (choice != 0) {
        cout << "\n==========================================" << endl;
        cout << " MAIN MENU" << endl;
        cout << "==========================================" << endl;
        cout << " 1. Carbon Emission Analysis" << endl;
        cout << " 2. Sorting Experiments" << endl;
        cout << " 3. Searching Experiments" << endl;
        cout << " 4. Performance Analysis" << endl;
        cout << " 5. Insights & Recommendations" << endl;
        cout << " 0. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if      (choice == 1) carbonAnalysis();
        else if (choice == 2) sortingExperiments();
        else if (choice == 3) searchingExperiments();
        else if (choice == 4) performanceAnalysis();
        else if (choice == 5) insightsAndRecommendations();
        else if (choice != 0) cout << "Invalid option, try again." << endl;
    }

    // free linked list memory
    clearList(head);

    cout << "\nProgram ended. Goodbye!" << endl;
    return 0;
}