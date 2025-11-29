# Backend library
project(Backend LANGUAGES CXX)

set(BACKEND_SOURCES
        Modules/TransactionsManager.cpp
        Modules/CurrenciesManager.cpp
        Modules/CategoriesManager.cpp
        Modules/AccountsManager.cpp
        Modules/NetworkManager.cpp
        Backend.cpp
        Model.cpp
)

set(BACKEND_HEADERS
        Modules/TransactionsManager.h
        Modules/CurrenciesManager.h
        Modules/CategoriesManager.h
        Modules/AccountsManager.h
        Modules/NetworkManager.h
        Modules/Category.h
        Modules/Transaction.h
        Modules/utils.h
        Backend.h
        Model.h
)

# Enable AUTOMOC so Q_OBJECT works
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_library(Backend SHARED ${BACKEND_SOURCES} ${BACKEND_HEADERS})

# Precompiled header
target_precompile_headers(Backend PRIVATE Modules/pch.h)

# Qt
find_package(Qt6 REQUIRED COMPONENTS Core Sql Network)
target_link_libraries(Backend PRIVATE Qt6::Core Qt6::Sql Qt6::Network)

# Defines
target_compile_definitions(Backend PRIVATE BACKEND_LIB)
