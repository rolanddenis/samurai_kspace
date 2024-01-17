#include <algorithm>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <tuple>

template <typename... T>
std::ostream & operator<< (std::ostream& out, std::tuple<T...> const& t);

template <typename... T, std::size_t... Index>
void disp_tuple_impl(std::ostream& out, std::tuple<T...> const& t, std::index_sequence<Index...>)
{
    ((out << std::get<Index>(t) << ", "), ...);
}

template <typename... T>
std::ostream & operator<< (std::ostream& out, std::tuple<T...> const& t)
{
    out << "tuple(";
    disp_tuple_impl(out, t, std::make_index_sequence<sizeof...(T)>{});
    out << ")";
    return out;
}

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

