#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#define OUTPUT_NAME "algorithm.rsp"
#define KEY_LENGTH 32

std::string encrypt(std::string &word, std::string &key) {
    char output[66];
    memset(output, '\0', sizeof(output));
    if (key.empty()) {
        // example using SHA256
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, word.c_str(), word.length());
        SHA256_Final(hash, &sha256);
        for(size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        {
            sprintf(output + (i * 2), "%02x", hash[i]);
        }
        output[SHA256_DIGEST_LENGTH] = '\n';
        output[SHA256_DIGEST_LENGTH+1] = '\0';
        // end of example SHA256
    }
    else {
        // example using AES256
        while (word.length() < 32) {
            word += '\1';
        }
        if (key.length() <= 32) {
            while (key.length() < 32) {
                key += '\0';
            }
        }
        else {
            std::cerr << "Error: key is too big" << std::endl;
            return "";
        }
        AES_KEY encrypt_key, decrypt_key;
        unsigned char encrypt[32];
        unsigned char decrypt[32];
        unsigned char real_word[word.length()];
        unsigned char real_key[KEY_LENGTH];
        memset(real_word, '\0', word.length());
        memset(real_key, '\0', KEY_LENGTH);
        memcpy(real_word, word.c_str(), word.length());
        memcpy(real_key, key.c_str(), key.length());

        AES_set_encrypt_key(real_key, KEY_LENGTH * 8, &encrypt_key);
        AES_set_decrypt_key(real_key, KEY_LENGTH * 8, &decrypt_key);
        
        for (int block = 0; block < 2; block++) {
            AES_encrypt(real_word + block * AES_BLOCK_SIZE, encrypt + block * AES_BLOCK_SIZE, &encrypt_key);
        }
        
        for (int block = 0; block < 2; block++) {
            AES_decrypt(encrypt + block * AES_BLOCK_SIZE, decrypt + block * AES_BLOCK_SIZE, &decrypt_key);
        }
        size_t i = 0;
        for (; i < 32; ++i) {
            output[i] = encrypt[i];
        }
        output[i] = '\n';
        ++i;
        for (; i < 65; ++i) {
            output[i] = decrypt[i-33];
        }
        output[i] = '\0';
        // end of example AES256
    }

    return std::string(output);
}
unsigned long long int amount_of_newlines(std::ifstream &file) {
    char c {};
    unsigned long long int newlines = 0;
    while (file.get(c)) {
        if (c == '\n') {
            newlines++;
        }
    }
    return newlines;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "Usage: " << argv[0] << " [Wordlist]" << std::endl;
        return 0;
    }
    std::ifstream input_file(argv[1]);
    if (!input_file) {
        std::cerr << "Error: cannot open file provided" << std::endl;
        return 1;
    }

    std::ofstream output_file(OUTPUT_NAME);
    if (!output_file) {
        std::cerr << "Error: cannot create/open response file" << std::endl;
        input_file.close();
        return 1;
    }
    input_file.seekg(0);
    auto obj = amount_of_newlines(input_file); 
    if (obj % 2 != 0) {
        std::cerr << "Error: check input file format documentation " << obj << std::endl;
        input_file.close();
        output_file.close();
        std::remove(OUTPUT_NAME);
        return 1;
    }
    input_file.clear();
    input_file.seekg(0);
    char c {};
    char c2 {};
    std::string word {};
    std::string word2 {};
    while (input_file.get(c)) {
        if (c == '\n') {
            while (input_file.get(c2)) {
                if (c2 == '\n' || c2 == '\0') {
                    break;
                }
                else {
                    word2 += c2;
                }
            }
            std::string digest = encrypt(word, word2);
            if (digest.empty()) {
                break;
            }
            digest += '\n';
            output_file.write(digest.c_str(), digest.size());
            word.assign('\0', word.length());
            word.clear();
            word2.assign('\0', word2.length());
            word2.clear();
        }
        else {
            word += c;
        }
    }

    input_file.close();
    output_file.close();

    return 0;
}
