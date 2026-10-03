#include "RegisterWindow.h"
#include "../modules/authentication/AuthenticationManager.h"

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
    QString name = nameInput->text();
    QString phone = phoneInput->text();
    QString email = emailInput->text();
    QString password = passwordInput->text();

    AuthenticationManager authManager;

    if (name.isEmpty() ||
        email.isEmpty() ||
        password.isEmpty())
    {
        messageLabel->setText(
            "Please fill Name, Email and Password.");
        return;
    }

    messageLabel->setText(
        "Registration details are valid!");
}