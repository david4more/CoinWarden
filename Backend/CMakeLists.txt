# Backend library
project(Backend LANGUAGES CXX)

set(BACKEND_SOURCES
    backend.cpp
    model.cpp
    Modules/category.cpp
    Modules/currency.cpp
    Modules/transaction.cpp
)

set(BACKEND_HEADERS
    backend.h
    model.h
    Modules/account.h
    Modules/c.h
    Modules/category.h
    Modules/currency.h
    Modules/t.h
    Modules/transaction.h
    Modules/utils.h
)

# Enable AUTOMOC so Q_OBJECT works
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(Backend SHARED ${BACKEND_SOURCES} ${BACKEND_HEADERS})

# Precompiled header
target_precompile_headers(Backend PRIVATE Modules/pch.h)

# Qt
find_package(Qt6 REQUIRED COMPONENTS Core Sql)
target_link_libraries(Backend PRIVATE Qt6::Core Qt6::Sql)

# Defines
target_compile_definitions(Backend PRIVATE BACKEND_LIB)
