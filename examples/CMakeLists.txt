set(EXAMPLES_FILES
)

foreach(FILE ${EXAMPLES_FILES})
  add_executable(${FILE} ${FILE})
endforeach(FILE)
