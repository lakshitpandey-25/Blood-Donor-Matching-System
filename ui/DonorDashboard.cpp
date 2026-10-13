#include "DonorDashboard.h"

#include "../modules/database/DatabaseManager.h"

#include <QLabel>
#include <QLineEdit>
#include <QVariantMap>
#include <QAbstractItemView>
#include <QDate>
#include <QDateEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>

DonorDashboard::DonorDashboard(int userId)
{
    this->userId = userId;

    setWindowTitle("Donor Dashboard");
    resize(650, 600);

    QLabel* titleLabel = new QLabel("Welcome to Donor Dashboard");
    infoLabel = new QLabel();

    // Add donation form
    hospitalEdit = new QLineEdit();
    hospitalEdit->setPlaceholderText("Hospital / location");

    dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    dateEdit->setMaximumDate(QDate::currentDate());
    dateEdit->setDisplayFormat("yyyy-MM-dd");

    QPushButton* addButton = new QPushButton("Add Donation");
    QPushButton* exportButton = new QPushButton("Export CSV");

    QHBoxLayout* formLayout = new QHBoxLayout();
    formLayout->addWidget(hospitalEdit);
    formLayout->addWidget(dateEdit);
    formLayout->addWidget(addButton);

    // History table
    historyTable = new QTableWidget();
    historyTable->setColumnCount(4);
    historyTable->setHorizontalHeaderLabels(
        QStringList() << "ID" << "Date" << "Hospital" << "Blood Group");
    historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyTable->horizontalHeader()->setStretchLastSection(true);

    // Pending Blood Requests
QLabel* requestsTitle =
    new QLabel("Pending Blood Requests");

requestsTable = new QTableWidget();
requestsTable->setColumnCount(8);

requestsTable->setHorizontalHeaderLabels(
    QStringList()
        << "Request ID"
        << "Recipient"
        << "Blood Group"
        << "City"
        << "Units"
        << "Status"
        << "Requested At"
        << "Action"
);
requestsTable->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeToContents);
requestsTable->setEditTriggers(
    QAbstractItemView::NoEditTriggers
);

requestsTable->horizontalHeader()->setStretchLastSection(true);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(titleLabel);
    layout->addWidget(infoLabel);
    layout->addLayout(formLayout);
    layout->addWidget(historyTable);
    layout->addWidget(requestsTitle);
layout->addWidget(requestsTable);
    layout->addWidget(exportButton);
    setLayout(layout);

    connect(addButton, &QPushButton::clicked, this, &DonorDashboard::addDonation);
    connect(exportButton, &QPushButton::clicked, this, &DonorDashboard::exportCsv);

    refreshInfo();
    loadHistory();
    loadBloodRequests();
}

void DonorDashboard::loadBloodRequests()
{
    requestsTable->setRowCount(0);

    DatabaseManager database;

    if (!database.openDatabase())
        return;

    Donor donor = database.getDonorById(userId);

    QList<QVariantMap> requests =
        database.getAllBloodRequests();

    for (const QVariantMap& request : requests)
    {
        QString status =
            request["status"].toString();

        QString requestedBloodGroup =
            request["bloodGroup"].toString().trimmed();

        QString requestedCity =
            request["city"].toString().trimmed();

        // Show only pending requests for matching
        // blood group and city.
        if (status.compare("Pending", Qt::CaseInsensitive) != 0)
            continue;

        if (requestedBloodGroup.compare(
                donor.getBloodGroup().trimmed(),
                Qt::CaseInsensitive) != 0)
            continue;

        if (requestedCity.compare(
                donor.getCity().trimmed(),
                Qt::CaseInsensitive) != 0)
            continue;

        int row = requestsTable->rowCount();
        requestsTable->insertRow(row);

        int requestId = request["requestId"].toInt();

        requestsTable->setItem(row, 0,
            new QTableWidgetItem(QString::number(requestId)));

        requestsTable->setItem(row, 1,
            new QTableWidgetItem(request["recipientName"].toString()));

        requestsTable->setItem(row, 2,
            new QTableWidgetItem(requestedBloodGroup));

        requestsTable->setItem(row, 3,
            new QTableWidgetItem(requestedCity));

        requestsTable->setItem(row, 4,
            new QTableWidgetItem(request["units"].toString()));

        requestsTable->setItem(row, 5,
            new QTableWidgetItem(status));

        requestsTable->setItem(row, 6,
            new QTableWidgetItem(request["createdAt"].toString()));

        // Accept button
        QPushButton *acceptButton = new QPushButton("Accept");

        requestsTable->setCellWidget(row, 7, acceptButton);

        connect(acceptButton, &QPushButton::clicked,
                this, [this, requestId]()
        {
            acceptBloodRequest(requestId);
        });
    }
}


