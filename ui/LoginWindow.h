#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QLabel;
class QComboBox;

class LoginWindow : public QWidget
{
public:
    LoginWindow();

private:
    QComboBox* roleInput;
    QLineEdit* emailInput;
    QLineEdit* passwordInput;
    QPushButton* registerButton;
    QPushButton* loginButton;
    QLabel* messageLabel;
    void handleLogin();
    void openRegisterWindow();
};

#endif