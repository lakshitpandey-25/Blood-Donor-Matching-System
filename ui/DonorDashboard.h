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
    Q_OBJECT

public:
    DonorDashboard(int userId);

private slots:
    void addDonation();
    void exportCsv();

private:
    int userId;

    QLabel* infoLabel;
    QLineEdit* hospitalEdit;
    QDateEdit* dateEdit;
    QTableWidget* historyTable;

    DonationHistory history;

    void refreshInfo();
    void loadHistory();
};

#endif