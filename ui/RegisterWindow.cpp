#include "RegisterWindow.h"
#include "../modules/authentication/AuthenticationManager.h"
#include "../modules/database/DatabaseManager.h"
#include "../models/Donor.h"
#include "../models/Recipient.h"
#include "../models/User.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>

RegisterWindow::RegisterWindow()
{
    setWindowTitle("Create Account");
    resize(400, 500);

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

    layout->addWidget(registerButton);
    layout->addWidget(messageLabel);

    setLayout(layout);

    connect(
        registerButton,
        &QPushButton::clicked,
        this,
        &RegisterWindow::handleRegistration);
}
void RegisterWindow::handleRegistration()
{
    QString name = nameInput->text().trimmed();
    QString phone = phoneInput->text().trimmed();
    QString email = emailInput->text().trimmed();
    QString password = passwordInput->text();
    QString role = roleInput->currentText();

    if (name.isEmpty() || phone.isEmpty() ||
        email.isEmpty() || password.isEmpty())
    {
        messageLabel->setText("Please fill all fields.");
        return;
    }

    DatabaseManager dbManager;

    if (!dbManager.openDatabase() ||
        !dbManager.createTables())
    {
        messageLabel->setText(
            "Database error: " + dbManager.getLastError());
        return;
    }

    int userId = -1;

    if (role == "Donor")
    {
        Donor donor(
            0, name, phone, email, password,
            "O+", 18, "Not specified", "Dehradun",
            "", true);

        userId = dbManager.addDonor(donor);
    }
    else
    {
        Recipient recipient(
            0, name, phone, email, password,
            "O+", "");

        userId = dbManager.addRecipient(recipient);
    }

    if (userId != -1)
    {
        messageLabel->setText(
            "Account created successfully!");
    }
    else
    {
        messageLabel->setText(
            "Registration failed: " +
            dbManager.getLastError());
    }
}
