#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>

class QLineEdit;
class QComboBox;
class QPushButton;
class QLabel;
class QSpinBox;
class QDateEdit;

class RegisterWindow : public QWidget
{
    Q_OBJECT

public:
    RegisterWindow();

private:
    QLineEdit* nameInput;
    QLineEdit* phoneInput;
    QLineEdit* emailInput;
    QLineEdit* passwordInput;

    QComboBox* roleInput;

    QComboBox* bloodGroupInput;
    QSpinBox* ageInput;
    QComboBox* genderInput;
    QLineEdit* cityInput;
    QDateEdit* lastDonationInput;

    QComboBox* requiredBloodGroupInput;
    QLineEdit* recipientCityInput;

    QPushButton* registerButton;
    QLabel* messageLabel;

    void handleRegistration();
};

#endif