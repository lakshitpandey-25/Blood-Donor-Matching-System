#include "RecipientDashboard.h"

#include "../modules/database/DatabaseManager.h"

#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QIntValidator>
#include <QAbstractItemView>

RecipientDashboard::RecipientDashboard(int recipientId)
    : recipientId(recipientId)
{
    setWindowTitle("Recipient Dashboard");
    resize(750, 550);

    auto* title = new QLabel("Recipient Dashboard");
    auto* form = new QFormLayout();

    bloodGroupInput = new QComboBox();
    bloodGroupInput->addItems(
        {"O-", "O+", "A-", "A+", "B-", "B+", "AB-", "AB+"});

    cityInput = new QLineEdit();
    cityInput->setPlaceholderText("Enter city");

    unitsInput = new QLineEdit();
    unitsInput->setPlaceholderText("Number of units");
    unitsInput->setValidator(new QIntValidator(1, 100, unitsInput));

    form->addRow("Required Blood Group:", bloodGroupInput);
    form->addRow("City:", cityInput);
    form->addRow("Units:", unitsInput);

    auto* createButton = new QPushButton("Create Blood Request");
    auto* refreshButton = new QPushButton("Refresh My Requests");

    messageLabel = new QLabel();

    requestsTable = new QTableWidget();
    requestsTable->setColumnCount(6);
    requestsTable->setHorizontalHeaderLabels(
        {"Request ID", "Blood Group", "City", "Units", "Status", "Created At"});
    requestsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    requestsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    requestsTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    auto* layout = new QVBoxLayout();
    layout->addWidget(title);
    layout->addLayout(form);
    layout->addWidget(createButton);
    layout->addWidget(refreshButton);
    layout->addWidget(messageLabel);
    layout->addWidget(new QLabel("My Blood Requests"));
    layout->addWidget(requestsTable);
    setLayout(layout);

    connect(createButton, &QPushButton::clicked,
            this, &RecipientDashboard::createBloodRequest);
    connect(refreshButton, &QPushButton::clicked,
            this, &RecipientDashboard::loadRequests);

    loadRequests();
}

void RecipientDashboard::createBloodRequest()
{
    const QString city = cityInput->text().trimmed();
    bool unitsOk = false;
    const int units = unitsInput->text().toInt(&unitsOk);

    if (recipientId <= 0 || city.isEmpty() ||
        !unitsOk || units < 1 || units > 100) {
        messageLabel->setText(
            "Enter a valid city and units between 1 and 100.");
        return;
    }

    DatabaseManager database;
    if (!database.openDatabase()) {
        messageLabel->setText("Database error: " + database.getLastError());
        return;
    }

    if (database.getUserRole(recipientId) != "Recipient") {
        messageLabel->setText("Only recipient accounts can create requests.");
        return;
    }

    const int requestId = database.addBloodRequest(
        recipientId, bloodGroupInput->currentText(), city, units);

    if (requestId < 0) {
        messageLabel->setText(
            "Could not create request: " + database.getLastError());
        return;
    }

    messageLabel->setText(
        "Blood request created successfully. Request ID: "
        + QString::number(requestId));

    unitsInput->clear();
    loadRequests();
}

void RecipientDashboard::loadRequests()
{
    requestsTable->setRowCount(0);

    if (recipientId <= 0) {
        messageLabel->setText("Invalid recipient account.");
        return;
    }

    DatabaseManager database;
    if (!database.openDatabase()) {
        messageLabel->setText("Database error: " + database.getLastError());
        return;
    }

    if (database.getUserRole(recipientId) != "Recipient") {
        messageLabel->setText("Recipient account could not be verified.");
        return;
    }

    const auto requests = database.getAllBloodRequests();

    for (const QVariantMap& request : requests) {
        if (request.value("recipientId").toInt() != recipientId)
            continue;

        const int row = requestsTable->rowCount();
        requestsTable->insertRow(row);

        requestsTable->setItem(row, 0, new QTableWidgetItem(
            QString::number(request.value("requestId").toInt())));
        requestsTable->setItem(row, 1, new QTableWidgetItem(
            request.value("bloodGroup").toString()));
        requestsTable->setItem(row, 2, new QTableWidgetItem(
            request.value("city").toString()));
        requestsTable->setItem(row, 3, new QTableWidgetItem(
            QString::number(request.value("units").toInt())));
        requestsTable->setItem(row, 4, new QTableWidgetItem(
            request.value("status").toString()));
        requestsTable->setItem(row, 5, new QTableWidgetItem(
            request.value("createdAt").toString()));
    }
}
