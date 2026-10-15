# 🩸 Blood Donor & Matching System

A desktop application developed using **C++, Qt Framework, and SQLite** to simplify blood donation requests and help recipients find suitable blood donors based on blood group and location.

## 📌 About the Project

In emergencies, finding a suitable blood donor quickly can be challenging. The Blood Donor & Matching System aims to make this process more organized by connecting recipients with potential donors through a simple graphical interface.

The project was developed as an **Object-Oriented Programming (OOP) Project-Based Learning (PBL)** project.

## ✨ Key Features

- **User Authentication:** Login and registration functionality.
- **Role-Based Access:** Separate dashboards for Donors, Recipients, and Admins.
- **Donor Management:** Manage donor information and donation-related details.
- **Blood Requests:** Allow recipients to submit blood requests.
- **Donor Matching:** Identify potential donors based on blood group, location, and eligibility.
- **Request Management:** Track and manage blood request statuses.
- **Admin Dashboard:** Provide administrative access to system information and request management.
- **Database Integration:** Store application data using SQLite.
- **Password Security:** Use password hashing rather than storing passwords as plain text.

## 🛠️ Technology Stack

| Technology | Purpose |
|---|---|
| C++ | Core application logic and OOP implementation |
| Qt 6 | Graphical user interface (Qt Widgets) |
| SQLite | Local database and persistent data storage |
| CMake | Project configuration and build management |
| MinGW | C++ compiler for Windows development |
| Git & GitHub | Version control and team collaboration |

## 🏗️ Project Structure

```text
Blood-Donor-Matching-System/
│
├── models/
│   ├── User.h / User.cpp
│   ├── Donor.h / Donor.cpp
│   ├── Recipient.h / Recipient.cpp
│   ├── Admin.h / Admin.cpp
│   └── BloodRequest.h / BloodRequest.cpp
│
├── modules/
│   ├── authentication/
│   │   ├── AuthenticationManager.cpp
│   │   └── LoginManager.cpp
│   │
│   ├── database/
│   │   └── DatabaseManager.h / DatabaseManager.cpp
│   │
│   └── matching/
│       └── MatchingSystem.h / MatchingSystem.cpp
│
├── ui/
│   ├── LoginWindow.h / LoginWindow.cpp
│   ├── RegisterWindow.h / RegisterWindow.cpp
│   ├── DonorDashboard.h / DonorDashboard.cpp
│   ├── RecipientDashboard.h / RecipientDashboard.cpp
│   └── AdminDashboard.h / AdminDashboard.cpp
│
├── main.cpp
├── CMakeLists.txt
└── README.md
```

*Note: The structure above summarizes the main modules. File names and folders may vary slightly depending on the latest version of the repository.*

## 🧠 Object-Oriented Programming Concepts

The project applies OOP principles to organize application logic into manageable components.

- **Classes and Objects:** Represent users, donors, recipients, blood requests, and application components.
- **Encapsulation:** Organize data and related functions within classes, using access modifiers to control access.
- **Constructors:** Initialize objects wherever constructors are implemented.
- **Inheritance:** Reuse functionality through derived classes wherever inheritance is implemented in the source code.
- **Abstraction:** Separate application functionality behind class interfaces and module boundaries.
- **Modularity:** Separate authentication, database operations, matching logic, and user interface components.

## ⚙️ Requirements

To build the project from source, you need:

- Windows or another supported desktop operating system
- Qt 6 with Qt Widgets and the SQLite SQL driver
- A compatible C++ compiler, such as MinGW
- CMake
- Git

The commands below assume a Windows environment with Qt and MinGW installed.

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/lakshitpandey-25/Blood-Donor-Matching-System.git
```

### 2. Open the Project Directory

```bash
cd Blood-Donor-Matching-System
```

### 3. Configure the Build

Make sure your Qt installation path matches the path on your computer.

```bash
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/QT/6.11.2/mingw_64"
```

### 4. Build the Application

Run this command from the project root:

```bash
cmake --build build
```

### 5. Run the Application

The executable location depends on the project's CMake configuration. For a typical single-configuration MinGW build, it may be located inside the `build` directory.

Run the generated executable from the appropriate location. Ensure the required Qt runtime libraries and platform plugins are available.

## 🗄️ Database

The application uses SQLite for local data persistence.

The database manages information such as:

- User accounts and roles
- Donor details
- Recipient details
- Blood requests and their statuses

The application initializes or accesses its database through the database management module. The actual database location depends on the application's configuration.

## 🔄 Application Workflow

1. A user registers or logs in.
2. The system verifies the user's credentials.
3. The user is directed to the appropriate role-based dashboard.
4. A recipient submits a blood request.
5. The matching module identifies eligible donor candidates.
6. The request can be managed through the application's available workflow.
7. Administrative functions support request and system management.

## 👥 Team Contributions

The project was developed collaboratively, with responsibilities divided across modules.

| Member | Primary Responsibility |
|---|---|
| Member 1 | User management, authentication, login and registration UI, and module integration |
| Member 2 | Donor management and donor-related functionality |
| Member 3 | Blood request handling and donor matching |
| Member 4 | Database management and admin functionality |

*Responsibilities are summarized by module; consult the repository history for the exact files and commits contributed by each member.*

## 🔐 Important Note

This project is an educational prototype developed for academic purposes. It is not a substitute for a verified blood bank or professional medical service. Donor eligibility and availability must be confirmed before any real-world donation.

## 🎯 Future Improvements

- Notifications for new blood requests
- Improved donor search and filtering
- More comprehensive validation and error handling
- Enhanced privacy and security controls
- Testing with additional edge cases
- Integration with verified blood banks or donor networks

## 🤝 Contributing

Contributions and suggestions are welcome. To propose a change:

1. Fork the repository.
2. Create a feature branch.
3. Commit your changes.
4. Open a pull request describing your contribution.

## 📄 License

No license has been specified yet. Add a suitable open-source license if you intend to permit reuse or redistribution.

---

**Developed as an academic OOP PBL project using C++, Qt, and SQLite.**
