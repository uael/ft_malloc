# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    CMakeLists.txt                                     :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alucas- <alucas-@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 1970/01/01 00:00:42 by alucas-           #+#    #+#              #
#    Updated: 1970/01/01 00:00:42 by alucas-          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(ft_malloc VERSION 0.0.1 LANGUAGES C)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

## ************************************************************************** ##
# Build types
##
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "San")

## ************************************************************************** ##
# Options
##
option(FT_MALLOC_LOG "Enable ft_malloc logging" ON)

## ************************************************************************** ##
# Dependencies
##
add_subdirectory(libft)
set(__DEPS libft)
find_package(Threads REQUIRED)
set(__DEPS ${__DEPS} Threads::Threads)

## ************************************************************************** ##
# Project directories definition
##
set(__INC_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(__SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

## ************************************************************************** ##
# Target
##

set(CMAKE_SHARED_LIBRARY_SUFFIX .so)
set(CMAKE_SHARED_MODULE_SUFFIX .so)

macro(wow NAME)
  add_library(${NAME} SHARED ${ARGN})
  add_dependencies(${NAME} libft)

  set_target_properties(${NAME} PROPERTIES
    C_STANDARD 90
    OUTPUT_NAME "${NAME}_$ENV{HOSTTYPE}"
    OUTPUT_NAME_DEBUG "${NAME}_$ENV{HOSTTYPE}.dev"
    OUTPUT_NAME_SAN "${NAME}_$ENV{HOSTTYPE}.san"
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

  target_include_directories(${NAME}
    PRIVATE ${__SRC_DIR}
    PUBLIC ${__INC_DIR})
  if (MSVC)
    target_link_libraries(${NAME} PUBLIC ${__DEPS})
    target_compile_definitions(${NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(${NAME} PRIVATE /Oy /W3)
  else ()
    target_link_libraries(${NAME}
      PUBLIC ${__DEPS}
      PRIVATE
      $<$<CONFIG:San>:-fsanitize=address>
      $<$<CONFIG:San>:-fno-omit-frame-pointer>)
    target_compile_options(${NAME}
      PRIVATE
        -Wall -Werror -Wextra -fPIC
        -ansi -fomit-frame-pointer
        -Wno-missing-field-initializers
        $<$<CONFIG:Release>:-O3 -flto>
        $<$<CONFIG:Debug>:-g3 -O0>
        $<$<CONFIG:San>:-g3 -O0 -fsanitize=address>)
  endif ()
endmacro()

wow(ft_malloc
  ${__SRC_DIR}/class.c
  ${__SRC_DIR}/pool.c
  ${__SRC_DIR}/chunk.c
  ${__SRC_DIR}/scope.c
  ${__SRC_DIR}/bin.c
  ${__SRC_DIR}/bin_dyn.c
  ${__SRC_DIR}/ufree.c
  ${__SRC_DIR}/urealloc.c
  ${__SRC_DIR}/bin_lookup.c
  ${__SRC_DIR}/malloc.c
  ${__SRC_DIR}/ualloc.c)

add_executable(wow test/wow.c)
target_link_libraries(wow PUBLIC ft_malloc)

add_executable(test0 test/test0.c)
target_include_directories(test0 PRIVATE include)

add_executable(test1 test/test1.c)
target_include_directories(test1 PRIVATE include)

add_executable(test9 test/test9.c)
target_link_libraries(test9 PUBLIC ft_malloc)

add_executable(test8 test/test8.c)
target_link_libraries(test8 PUBLIC ft_malloc)

