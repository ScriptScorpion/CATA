CC := g++
CCF := -lcrypto -lssl -std=c++17
ALGORITHM := algorithm_test.cpp
WORDLIST := wordlist_gen.cpp
main:
	@$(CC) $(ALGORITHM) -o cata $(CCF)
	@$(CC) $(WORDLIST) -o wordlist_gen -std=c++17
