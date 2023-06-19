#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <ostream>
#include <memory>

template <typename ValueType>
struct Pair {
    unsigned key;
    ValueType value;
    bool isEmpty = true;
};

/**
 * Implementation of a hash table that stores key-value
 * pairs mapping unsigned integers to instances of
 * ValueType.
 *
 * Hash function: key % tableSize
 * Collision resolution: quadratic probing.
 * Non-unique keys are not supported.
 *
 * The table rehashes whenever the insertion of a new
 * element would put the load factor at at least 1/2.
 * (The rehashing is done before the element would've been inserted.)
 * Upon a rehash, the table size (let's call it m) should
 * be increased to the lowest prime number that is greater
 * than or equal to 2m. Elements are then transferred
 * from the "old table" to the "new/larger table" in the
 * order in which they appear in the old table, and then
 * the new element is finally inserted.
 */
template <typename ValueType>
class HashTable
{
public:
    /**
     * Creates a hash table with the given number of
     * buckets/slots.
     *
     * Throws std::runtime_error if @tableSize is 0 or not
     * prime.
     */
    explicit HashTable(unsigned tableSize) : size(tableSize), elementCount(0) {
        if(tableSize == 0 || !isPrime(tableSize)) {
            throw std::runtime_error("Table size is <= 0 or not prime!");
        }

        table = new Pair<ValueType>[tableSize];
    };

    ~HashTable() {
        delete[] table;
    };

    /**
     * Makes the underlying hash table of this object look
     * exactly the same as that of @rhs.
     */
    HashTable(const HashTable& rhs) {
        table = new Pair<ValueType>[rhs.tableSize()];
        size = rhs.tableSize();
        elementCount = rhs.numElements();

        for(unsigned i = 0; i < tableSize(); i++) {
            table[i].key = rhs.table[i].key;
            table[i].value = rhs.table[i].value;
            table[i].isEmpty = rhs.table[i].isEmpty;
        }
    };

    HashTable& operator=(const HashTable& rhs) {
        if(this == &rhs) {
			return *this;
		}

        delete[] table;
        table = new Pair<ValueType>[rhs.tableSize()];
        size = rhs.tableSize();
        elementCount = rhs.numElements();
        
        for(unsigned i = 0; i < tableSize(); i++) {
            table[i].key = rhs.table[i].key;
            table[i].value = rhs.table[i].value;
            table[i].isEmpty = rhs.table[i].isEmpty;
        }

        return *this;
    };

    /**
     * Takes the underlying implementation details of @rhs
     * and gives them to "this" object.
     * After this, @rhs should be in a "moved from" state.
     */
    HashTable(HashTable&& rhs) noexcept : table(nullptr), size(0), elementCount(0) {
        table = rhs.table;
        size = rhs.size;
        elementCount = rhs.elementCount;

        rhs.table = nullptr;
        rhs.size = 0;
        rhs.elementCount = 0;
    };
    HashTable& operator=(HashTable&& rhs) noexcept {
        if(this == &rhs) {
			return *this;
		}
        
        delete[] table;

        table = rhs.table;
        size = rhs.size;
        elementCount = rhs.elementCount;

        rhs.table = nullptr;
        rhs.size = 0;
        rhs.elementCount = 0;

        return *this;
    };

    /**
     * Both of these must run in constant time.
     */
    unsigned tableSize() const {
        return size;
    };

    unsigned numElements() const {
        return elementCount;
    };

    /**
     * Prints each bucket in the hash table.
     *
     * I don't think that a friend of a templated class can
     * be defined outside of the class. If you can figure
     * out how to do that in a way that works on the CSIF,
     * go for it.
     */
    friend std::ostream& operator<<(std::ostream& os, const HashTable<ValueType>& ht)
    {
        for(unsigned i = 0; i < ht.tableSize(); i++) {
            if(!ht.table[i].isEmpty) {
                os << "Bucket " << i << ": " << ht.table[i].key << " -> " << ht.table[i].value << std::endl;
            } else {
                os << "Bucket " << i << ": (empty)" << std::endl;
            }
        }
        return os;
    }


