#include "AdminDashboard.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QTableWidgetItem>
#include <QFileInfo>

// ---------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------

AdminDashboard::AdminDashboard(QWidget* parent)
    : QWidget(parent)
{
    setupUI();
    refreshDashboard();
}

// ---------------------------------------------------------------
// UI Setup
// ---------------------------------------------------------------

void AdminDashboard::setupUI()
{
    setWindowTitle("Blood Donor Matching System - Admin Dashboard");
    resize(1100, 700);

    // ---------------- Statistics ----------------

    QGroupBox* statisticsGroup =
        new QGroupBox("System Statistics");

    QGridLayout* statisticsLayout =
        new QGridLayout();

    donorsLabel = new QLabel("Donors: 0");
    recipientsLabel = new QLabel("Recipients: 0");
    requestsLabel = new QLabel("Requests: 0");
    pendingLabel = new QLabel("Pending: 0");

    statisticsLayout->addWidget(donorsLabel, 0, 0);
    statisticsLayout->addWidget(recipientsLabel, 0, 1);
    statisticsLayout->addWidget(requestsLabel, 1, 0);
    statisticsLayout->addWidget(pendingLabel, 1, 1);

    statisticsGroup->setLayout(statisticsLayout);

    // ---------------- Donors Table ----------------

    QGroupBox* donorsGroup =
        new QGroupBox("Donors");

    donorsTable =
        new QTableWidget();

    donorsTable->setColumnCount(7);

    donorsTable->setHorizontalHeaderLabels({
        "User ID",
        "Name",
        "Phone",
        "Email",
        "Blood Group",
        "City",
        "Eligible"
    });

    donorsTable->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    donorsTable->setSelectionMode(
        QAbstractItemView::SingleSelection);

    donorsTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    donorsTable->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    QVBoxLayout* donorsLayout =
        new QVBoxLayout();

    donorsLayout->addWidget(donorsTable);
    donorsGroup->setLayout(donorsLayout);

    // ---------------- Recipients Table ----------------

    QGroupBox* recipientsGroup =
        new QGroupBox("Recipients");

    recipientsTable =
        new QTableWidget();

    recipientsTable->setColumnCount(6);

    recipientsTable->setHorizontalHeaderLabels({
        "User ID",
        "Name",
        "Phone",
        "Email",
        "Required Blood",
        "City"
    });

    recipientsTable->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    recipientsTable->setSelectionMode(
        QAbstractItemView::SingleSelection);

    recipientsTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    recipientsTable->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    QVBoxLayout* recipientsLayout =
        new QVBoxLayout();

    recipientsLayout->addWidget(recipientsTable);
    recipientsGroup->setLayout(recipientsLayout);

    // ---------------- Requests Table ----------------

    QGroupBox* requestsGroup =
        new QGroupBox("Blood Requests");

    requestsTable =
        new QTableWidget();

    requestsTable->setColumnCount(8);

    requestsTable->setHorizontalHeaderLabels({
        "Request ID",
        "Recipient ID",
        "Recipient",
        "Blood Group",
        "City",
        "Units",
        "Status",
        "Created At"
    });

    requestsTable->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    requestsTable->setSelectionMode(
        QAbstractItemView::SingleSelection);

    requestsTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    requestsTable->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    QVBoxLayout* requestsLayout =
        new QVBoxLayout();

    requestsLayout->addWidget(requestsTable);
    requestsGroup->setLayout(requestsLayout);

    // ---------------- Buttons ----------------

    refreshButton =
        new QPushButton("Refresh");

    generateReportButton =
        new QPushButton("Generate Report");

    deleteUserButton =
        new QPushButton("Delete Selected User");

    updateEligibilityButton =
        new QPushButton("Toggle Donor Eligibility");

    deleteRequestButton =
        new QPushButton("Delete Selected Request");

    updateStatusButton =
        new QPushButton("Update Request Status");

    statusComboBox =
        new QComboBox();

    statusComboBox->addItems({
        "Pending",
        "Matched",
        "Fulfilled",
        "Cancelled"
    });

    QHBoxLayout* buttonLayout =
        new QHBoxLayout();

    buttonLayout->addWidget(refreshButton);
    buttonLayout->addWidget(generateReportButton);
    buttonLayout->addWidget(deleteUserButton);
    buttonLayout->addWidget(updateEligibilityButton);
    buttonLayout->addWidget(deleteRequestButton);
    buttonLayout->addWidget(statusComboBox);
    buttonLayout->addWidget(updateStatusButton);

    // ---------------- Main Layout ----------------

    QVBoxLayout* mainLayout =
        new QVBoxLayout();

    mainLayout->addWidget(statisticsGroup);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(donorsGroup);
    mainLayout->addWidget(recipientsGroup);
    mainLayout->addWidget(requestsGroup);

    setLayout(mainLayout);

    // ---------------- Connections ----------------

    connect(
        refreshButton,
        &QPushButton::clicked,
        this,
        &AdminDashboard::refreshDashboard
    );

    connect(
        generateReportButton,
        &QPushButton::clicked,
        this,
        &AdminDashboard::generateReport
    );

    connect(
        deleteUserButton,
        &QPushButton::clicked,
        this,
        &AdminDashboard::deleteSelectedUser
    );

    connect(
        updateEligibilityButton,
        &QPushButton::clicked,
        this,
        &AdminDashboard::updateSelectedDonorEligibility
    );

    connect(
        deleteRequestButton,
        &QPushButton::clicked,
        this,
        &AdminDashboard::deleteSelectedRequest
    );

    connect(
        updateStatusButton,
        &QPushButton::clicked,
        this,
        &AdminDashboard::updateSelectedRequestStatus
    );
}

