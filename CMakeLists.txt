
cmake_minimum_required(VERSION 3.16)

project(BloodDonorMatchingSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Test)

# Main Application
add_executable(BloodDonorMatchingSystem
    main.cpp

    models/User.cpp
    models/User.h

    models/Donor.cpp
    models/Donor.h

    models/Recipient.cpp
    models/Recipient.h

    models/BloodRequest.cpp
    models/BloodRequest.h

    models/Admin.cpp
    models/Admin.h

    modules/authentication/AuthenticationManager.cpp
    modules/authentication/AuthenticationManager.h

    modules/authentication/LoginManager.cpp
    modules/authentication/LoginManager.h

    modules/database/DatabaseManager.cpp
    modules/database/DatabaseManager.h

    modules/admin/AdminManager.cpp
    modules/admin/AdminManager.h

    modules/matching/MatchingSystem.cpp
    modules/matching/MatchingSystem.h

    ui/LoginWindow.cpp
    ui/LoginWindow.h

    ui/DonorDashboard.cpp
    ui/DonorDashboard.h

    ui/RecipientDashboard.cpp
    ui/RecipientDashboard.h

    ui/AdminDashboard.cpp
    ui/AdminDashboard.h

    ui/RegisterWindow.cpp
    ui/RegisterWindow.h
)

target_include_directories(BloodDonorMatchingSystem PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(BloodDonorMatchingSystem
    Qt6::Widgets
    Qt6::Sql
)

# Member 3 - Matching System Tests
add_executable(MatchingSystemTest
    tests/MatchingSystemTest.cpp

    modules/matching/MatchingSystem.cpp
    modules/matching/MatchingSystem.h

    models/User.cpp
    models/User.h

    models/Donor.cpp
    models/Donor.h

    models/BloodRequest.cpp
    models/BloodRequest.h
)

target_include_directories(MatchingSystemTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MatchingSystemTest
    Qt6::Test
    Qt6::Core
)

enable_testing()

add_test(
    NAME MatchingSystemTest
    COMMAND MatchingSystemTest
)
# Member 3 - Database Manager Tests
add_executable(DatabaseManagerTest
    tests/DatabaseManagerTest.cpp

    modules/database/DatabaseManager.cpp
    modules/database/DatabaseManager.h

    models/User.cpp
    models/User.h

    models/Donor.cpp
    models/Donor.h

    models/Recipient.cpp
    models/Recipient.h

    models/Admin.cpp
    models/Admin.h
)

target_include_directories(DatabaseManagerTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(DatabaseManagerTest
    Qt6::Test
    Qt6::Core
    Qt6::Sql
)

add_test(
    NAME DatabaseManagerTest
    COMMAND DatabaseManagerTest
)
