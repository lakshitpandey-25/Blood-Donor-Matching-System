#ifndef RECIPIENTDASHBOARD_H
#define RECIPIENTDASHBOARD_H

#include <QWidget>

class QComboBox;
class QLineEdit;
class QLabel;
class QTableWidget;

class RecipientDashboard : public QWidget
{
public:
 explicit RecipientDashboard(int recipientId);

private:
    QComboBox* bloodGroupInput;
    QLineEdit* cityInput;
    QLineEdit* unitsInput;
    QLabel* messageLabel;
    QTableWidget* requestsTable;

    int recipientId;

    void createBloodRequest();
    void loadRequests();
};

#endif
