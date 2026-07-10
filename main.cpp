#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

// ============================================
// PART 1: SVG FUNCTIONS
// ============================================

// Start SVG document
void svg_begin(double width, double height) {
    cout << "<?xml version='1.0' encoding='UTF-8'?>\n";
    cout << "<svg ";
    cout << "width='" << width << "' ";
    cout << "height='" << height << "' ";
    cout << "viewBox='0 0 " << width << " " << height << "' ";
    cout << "xmlns='http://www.w3.org/2000/svg'>\n";
}

// End SVG document
void svg_end() {
    cout << "</svg>\n";
}

// Draw text at position
void svg_text(double left, double baseline, string text) {
    cout << "<text x='" << left << "' y='" << baseline 
         << "' font-family='Arial' font-size='14' fill='black'>" 
         << text << "</text>\n";
}

// Draw rectangle with optional colors
void svg_rect(double x, double y, double width, double height, 
              string stroke = "steelblue", string fill = "#4A90D9") {
    cout << "<rect x='" << x << "' y='" << y 
         << "' width='" << width << "' height='" << height 
         << "' stroke='" << stroke << "' fill='" << fill << "'/>\n";
}

// ============================================
// PART 2: HISTOGRAM FUNCTIONS
// ============================================

// Input numbers from user
vector<double> input_numbers(size_t count) {
    vector<double> result(count);
    for (size_t i = 0; i < count; i++) {
        cin >> result[i];
    }
    return result;
}

// Find minimum and maximum values
void find_minmax(const vector<double>& numbers, double& min, double& max) {
    if (numbers.empty()) {
        return;
    }
    min = numbers[0];
    max = numbers[0];
    for (double x : numbers) {
        if (x < min) min = x;
        if (x > max) max = x;
    }
}

// Create histogram bins
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

// PART 3: SVG HISTOGRAM - VARIANT 1


void show_histogram_svg(const vector<size_t>& bins, size_t custom_width, size_t number_count) {
    // ----- LAYOUT CONSTANTS -----
    const double IMAGE_HEIGHT = 300;      // Total height
    const double TEXT_LEFT = 20;          // X position for labels
    const double TEXT_BASELINE = 20;      // Y position for text baseline
    const double TEXT_WIDTH = 50;         // Width reserved for labels
    const double BIN_HEIGHT = 30;         // Height of each bar
    const double BLOCK_WIDTH = 10;        // Width per count
    const double GAP = 5;                 // Gap between bars
    
    // ----- VARIANT 1: VALIDATE CUSTOM WIDTH -----
    const size_t MIN_WIDTH = 70;
    const size_t MAX_WIDTH = 800;
    size_t valid_width = custom_width;
    
    // Check rule 1: width >= 70
    if (custom_width < MIN_WIDTH) {
        cerr << "Warning: Width " << custom_width << " is less than minimum " << MIN_WIDTH 
             << ". Using " << MIN_WIDTH << " instead." << endl;
        valid_width = MIN_WIDTH;
    }
    
    // Check rule 2: width <= 800
    if (custom_width > MAX_WIDTH) {
        cerr << "Warning: Width " << custom_width << " exceeds maximum " << MAX_WIDTH 
             << ". Using " << MAX_WIDTH << " instead." << endl;
        valid_width = MAX_WIDTH;
    }
    
    // Check rule 3: width >= number_count * BLOCK_WIDTH / 3
    size_t min_width = number_count * BLOCK_WIDTH / 3;
    if (valid_width < min_width && number_count > 0) {
        cerr << "Warning: Width " << valid_width << " is less than one-third of values * block width (" 
             << min_width << "). Using " << min_width << " instead." << endl;
        valid_width = min_width;
    }
    
    const double IMAGE_WIDTH = valid_width;
    
    // ----- DRAW SVG HEADER -----
    svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT);
    
    // ----- FIND MAX COUNT FOR SCALING -----
    size_t max_count = 0;
    for (size_t count : bins) {
        if (count > max_count) max_count = count;
    }
    
    // ----- CALCULATE SCALING -----
    double max_bin_width = IMAGE_WIDTH - TEXT_WIDTH - 20; // Leave right margin
    double scale = 1.0;
    if (max_count > 0) {
        scale = max_bin_width / max_count;
    }
    
    // ----- DRAW HISTOGRAM BARS -----
    double top = 0;
    for (size_t bin : bins) {
        // Calculate bar width with scaling
        double bin_width = BLOCK_WIDTH * bin * scale;
        
        // Draw label (the number on left)
        svg_text(TEXT_LEFT, top + TEXT_BASELINE, to_string(bin));
        
        // Draw bar (rectangle)
        svg_rect(TEXT_WIDTH, top, bin_width, BIN_HEIGHT);
        
        // Move down for next bar
        top += BIN_HEIGHT + GAP;
    }
    
    // ----- DRAW SVG FOOTER -----
    svg_end();
}

// ============================================
// PART 4: MAIN FUNCTION
// ============================================

int main() {
    // ----- INPUT: Number Count -----
    size_t number_count;
    cerr << "Enter number count: ";
    cin >> number_count;
    
    // ----- INPUT: Numbers -----
    const auto numbers = input_numbers(number_count);
    
    // ----- INPUT: Bin Count -----
    size_t bin_count;
    cerr << "Enter bin count: ";
    cin >> bin_count;
    
    // ----- VARIANT 1: INPUT CUSTOM WIDTH WITH VALIDATION -----
    size_t custom_width;
    const size_t MIN_WIDTH = 70;
    const size_t MAX_WIDTH = 800;
    const size_t BLOCK_WIDTH = 10;
    
    while (true) {
        cerr << "Enter SVG image width (" << MIN_WIDTH << "-" << MAX_WIDTH 
             << ", at least " << number_count * BLOCK_WIDTH / 3 << "): ";
        cin >> custom_width;
        
        bool valid = true;
        
        // Check rule 1: width >= 70
        if (custom_width < MIN_WIDTH) {
            cerr << "Error: Width must be at least " << MIN_WIDTH << " pixels." << endl;
            valid = false;
        }
        
        // Check rule 2: width <= 800
        if (custom_width > MAX_WIDTH) {
            cerr << "Error: Width must not exceed " << MAX_WIDTH << " pixels." << endl;
            valid = false;
        }
        
        // Check rule 3: width >= number_count * BLOCK_WIDTH / 3
        size_t min_width = number_count * BLOCK_WIDTH / 3;
        if (number_count > 0 && custom_width < min_width) {
            cerr << "Error: Width must be at least one-third of values * block width (" 
                 << min_width << ")." << endl;
            valid = false;
        }
        
        if (valid) break;
    }
    
    // ----- PROCESS: Create Histogram -----
    const auto bins = make_histogram(numbers, bin_count);
    
    // ----- OUTPUT: Display as SVG -----
    show_histogram_svg(bins, custom_width, number_count);
    
    return 0;
}
