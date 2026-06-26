/*
 * ============================================================
 *  SELECTION SORT - Analisis Algoritma Sorting
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

// Selection Sort dengan timeout check
// Returns: waktu dalam ms, -1 jika timeout
double selectionSort(vector<int>& arr) {
    int n = arr.size();
    auto startTime = high_resolution_clock::now();

    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx])
                minIdx = j;
        }
        if (minIdx != i)
            swap(arr[minIdx], arr[i]);

        // Cek timeout setiap 100 iterasi luar
        if (i % 100 == 0) {
            auto now = high_resolution_clock::now();
            double elapsed = duration_cast<milliseconds>(now - startTime).count() / 1000.0;
            if (elapsed > TIMEOUT_SECONDS) {
                return -1.0; // TIMEOUT
            }
        }
    }

    auto endTime = high_resolution_clock::now();
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
    cout << "           SELECTION SORT - ANALISIS PERFORMA" << endl;
    cout << "           Timeout: 20 menit per ukuran data" << endl;
    cout << "============================================================" << endl;
    cout << endl;
    cout << left << setw(15) << "Ukuran Data"
         << setw(20) << "Waktu (ms)"
         << setw(20) << "Waktu (detik)"
         << setw(15) << "Status" << endl;
    cout << string(70, '-') << endl;

    ofstream logFile("selection_sort_results.txt");
    logFile << "Ukuran Data,Waktu (ms),Waktu (detik),Status" << endl;

    for (int n : sizes) {
        cout << left << setw(15) << n;
        cout.flush();

        // Skip otomatis jika data terlalu besar
        if (n > 100000) {
            cout << setw(20) << "N/A"
                 << setw(20) << "N/A"
                 << setw(15) << "SKIP (>O(n^2) limit)" << endl;
            logFile << n << ",N/A,N/A,SKIP" << endl;
            continue;
        }

        vector<int> data = generateRandom(n);
        double timeMs = selectionSort(data);

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
    cout << "  Best Case    : O(n^2)    - selalu sama" << endl;
    cout << "  Average Case : O(n^2)    - data acak" << endl;
    cout << "  Worst Case   : O(n^2)    - selalu sama" << endl;
    cout << "Kompleksitas Ruang: O(1)   - in-place sorting" << endl;
    cout << "============================================================" << endl;
    cout << "Hasil disimpan di: selection_sort_results.txt" << endl;

    logFile.close();
    return 0;
}
