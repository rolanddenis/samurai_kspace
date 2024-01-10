set(TESTS_FILES
    test_kcell.cpp
)

foreach(FILE ${TESTS_FILES})
  add_executable(${FILE} ${FILE})
  add_test(${FILE} ${FILE})
endforeach(FILE)
