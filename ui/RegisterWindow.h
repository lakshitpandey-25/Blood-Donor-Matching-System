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
    QLineEdit* bloodGroupInput;
    QLineEdit* ageInput;
    QLineEdit* genderInput;
    QLineEdit* cityInput;
    QLineEdit* lastDonationDateInput;

    QComboBox* roleInput;

    QPushButton* registerButton;
    QLabel* messageLabel;

    void handleRegistration();
};

#endif