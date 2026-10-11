#include "RecipientDashboard.h"

#include "../modules/database/DatabaseManager.h"
#include "../modules/matching/MatchingSystem.h"
#include "../models/BloodRequest.h"
#include "../models/Donor.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>
#include <QVariantMap>

RecipientDashboard::RecipientDashboard(int userId)
    : recipientId(userId)
{
    setWindowTitle("Recipient Dashboard");
    resize(900, 650);

    // Heading
    QLabel *titleLabel =
        new QLabel("Blood Recipient Dashboard");

    QLabel *requestTitle =
        new QLabel("Create Blood Request");

    // Blood group selection
    QComboBox *bloodGroupInput = new QComboBox();

    bloodGroupInput->addItems({
        "O-", "O+", "A-", "A+",
        "B-", "B+", "AB-", "AB+"
    });

    // City and hospital
    QLineEdit *cityInput = new QLineEdit();
    cityInput->setPlaceholderText("Enter city");

    QLineEdit *hospitalInput = new QLineEdit();
    hospitalInput->setPlaceholderText("Enter hospital name");

    // Units required
    QSpinBox *unitsInput = new QSpinBox();
    unitsInput->setRange(1, 20);
    unitsInput->setValue(1);

    // Buttons
    QPushButton *submitButton =
        new QPushButton("Submit Request");

    QPushButton *matchButton =
        new QPushButton("Find Matching Donors");

    QPushButton *refreshButton =
        new QPushButton("Refresh Requests");

    // Matching donors table
    QTableWidget *donorTable =
        new QTableWidget(0, 5);

    donorTable->setHorizontalHeaderLabels({
        "Name", "Blood Group", "City",
        "Phone", "Eligibility"
    });

    donorTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
    );

    // Request history table
    QTableWidget *requestTable =
        new QTableWidget(0, 5);

    requestTable->setHorizontalHeaderLabels({
        "Request ID", "Blood Group",
        "City", "Units", "Status"
    });

    requestTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
    );

    // Request form
    QFormLayout *formLayout = new QFormLayout();

    formLayout->addRow("Blood Group:", bloodGroupInput);
    formLayout->addRow("City:", cityInput);
    formLayout->addRow("Hospital:", hospitalInput);
    formLayout->addRow("Units Required:", unitsInput);

    // Main layout
    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(titleLabel);
    layout->addWidget(requestTitle);
    layout->addLayout(formLayout);
    layout->addWidget(submitButton);
    layout->addWidget(matchButton);
    layout->addWidget(new QLabel("Matching Donors"));
    layout->addWidget(donorTable);
    layout->addWidget(new QLabel("Blood Request History"));
    layout->addWidget(requestTable);
    layout->addWidget(refreshButton);

    setLayout(layout);

    // Database connection
    DatabaseManager database;

    if (!database.openDatabase())
    {
        QMessageBox::critical(
            this,
            "Database Error",
            "Could not open database."
        );

        submitButton->setEnabled(false);
        matchButton->setEnabled(false);
        refreshButton->setEnabled(false);

        return;
    }

    // Refresh request history
    auto refreshRequests = [=]() {
        requestTable->setRowCount(0);

        const QList<QVariantMap> requests =
            database.getAllBloodRequests();

        for (const QVariantMap &request : requests)
        {
            if (request.value("recipientId").toInt()
                != recipientId)
            {
                continue;
            }

            int row = requestTable->rowCount();
            requestTable->insertRow(row);

            requestTable->setItem(
                row, 0,
                new QTableWidgetItem(
                    request.value("requestId").toString()
                )
            );

            requestTable->setItem(
                row, 1,
                new QTableWidgetItem(
                    request.value("bloodGroup").toString()
                )
            );

            requestTable->setItem(
                row, 2,
                new QTableWidgetItem(
                    request.value("city").toString()
                )
            );

            requestTable->setItem(
                row, 3,
                new QTableWidgetItem(
                    request.value("units").toString()
                )
            );

            requestTable->setItem(
                row, 4,
                new QTableWidgetItem(
                    request.value("status").toString()
                )
            );
        }
    };

    // Submit a blood request
    connect(
        submitButton,
        &QPushButton::clicked,
        this,
        [=]() mutable {
            const QString city =
                cityInput->text().trimmed();

            const QString hospital =
                hospitalInput->text().trimmed();

            const QString bloodGroup =
                bloodGroupInput->currentText();

            const int units = unitsInput->value();

            if (city.isEmpty() || hospital.isEmpty())
            {
                QMessageBox::warning(
                    this,
                    "Missing Details",
                    "Please enter city and hospital."
                );

                return;
            }

            // Save request in the database
            int requestId = database.addBloodRequest(
                recipientId,
                bloodGroup,
                city,
                units
            );

            if (requestId < 0)
            {
                QMessageBox::critical(
                    this,
                    "Request Failed",
                    "Could not save blood request."
                );

                return;
            }

            QMessageBox::information(
                this,
                "Success",
                "Blood request created successfully."
            );

            // Update request history
            refreshRequests();
        }
    );

    // Find matching donors
    connect(
        matchButton,
        &QPushButton::clicked,
        this,
        [=]() {
            const QString city =
                cityInput->text().trimmed();

            const QString hospital =
                hospitalInput->text().trimmed();

            const QString bloodGroup =
                bloodGroupInput->currentText();

            const int units = unitsInput->value();

            if (city.isEmpty())
            {
                QMessageBox::warning(
                    this,
                    "Missing City",
                    "Enter a city before searching."
                );

                return;
            }

            if (hospital.isEmpty())
            {
                QMessageBox::warning(
                    this,
                    "Missing Hospital",
                    "Enter a hospital name before searching."
                );

                return;
            }

            // Get donors from database
            QList<Donor> storedDonors =
                database.getAllDonors();

            QList<Donor *> donorPointers;

            for (Donor &donor : storedDonors)
            {
                donorPointers.append(&donor);
            }

            // Create a temporary blood request
            BloodRequest request(
                0,
                recipientId,
                bloodGroup,
                units,
                hospital,
                city,
                QDate::currentDate(),
                "Pending"
            );

            // Match compatible donors
            MatchingSystem matchingSystem;

            const QList<Donor *> matches =
                matchingSystem.findMatchingDonors(
                    request,
                    donorPointers
                );

            donorTable->setRowCount(0);

            // Display matching donors
            for (const Donor *donor : matches)
            {
                int row = donorTable->rowCount();
                donorTable->insertRow(row);

                donorTable->setItem(
                    row, 0,
                    new QTableWidgetItem(
                        donor->getName()
                    )
                );

                donorTable->setItem(
                    row, 1,
                    new QTableWidgetItem(
                        donor->getBloodGroup()
                    )
                );

                donorTable->setItem(
                    row, 2,
                    new QTableWidgetItem(
                        donor->getCity()
                    )
                );

                donorTable->setItem(
                    row, 3,
                    new QTableWidgetItem(
                        donor->getPhone()
                    )
                );

                donorTable->setItem(
                    row, 4,
                    new QTableWidgetItem(
                        donor->isEligible()
                            ? "Eligible"
                            : "Not Eligible"
                    )
                );
            }

            // Handle no matches
            if (matches.isEmpty())
            {
                QMessageBox::information(
                    this,
                    "No Matches",
                    "No matching donors found."
                );
            }
        }
    );

    // Refresh button
    connect(
        refreshButton,
        &QPushButton::clicked,
        this,
        refreshRequests
    );

    // Load existing requests
    refreshRequests();
}