// ---------------------------------------------------------------
// Statistics
// ---------------------------------------------------------------

void AdminDashboard::setupStatistics()
{
    loadStatistics();
}

void AdminDashboard::loadStatistics()
{
    if (!adminManager.isReady())
    {
        return;
    }

    donorsLabel->setText(
        "Donors: "
        + QString::number(
            adminManager.getTotalDonors())
    );

    recipientsLabel->setText(
        "Recipients: "
        + QString::number(
            adminManager.getTotalRecipients())
    );

    requestsLabel->setText(
        "Requests: "
        + QString::number(
            adminManager.getTotalRequests())
    );

    pendingLabel->setText(
        "Pending: "
        + QString::number(
            adminManager.getPendingRequests())
    );
}

// ---------------------------------------------------------------
// Donors
// ---------------------------------------------------------------

void AdminDashboard::setupDonorsTable()
{
    loadDonors();
}

void AdminDashboard::loadDonors()
{
    if (!adminManager.isReady())
    {
        return;
    }

    QList<Donor> donors =
        adminManager.getAllDonors();

    donorsTable->setRowCount(0);

    for (const Donor& donor : donors)
    {
        int row = donorsTable->rowCount();

        donorsTable->insertRow(row);

        donorsTable->setItem(
            row, 0,
            new QTableWidgetItem(
                QString::number(donor.getUserId()))
        );

        donorsTable->setItem(
            row, 1,
            new QTableWidgetItem(
                donor.getName())
        );

        donorsTable->setItem(
            row, 2,
            new QTableWidgetItem(
                donor.getPhone())
        );

        donorsTable->setItem(
            row, 3,
            new QTableWidgetItem(
                donor.getEmail())
        );

        donorsTable->setItem(
            row, 4,
            new QTableWidgetItem(
                donor.getBloodGroup())
        );

        donorsTable->setItem(
            row, 5,
            new QTableWidgetItem(
                donor.getCity())
        );

        donorsTable->setItem(
            row, 6,
            new QTableWidgetItem(
                donor.isEligible() ? "Yes" : "No")
        );
    }
}

// ---------------------------------------------------------------
// Recipients
// ---------------------------------------------------------------

void AdminDashboard::setupRecipientsTable()
{
    loadRecipients();
}

