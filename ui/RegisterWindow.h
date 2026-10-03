#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>

class QLineEdit;
class QComboBox;
class QPushButton;
class QLabel;

class RegisterWindow : public QWidget
{
public:
    RegisterWindow();

private:
    QLineEdit* nameInput;
    QLineEdit* phoneInput;
    QLineEdit* emailInput;
    QLineEdit* passwordInput;

    QComboBox* roleInput;

    QPushButton* registerButton;
    QLabel* messageLabel;

    void handleRegistration();
};

#endif