#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include "hash_table.hpp"

/**
 * Implementation of a priority queue that supports the
 * extended API. This priority queue maps unsigned values 
 * to instances of ValueType. Hash table is used to support
 * operations of the extended API.
 */
template <typename ValueType>
class PriorityQueue
{
public:
    /**
     * Creates a priority queue that can have at most @maxSize elements.
     *
     * Throws std::runtime_error if @maxSize is 0.
     */
    explicit PriorityQueue(unsigned maxSize) : data(nextPrime(maxSize)), size(maxSize), elementCount(0) {
        if(maxSize == 0) {
            throw std::runtime_error("maxSize cannot be <= 0!");
        }

        heap = new Pair<ValueType>[maxSize+1];
    };

    ~PriorityQueue() {
        delete[] heap;
    };

    /**
     * Makes the underlying implementation details (including the max size) look
     * exactly the same as that of @rhs.
     */
    PriorityQueue(const PriorityQueue& rhs) : data(rhs.data), size(rhs.maxSize()), elementCount(rhs.elementCount) {
        heap = new Pair<ValueType>[rhs.maxSize()+1];
        
        for(unsigned i = 0; i < rhs.maxSize()+1; i++) {
            heap[i] = rhs.heap[i];
        }
    };

    PriorityQueue& operator=(const PriorityQueue& rhs) {
        if(this == &rhs) {
			return *this;
		}

        delete[] heap;
        heap = new Pair<ValueType>[rhs.maxSize()+1];
        data = rhs.data;
        elementCount = rhs.elementCount;

        for(unsigned i = 0; i < rhs.maxSize()+1; i++) {
            heap[i] = rhs.heap[i];
        }

        return *this;
    };

    /**
     * Takes the underlying implementation details of @rhs
     * and gives them to "this" object.
     * After this, @rhs should be in a "moved from" state.
     */
    PriorityQueue(PriorityQueue&& rhs) noexcept : data(std::move(rhs.data)) {
        heap = rhs.heap;
        size = rhs.size;
        elementCount = rhs.elementCount;

        rhs.heap = nullptr;
        rhs.size = 0;
        rhs.elementCount = 0;
    };

    PriorityQueue& operator=(PriorityQueue&& rhs) noexcept {
        if(this == &rhs) {
			return *this;
		}

        delete[] heap;

        heap = rhs.heap;
        size = rhs.size;
        elementCount = rhs.elementCount;

        data = std::move(rhs.data);

        rhs.heap = nullptr;
        rhs.size = 0;
        rhs.elementCount = 0;      

        return *this;
    };

    /**
     * Both of these must run in constant time.
     */
    unsigned numElements() const {
        return elementCount;
    };
    unsigned maxSize() const {
        return size;
    };

    /**
     * Print the underlying heap level-by-level.
     */
    friend std::ostream& operator<<(std::ostream& os, const PriorityQueue<ValueType>& pq)
    {
        // TODO: Implement this method.
        unsigned counter = 0;
        unsigned current = 1;

        for(unsigned i = 0; i < pq.numElements(); i++) {
            os << "(" << pq.heap[i+1].key << "," << pq.heap[i+1].value << ")";
            counter++;

            if(counter == current) {
                os << std::endl;
                counter = 0;
                current+=current;
            } else {
                os << " ";
            }
        }

        if(counter != 0) {
            os << std::endl;
        }
        
        return os;
    }

    /**
     * Inserts a key-value pair mapping @key to @value into
     * the priority queue.
     *
     * Returns true if success.
     * In this case, must run in logarithmic time.
     *
     * Returns false if @key is already in the priority queue
     * or if max size would be exceeded.
     * (In either of these cases, the insertion is not performed.)
     * In this case, must run in "constant time".
     */
    bool insert(unsigned key, const ValueType& value) {
        if(elementCount >= maxSize() || data.get(key) != NULL) {
            return false;
        }

        elementCount++;

        heap[elementCount].key = key;
        heap[elementCount].value = value;
        
        unsigned newIndex = percolateUp(elementCount); //Maintain min heap properties by moving inserted key up.
        data.insert(key, newIndex);

        return true;
    };

    /**
     * Returns key of the smallest element in the priority queue
     * or null pointer if empty.
     *
     * This function runs in constant time.
     *
     * The pointer may be invalidated if the priority queue is modified.
     */
    const unsigned* getMinKey() const {
        return &heap[1].key;
    };

    /**
     * Returns value of the smallest element in the priority queue
     * or null pointer if empty.
     *
     * This function runs in constant time.
     *
     * The pointer may be invalidated if the priority queue is modified.
     */
    const ValueType* getMinValue() const {
        return &heap[1].value;
    };

