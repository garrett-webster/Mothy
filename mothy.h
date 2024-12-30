#pragma once

#include <functional>
#include <iostream>

void print_param_with_index(const std::vector<std::string>&, int) {
    // This is the base case. I'm sure I could do this most elgantly, but that's a problem for future me
    return;
}

template <typename T, typename... Args>
void print_param_with_index(const std::vector<std::string>& param_names, int idx, T&& first, Args&&... rest) {
    // Print the parameter name and value
    std::cout << param_names[idx] << ": " << first << std::endl;

    // Recursively call the function for the rest of the arguments
    if constexpr (sizeof...(rest) > 0) {
        print_param_with_index(param_names, idx + 1, std::forward<Args>(rest)...);
    }
}

template <typename T, typename... Args>
void member_func_print_param_with_index(const std::vector<std::string>& param_names, T&& first, Args&&... rest) {
    std::cout << std::endl << "+++Parameters+++:" << std::endl;

    print_param_with_index(param_names, 0, std::forward<Args>(rest)...);
}

template<typename... Args>
void print_params(std::vector<std::string> &param_names, Args&&... args) {
    if (param_names.size() != sizeof...(args)) {
        std::cout << "Different number of arguments and parameter names passed -- can't print parameters" << std::endl;
        std:: cout << "param_names.size(): " << param_names.size() << std::endl << "sizeof...(args)" << sizeof...(args) << std::endl;
        return;
    }

    std::cout << std::endl << "+++Parameters+++" << std::endl;
    print_param_with_index(param_names, 0, std::forward<Args>(args)...);
}

template<typename... Args>
void member_func_print_params(std::vector<std::string> &param_names, Args&&... args) {
    if (param_names.size() != sizeof...(args) - 1) {
        std::cout << "Different number of arguments and parameter names passed -- can't print parameters" << std::endl;
        std:: cout << "param_names.size(): " << param_names.size() << std::endl << "sizeof...(args)" << sizeof...(args) << std::endl;
        return;
    }
    member_func_print_param_with_index(param_names, std::forward<Args>(args)...);
}


// test() -- void
// Checks to see if the passed function returns the expected value with the passed arguments, then
// prints out the result of the test
template <typename Func, typename T, typename... Args>
void test(Func&& func, T expected, const std::string &test_name, std::vector<std::string> param_names = {}, Args&&... args) {
    std::cout << std::endl;
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

        if (!empty(param_names)) {
            print_params(param_names, args...);
        }
    }
}

template <typename Func, typename T, typename... Args>
void member_func_test(Func&& func, T expected, const std::string &test_name, std::vector<std::string> param_names = {}, Args&&... args) {
    std::cout << std::endl;
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

        if (!empty(param_names)) {
            member_func_print_params(param_names, args...);
        }
    }
}