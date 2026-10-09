#ifndef ADMINMANAGER_H
#define ADMINMANAGER_H

#include <QString>
#include <QList>
#include <QVariantMap>

#include "../database/DatabaseManager.h"
#include "../../models/Donor.h"
#include "../../models/Recipient.h"

class AdminManager
{
private:
    DatabaseManager database;
    bool ready;

public:
    AdminManager();

    bool isReady() const;
    QString getLastError() const;

    // Admin login check (used with the Admin option in LoginWindow)
    bool verifyAdmin(const QString& email,
                     const QString& password) const;

    // Viewing records
    QList<Donor> getAllDonors() const;
    QList<Recipient> getAllRecipients() const;
    QList<QVariantMap> getAllBloodRequests() const;

    // Managing users
    bool deleteUser(int userId);
    bool setDonorEligibility(int userId, bool eligible);

    // Managing blood requests
    bool updateRequestStatus(int requestId, const QString& status);
    bool deleteRequest(int requestId);

    // Dashboard statistics
    int getTotalDonors() const;
    int getTotalRecipients() const;
    int getTotalRequests() const;
    int getPendingRequests() const;
};

#endif