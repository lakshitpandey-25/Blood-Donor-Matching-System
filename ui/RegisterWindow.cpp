#include "RegisterWindow.h"
#include "../modules/authentication/AuthenticationManager.h"

#include "../models/User.h"
#include "../models/Donor.h"
#include "../models/Recipient.h"
#include "../modules/database/DatabaseManager.h"

#include <QDate>

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDateEdit>
#include <QIntValidator>

RegisterWindow::RegisterWindow()
{
    setWindowTitle("Create Account");
    resize(420, 720);

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

    QLabel *bloodGroupLabel = new QLabel("Blood Group / Required Blood Group:");
    bloodGroupInput = new QLineEdit();
    bloodGroupInput->setPlaceholderText("e.g. O+");

    QLabel *ageLabel = new QLabel("Age (Donors only):");
    ageInput = new QLineEdit();
    ageInput->setValidator(new QIntValidator(18, 100, this));
    ageInput->setPlaceholderText("18-100");

    QLabel *genderLabel = new QLabel("Gender (Donors only):");
    genderInput = new QLineEdit();
    genderInput->setPlaceholderText("e.g. Male, Female, Other");

    QLabel *cityLabel = new QLabel("City:");
    cityInput = new QLineEdit();

    QLabel *lastDonationDateLabel = new QLabel("Last Donation Date (optional):");
    lastDonationDateInput = new QLineEdit();
    lastDonationDateInput->setPlaceholderText("YYYY-MM-DD or leave blank");
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
    layout->addWidget(bloodGroupLabel);
    layout->addWidget(bloodGroupInput);
    layout->addWidget(ageLabel);
    layout->addWidget(ageInput);
    layout->addWidget(genderLabel);
    layout->addWidget(genderInput);
    layout->addWidget(cityLabel);
    layout->addWidget(cityInput);
    layout->addWidget(lastDonationDateLabel);
    layout->addWidget(lastDonationDateInput);

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
    QString bloodGroup = bloodGroupInput->text().trimmed().toUpper();
    QString city = cityInput->text().trimmed();
    QString role = roleInput->currentText();

    if (name.isEmpty() || phone.isEmpty() ||
        email.isEmpty() || password.isEmpty())
    {
        messageLabel->setText(
            "Please fill Name, Phone, Email and Password.");
        return;
    }

    DatabaseManager db;
    if (!db.openDatabase() || !db.createTables())
    {
        messageLabel->setText(
            "Database error: " + db.getLastError());
        return;
    }

    if (db.emailExists(email))
    {
        messageLabel->setText(
            "This email is already registered.");
        return;
    }

    int userId = -1;

    if (role == "Donor")
    {
        bool ageOk = false;
        int age = ageInput->text().toInt(&ageOk);

        const QStringList validBloodGroups = {
            "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"
        };

        if (!validBloodGroups.contains(bloodGroup) ||
            !ageOk || age < 18 || age > 100 ||
            genderInput->text().trimmed().isEmpty() ||
            city.isEmpty())
        {
            messageLabel->setText(
                "Enter a valid blood group, age (18-100), gender and city.");
            return;
        }

        QString lastDonationDate =
            lastDonationDateInput->text().trimmed();

        if (!lastDonationDate.isEmpty() &&
            !QDate::fromString(lastDonationDate, "yyyy-MM-dd").isValid())
        {
            messageLabel->setText(
                "Last donation date must use YYYY-MM-DD.");
            return;
        }

        Donor donor(
            0, name, phone, email, password,
            bloodGroup, age, genderInput->text().trimmed(),
            city, lastDonationDate, true
        );

        userId = db.addDonor(donor);
    }
    else
    {
        const QStringList validBloodGroups = {
            "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"
        };

        if (!validBloodGroups.contains(bloodGroup) || city.isEmpty())
        {
            messageLabel->setText(
                "Enter a valid required blood group and city.");
            return;
        }

        Recipient recipient(
            0, name, phone, email, password, bloodGroup, city
        );

        userId = db.addRecipient(recipient);
    }

    if (userId < 0)
    {
        messageLabel->setText(
            "Registration failed: " + db.getLastError());
        return;
    }

    messageLabel->setText(
        "Registration successful! You can now log in.");
    nameInput->clear();
    phoneInput->clear();
    emailInput->clear();
    passwordInput->clear();
    bloodGroupInput->clear();
    ageInput->clear();
    genderInput->clear();
    cityInput->clear();
    lastDonationDateInput->clear();
}
