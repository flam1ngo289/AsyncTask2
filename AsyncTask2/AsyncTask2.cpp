#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <iterator>
#include <algorithm>
#include <numeric>

template<typename Iterator, typename Function>
void parallel_for_each(Iterator first, Iterator last, Function func, std::size_t max_depth = 4) {
    auto length = std::distance(first, last);

    if (length <= 1000 || max_depth == 0) {
        std::for_each(first, last, func);
    }
    else {
        Iterator middle = first;
        std::advance(middle, length / 2);

        auto handle = std::async(std::launch::async, parallel_for_each<Iterator, Function>, first, middle, func, max_depth - 1);
        parallel_for_each(middle, last, func, max_depth - 1);

        handle.get();
    }
}

int main() {
    std::vector<int> data(10000);
    std::iota(data.begin(), data.end(), 1); 

    auto func = [](int& x) { x *= 2; }; 

    parallel_for_each(data.begin(), data.end(), func);

    for (int i = 0; i < 10; ++i) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
