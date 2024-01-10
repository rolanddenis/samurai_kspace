set(TESTS_FILES
    test_kcell
    test_kcells
    test_kcellnd
)

foreach(FILE ${TESTS_FILES})
  add_executable(${FILE} ${FILE}.cpp)
  add_test(${FILE} ${FILE})
endforeach(FILE)
