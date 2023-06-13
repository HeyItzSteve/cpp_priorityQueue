# Hash Table & Priority Queue with Extended API Implementation #
#### By Steven, Implemented in Data Structures Course ####

## Hash Table Implementation ##
The implementation of the hash table involves the use of a Key-Value pair to
store the key of the hash table entry and the value of the entry. 

To add entries to the hash table, the following hash function is used:
* Entry Index = key % tableSize

If a collision occurs upon inserting an element into the hash table, quadratic
probing is used to resolve the collision. If the insertion of an element would
put the load factor at atleast 1/2, a rehash occurs. Upon a rehash, the table
size is increased to the lowest prime number that is greater than or equal to
2*tableSize. Elements are then transferred from the "old table" to the
"new/larger table" in the order which they appear in the old table, and then the
new element is finally inserted.

## Priority Queue Implementation ##
The implementation of the priority queue with extended API utilizes the hash
table implementation to support its operations. 

Extended API functions include decreaseKey/increaseKey functions, which will modify the value, given a key and value "change" parameter, and a remove function, which removes an element, given a key. The hash table is utilized to ensure the functions run in constant + logarithmic time.

### Bugs ###
~~- Calling the Priority Queue get() function causes a segmentation fault.~~ RESOLVED.
- increaseKey() function displays undefinded behavior after calling deleteMin().