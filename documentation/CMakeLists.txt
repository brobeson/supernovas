find_package(Doxygen 1.9)
if(NOT Doxygen_FOUND)
  message(
    SEND_ERROR
    "${PROJECT_NAME}_BUILD_DOCS is ${${PROJECT_NAME}_BUILD_DOCS}, but Doxygen "
    "is not found. Install Doxygen to build the documentation."
  )
  return()
endif()
