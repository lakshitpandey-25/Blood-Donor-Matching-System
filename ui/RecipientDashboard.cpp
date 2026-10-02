#include "RecipientDashboard.h"

#include <QLabel>
#include <QVBoxLayout>

RecipientDashboard::RecipientDashboard()
{
    setWindowTitle("Recipient Dashboard");
    resize(500, 400);

    QLabel* titleLabel =
        new QLabel("Welcome to Recipient Dashboard");

    QLabel* infoLabel =
        new QLabel(
            "Here you will create blood requests,\n"
            "find matching donors, and track requests."
        );

    QVBoxLayout* layout =
        new QVBoxLayout();

    layout->addWidget(titleLabel);
    layout->addWidget(infoLabel);

    setLayout(layout);
}