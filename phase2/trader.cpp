// This is your trader. Place your orders from here


#include <string>
#include <iostream>
#include <thread>
#include <fstream>
#include <atomic>
#include <mutex>
#include <vector>



//extern std::atomic<int> commonTimer;

int reader(int time)
{
    // Perform some work based on the current time
    return 1;
}
int trader(std::string *message)
{
    return 1;
}