void DonorDashboard::acceptBloodRequest(int requestId)
{
    QMessageBox::StandardButton reply =
        QMessageBox::question(
            this,
            "Accept Blood Request",
            "Are you sure you want to accept this blood request?"
        );

    if (reply != QMessageBox::Yes)
        return;

    DatabaseManager database;

    if (!database.openDatabase())
    {
        QMessageBox::warning(
            this, "Database Error",
            "Unable to open the database."
        );
        return;
    }

    if (database.acceptBloodRequest(requestId, userId))
    {
        QMessageBox::information(
            this, "Success",
            "Blood request accepted successfully."
        );

        loadBloodRequests();
    }
    else
    {
        QMessageBox::warning(
            this, "Error",
            "Could not accept the request.\n" +
            database.getLastError()
        );
    }
}




void DonorDashboard::refreshInfo()
{
    DatabaseManager database;

    if (!database.openDatabase())
    {
        infoLabel->setText("Unable to load donor details.");
        return;
    }

    database.createTables();

    Donor donor = database.getDonorById(userId);

    // Cooldown over -> eligible again
    QDate last = QDate::fromString(donor.getLastDonationDate(), Qt::ISODate);
    if (!donor.isEligible() && donor.getAge() >= 18 && donor.getAge() <= 65 && last.isValid()
        && last.daysTo(QDate::currentDate()) >= 90)
    {
        database.updateDonorEligibility(userId, true);
        donor.setEligible(true);
    }

    infoLabel->setText(
        "Name: " + donor.getName() + "\n"
        "Email: " + donor.getEmail() + "\n"
        "Phone: " + donor.getPhone() + "\n"
        "Blood Group: " + donor.getBloodGroup() + "\n"
        "Age: " + QString::number(donor.getAge()) + "\n"
        "Gender: " + donor.getGender() + "\n"
        "City: " + donor.getCity() + "\n"
        "Last Donation: " + donor.getLastDonationDate() + "\n"
        "Eligible: " + (donor.isEligible() ? "Yes" : "No")
    );
}

void DonorDashboard::loadHistory()
{
    history = DonationHistory(userId);
    historyTable->setRowCount(0);

    DatabaseManager database;
    if (!database.openDatabase())
        return;

    const QList<Donation> donations = database.getDonationsByDonor(userId);

    for (const Donation &d : donations)
    {
        history.addDonation(d);

        int row = historyTable->rowCount();
        historyTable->insertRow(row);
        historyTable->setItem(row, 0,
            new QTableWidgetItem(QString::number(d.getDonationId())));
        historyTable->setItem(row, 1,
            new QTableWidgetItem(d.getDonationDate().toString("yyyy-MM-dd")));
        historyTable->setItem(row, 2,
            new QTableWidgetItem(d.getHospital()));
        historyTable->setItem(row, 3,
            new QTableWidgetItem(d.getBloodGroup()));
    }
}

void DonorDashboard::addDonation()
{
    QString hospital = hospitalEdit->text().trimmed();

    if (hospital.isEmpty())
    {
        QMessageBox::warning(this, "Missing hospital",
                             "Please enter the hospital or location.");
        return;
    }

    DatabaseManager database;
    if (!database.openDatabase())
    {
        QMessageBox::warning(this, "Database", "Unable to open the database.");
        return;
    }

    Donor donor = database.getDonorById(userId);

    Donation donation(0, userId, dateEdit->date(), hospital,
                      donor.getBloodGroup());

    QString error;
    if (database.addDonation(donation, error) == -1)
    {
        QMessageBox::warning(this, "Donation not allowed", error);
        return;
    }

    hospitalEdit->clear();
    refreshInfo();
    loadHistory();

    QMessageBox::information(this, "Saved", "Donation recorded.");
}

void DonorDashboard::exportCsv()
{
    if (history.getTotalDonations() == 0)
    {
        QMessageBox::information(this, "Export", "No donations to export yet.");
        return;
    }

    QString path = QFileDialog::getSaveFileName(
        this, "Export donation history",
        "donation_history.csv", "CSV Files (*.csv)");

    if (path.isEmpty())
        return;

    if (history.exportToCsv(path))
        QMessageBox::information(this, "Export", "History exported successfully.");
    else
        QMessageBox::warning(this, "Export", "Could not write the file.");
}