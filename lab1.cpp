#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

int main() {
    // Input data
    size_t number_count;
    cerr << "Enter number count: ";
    cin >> number_count;

    vector<double> numbers(number_count);
    cerr << "Enter numbers: ";
    for (size_t i = 0; i < number_count; i++) {
        cin >> numbers[i];
    }

    size_t bin_count;
    cerr << "Enter bin count: ";
    cin >> bin_count;

    // Custom width input with validation
    size_t screen_width;
    const size_t MIN_WIDTH = 7;  // Minimum width: 3 digits + 1 axis + 3 asterisks
    const size_t MAX_WIDTH = 80;
    const size_t MIN_RATIO = 3;  // Width must be at least number_count / 3

    while (true) {
        cerr << "Enter histogram width (" << MIN_WIDTH << "-" << MAX_WIDTH 
             << ", at least " << number_count / MIN_RATIO << "): ";
        cin >> screen_width;

        bool valid = true;
        if (screen_width < MIN_WIDTH) {
            cerr << "Error: Width must be at least " << MIN_WIDTH << " characters." << endl;
            valid = false;
        }
        if (screen_width > MAX_WIDTH) {
            cerr << "Error: Width must not exceed " << MAX_WIDTH << " characters." << endl;
            valid = false;
        }
        if (screen_width < number_count / MIN_RATIO) {
            cerr << "Error: Width must be at least one-third of the number of values (" 
                 << number_count / MIN_RATIO << ")." << endl;
            valid = false;
        }

        if (valid) break;
    }

    // Find min and max
    double min_val = numbers[0];
    double max_val = numbers[0];
    for (double x : numbers) {
        if (x < min_val) min_val = x;
        if (x > max_val) max_val = x;
    }

    // Calculate bin counts
    vector<size_t> bins(bin_count, 0);
    double bin_size = (max_val - min_val) / bin_count;

    for (size_t i = 0; i < number_count; i++) {
        bool found = false;
        for (size_t j = 0; j < bin_count - 1 && !found; j++) {
            double lo = min_val + j * bin_size;
            double hi = min_val + (j + 1) * bin_size;
            if (lo <= numbers[i] && numbers[i] < hi) {
                bins[j]++;
                found = true;
            }
        }
        if (!found) {
            bins[bin_count - 1]++;
        }
    }

    // Find maximum count in any bin
    size_t max_count = 0;
    for (size_t count : bins) {
        if (count > max_count) max_count = count;
    }

    // Calculate maximum asterisks (accounting for 3 digits + 1 axis)
    const size_t MAX_ASTERISK = screen_width - 3 - 1;

    // Display histogram
    for (size_t count : bins) {
        // Display aligned count (3 characters)
        if (count < 100) cout << " ";
        if (count < 10) cout << " ";
        cout << count << "|";

        // Calculate and display asterisks
        size_t height;
        if (max_count <= MAX_ASTERISK) {
            height = count;
        } else {
            height = MAX_ASTERISK * (static_cast<double>(count) / max_count);
        }

        for (size_t i = 0; i < height; i++) {
            cout << "*";
        }
        cout << endl;
    }

    return 0;
}
