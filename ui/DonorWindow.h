#ifndef DONORWINDOW_H
#define DONORWINDOW_H

#include <QWidget>
#include "../models/Donor.h"

class QLabel;
class QLineEdit;
class QDateEdit;
class QTableWidget;

class DonorWindow : public QWidget
{
public:
    explicit DonorWindow(int userId, QWidget *parent = nullptr);

private:
    int userId;
    Donor donor;

    QLabel *profileLabel;
    QLineEdit *hospitalEdit;
    QDateEdit *dateEdit;
    QTableWidget *historyTable;

    void refreshProfile();
    void refreshHistory();
    void onAddDonation();
    void onExportCsv();
};

#endif
