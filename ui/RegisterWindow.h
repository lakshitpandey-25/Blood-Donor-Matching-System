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

    QLineEdit* bloodGroupInput;
    QLineEdit* ageInput;
    QLineEdit* genderInput;
    QLineEdit* cityInput;
    QLineEdit* lastDonationDateInput;

    QComboBox* roleInput;
	QComboBox* requiredBloodGroupInput;
QLineEdit* recipientCityInput;
QDateEdit* lastDonationInput;
    QPushButton* registerButton;
    QLabel* messageLabel;

    void handleRegistration();
};
#endif