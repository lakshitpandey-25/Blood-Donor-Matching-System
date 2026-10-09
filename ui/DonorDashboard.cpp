#include "DonorDashboard.h"

#include "../modules/database/DatabaseManager.h"

#include <QLabel>
#include <QVBoxLayout>

DonorDashboard::DonorDashboard(int userId)
{
    this->userId = userId;

    setWindowTitle("Donor Dashboard");
    resize(500, 400);

    DatabaseManager database;

    QLabel* titleLabel =
        new QLabel("Welcome to Donor Dashboard");

    QLabel* infoLabel =
        new QLabel();

    if (database.openDatabase())
    {
        database.createTables();

        Donor donor = database.getDonorById(userId);

        infoLabel->setText(
            "Name: " + donor.getName() + "\n"
            "Email: " + donor.getEmail() + "\n"
            "Phone: " + donor.getPhone() + "\n"
            "Blood Group: " + donor.getBloodGroup() + "\n"
            "Age: " + QString::number(donor.getAge()) + "\n"
            "Gender: " + donor.getGender() + "\n"
            "City: " + donor.getCity() + "\n"
            "Eligible: " + (donor.isEligible() ? "Yes" : "No")
        );
    }
    else
    {
        infoLabel->setText("Unable to load donor details.");
    }

    QVBoxLayout* layout =
        new QVBoxLayout();

    layout->addWidget(titleLabel);
    layout->addWidget(infoLabel);

    setLayout(layout);
}