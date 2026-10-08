#include "DonorWindow.h"
#include "../modules/database/DatabaseManager.h"

#include <QLabel>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>

DonorWindow::DonorWindow(int userId, QWidget *parent)
    : QWidget(parent)
{
    this->userId = userId;

    setWindowTitle("Donor Management");
    resize(650, 550);

    DatabaseManager database;
    if (database.openDatabase())
    {
        database.createTables();
        donor = database.getDonorById(userId);
        donor.updateEligibility();
    }

    profileLabel = new QLabel();

    hospitalEdit = new QLineEdit();
    hospitalEdit->setPlaceholderText("Hospital name");

    dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("yyyy-MM-dd");

    QPushButton *addButton = new QPushButton("Add Donation");
    QPushButton *exportButton = new QPushButton("Export History to CSV");

    historyTable = new QTableWidget();
    historyTable->setColumnCount(4);
    historyTable->setHorizontalHeaderLabels(
        {"ID", "Date", "Hospital", "Blood Group"});
    historyTable->horizontalHeader()->setStretchLastSection(true);
    historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout *formLayout = new QHBoxLayout();
    formLayout->addWidget(hospitalEdit);
    formLayout->addWidget(dateEdit);
    formLayout->addWidget(addButton);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new QLabel("Donor Management"));
    layout->addWidget(profileLabel);
    layout->addLayout(formLayout);
    layout->addWidget(historyTable);
    layout->addWidget(exportButton);
    setLayout(layout);

    connect(addButton, &QPushButton::clicked, this,
            [this]() { onAddDonation(); });
    connect(exportButton, &QPushButton::clicked, this,
            [this]() { onExportCsv(); });

    refreshProfile();
    refreshHistory();
}

void DonorWindow::refreshProfile()
{
    profileLabel->setText(donor.displayProfile());
}

void DonorWindow::refreshHistory()
{
    QVector<Donation> list = donor.getHistory().getDonations();
    historyTable->setRowCount(list.size());

    for (int i = 0; i < list.size(); ++i)
    {
        historyTable->setItem(i, 0, new QTableWidgetItem(
            QString::number(list[i].getDonationId())));
        historyTable->setItem(i, 1, new QTableWidgetItem(
            list[i].getDonationDate().toString("yyyy-MM-dd")));
        historyTable->setItem(i, 2, new QTableWidgetItem(
            list[i].getHospital()));
        historyTable->setItem(i, 3, new QTableWidgetItem(
            list[i].getBloodGroup()));
    }
}

void DonorWindow::onAddDonation()
{
    QString hospital = hospitalEdit->text().trimmed();
    if (hospital.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please enter hospital name.");
        return;
    }

    if (dateEdit->date() > QDate::currentDate())
    {
        QMessageBox::warning(this, "Error",
                             "Donation date cannot be in the future.");
        return;
    }

    int id = donor.getHistory().getTotalDonations() + 1;
    Donation donation(id, userId, dateEdit->date(),
                      hospital, donor.getBloodGroup());

    donor.addDonation(donation);

    hospitalEdit->clear();
    refreshProfile();
    refreshHistory();
}

void DonorWindow::onExportCsv()
{
    QString path = QFileDialog::getSaveFileName(
        this, "Export CSV", "donation_history.csv", "CSV Files (*.csv)");

    if (path.isEmpty())
        return;

    if (donor.getHistory().exportToCsv(path))
        QMessageBox::information(this, "Done", "History exported.");
    else
        QMessageBox::warning(this, "Error", "Could not export file.");
}