void AdminDashboard::loadRecipients()
{
    if (!adminManager.isReady())
    {
        return;
    }

    QList<Recipient> recipients =
        adminManager.getAllRecipients();

    recipientsTable->setRowCount(0);

    for (const Recipient& recipient : recipients)
    {
        int row = recipientsTable->rowCount();

        recipientsTable->insertRow(row);

        recipientsTable->setItem(
            row, 0,
            new QTableWidgetItem(
                QString::number(
                    recipient.getUserId()))
        );

        recipientsTable->setItem(
            row, 1,
            new QTableWidgetItem(
                recipient.getName())
        );

        recipientsTable->setItem(
            row, 2,
            new QTableWidgetItem(
                recipient.getPhone())
        );

        recipientsTable->setItem(
            row, 3,
            new QTableWidgetItem(
                recipient.getEmail())
        );

        recipientsTable->setItem(
            row, 4,
            new QTableWidgetItem(
                recipient.getRequiredBloodGroup())
        );

        recipientsTable->setItem(
            row, 5,
            new QTableWidgetItem(
                recipient.getCity())
        );
    }
}

// ---------------------------------------------------------------
// Blood Requests
// ---------------------------------------------------------------

void AdminDashboard::setupRequestsTable()
{
    loadRequests();
}

void AdminDashboard::loadRequests()
{
    if (!adminManager.isReady())
    {
        return;
    }

    QList<QVariantMap> requests =
        adminManager.getAllBloodRequests();

    requestsTable->setRowCount(0);

    for (const QVariantMap& request : requests)
    {
        int row = requestsTable->rowCount();

        requestsTable->insertRow(row);

        requestsTable->setItem(
            row, 0,
            new QTableWidgetItem(
                QString::number(
                    request.value("requestId").toInt()))
        );

        requestsTable->setItem(
            row, 1,
            new QTableWidgetItem(
                QString::number(
                    request.value("recipientId").toInt()))
        );

        requestsTable->setItem(
            row, 2,
            new QTableWidgetItem(
                request.value("recipientName").toString())
        );

        requestsTable->setItem(
            row, 3,
            new QTableWidgetItem(
                request.value("bloodGroup").toString())
        );

        requestsTable->setItem(
            row, 4,
            new QTableWidgetItem(
                request.value("city").toString())
        );

        requestsTable->setItem(
            row, 5,
            new QTableWidgetItem(
                QString::number(
                    request.value("units").toInt()))
        );

        requestsTable->setItem(
            row, 6,
            new QTableWidgetItem(
                request.value("status").toString())
        );

        requestsTable->setItem(
            row, 7,
            new QTableWidgetItem(
                request.value("createdAt").toString())
        );
    }
}

// ---------------------------------------------------------------
// Refresh Dashboard
// ---------------------------------------------------------------

void AdminDashboard::refreshDashboard()
{
    if (!adminManager.isReady())
    {
        QMessageBox::warning(
            this,
            "Database Error",
            "Unable to connect to the database.\n\n"
            + adminManager.getLastError()
        );

        return;
    }

    loadStatistics();
    loadDonors();
    loadRecipients();
    loadRequests();
}

// ---------------------------------------------------------------
// Generate Report
// ---------------------------------------------------------------

void AdminDashboard::generateReport()
{
    if (reportManager.generateAdminReport())
    {
        QMessageBox::information(
            this,
            "Report Generated",
            "Admin report generated successfully.\n\n"
            + reportManager.getExportDirectory()
            + "/admin_report.csv"
        );
    }
    else
    {
        QMessageBox::warning(
            this,
            "Report Error",
            "Unable to generate admin report."
        );
    }
}

// ---------------------------------------------------------------
// Delete Selected User
// ---------------------------------------------------------------

