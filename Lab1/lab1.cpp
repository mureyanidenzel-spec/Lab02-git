#include <iostream>
#include <vector>
using namespace std;

vector<double> input_numbers(size_t count) {
    vector<double> result(count);
    for (size_t i = 0; i < count; i++) {
        cin >> result[i];
    }
    return result;
}

void find_minmax(const vector<double>& numbers, double& min, double& max) {
    if (numbers.empty()) return;
    min = numbers[0];
    max = numbers[0];
    // Use index-based loop instead of range-based
    for (size_t i = 0; i < numbers.size(); i++) {
        if (numbers[i] < min) min = numbers[i];
        if (numbers[i] > max) max = numbers[i];
    }
}

vector<size_t> make_histogram(const vector<double>& numbers, size_t bin_count) {
    if (numbers.empty() || bin_count == 0) {
        return vector<size_t>();
    }
    
    double min, max;
    find_minmax(numbers, min, max);
    
    vector<size_t> bins(bin_count, 0);
    double bin_size = (max - min) / bin_count;
    
    for (size_t i = 0; i < numbers.size(); i++) {
        bool found = false;
        for (size_t j = 0; j < bin_count - 1 && !found; j++) {
            double lo = min + j * bin_size;
            double hi = min + (j + 1) * bin_size;
            if (lo <= numbers[i] && numbers[i] < hi) {
                bins[j]++;
                found = true;
            }
        }
        if (!found) {
            bins[bin_count - 1]++;
        }
    }
    return bins;
}

void show_histogram(const vector<size_t>& bins) {
    // Use index-based loop instead of range-based
    for (size_t i = 0; i < bins.size(); i++) {
        size_t count = bins[i];
        if (count < 100) cout << " ";
        if (count < 10) cout << " ";
        cout << count << "|";
        for (size_t j = 0; j < count; j++) {
            cout << "*";
        }
        cout << endl;
    }
}

int main() {
    size_t number_count;
    cerr << "Enter number count: ";
    cin >> number_count;
    
    // Use explicit type instead of 'auto'
    vector<double> numbers = input_numbers(number_count);
    
    size_t bin_count;
    cerr << "Enter bin count: ";
    cin >> bin_count;
    
    // Use explicit type instead of 'auto'
    vector<size_t> bins = make_histogram(numbers, bin_count);
    show_histogram(bins);
    
    return 0;
}