    /**
     * Inserts a key-value pair mapping @key to @value into
     * the table.
     *
     * This function runs in "constant time".
     *
     * Returns true if success.
     * Returns false if @key is already in the table
     * (in which case, the insertion is not performed).
     */
    bool insert(unsigned key, const ValueType& value) {
        unsigned index = key % tableSize();
        
        if(table[index].isEmpty) { //Checking for rehash. (This is inefficient, but still "constant".)
            elementCount++;
            index = checkRehash(index, key);
        } else {
            for(unsigned i = 0; i < tableSize(); i++) {
                int newIndex = (index + (i*i)) % tableSize();

                if(!table[newIndex].isEmpty && key == table[newIndex].key) {
                    return false;
                }

                if(table[newIndex].isEmpty) {
                    elementCount++;
                    index = checkRehash(index, key);
                    break;                
                }
            }
        }

        if(table[index].isEmpty) { //Insert key-value pair if no collison.
            table[index].key = key;
            table[index].value = value;
            table[index].isEmpty = false;
            return true;
        } else { //Perform quadratic probing if there is collision.
            for(unsigned i = 0; i < tableSize(); i++) {
                int newIndex = (index + (i*i)) % tableSize();

                if(table[newIndex].isEmpty) {
                    table[newIndex].key = key;
                    table[newIndex].value = value;
                    table[newIndex].isEmpty = false;
                    return true;                    
                }
            }
        }
        
        return false;
        
    };

    /**
     * Finds the value corresponding to the given key
     * and returns its address.
     *
     * This function runs in "constant time".
     *
     * Returns null pointer if @key is not in the table.
     */
    ValueType* get(unsigned key) {
        unsigned index = key % tableSize();
        
        if(!table[index].isEmpty && key == table[index].key) {
            return &table[index].value;
        } else {
            for(unsigned i = 0; i < tableSize(); i++) {
                int newIndex = (index + (i*i)) % tableSize();

                if(!table[newIndex].isEmpty && key == table[newIndex].key) {
                    return &table[newIndex].value;
                }
            }
        }
        return nullptr;
    };
    const ValueType* get(unsigned key) const {
        unsigned index = key % tableSize();
        
        if(!table[index].isEmpty && key == table[index].key) {
            return &table[index].value;
        } else {
            for(unsigned i = 0; i < tableSize(); i++) {
                int newIndex = (index + (i*i)) % tableSize();

                if(!table[newIndex].isEmpty && key == table[newIndex].key) {
                    return &table[newIndex].value;
                }
            }
        }
        return nullptr;
    };

    /**
     * Updates the key-value pair with key @key to be
     * mapped to @newValue.
     *
     * This function runs in "constant time".
     *
     * Returns true if success.
     * Returns false if @key is not in the table.
     */
    bool update(unsigned key, const ValueType& newValue) {
        unsigned index = key % tableSize();
        if(!table[index].isEmpty && key == table[index].key) {
            table[index].value = newValue;
            return true;
        } else { //Quadratic Probing.
            for(unsigned i = 0; i < tableSize(); i++) {
                int newIndex = (index + (i*i)) % tableSize();

                if(!table[newIndex].isEmpty && key == table[newIndex].key) {
                    table[newIndex].value = newValue;
                    return true;
                }
            }
        }
        return false;
    };

    /**
     * Deletes the element that has the given key.
     *
     * This function runs in "constant time".
     *
     * Returns true if success.
     * Returns false if @key not found.
     */
    bool remove(unsigned key) {
        unsigned index = key % tableSize();
        if(!table[index].isEmpty && key == table[index].key) {
            table[index].isEmpty = true;
            elementCount--;
            return true;
        } else { //Quadratic Probing.
            for(unsigned i = 0; i < tableSize(); i++) {
                int newIndex = (index + (i*i)) % tableSize();

                if(!table[newIndex].isEmpty && key == table[newIndex].key) {
                    table[newIndex].isEmpty = true;
                    elementCount--;
                    return true;
                }
            }
        }
        return false;
    };

    /**
     * Deletes all elements that have the given value.
     *
     * Returns the number of elements deleted.
     */
    unsigned removeAllByValue(const ValueType& value) {
        unsigned counter = 0;
        for(unsigned i = 0; i < tableSize(); i++) {
            if(!table[i].isEmpty && table[i].value == value) {
                table[i].isEmpty = true;
                elementCount--;
                counter++;
            }
        }
        return counter;
    };

