#include <iostream>
#include <cstdint>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

void gen_characters_wordlist(const char *charset, const int &alphabet_len, const int &max_len) {
    std::ofstream output_file("char_wordlist.txt");
    if (!output_file) {
        std::cerr << "Error: cannot create/open response file" << std::endl;
        return;
    }
    std::stringstream output {};
    std::vector<int> indexes(max_len, 0);
    int pos {};
    
    for (int i = 1; i <= max_len; ++i) {
        while (true) {
            for (int j = 0; j < i; ++j) {
                output << charset[indexes[j]];
            }
            output << std::endl;
            
            pos = i - 1;
            while (pos >= 0 && indexes[pos] == (alphabet_len - 1)) {
                indexes[pos] = 0;
                pos--; 
            }
            
            if (pos < 0) {
                break;
            }
            indexes[pos]++;
        }
    }
    output_file.write(output.str().c_str(), output.str().length());
}

void gen_numbers_wordlist(const long long &start, const long long &end) {
    std::ofstream output_file("int_wordlist.txt");
    if (!output_file) {
        std::cerr << "Error: cannot create/open response file" << std::endl;
        return;
    }
    std::stringstream output {};
    for (long long i = start; i <= end; ++i) {
        output << i << std::endl;
    }
    output_file.write(output.str().c_str(), output.str().length());
}
int main() {
    unsigned int choice_number {};
    std::cout << "Select wordlist to generate: \n" <<
        "1. With only characters and symbols\n"
        "2. With only numbers\n";
    
    std::cin >> choice_number;
    
    if (!std::cin || choice_number > 2 || choice_number == 0) {
        std::cerr << "Error: Only number from the list is accepted" << std::endl;
        return 1;
    }
    if (choice_number == 1) {
        std::string input {};
        std::cout << "Enter characters to include: ";
        std::cin >> input;
        
        int max_len {};
        std::cout << "Enter max word length: ";
        std::cin >> max_len;
        if (!std::cin || max_len <= 0) {
            std::cerr << "Error: Word cannot have negative length" << std::endl;
            return 1;
        }
        
        gen_characters_wordlist(input.c_str(), input.length(), max_len);
    }
    else {
        long long int start {};
        long long int end {};
        std::cout << "Enter range for numbers (start, end): ";
        std::cin >> start >> end;
        if (!std::cin || start < 0 || end < 0 || end < start) {
            std::cerr << "Error: Incorrect range entered" << std::endl;
            return 1;
        }
        gen_numbers_wordlist(start, end);
    }
    return 0;
}
