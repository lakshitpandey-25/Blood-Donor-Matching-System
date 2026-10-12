#include "RegisterWindow.h"

#include "../modules/authentication/AuthenticationManager.h"
#include "../modules/database/DatabaseManager.h"
#include "../models/Donor.h"
#include "../models/Recipient.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

RegisterWindow::RegisterWindow()
{
    setWindowTitle("Create Account");
    resize(400, 500);

    QLabel *titleLabel = new QLabel("Create New Account");

    QLabel *nameLabel = new QLabel("Name:");
    nameInput = new QLineEdit();

    QLabel *phoneLabel = new QLabel("Phone:");
    phoneInput = new QLineEdit();
    phoneInput->setPlaceholderText("10-digit phone number");
    phoneInput->setValidator(
        new QRegularExpressionValidator(
            QRegularExpression("[0-9]{0,10}"), phoneInput));

    QLabel *emailLabel = new QLabel("Email:");
    emailInput = new QLineEdit();
    emailInput->setPlaceholderText("Enter email");

    QLabel *passwordLabel = new QLabel("Password:");
    passwordInput = new QLineEdit();
    passwordInput->setEchoMode(QLineEdit::Password);

    QLabel *roleLabel = new QLabel("Register As:");
    roleInput = new QComboBox();
    roleInput->addItem("Donor");
    roleInput->addItem("Recipient");

    registerButton = new QPushButton("Register");
    messageLabel = new QLabel("");

    QVBoxLayout *layout = new QVBoxLayout();

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

    connect(registerButton, &QPushButton::clicked,
            this, &RegisterWindow::handleRegistration);
}

void RegisterWindow::handleRegistration()
{
    const QString name = nameInput->text().trimmed();
    const QString phone = phoneInput->text().trimmed();
    const QString email = emailInput->text().trimmed();
    const QString password = passwordInput->text();

    if (name.isEmpty() || phone.isEmpty() ||
        email.isEmpty() || password.isEmpty())
    {
        messageLabel->setText("Please fill in all fields.");
        return;
    }

    if (phone.size() != 10)
    {
        messageLabel->setText(
            "Phone number must contain 10 digits.");
        return;
    }

    if (!email.contains('@') ||
        !email.section('@', 1).contains('.'))
    {
        messageLabel->setText("Enter a valid email address.");
        return;
    }

    AuthenticationManager authManager;
    DatabaseManager database;

    if (!database.openDatabase())
    {
        messageLabel->setText(
            "Database error: " + database.getLastError());
        return;
    }

    if (!database.createTables())
    {
        messageLabel->setText(
            "Database error: " + database.getLastError());
        return;
    }

    if (database.emailExists(email))
    {
        messageLabel->setText(
            "This email is already registered.");
        return;
    }

    int userId = -1;

    if (roleInput->currentText() == "Recipient")
    {
        Recipient recipient(
            0, name, phone, email, password,
            "O+", ""
        );

        if (!authManager.validateRegistration(recipient))
        {
            messageLabel->setText(
                "Please check your registration details.");
            return;
        }

        userId = database.addRecipient(recipient);
    }
    else
    {
        Donor donor(
            0, name, phone, email, password,
            "O+", 18, "Not specified", "",
            "", true
        );

        if (!authManager.validateRegistration(donor))
        {
            messageLabel->setText(
                "Please check your registration details.");
            return;
        }

        userId = database.addDonor(donor);
    }

    if (userId < 0)
    {
        messageLabel->setText(
            "Registration failed: " + database.getLastError());
        return;
    }

    messageLabel->setText(
        "Account created successfully! You can now log in.");
    registerButton->setEnabled(false);
}
