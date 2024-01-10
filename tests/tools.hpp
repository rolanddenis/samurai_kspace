#include <algorithm>
#include <vector>
#include <iostream>
#include <cstdlib>

std::vector<bool> tests_results;

bool check_assertion(bool expr, std::size_t line)
{
    tests_results.push_back((expr));
    std::cout << "Check #" << tests_results.size() << " at line " << line << " "
              << (tests_results.back() ? "succeeded" : "failed !!")
              << std::endl;
    return tests_results.back();
}

bool check_results()
{
  using std::cbegin;
  using std::cend;

  return std::all_of(cbegin(tests_results), cend(tests_results), [] (bool v) { return v; });
}

int return_code()
{
    return check_results() ? EXIT_SUCCESS : EXIT_FAILURE;
}

#define CHECK(expr) check_assertion((expr), __LINE__)

