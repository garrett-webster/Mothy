#pragma once

#include <any>
#include <iostream>
#include <vector>

// print_params() -- void
// Utility method used to print each parameter pair after casting to a type that can be printed via cout
void print_params(std::vector<std::pair<std::any, std::any>>& parameters) {
    for (const auto& p : parameters) {
        if (p.first.type() == typeid(int)) {
            std::cout << "\033[0m" << std::any_cast<int>(p.first);
        } else if (p.first.type() == typeid(const char*)) {
            std::cout << "\033[0m" << std::any_cast<const char*>(p.first);
        } else if (p.first.type() == typeid(double)) {
            std::cout << "\033[0m" << std::any_cast<double>(p.first);
        }

        std::cout << ": ";

        if (p.second.type() == typeid(int)) {
            std::cout << std::any_cast<int>(p.second);
        } else if (p.second.type() == typeid(const char*)) {
            std::cout << std::any_cast<const char*>(p.second);
        } else if (p.second.type() == typeid(double)) {
            std::cout<< std::any_cast<double>(p.second);
        }

        std::cout << '\n';
    }
}

// set_params() -- std::vector<std::pair<std::any, std::any>>
// Takes an unspecified number of arguments of unspecified type and returns a vector of pairs, where each pair
// is made up of a parameter's name and value. These are taken as the first/second values, then the third and
// fourth, etc. Throws an error if there are not an equal amount
template <typename... Args>
// For some reason, double ampersands makes it so that you can pass both I and R values. I... Don't really get it.
std::vector<std::pair<std::any, std::any>> set_params(Args&&... args) {
    std::vector<std::pair<std::any, std::any>> parameters;

    // Ensure that arguments are in pairs
    if (sizeof...(args) % 2 != 0) {
        throw std::invalid_argument("Must have pairs of parameters");
    }

    // Create a vector from the parameter pack to use in iteration
    std::vector<std::any> arg_vector = { std::forward<Args>(args)... };

    // Iterate through the vector and process pairs
    for (size_t i = 0; i < arg_vector.size(); i += 2) {
        // Pair the current argument and the next one
        parameters.emplace_back(arg_vector[i], arg_vector[i + 1]);
    }

    return parameters;
}

// test() -- void
// Prints out whether the test was successful or not. If not, it then prints out the parameters and their values
// Usually, you will be checking a result or value after a block of code is ran. This expression will be placed
// directly into the test_result parameter. expected is just the value that should result (usually what goes on
// the right of the == operator in the test_result parameter) for debugging help purposes. Next, also for
// debugging help purposes, the parameters are put with a name, comma, value, repeated for each parameter
template <typename T, typename... Args>
void test(bool test_result, const T expected, const std::string& test_name, Args&&... args) {
    std::string color;

    std::vector<std::pair<std::any, std::any>> parameters = set_params(std::forward<Args>(args)...);

    if (test_result) {
        // ANSI code to set the text green
        color = "\033[32m";
        std::cout << color << "----- Passed: " << test_name << " -----" << std::endl;
    } else {
        // ANSI code to set the text red
        color = "\033[31m";
        std::cout << color << "----- Failed: " << test_name << " -----" << std::endl;
        std::cout << color << "Expected: " << expected << std::endl;
    }

    if (!parameters.empty()) {
        print_params(parameters);
        std::cout << color << "---------------" << std::endl;
    }
}