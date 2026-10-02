#include "AdminDashboard.h"

#include <QLabel>
#include <QVBoxLayout>

AdminDashboard::AdminDashboard()
{
    setWindowTitle("Admin Dashboard");
    resize(500, 400);

    QLabel* titleLabel =
        new QLabel("Welcome to Admin Dashboard");

    QLabel* infoLabel =
        new QLabel(
            "Here you will manage users,\n"
            "donors, recipients, and system records."
        );

    QVBoxLayout* layout =
        new QVBoxLayout();

    layout->addWidget(titleLabel);
    layout->addWidget(infoLabel);

    setLayout(layout);
}