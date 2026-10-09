#include "RegisterWindow.h"

#include "../modules/database/DatabaseManager.h"
#include "../models/Donor.h"
#include "../models/Recipient.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpinBox>
#include <QDateEdit>
#include <QDate>

RegisterWindow::RegisterWindow()
{
    setWindowTitle("Create Account");
    resize(400, 650);

    QLabel *titleLabel =
        new QLabel("Create New Account");

    QLabel *nameLabel =
        new QLabel("Name:");

    nameInput =
        new QLineEdit();

    QLabel *phoneLabel =
        new QLabel("Phone:");

    phoneInput =
        new QLineEdit();

    QLabel *emailLabel =
        new QLabel("Email:");

    emailInput =
        new QLineEdit();

    QLabel *passwordLabel =
        new QLabel("Password:");

    passwordInput =
        new QLineEdit();

    passwordInput->setEchoMode(QLineEdit::Password);

    QLabel *roleLabel =
        new QLabel("Register As:");

    roleInput =
        new QComboBox();

    roleInput->addItem("Donor");
    roleInput->addItem("Recipient");

    // Donor fields

    bloodGroupInput =
        new QComboBox();

    bloodGroupInput->addItems({
        "A+", "A-", "B+", "B-",
        "AB+", "AB-", "O+", "O-"
    });

    ageInput =
        new QSpinBox();

    ageInput->setRange(18, 100);
    ageInput->setValue(18);

    genderInput =
        new QComboBox();

    genderInput->addItems({
        "Male",
        "Female",
        "Other"
    });

    cityInput =
        new QLineEdit();

    lastDonationInput =
        new QDateEdit();

    lastDonationInput->setCalendarPopup(true);
    lastDonationInput->setDate(QDate::currentDate());

    // Recipient fields

    requiredBloodGroupInput =
        new QComboBox();

    requiredBloodGroupInput->addItems({
        "A+", "A-", "B+", "B-",
        "AB+", "AB-", "O+", "O-"
    });

    recipientCityInput =
        new QLineEdit();

    registerButton =
        new QPushButton("Register");

    messageLabel =
        new QLabel("");

    QVBoxLayout *layout =
        new QVBoxLayout();

    layout->addWidget(titleLabel);

    layout->addWidget(nameLabel);
    layout->addWidget(nameInput);

    layout->addWidget(phoneLabel);
    layout->addWidget(phoneInput);

    layout->addWidget(emailLabel);
    layout->addWidget(emailInput);

    layout->addWidget(passwordLabel);
    layout->addWidget(passwordInput);

    layout->addWidget(roleLabel);
    layout->addWidget(roleInput);

    // Donor fields
    layout->addWidget(new QLabel("Blood Group:"));
    layout->addWidget(bloodGroupInput);

    layout->addWidget(new QLabel("Age:"));
    layout->addWidget(ageInput);

    layout->addWidget(new QLabel("Gender:"));
    layout->addWidget(genderInput);

    layout->addWidget(new QLabel("City:"));
    layout->addWidget(cityInput);

    layout->addWidget(new QLabel("Last Donation Date:"));
    layout->addWidget(lastDonationInput);

    // Recipient fields
    layout->addWidget(new QLabel("Required Blood Group:"));
    layout->addWidget(requiredBloodGroupInput);

    layout->addWidget(new QLabel("City:"));
    layout->addWidget(recipientCityInput);

    layout->addWidget(registerButton);
    layout->addWidget(messageLabel);

    setLayout(layout);

    // Initially show donor fields
    requiredBloodGroupInput->hide();
    recipientCityInput->hide();

    // Change fields according to role
    connect(
        roleInput,
        &QComboBox::currentTextChanged,
        this,
        [this](const QString &role)
        {
            bool isDonor = (role == "Donor");

            bloodGroupInput->setVisible(isDonor);
            ageInput->setVisible(isDonor);
            genderInput->setVisible(isDonor);
            cityInput->setVisible(isDonor);
            lastDonationInput->setVisible(isDonor);

            requiredBloodGroupInput->setVisible(!isDonor);
            recipientCityInput->setVisible(!isDonor);
        });

    connect(
        registerButton,
        &QPushButton::clicked,
        this,
        &RegisterWindow::handleRegistration);
}

void RegisterWindow::handleRegistration()
{
    QString name =
        nameInput->text().trimmed();

    QString phone =
        phoneInput->text().trimmed();

    QString email =
        emailInput->text().trimmed();

    QString password =
        passwordInput->text();

    QString role =
        roleInput->currentText();

    // Basic validation

    if (name.isEmpty() ||
        email.isEmpty() ||
        password.isEmpty())
    {
        messageLabel->setText(
            "Please fill Name, Email and Password.");
        return;
    }

    DatabaseManager database;

    if (!database.openDatabase())
    {
        messageLabel->setText(
            "Database connection failed.");
        return;
    }

    if (!database.createTables())
    {
        messageLabel->setText(
            "Could not create database tables.");
        return;
    }

    // Check duplicate email

    if (database.emailExists(email))
    {
        messageLabel->setText(
            "Email already registered.");
        return;
    }

    int userId = 0;

    if (role == "Donor")
    {
        QString bloodGroup =
            bloodGroupInput->currentText();

        int age =
            ageInput->value();

        QString gender =
            genderInput->currentText();

        QString city =
            cityInput->text().trimmed();

        QString lastDonationDate =
            lastDonationInput->date()
            .toString("yyyy-MM-dd");

        if (city.isEmpty())
        {
            messageLabel->setText(
                "Please enter your city.");
            return;
        }

        // New donor is initially eligible
        bool eligible = true;

        Donor donor(
            0,
            name,
            phone,
            email,
            password,
            bloodGroup,
            age,
            gender,
            city,
            lastDonationDate,
            eligible
        );

        userId =
            database.addDonor(donor);
    }
    else
    {
        QString requiredBloodGroup =
            requiredBloodGroupInput->currentText();

        QString city =
            recipientCityInput->text().trimmed();

        if (city.isEmpty())
        {
            messageLabel->setText(
                "Please enter your city.");
            return;
        }

        Recipient recipient(
            0,
            name,
            phone,
            email,
            password,
            requiredBloodGroup,
            city
        );

        userId =
            database.addRecipient(recipient);
    }

    if (userId > 0)
    {
        messageLabel->setText(
            "Registration successful! User ID: "
            + QString::number(userId));

        nameInput->clear();
        phoneInput->clear();
        emailInput->clear();
        passwordInput->clear();
        cityInput->clear();
        recipientCityInput->clear();
    }
    else
    {
        messageLabel->setText(
            "Registration failed: "
            + database.getLastError());
    }
}