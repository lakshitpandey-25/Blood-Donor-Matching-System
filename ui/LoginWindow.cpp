#include "LoginWindow.h"
#include "DonorDashboard.h"
#include "RecipientDashboard.h"
#include "AdminDashboard.h"
#include "RegisterWindow.h"

#include "../modules/authentication/LoginManager.h"

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

LoginWindow::LoginWindow()
{
    setWindowTitle("Blood Donor & Matching System");
    resize(400, 300);

    QLabel *titleLabel =
        new QLabel("Blood Donor & Matching System");

    QLabel *emailLabel =
        new QLabel("Email:");

    emailInput = new QLineEdit();

    QLabel *passwordLabel =
        new QLabel("Password:");

    passwordInput = new QLineEdit();
    passwordInput->setEchoMode(QLineEdit::Password);

    QLabel *roleLabel =
        new QLabel("Login As:");

    roleInput = new QComboBox();
    roleInput->addItem("Donor");
    roleInput->addItem("Recipient");
    roleInput->addItem("Admin");

    loginButton = new QPushButton("Login");
    registerButton = new QPushButton("Create Account");

    messageLabel = new QLabel("");

    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(titleLabel);
    layout->addWidget(emailLabel);
    layout->addWidget(emailInput);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordInput);
    layout->addWidget(roleLabel);
    layout->addWidget(roleInput);
    layout->addWidget(loginButton);
    layout->addWidget(registerButton);
    layout->addWidget(messageLabel);

    setLayout(layout);

    connect(loginButton, &QPushButton::clicked,
            this, &LoginWindow::handleLogin);

    connect(registerButton, &QPushButton::clicked,
            this, &LoginWindow::openRegisterWindow);
}

void LoginWindow::handleLogin()
{
    LoginManager loginManager;

    QString role;
    int userId = 0;

    bool result = loginManager.authenticate(
        emailInput->text(),
        passwordInput->text(),
        role,
        userId
    );

    if (!result)
    {
        messageLabel->setText(
            "Invalid Email or Password!");
        return;
    }

    if (role != roleInput->currentText())
    {
        messageLabel->setText(
            "Selected role does not match account role!");
        return;
    }

    messageLabel->setText(
        "Login Successful as " + role);

    if (role == "Donor")
    {
        DonorDashboard *dashboard =
            new DonorDashboard();

        dashboard->show();
    }
    else if (role == "Recipient")
    {
        RecipientDashboard *dashboard =
            new RecipientDashboard(userId);

        dashboard->show();
    }
    else if (role == "Admin")
    {
        AdminDashboard *dashboard =
            new AdminDashboard();

        dashboard->show();
    }
}

void LoginWindow::openRegisterWindow()
{
    RegisterWindow *window =
        new RegisterWindow();

    window->show();
}