    /**
     * Removes the root of the priority queue.
     *
     * This function runs in logarithmic time.
     *
     * Returns true if success.
     * Returns false if priority queue is empty, i.e. nothing to delete.
     */
    bool deleteMin() {
        if(elementCount == 0) {
            return false;
        }
        
        data.remove(*getMinKey());
        heap[1] = heap[elementCount];
        elementCount--;

        unsigned newIndex = percolateDown(1); //Maintain min heap properties by moving new root down.
        data.update(heap[newIndex].key, newIndex);
        return true;
    };

    /**
     * Returns address of the value that @key is mapped to in the priority queue.
     *
     * These functions run in "constant time".
     *
     * Returns null pointer if @key is not in the table.
     */
    ValueType* get(unsigned key) {
        if(data.get(key) == NULL) {
            return nullptr;
        }

        return &heap[*data.get(key)].value;
    };

    const ValueType* get(unsigned key) const {
        if(data.get(key) == NULL) {
            return nullptr;
        }
        
        return &heap[*data.get(key)].value;
    };

    /**
     * Subtracts/adds @change from/to the key of
     * the element that has key @key.
     *
     * These functions run in "constant time" + logarithmic time.
     * This means you must use the required hash table to find the
     * location of @key in the underlying binary heap array.
     *
     * Returns true if success.
     * Returns false if any of the following:
     * - @change is 0.
     * - @key not found.
     * - If the change would lead to a duplicate key.
     *
     * The function does not do anything about  overflow/underflow.
     * For example, an operation like decreaseKey(2, 10)
     * has an undefined effect.
     */
    bool decreaseKey(unsigned key, unsigned change) {
        if(change == 0 || data.get(key) == nullptr || data.get(key - change) != nullptr) {
            return false;
        }
        unsigned index = *data.get(key);
        data.remove(key);

        heap[index].key -= change;
        unsigned newIndex = percolateUp(index);

        data.insert(heap[newIndex].key, newIndex);

        return true;
    };

    bool increaseKey(unsigned key, unsigned change) {
        if(change == 0 || data.get(key) == nullptr || data.get(key + change) != nullptr) {
            return false;
        }
        unsigned index = *data.get(key);
        data.remove(key);

        heap[index].key += change;
        unsigned newIndex = percolateDown(index);

        data.insert(heap[newIndex].key, newIndex);        

        return true;
    };

    /**
     * Removes element that has key @key.
     *
     * These functions run in "constant time" + logarithmic time.
     * This means you must use the required hash table to find the
     * location of @key in the underlying binary heap array.
     *
     * Returns true if success.
     * Returns false if @key not found.
     */
    bool remove(unsigned key) {
        if(data.get(key) == NULL) {
            return false;
        }

        bool up = false;
        unsigned newIndex;

        unsigned index = *data.get(key);
        data.remove(key);

        heap[index] = heap[numElements()];

        newIndex = percolateDown(index); //Attempts percolate down.

        if(newIndex == index) { //If no percolation down occurs, attempts percolate up.
            newIndex = percolateUp(index);
            up = true;
        }

        if(up) {
            index = newIndex;
        }

        data.update(heap[index].key, index);

        elementCount--;

        while(index <= elementCount) { //Updates indexes in hash table.
            if(*data.get(heap[index].key) != index) {
                data.update(heap[index].key, index);
            }
            index++;
        }

        return true; 
    };

private:
    Pair<ValueType>* heap;
    HashTable<unsigned> data;
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

    unsigned leftChild(unsigned index) {
        return 2*index;
    }
    unsigned rightChild(unsigned index) {
        return (2*index)+1;
    }
    unsigned parent(unsigned index) {
        return index/2;
    }

    unsigned percolateUp(unsigned index) {
        Pair<ValueType> temp;
        while(index > 1 && heap[parent(index)].key > heap[index].key) {
            temp = heap[parent(index)];
            heap[parent(index)] = heap[index];
            heap[index] = temp;

            data.update(heap[index].key, index);

            index = parent(index);
        }
        return index;
    }

    unsigned percolateDown(unsigned index) {
        Pair<ValueType> temp;
        unsigned smallest = index;

        bool baseCase = false;

        if(elementCount <= 2) { //Base case for when only two elements in heap.
            baseCase = true;
        }

        while((rightChild(index) < elementCount || leftChild(index) < elementCount || baseCase) && (heap[index].key > heap[leftChild(index)].key || heap[index].key > heap[rightChild(index)].key)) {
            
            if(heap[leftChild(index)].key > heap[rightChild(index)].key) {
                smallest = rightChild(index);
            } else {
                smallest = leftChild(index);
            }

            if(smallest != index) {
                temp = heap[index];
                heap[index] = heap[smallest];
                heap[smallest] = temp;

                data.update(heap[index].key, index);

                index = smallest;
            }
            baseCase = false;
        }
        return index;
    }
};

#endif  // PRIORITY_QUEUE_HPP