    /**
     * Two instances of HashTable<ValueType> are considered 
     * equal if they contain the same elements, even if those
     * elements are in different buckets (i.e. even if the
     * hash tables have different sizes).
     */
    bool operator==(const HashTable& rhs) const {
        if(numElements() != rhs.numElements()) {
            return false;
        } else {
            unsigned counter = 0;
            for(unsigned i = 0; i < tableSize(); i++) {
                if(!table[i].isEmpty) {
                    for(unsigned j = 0; j < rhs.tableSize(); j++) {
                        if(!rhs.table[j].isEmpty && (table[i].key == rhs.table[j].key && table[i].value == rhs.table[j].value)) {
                            counter++;
                        }
                    }
                }
            }
            if(counter == numElements()) {
                return true;
            }
            return false;
        }
    };

    bool operator!=(const HashTable& rhs) const {
        if(numElements() != rhs.numElements()) {
            return true;
        } else {
            unsigned counter = 0;
            for(unsigned i = 0; i < tableSize(); i++) {
                if(!table[i].isEmpty) {
                    for(unsigned j = 0; j < rhs.tableSize(); j++) {
                        if(!rhs.table[j].isEmpty && (table[i].key == rhs.table[j].key && table[i].value == rhs.table[j].value)) {
                            counter++;
                        }
                    }
                }
            }
            if(counter == numElements()) {
                return false;
            }
            return true;
        }
    };

    /**
     * Returns a newly constructed hash table that is the hash table
     * that would result from inserting each element from @rhs
     * (in the order that they appear in the buckets)
     * into this (i.e. *this) hash table.
     */
    HashTable operator+(const HashTable& rhs) const {
        HashTable<ValueType> newTable(*this); //copy of *this

        for(unsigned i = 0; i < rhs.tableSize(); i++) {
            if(!rhs.table[i].isEmpty) { //Inserts non-empty buckets into *this
                newTable.insert(rhs.table[i].key, rhs.table[i].value);
            }
        }
        return newTable;
    };

private:
    Pair<ValueType>* table;
    unsigned size;
    unsigned elementCount;

    bool isPrime(unsigned value) {
        if(value == 1) {
            return false;
        }

        for(unsigned i = 2; i < value; i++) {
            if(value % i == 0) {
                return false;
            }
        }

        return true;
    }

    int nextPrime(unsigned value) {
        int primeNum = value;
        
        while(!isPrime(primeNum)) {
            primeNum++;
        }
        return primeNum;
    }

    int checkRehash(unsigned index, unsigned key) {
        double loadFactor = elementCount*1.0 / size;
        unsigned prevElementCount = elementCount - 1;
        
        if(loadFactor >= 0.5) {
            Pair<ValueType>* temp = new Pair<ValueType>[prevElementCount];
            unsigned count = 0;
            for(unsigned i = 0; i < size; i++) {
                if(!table[i].isEmpty) {
                    temp[count].key = table[i].key;
                    temp[count].value = table[i].value;
                    count++;
                }
            }
            
            size = nextPrime((2*size));
            delete[] table;
            table = new Pair<ValueType>[size];

            unsigned ind = 0;

            for(unsigned i = 0; i < prevElementCount; i++) { //Inserts elements in resized hash table.
                ind = temp[i].key % size;

                if(table[ind].isEmpty) { //Insert key-value pair if no collison.
                    table[ind].key = temp[i].key;
                    table[ind].value = temp[i].value;
                    table[ind].isEmpty = false;
                } else { //Perform quadratic probing if there is collision.
                    for(unsigned j = 0; j < size; j++) {
                        int newIndex = (ind + (j*j)) % size;

                        if(table[newIndex].isEmpty) {
                            table[newIndex].key = temp[i].key;
                            table[newIndex].value = temp[i].value;
                            table[newIndex].isEmpty = false;
                            break;             
                        }
                    }
                }
            }
            delete[] temp;
            return (key % size);
        } else {
            return index;
        }
    }
};

#endif  // HASH_TABLE_HPP
