#include "LoginWindow.h"
#include "DonorDashboard.h"
#include "RecipientDashboard.h"
#include "AdminDashboard.h"
#include "RegisterWindow.h"

#include <QComboBox>

#include "../modules/authentication/LoginManager.h"
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

    emailInput =
        new QLineEdit();

    QLabel *passwordLabel =
        new QLabel("Password:");

    passwordInput =
        new QLineEdit();

    passwordInput->setEchoMode(QLineEdit::Password);

    QLabel *roleLabel =
        new QLabel("Login As:");

    roleInput =
        new QComboBox();

    roleInput->addItem("Donor");
    roleInput->addItem("Recipient");
    roleInput->addItem("Admin");

    loginButton =
        new QPushButton("Login");
    registerButton = new QPushButton("Create Account");
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::openRegisterWindow);

    messageLabel =
        new QLabel("");
    QVBoxLayout *layout =
        new QVBoxLayout();

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
}

void LoginWindow::handleLogin()
{
    LoginManager loginManager;

    QString email = emailInput->text().trimmed();
    QString password = passwordInput->text();
    QString role = roleInput->currentText();

    int userId = -1;

    bool result = loginManager.validateLogin(
        email, password, role, userId);

    if (!result)
    {
        messageLabel->setText(
            "Invalid email, password, or role!");
        return;
    }

    messageLabel->setText(
        "Login Successful as " + role);

    if (role == "Donor")
    {
        DonorDashboard *dashboard =
            new DonorDashboard(userId);
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