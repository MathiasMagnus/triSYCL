cmake_minimum_required (VERSION 3.0) # The minimum version of CMake necessary to build this project
project (queue) # The name of our project

declare_trisycl_test(TARGET default_queue)
declare_trisycl_test(TARGET queue)
declare_trisycl_test(TARGET wait TEST_REGEX
"First
Second")
if(${BUILD_OPENCL})
  declare_trisycl_test(TARGET opencl_queue USES_OPENCL)
endif(${BUILD_OPENCL})
