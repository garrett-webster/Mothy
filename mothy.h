#pragma once

#include <functional>
#include <iostream>

// This is the base case. I'm sure I could do this more elegantly, but that's a problem for future me. Also, really
// don't understand why it's needed, but it has something to do with the print_param_helper being called when rest
// doesn't exist or something like that. Don't quite get it, but deleting this function makes the program
// not compile. I tried testing sizeof...(rest), but it doesn't work. Also, must come before the other declaration
void print_param_helper(const std::vector<std::string>&, int) {
    std::cout << "Ahh!" << std::endl;
    return;
}

// print_param_helper() -- void
// Recursively prints the parameters and passed values for each parameter
template <typename T, typename... Args>
void print_param_helper(const std::vector<std::string>& param_names, int idx, T&& first, Args&&... rest) {
    // Print the parameter name and value
    std::cout << param_names[idx] << ": " << first << std::endl;

    // Recursively call the function for the rest of the arguments
    print_param_helper(param_names, idx + 1, std::forward<Args>(rest)...);

}

// print_params() -- void
// Ensures that there are the same number of parameters as parameter names passed. If yes, then calls the recursive
// helper function to print the parameters.
template<typename... Args>
void print_params(std::vector<std::string> &param_names, Args&&... args) {
    if (param_names.size() != sizeof...(args)) {
        std::cout << "Different number of arguments and parameter names passed -- can't print parameters" << std::endl;
        std:: cout << "param_names.size(): " << param_names.size() << std::endl << "sizeof...(args)" << sizeof...(args) << std::endl;
        return;
    }

    std::cout << std::endl << "+++Parameters+++" << std::endl;
    print_param_helper(param_names, 0, std::forward<Args>(args)...);
}

// print_results() -- void
// Prints out the result of the test. If the test failed, then it also prints out the parameters and associated names
template<typename T, typename S, typename... Args>
void print_result(S expected, T result, const std::string &test_name, std::vector<std::string> param_names = {}, Args&&... args) {
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

// test() -- void
// Checks to see if the passed function returns the expected value with the passed arguments, then
// prints out the result of the test.
// NOTE: This function should be used to test functions that are NOT being called from an instance of a class
//       To test a function called from a class instance, use member_func_test()
template <typename Func, typename T, typename... Args>
void test(Func&& func, T expected, const std::string &test_name, std::vector<std::string> param_names = {}, Args&&... args) {
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
    print_result(expected, result, test_name, param_names, args...);
}

// member_func_test() -- void
// Checks to see if the passed function returns the expected value with the passed arguments, then
// prints out the result of the test.
// NOTE: This function should be used to test functions that ARE being called from an instance of a class
//       To test a function not called from a class instance, use test()
template <typename Func, typename T, typename U, typename... Args>
void member_func_test(Func&& func, T expected, const std::string &test_name, U object, std::vector<std::string> param_names = {}, Args&&... args) {
    using S = decltype(std::invoke(std::forward<Func>(func), object, std::forward<Args>(args)...));
    S result;

    if constexpr (sizeof...(args) == 0) {
        // No arguments provided
        result = std::invoke(std::forward<Func>(func), object);
    } else {
        // Arguments provided
        result = std::invoke(std::forward<Func>(func), object, std::forward<Args>(args)...);
    }

    // Print results of the test
    print_result(expected, result, test_name, param_names, args...);
}