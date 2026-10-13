
#ifndef DONORDASHBOARD_H
#define DONORDASHBOARD_H

#include <QWidget>
#include "../models/DonationHistory.h"

class QLabel;
class QLineEdit;
class QDateEdit;
class QTableWidget;

class DonorDashboard : public QWidget
{
public:
    explicit DonorDashboard(int userId);

private:
    int userId;

    QLabel* infoLabel;

    QLineEdit* hospitalEdit;
    QDateEdit* dateEdit;

    QTableWidget* historyTable;
    QTableWidget* requestsTable;

    DonationHistory history;

    void refreshInfo();
    void loadHistory();
    void loadBloodRequests();

    void addDonation();
    void exportCsv();
    void acceptBloodRequest(int requestId);
};

#endif
