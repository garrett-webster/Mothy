#pragma once

#include <functional>
#include <iostream>

// This is the base case. I'm sure I could do this most elegantly, but that's a problem for future me. Also, really
// don't understand why it's needed, but it has something to do with the print_param_helper being called when rest
// doesn't exist or something like that. Don't quite get it, but deleting this function makes the program
// not compile. I tried testing sizeof...(rest), but it doesn't work
void print_param_helper(const std::vector<std::string>&, int) {
    return;
}

// Recursively prints the parameters and passed values for each parameter
template <typename T, typename... Args>
void print_param_helper(const std::vector<std::string>& param_names, int idx, T&& first, Args&&... rest) {
    // Print the parameter name and value
    std::cout << param_names[idx] << ": " << first << std::endl;

    // Recursively call the function for the rest of the arguments
    print_param_helper(param_names, idx + 1, std::forward<Args>(rest)...);
}

// Strips the object that the function was tested on and passes the remaining parameters to print_param_helper
template <typename T, typename... Args>
void member_func_print_param_helper(const std::vector<std::string>& param_names, T&& first, Args&&... rest) {
    print_param_helper(param_names, 0, std::forward<Args>(rest)...);
}

//---------------------------------------------------------------------------------------------------------------------

// Because the member function test function has to be passed the object, the pack will contain one too many items. The
// member_function_print_params uses a helper function to strip this extraneous info before passing it to the same
// function as print_params

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

template<typename... Args>
void member_func_print_params(std::vector<std::string> &param_names, Args&&... args) {
    if (param_names.size() != sizeof...(args) - 1) {
        std::cout << "Different number of arguments and parameter names passed -- can't print parameters" << std::endl;
        std:: cout << "param_names.size(): " << param_names.size() << std::endl << "sizeof...(args)" << sizeof...(args) << std::endl;
        return;
    }

    std::cout << std::endl << "+++Parameters+++:" << std::endl;
    member_func_print_param_helper(param_names, std::forward<Args>(args)...);
}

//---------------------------------------------------------------------------------------------------------------------


// test() -- void
// Checks to see if the passed function returns the expected value with the passed arguments, then
// prints out the result of the test.
// NOTE: This function should be used to test functions that are NOT being called from an instance of a class
//       To test a function called from a class instance, use member_func_test()
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

// member_func_test() -- void
// Checks to see if the passed function returns the expected value with the passed arguments, then
// prints out the result of the test.
// NOTE: This function should be used to test functions that ARE being called from an instance of a class
//       To test a function not called from a class instance, use test()
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