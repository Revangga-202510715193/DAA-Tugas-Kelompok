/*
 * ============================================================
 *  QUICK SORT - Analisis Algoritma Sorting
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

// Partition dengan median-of-three pivot untuk menghindari worst case
int partition(vector<int>& arr, int low, int high) {
    // Median of three: pilih pivot terbaik dari low, mid, high
    int mid = low + (high - low) / 2;
    if (arr[low] > arr[mid]) swap(arr[low], arr[mid]);
    if (arr[low] > arr[high]) swap(arr[low], arr[high]);
    if (arr[mid] > arr[high]) swap(arr[mid], arr[high]);
    
    // Pivot = median (di posisi mid), swap ke high-1
    swap(arr[mid], arr[high]);
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Quick Sort rekursif
void quickSortHelper(vector<int>& arr, int low, int high,
                     const high_resolution_clock::time_point& startTime,
                     bool& timedOut) {
    if (timedOut) return;
    if (low < high) {
        // Cek timeout
        auto now = high_resolution_clock::now();
        double elapsed = duration_cast<milliseconds>(now - startTime).count() / 1000.0;
        if (elapsed > TIMEOUT_SECONDS) {
            timedOut = true;
            return;
        }

        int pi = partition(arr, low, high);
        quickSortHelper(arr, low, pi - 1, startTime, timedOut);
        quickSortHelper(arr, pi + 1, high, startTime, timedOut);
    }
}

// Returns: waktu dalam ms, -1 jika timeout
double quickSort(vector<int>& arr) {
    auto startTime = high_resolution_clock::now();
    bool timedOut = false;

    if (!arr.empty())
        quickSortHelper(arr, 0, arr.size() - 1, startTime, timedOut);

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
    cout << "           QUICK SORT - ANALISIS PERFORMA" << endl;
    cout << "           Timeout: 20 menit per ukuran data" << endl;
    cout << "============================================================" << endl;
    cout << endl;
    cout << left << setw(15) << "Ukuran Data"
         << setw(20) << "Waktu (ms)"
         << setw(20) << "Waktu (detik)"
         << setw(15) << "Status" << endl;
    cout << string(70, '-') << endl;

    ofstream logFile("quick_sort_results.txt");
    logFile << "Ukuran Data,Waktu (ms),Waktu (detik),Status" << endl;

    for (int n : sizes) {
        cout << left << setw(15) << n;
        cout.flush();

        vector<int> data = generateRandom(n);
        double timeMs = quickSort(data);

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
    cout << "  Best Case    : O(n log n) - pivot selalu membagi sama rata" << endl;
    cout << "  Average Case : O(n log n) - data acak" << endl;
    cout << "  Worst Case   : O(n^2)     - data sudah terurut (tanpa optimasi)" << endl;
    cout << "  [Implementasi ini menggunakan Median-of-Three untuk hindari worst case]" << endl;
    cout << "Kompleksitas Ruang: O(log n) - rekursi stack" << endl;
    cout << "============================================================" << endl;
    cout << "Hasil disimpan di: quick_sort_results.txt" << endl;

    logFile.close();
    return 0;
}
