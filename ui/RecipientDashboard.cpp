#include "RecipientDashboard.h"

#include "../modules/database/DatabaseManager.h"

#include <QLabel>
#include <QVBoxLayout>

RecipientDashboard::RecipientDashboard(int userId)
{
    this->userId = userId;

    setWindowTitle("Recipient Dashboard");
    resize(500, 400);

    DatabaseManager database;

    QLabel* titleLabel =
        new QLabel("Welcome to Recipient Dashboard");

    QLabel* infoLabel =
        new QLabel();

    if (database.openDatabase())
    {
        database.createTables();

        Recipient recipient = database.getRecipientById(userId);

        infoLabel->setText(
            "Name: " + recipient.getName() + "\n"
            "Email: " + recipient.getEmail() + "\n"
            "Phone: " + recipient.getPhone() + "\n"
            "Required Blood Group: " +
                recipient.getRequiredBloodGroup() + "\n"
            "City: " + recipient.getCity()
        );
    }
    else
    {
        infoLabel->setText("Unable to load recipient details.");
    }

    QVBoxLayout* layout =
        new QVBoxLayout();

    layout->addWidget(titleLabel);
    layout->addWidget(infoLabel);

    setLayout(layout);
}