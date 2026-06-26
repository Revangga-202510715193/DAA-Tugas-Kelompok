/*
 * ============================================================
 *  MERGE SORT - Analisis Algoritma Sorting
 *  Timeout: 20 menit per ukuran data
 *  Variasi data: 10000, 20000, 50000, 100000, 1000000, 2000000
 * ============================================================
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>
#include <fstream>

using namespace std;
using namespace chrono;

const int TIMEOUT_SECONDS = 1200; // 20 menit

// Generate data random
vector<int> generateRandom(int n, int seed = 42) {
    vector<int> data(n);
    mt19937 rng(seed);
    uniform_int_distribution<int> dist(1, 1000000);
    for (int i = 0; i < n; i++)
        data[i] = dist(rng);
    return data;
}

// Merge dua sub-array
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

// Merge Sort rekursif
void mergeSortHelper(vector<int>& arr, int left, int right,
                     const high_resolution_clock::time_point& startTime,
                     bool& timedOut) {
    if (timedOut) return;
    if (left >= right) return;

    // Cek timeout pada level atas rekursi
    if ((right - left) > 10000) {
        auto now = high_resolution_clock::now();
        double elapsed = duration_cast<milliseconds>(now - startTime).count() / 1000.0;
        if (elapsed > TIMEOUT_SECONDS) {
            timedOut = true;
            return;
        }
    }

    int mid = left + (right - left) / 2;
    mergeSortHelper(arr, left, mid, startTime, timedOut);
    mergeSortHelper(arr, mid + 1, right, startTime, timedOut);
    if (!timedOut)
        merge(arr, left, mid, right);
}

// Returns: waktu dalam ms, -1 jika timeout
double mergeSort(vector<int>& arr) {
    auto startTime = high_resolution_clock::now();
    bool timedOut = false;

    if (!arr.empty())
        mergeSortHelper(arr, 0, arr.size() - 1, startTime, timedOut);

    auto endTime = high_resolution_clock::now();

    if (timedOut) return -1.0;
    return duration_cast<microseconds>(endTime - startTime).count() / 1000.0; // ms
}

// Verifikasi hasil sorting
bool isSorted(const vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); i++)
        if (arr[i] < arr[i-1]) return false;
    return true;
}

int main() {
    vector<int> sizes = {10000, 20000, 50000, 100000, 1000000, 2000000};

    cout << "============================================================" << endl;
    cout << "           MERGE SORT - ANALISIS PERFORMA" << endl;
    cout << "           Timeout: 20 menit per ukuran data" << endl;
    cout << "============================================================" << endl;
    cout << endl;
    cout << left << setw(15) << "Ukuran Data"
         << setw(20) << "Waktu (ms)"
         << setw(20) << "Waktu (detik)"
         << setw(15) << "Status" << endl;
    cout << string(70, '-') << endl;

    ofstream logFile("merge_sort_results.txt");
    logFile << "Ukuran Data,Waktu (ms),Waktu (detik),Status" << endl;

    for (int n : sizes) {
        cout << left << setw(15) << n;
        cout.flush();

        vector<int> data = generateRandom(n);
        double timeMs = mergeSort(data);

        if (timeMs < 0) {
            cout << setw(20) << "TIMEOUT"
                 << setw(20) << "TIMEOUT"
                 << setw(15) << "TIMEOUT (>1200s)" << endl;
            logFile << n << ",TIMEOUT,TIMEOUT,TIMEOUT" << endl;
        } else {
            string status = isSorted(data) ? "BERHASIL" : "ERROR";
            cout << setw(20) << fixed << setprecision(3) << timeMs
                 << setw(20) << fixed << setprecision(3) << timeMs / 1000.0
                 << setw(15) << status << endl;
            logFile << n << "," << fixed << setprecision(3) << timeMs
                    << "," << timeMs / 1000.0 << "," << status << endl;
        }
    }

    cout << endl;
    cout << "============================================================" << endl;
    cout << "Kompleksitas Waktu:" << endl;
    cout << "  Best Case    : O(n log n) - selalu konsisten" << endl;
    cout << "  Average Case : O(n log n) - data acak" << endl;
    cout << "  Worst Case   : O(n log n) - selalu konsisten" << endl;
    cout << "Kompleksitas Ruang: O(n)    - membutuhkan array tambahan" << endl;
    cout << "============================================================" << endl;
    cout << "Hasil disimpan di: merge_sort_results.txt" << endl;

    logFile.close();
    return 0;
}
