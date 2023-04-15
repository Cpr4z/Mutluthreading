#include "List.h";
#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
#include <mutex>
#include <future>
#include <string>



int main()
{
    FineGrainedQueue FGQ;


    std::future<void> f1 = std::async(std::launch::async, [&FGQ]() { FGQ.pushback(1); });
    std::future<void> f2 = std::async(std::launch::async, [&FGQ]() { FGQ.pushback(2); });
    std::future<void> f3 = std::async(std::launch::async, [&FGQ]() { FGQ.pushback(3); });

    std::future<void> f7 = std::async(std::launch::async, [&FGQ]() { FGQ.print(); });

    std::future<void> f4 = std::async(std::launch::async, [&FGQ]() { FGQ.pushback(4); });
    std::future<void> f5 = std::async(std::launch::async, [&FGQ]() { FGQ.pushback(5); });
    std::future<void> f6 = std::async(std::launch::async, [&FGQ]() { FGQ.pushback(6); });

    std::future<void> f8 = std::async(std::launch::async, [&FGQ]() { FGQ.print(); });

    f1.wait();
    f2.wait();
    f3.wait();
    f4.wait();
    f5.wait();
    f6.wait();

    f7.wait();

    f8.wait();

    std::cout << "FULL DATA: \n";
    FGQ.print();

    std::cout << "\n\n\n";

    std::future<void> f9 = std::async(std::launch::async, [&FGQ]() { FGQ.insertIntoMiddle(10, 1); });
    std::future<void> f10 = std::async(std::launch::async, [&FGQ]() { FGQ.insertIntoMiddle(11, 1); });
    std::future<void> f11 = std::async(std::launch::async, [&FGQ]() { FGQ.insertIntoMiddle(12, 1); });

    std::future<void> f14 = std::async(std::launch::async, [&FGQ]() { FGQ.print(); });

    std::future<void> f12 = std::async(std::launch::async, [&FGQ]() { FGQ.insertIntoMiddle(13, 1); });
    std::future<void> f13 = std::async(std::launch::async, [&FGQ]() { FGQ.insertIntoMiddle(14, 222); });

    std::future<void> f15 = std::async(std::launch::async, [&FGQ]() { FGQ.print(); });


    f9.wait();
    f10.wait();
    f11.wait();
    f12.wait();
    f13.wait();
    f14.wait();
    f15.wait();

    std::cout << "FULL DATA 2: \n";
    FGQ.print();

    return 0;
}