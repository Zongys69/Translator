#include <iostream>
#include <vector>


template<class TKey, class TValue>
class HashTable {
private:
    enum EntryState { EMPTY, OCCUPIED, DELETED };

    struct Entry {
        TKey key;
        TValue value;
        EntryState state = EMPTY;
    };

    std::vector<Entry> table;
    int mod; 
    int R = 31;
    int size = 0;

    int getHash(std::string s)  {
        int hash = 0, m = 1;
        for (int i = 0; i < s.size(); i++) {
            hash = (R * hash + (int)(s[i])) % mod;
        }
        return hash;
    }

    int getHash2(std::string s) {
        int hash = getHash(s);
        return (1 + (hash % (mod - 1)));
    }

    int getHash(int key) {
        return key % mod;
    }
    int getHash2(int key) {
        return (1 + (key % (mod - 1)));
    }
    
    int probe(const TKey& key, int i) {
        int h1 = getHash(key);
        int h2 = getHash2(key);
        return (h1 + (i * h2)) % mod;
    }
    int probe_pow2(const TKey& key, int i) {
        int hash = getHash(key);
        long long i_ll = i;
        int c1 = 1;
        long long offset = (i_ll * i_ll + i_ll) / 2;
        offset %= mod;
        return (hash + (int)(offset)) % mod;
    }
    void resize() {
        int oldMod = mod;
        mod *= 2; 
        std::vector<Entry> oldTable = std::move(table);
        table.clear();
        table.resize(mod);
        size = 0;

        
        for (int i = 0; i < oldMod; ++i) {
            if (oldTable[i].state == OCCUPIED) {
                insert(oldTable[i].key, oldTable[i].value);
            }
        }
    }
public:
    HashTable(int capacity = 4) : mod(capacity), table(capacity) {}

    void insert(const TKey& key, const TValue& value) {

        if (size * 10 >= mod * 7) {
            resize();
        }

        for (int i = 0; i < mod; ++i) {
            int j = probe(key, i);
            if (table[j].state != OCCUPIED) {
                table[j].key = key;
                table[j].value = value;
                table[j].state = OCCUPIED;
                size++;
                return;
            }
            else if (table[j].key == key) {
                table[j].value = value; 
                return;
            }
        }
    }

    void remove(const TKey& key) {
        for (int i = 0; i < mod; ++i) {
            int j = probe(key, i);
            if (table[j].state == EMPTY) break;
            if (table[j].state == OCCUPIED && table[j].key == key) {
                table[j].state = DELETED;
                size--;
                return;
            }
        }
    }

    TValue find(const TKey& key) {
        for (int i = 0; i < mod; ++i) {
            int j = probe(key, i);
            if (table[j].state == EMPTY) break;
            if (table[j].state == OCCUPIED && table[j].key == key) {
                return table[j].value;
            }
        }
        return TValue{}; 
    }

    int getSize() const {
        return size;
    }
};