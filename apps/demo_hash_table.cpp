#include "hash_table.hpp"

#include <iostream>

// For forcing use of const overload of get().
static void foo(const HashTable<std::string>& ht)
{
    const std::string* a = ht.get(18);
    std::cout << *a << '\n';
}

int main()
{
    std::cout << std::boolalpha;
    HashTable<std::string> ht(7);
    std::cout << ht.tableSize() << ' ' << ht.numElements() << '\n';

    std::cout << "-------\n";
    std::cout << ht;

    std::cout << "&&&&&&&\n";
    std::cout << ht.insert(18, "Hello") << '\n';
    std::cout << ht.insert(25, "AA") << '\n';
    std::cout << ht.insert(4, "BB") << '\n';
    std::cout << ht.insert(4, "CC") << '\n';
    std::cout << ht;

    std::cout << "@@@@@@@\n";
    std::string* s = ht.get(25);
    std::cout << *s << '\n';
    *s = "WW";
    std::cout << ht;
    foo(ht);

    std::cout << "$$$$$$$\n";
    std::cout << ht.update(25, "XX") << '\n';
    std::cout << ht.update(30, "YY") << '\n';
    std::cout << ht;

    std::cout << "=======\n";
    std::cout << ht.remove(80) << '\n';
    std::cout << ht.remove(25) << '\n';
    std::cout << ht.tableSize() << ' ' << ht.numElements() << '\n';
    std::cout << ht;
    std::cout << ht.update(4, "CC") << '\n';
    std::cout << ht.insert(11, "Hello") << '\n';
    std::cout << ht.tableSize() << ' ' << ht.numElements() << '\n';
    std::cout << ht;

    std::cout << "*******\n";
    std::cout << ht.removeAllByValue("BB") << '\n';
    std::cout << ht.removeAllByValue("Hello") << '\n';
    std::cout << ht;

    std::cout << "#######\n";
    HashTable<std::string> ht2(31);
    std::cout << "1: " << (ht == ht2) << '\n';
    ht2.insert(4, "CC");
    std::cout << "2a: " << (ht == ht2) << '\n';
    std::cout << "2b: " << (ht2 == ht) << '\n';
    ht.insert(3, "DD");
    ht.insert(5, "EE");
    ht2.insert(5, "EE");
    ht2.insert(3, "DD");
    std::cout << "3: " << (ht == ht2) << '\n';
    std::string* s2 = ht.get(4);
    *s2 = "different value";
    std::cout << "4: " << (ht == ht2) << '\n';

    // Rehashing.
    std::cout << "!!!!!!!\n";
    HashTable<int> ht3(7);
    ht3.insert(3, 0);
    ht3.insert(54, 30);
    ht3.insert(10, 50);
    std::cout << ht3;
    ht3.insert(37, -15);  // should trigger rehash
    std::cout << ht3.tableSize() << ' ' << ht3.numElements() << '\n';
    std::cout << ht3;

    // Copy constructor and copy assignment.
    std::cout << "=== copy semantics ===\n";
    HashTable<bool> ht4(5);
    ht4.insert(8, false);
    ht4.insert(4, true);
    HashTable<bool> ht5(ht4);
    std::cout << ht4;
    std::cout << "---\n";
    std::cout << ht5;
    std::cout << "-----\n";
    ht5.insert(10, true);
    ht4 = ht5;
    std::cout << ht4;
    std::cout << "---\n";
    std::cout << ht5;

    // operator+
    std::cout << "=== operator+ ===\n";
    HashTable<std::string> a(11);
    a.insert(3, "AA");
    a.insert(5, "BB");
    HashTable<std::string> b(7);
    b.insert(25, "CC");
    b.insert(14, "DD");
    b.insert(3, "EE");
    HashTable<std::string> c = a + b;
    std::cout << c;
}
