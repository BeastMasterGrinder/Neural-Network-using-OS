#include <iostream>
#include <string>
#include <mutex>

//safe print function
void print(std::string str){
    static std::mutex m;
    std::lock_guard<std::mutex> lock(m);
    std::cout<<str<<std::endl;
    std::cout.flush();
}