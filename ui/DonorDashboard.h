#ifndef DONORDASHBOARD_H
#define DONORDASHBOARD_H

#include <QWidget>

class DonorDashboard : public QWidget
{
public:
    DonorDashboard(int userId);

private:
    int userId;
};

#endif