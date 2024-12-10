#pragma once

#include <functional>
#include <iostream>

// test() -- void
// Checks to see if the passed function returns the expected value with the passed arguments, then
// prints out the result of the test
template <typename Func, typename T, typename... Args>
void test(Func&& func, T expected, const std::string &test_name, Args&&... args) {
    using S = decltype(std::invoke(std::forward<Func>(func), std::forward<Args>(args)...));
    S result;

    if constexpr (sizeof...(args) == 0) {
        // No arguments provided
        result = std::invoke(std::forward<Func>(func));
    } else {
        // Arguments provided
        result = std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
    }

    // Print results of the test
    std::string color;
    if (result == expected) {
        // ANSI code to set the text green
        color = "\033[32m";
        std::cout << color << "----- Passed: " << test_name << " -----" << std::endl;
    } else {
        // ANSI code to set the text red
        color = "\033[31m";
        std::cout << color << "----- Failed: " << test_name << " -----" << std::endl;
        std::cout << color << "Result: " << result << std::endl;
        std::cout << color << "Expected: " << expected << std::endl;
    }
}
