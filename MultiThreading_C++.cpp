// multiThreading.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <mutex>
#include <condition_variable>

int counter = 0;

std::mutex mtx;           // mutex for critical section
std::condition_variable cv;

bool checkOdd() { return counter % 2 != 0; }
bool checkEven() { return counter % 2 == 0; }

void printEven(int n)
{
    for (int i = 0; i < n; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::unique_lock<std::mutex> lck(mtx);
        cv.wait(lck, []() {return checkEven(); });

        std::cout << counter << "\n";
        counter += 1;
        if (counter >= n) break;
        cv.notify_all();
    }
}
void printOdd(int n)
{
    for (int i = 0; i < n; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::unique_lock<std::mutex> lck(mtx);
        cv.wait(lck, [&]() {return checkOdd(); });
        std::cout << counter << "\n";
        counter += 1;
        if (counter >= n) break;
        cv.notify_all();
    }
}


int main()
{
    int n;
    std::cout << "Enter number:" << std::endl;
    std::cin >> n;
    std::cout << "Printing numbers\n";
    auto p1 = std::thread(printOdd, n);
    auto p2 = std::thread(printEven, n);

    p1.join();
    p2.join();
}


