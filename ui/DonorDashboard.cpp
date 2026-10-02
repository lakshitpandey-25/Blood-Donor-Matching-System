#include "DonorDashboard.h"

#include <QLabel>
#include <QVBoxLayout>

DonorDashboard::DonorDashboard()
{
    setWindowTitle("Donor Dashboard");
    resize(500, 400);

    QLabel* titleLabel =
        new QLabel("Welcome to Donor Dashboard");

    QLabel* infoLabel =
        new QLabel(
            "Here you will manage your donor profile,\n"
            "donation history, and eligibility."
        );

    QVBoxLayout* layout =
        new QVBoxLayout();

    layout->addWidget(titleLabel);
    layout->addWidget(infoLabel);

    setLayout(layout);
}