void AdminDashboard::deleteSelectedUser()
{
    int row =
        donorsTable->currentRow();

    bool isDonor = row >= 0;

    if (!isDonor)
    {
        row = recipientsTable->currentRow();
    }

    if (row < 0)
    {
        QMessageBox::warning(
            this,
            "No Selection",
            "Please select a donor or recipient."
        );

        return;
    }

    QTableWidget* table =
        isDonor ? donorsTable : recipientsTable;

    QTableWidgetItem* item =
        table->item(row, 0);

    if (!item)
    {
        return;
    }

    int userId =
        item->text().toInt();

    QMessageBox::StandardButton reply =
        QMessageBox::question(
            this,
            "Confirm Delete",
            "Are you sure you want to delete "
            "this user?",
            QMessageBox::Yes |
            QMessageBox::No
        );

    if (reply != QMessageBox::Yes)
    {
        return;
    }

    if (adminManager.deleteUser(userId))
    {
        reportManager.logUserDeleted(userId);

        QMessageBox::information(
            this,
            "Success",
            "User deleted successfully."
        );

        refreshDashboard();
    }
    else
    {
        QMessageBox::warning(
            this,
            "Delete Failed",
            "Unable to delete user."
        );
    }
}

// ---------------------------------------------------------------
// Update Donor Eligibility
// ---------------------------------------------------------------

void AdminDashboard::updateSelectedDonorEligibility()
{
    int row =
        donorsTable->currentRow();

    if (row < 0)
    {
        QMessageBox::warning(
            this,
            "No Selection",
            "Please select a donor first."
        );

        return;
    }

    QTableWidgetItem* idItem =
        donorsTable->item(row, 0);

    QTableWidgetItem* eligibilityItem =
        donorsTable->item(row, 6);

    if (!idItem || !eligibilityItem)
    {
        return;
    }

    int userId =
        idItem->text().toInt();

    bool currentEligibility =
        eligibilityItem->text()
            .compare("Yes",
                     Qt::CaseInsensitive) == 0;

    bool newEligibility =
        !currentEligibility;

    if (adminManager.setDonorEligibility(
            userId,
            newEligibility))
    {
        QMessageBox::information(
            this,
            "Success",
            "Donor eligibility updated."
        );

        refreshDashboard();
    }
    else
    {
        QMessageBox::warning(
            this,
            "Update Failed",
            "Unable to update donor eligibility."
        );
    }
}

// ---------------------------------------------------------------
// Delete Selected Request
// ---------------------------------------------------------------

void AdminDashboard::deleteSelectedRequest()
{
    int row =
        requestsTable->currentRow();

    if (row < 0)
    {
        QMessageBox::warning(
            this,
            "No Selection",
            "Please select a blood request."
        );

        return;
    }

    QTableWidgetItem* item =
        requestsTable->item(row, 0);

    if (!item)
    {
        return;
    }

    int requestId =
        item->text().toInt();

    QMessageBox::StandardButton reply =
        QMessageBox::question(
            this,
            "Confirm Delete",
            "Are you sure you want to delete "
            "this blood request?",
            QMessageBox::Yes |
            QMessageBox::No
        );

    if (reply != QMessageBox::Yes)
    {
        return;
    }

    if (adminManager.deleteRequest(requestId))
    {
        reportManager.logRequestDeleted(
            requestId
        );

        QMessageBox::information(
            this,
            "Success",
            "Blood request deleted successfully."
        );

        refreshDashboard();
    }
    else
    {
        QMessageBox::warning(
            this,
            "Delete Failed",
            "Unable to delete blood request."
        );
    }
}

// ---------------------------------------------------------------
// Update Request Status
// ---------------------------------------------------------------

void AdminDashboard::updateSelectedRequestStatus()
{
    int row =
        requestsTable->currentRow();

    if (row < 0)
    {
        QMessageBox::warning(
            this,
            "No Selection",
            "Please select a blood request."
        );

        return;
    }

    QTableWidgetItem* item =
        requestsTable->item(row, 0);

    if (!item)
    {
        return;
    }

    int requestId =
        item->text().toInt();

    QString status =
        statusComboBox->currentText();

    if (adminManager.updateRequestStatus(
            requestId,
            status))
    {
        reportManager.logRequestStatusChanged(
            requestId,
            status
        );

        QMessageBox::information(
            this,
            "Success",
            "Request status updated."
        );

        refreshDashboard();
    }
    else
    {
        QMessageBox::warning(
            this,
            "Update Failed",
            "Unable to update request status."
        );
    }
}