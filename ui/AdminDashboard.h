#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

#include "../modules/admin/AdminManager.h"
#include "../modules/reports/ReportManager.h"

class AdminDashboard : public QWidget
{
    Q_OBJECT

private:
    AdminManager adminManager;
    ReportManager reportManager;

    // Statistics
    QLabel* donorsLabel;
    QLabel* recipientsLabel;
    QLabel* requestsLabel;
    QLabel* pendingLabel;

    // Tables
    QTableWidget* donorsTable;
    QTableWidget* recipientsTable;
    QTableWidget* requestsTable;

    // Buttons
    QPushButton* refreshButton;
    QPushButton* generateReportButton;
    QPushButton* deleteUserButton;
    QPushButton* deleteRequestButton;
    QPushButton* updateEligibilityButton;
    QPushButton* updateStatusButton;

    // Request status
    QComboBox* statusComboBox;

    // UI setup
    void setupUI();
    void setupStatistics();
    void setupDonorsTable();
    void setupRecipientsTable();
    void setupRequestsTable();

    // Data loading
    void loadStatistics();
    void loadDonors();
    void loadRecipients();
    void loadRequests();

private slots:
    void refreshDashboard();
    void generateReport();
    void deleteSelectedUser();
    void deleteSelectedRequest();
    void updateSelectedDonorEligibility();
    void updateSelectedRequestStatus();

public:
    explicit AdminDashboard(QWidget* parent = nullptr);
};

#endif // ADMINDASHBOARD_H