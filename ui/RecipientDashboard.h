
#ifndef RECIPIENTDASHBOARD_H
#define RECIPIENTDASHBOARD_H

#include <QWidget>

class RecipientDashboard : public QWidget
{
public:
    explicit RecipientDashboard(int recipientId,
                                QWidget* parent = nullptr);

private:
    int recipientId;
};

#endif