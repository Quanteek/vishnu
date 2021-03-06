link_directories(
 ${VISHNU_EXCEPTION_INCLUDE_DIR}
)

include_directories(
  ${EMF_DATA_DIR}
  ${UMS_DATA_DIR}
  ${EMF4CPP_INCLUDE_DIR}
  ${VISHNU_EXCEPTION_INCLUDE_DIR}
  ${UMS_CLIENT_PROXY_SOURCE_DIR}
  ${UTILVISHNU_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${UMS_API_SOURCE_DIR}
  ${TMS_API_SOURCE_DIR}
  ${IMS_API_SOURCE_DIR}
  ${FMS_API_SOURCE_DIR}
  ${TMS_EMF_DATA_DIR}
  ${TMS_DATA_DIR}
  ${IMS_UTIL_DIR}
  ${TMS_CLIENT_PROXY_SOURCE_DIR}
  ${IMS_EMF_DATA_DIR}
  ${IMS_DATA_DIR}
  ${IMS_CLIENT_PROXY_SOURCE_DIR}
  ${FMS_EMF_DATA_DIR}
  ${FMS_DATA_DIR}
  ${FMS_CLIENT_PROXY_SOURCE_DIR}
)

# Java and/or python are options
if(ENABLE_PYTHON OR ENABLE_JAVA)

if(NOT ENABLE_SWIG AND NOT COMPILE_ALL_MODULES)
  message(FATAL_ERROR "If all VISHNU modules are not compiled together, SWIG is required to regenerate the Python and Java wrappers code. Please set the option ENABLE_SWIG to ON or compile all VISHNU modules.")
endif(NOT ENABLE_SWIG AND NOT COMPILE_ALL_MODULES)

if(ENABLE_SWIG)
  find_package(SWIG 1.3.40 REQUIRED)
  include(${SWIG_USE_FILE})
  mark_as_advanced(SWIG_DIR)
  mark_as_advanced(SWIG_EXECUTABLE)
  mark_as_advanced(SWIG_VERSION)

  # General SWIG configuration
  set_source_files_properties(vishnu.i PROPERTIES CPLUSPLUS ON)

  #Suppress warnings about ignored operators (out_ptr::operator= and EList::operator[])
  set(CMAKE_SWIG_FLAGS "-w362,389")
endif(ENABLE_SWIG)

#Install swig language-specific generated files in a dedicated directory
set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/generated/swig_output)
file(MAKE_DIRECTORY ${CMAKE_SWIG_OUTDIR})

if(ENABLE_PYTHON)
  if(ENABLE_PYTHON3)
    set(VISHNU_PYTHON_SUPPORT 3)
  else()
    set(VISHNU_PYTHON_SUPPORT 2)
  endif()
  find_package(PythonLibs ${VISHNU_PYTHON_SUPPORT} REQUIRED)
  include_directories(${PYTHON_INCLUDE_PATH})
endif(ENABLE_PYTHON)

if(ENABLE_JAVA)
  find_package(Java REQUIRED)
  find_path(JAVA_INCLUDE_DIR jni.h
    $ENV{JAVA_HOME}/include
    /usr/lib/jvm/java/include
    /usr/lib/jvm/java-6*/include
    /usr/lib/jvm/java-6-sun-1.6.0.22/include
  )
  include_directories(
    ${JAVA_INCLUDE_DIR}
    ${JAVA_INCLUDE_DIR}/linux
  )
endif(ENABLE_JAVA)


add_definitions(-DCOMPILE_UMS)
set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} "-DCOMPILE_UMS" )

set(SWIG_LINK_LIB
  ${SWIG_LINK_LIB}
  emf4cpp-vishnu
  vishnu-core
  vishnu-ums-client)

if(COMPILE_TMS)
  add_definitions(-DCOMPILE_TMS)
  set( CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} "-DCOMPILE_TMS" )
  set (SWIG_LINK_LIB
    ${SWIG_LINK_LIB}
    vishnu-tms-client)
endif()
if(COMPILE_IMS)
  add_definitions(-DCOMPILE_IMS)
  set( CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} "-DCOMPILE_IMS" )
  set (SWIG_LINK_LIB
    ${SWIG_LINK_LIB}
    vishnu-ims-client)
endif()
if(COMPILE_FMS)
  set( CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} "-DCOMPILE_FMS" )
  add_definitions(-DCOMPILE_FMS)
  set (SWIG_LINK_LIB
    ${SWIG_LINK_LIB}
    vishnu-fms-client)
endif()


if(ENABLE_SWIG)
  ## PYTHON MODULE
  #The module name must match the %module declaration in the interface file
  if(ENABLE_PYTHON)
    swig_add_module(VISHNU python vishnu.i)
    set_target_properties(${SWIG_MODULE_VISHNU_REAL_NAME} PROPERTIES COMPILE_FLAGS "-fPIC")
    swig_link_libraries(VISHNU ${PYTHON_LIBRARIES} ${SWIG_LINK_LIB})

    install(TARGETS ${SWIG_MODULE_VISHNU_REAL_NAME} DESTINATION ${LIB_INSTALL_DIR})
  endif(ENABLE_PYTHON)

  ## JAVA MODULE
  if(ENABLE_JAVA)
    set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} "-w503" "-package" "com.sysfera.vishnu.api.vishnu.internal")
    swig_add_module(VISHNU java vishnu.i)
    set_target_properties(${SWIG_MODULE_VISHNU_REAL_NAME} PROPERTIES COMPILE_FLAGS "-fPIC")
    swig_link_libraries(VISHNU ${JAVA_LIBRARIES} ${SWIG_LINK_LIB})

    install(TARGETS ${SWIG_MODULE_VISHNU_REAL_NAME} DESTINATION ${LIB_INSTALL_DIR})
  endif(ENABLE_JAVA)

  install(DIRECTORY ${CMAKE_SWIG_OUTDIR} DESTINATION ${LIB_INSTALL_DIR})

else(ENABLE_SWIG)
# Use pre-generated wrappers
  if(ENABLE_JAVA)
    add_library(VISHNU generated/vishnuJAVA_wrap.cxx)
    set_target_properties(VISHNU PROPERTIES COMPILE_FLAGS "-fPIC")
    target_link_libraries(VISHNU ${JAVA_LIBRARIES} ${SWIG_LINK_LIB})

    install(TARGETS VISHNU DESTINATION ${LIB_INSTALL_DIR})
    file(GLOB JAVAFILES "${CMAKE_CURRENT_SOURCE_DIR}/generated/swig_output/*.java")
    foreach(javafile ${JAVAFILES})
      install(FILES ${javafile} DESTINATION ${LIB_INSTALL_DIR}/swig_output)
    endforeach(javafile)
  endif(ENABLE_JAVA)
  
  if(ENABLE_PYTHON)
    add_library(_VISHNU generated/vishnuPYTHON_wrap.cxx)
    set_target_properties(_VISHNU PROPERTIES COMPILE_FLAGS "-fPIC")
    set_target_properties(_VISHNU PROPERTIES PREFIX "")
    target_link_libraries(_VISHNU ${PYTHON_LIBRARIES} ${SWIG_LINK_LIB})

    install(TARGETS _VISHNU DESTINATION ${LIB_INSTALL_DIR} RENAME _VISHNU.so)
    install(FILES generated/swig_output/VISHNU.py DESTINATION ${LIB_INSTALL_DIR}/swig_output)
  endif(ENABLE_PYTHON)

endif(ENABLE_SWIG)

endif(ENABLE_PYTHON OR ENABLE_JAVA)

