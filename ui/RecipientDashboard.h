#ifndef RECIPIENTDASHBOARD_H
#define RECIPIENTDASHBOARD_H

#include <QWidget>

class RecipientDashboard : public QWidget
{
public:
    RecipientDashboard(int userId);

private:
    int userId;
};

#endif