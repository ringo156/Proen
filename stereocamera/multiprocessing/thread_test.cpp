#include <iostream>
#include <future>

void timewaster(unsigned iterations){
    for (unsigned i = 0; i < iterations; ++i){
        fibonacci(n);
    }
}

void multithreaded_timewaster(
        unsigned iterations,
        unsigned threads){
    std::vector<std::thread> t;
    t.reserve(threads);
    for(unsigned i = 0; i < threads; ++i){
        t.push_back(std::thread(timewaster, iterations / threads));
    }

    for (unsigned i = 0; i < threads; ++i){
        t[i].join();
    }
}

int main(int argc, const char** argv){
    std::cout << std::thread::hardware_concurrency() << std::endl;

    multithreaded_timewaster(10, 1);

    return 0;
}
