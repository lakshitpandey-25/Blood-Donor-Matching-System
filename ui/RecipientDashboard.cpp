#include "RecipientDashboard.h"

#include "../modules/matching/MatchingSystem.h"
#include "../models/Donor.h"
#include "../models/BloodRequest.h"
#include "../modules/database/DatabaseManager.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDate>
#include <QList>

RecipientDashboard::RecipientDashboard(int recipientId, QWidget* parent)
    : QWidget(parent), recipientId(recipientId)
{
    setWindowTitle("Recipient Dashboard");
    resize(500, 400);

    QLabel* titleLabel = new QLabel("Find Blood Donors", this);

    QComboBox* bloodGroupInput = new QComboBox(this);
    bloodGroupInput->addItems({
        "A+", "A-", "B+", "B-",
        "AB+", "AB-", "O+", "O-"
    });

    QLineEdit* cityInput = new QLineEdit(this);
    cityInput->setPlaceholderText("Enter your city");

    QPushButton* searchButton =
        new QPushButton("Find Matching Donors", this);

    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(titleLabel);
    layout->addWidget(new QLabel("Required blood group:", this));
    layout->addWidget(bloodGroupInput);
    layout->addWidget(new QLabel("City:", this));
    layout->addWidget(cityInput);
    layout->addWidget(searchButton);

    connect(searchButton, &QPushButton::clicked, this, [=]() {
        QString city = cityInput->text().trimmed();
        QString bloodGroup = bloodGroupInput->currentText();

        if (city.isEmpty()) {
            QMessageBox::warning(
                this, "Missing City", "Please enter your city."
            );
            return;
        }

        DatabaseManager dbManager;

        if (!dbManager.openDatabase()) {
            QMessageBox::critical(
                this, "Database Error", dbManager.getLastError()
            );
            return;
        }

        int requestId = dbManager.addBloodRequest(
            recipientId, bloodGroup, city, 1
        );

        if (requestId == -1) {
            QMessageBox::critical(
                this, "Request Error",
                "Could not save the blood request.\n" +
                dbManager.getLastError()
            );
            return;
        }

        QList<Donor> donorRecords = dbManager.getAllDonors();
        QList<Donor*> donors;

        for (Donor& donor : donorRecords) {
            donors.append(&donor);
        }

        BloodRequest request(
            requestId,
            recipientId,
            bloodGroup,
            1,
            city,
            city,
            QDate::currentDate(),
            "Pending"
        );

        MatchingSystem matchingSystem;

        QList<Donor*> matches =
            matchingSystem.findMatchingDonors(request, donors);

        QString result = "Matching donors:\n\n";

        for (Donor* donor : matches) {
            result += donor->getName()
                + " — " + donor->getBloodGroup()
                + " — " + donor->getCity()
                + "\n";
        }

        if (matches.isEmpty()) {
            result = "No matching donors found.";
        }

        QMessageBox::information(
            this, "Matching Results", result
        );
    });
}
