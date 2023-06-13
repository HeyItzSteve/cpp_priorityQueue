#include "priority_queue.hpp"
#include <iostream>

int main()
{
    /*
    PriorityQueue<int> x(10);
    x.insert(2,1);
    x.insert(7,1);
    x.insert(5,1);
    x.insert(9,1);
    x.insert(8,1);
    x.insert(6,1);
    std::cout << x << std::endl;    
    x.remove(9);
    std::cout << x << std::endl;  
    */

   PriorityQueue<int> x(14);
   x.insert(18,1);
   x.insert(27,1);
   x.insert(20,1);
   x.insert(34,1);
   x.insert(30,1);
   x.insert(24,1);
   x.insert(99,1);
   x.insert(45,1);
   x.insert(80,1);
   x.insert(65,1);
   x.insert(78,1);
   x.insert(98,1);
   x.insert(97,1);

   std::cout << x << std::endl;
   x.deleteMin();
   std::cout << x << std::endl;
   x.remove(24);
   std::cout << x << std::endl;
   x.increaseKey(20,100);
   std::cout << x << std::endl;
   x.remove(98);
   std::cout << x << std::endl;
   x.decreaseKey(120,119);
   std::cout << x << std::endl;

}
