cmake_minimum_required(VERSION 3.16)

project(BloodDonorMatchingSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(Qt6_DIR "C:/QT/6.11.2/mingw_64/lib/cmake/Qt6")

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

add_executable(BloodDonorMatchingSystem
    main.cpp

    models/User.cpp
    models/User.h

    models/Donor.cpp
    models/Donor.h
    
    models/Donation.cpp
    models/Donation.h

    models/DonationHistory.cpp
    models/DonationHistory.h
    
    models/Donation.cpp
    models/Donation.h

    models/DonationHistory.cpp
    models/DonationHistory.h

    models/Recipient.cpp
    models/Recipient.h


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