#include "priority_queue.hpp"

#include <iostream>

int main()
{
    std::cout << std::boolalpha;

    // Basic operations, including insert().
    PriorityQueue<std::string> p1(20);


    std::cout << p1.insert(10, "AA") << '\n';
    p1.insert(13, "BB");
    p1.insert(8, "CC");
    p1.insert(5, "DD");
    std::cout << p1;
    std::cout << "-----\n";
    p1.insert(3, "EE");
    std::cout << p1;
    std::cout << "Min: " << *(p1.getMinKey()) << ' '
        << *(p1.getMinValue()) << '\n';

    std::cout << "-----\n";
    p1.insert(1, "FF");
    p1.insert(7, "GG");
    std::cout << p1;
    std::cout << "Min: " << *(p1.getMinKey()) << ' '
        << *(p1.getMinValue()) << '\n';

    // deleteMin().
    std::cout << "=======\n";
    std::cout << p1.deleteMin() << '\n';
    std::cout << p1;
    std::cout << "-----\n";
    p1.deleteMin();
    std::cout << p1;
    std::cout << "Min: " << *(p1.getMinKey()) << ' '
        << *(p1.getMinValue()) << '\n';

    // Priority queue can get full.
    std::cout << "$$$$$$$\n";
    PriorityQueue<int> p2(2);
    std::cout << p2.insert(1, -3) << '\n';
    std::cout << p2.insert(2, -4) << '\n';
    std::cout << p2.insert(3, -5) << '\n';
    
}
