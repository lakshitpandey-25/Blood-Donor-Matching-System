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
    // ---------- Constructor ----------
    AdminManager();

    // ---------- Database Status ----------
    bool isReady() const;
    QString getLastError() const;

    // ---------- Admin Authentication ----------
    bool verifyAdmin(
        const QString& email,
        const QString& password
    ) const;

    // ---------- View Records ----------
    QList<Donor> getAllDonors() const;
    QList<Recipient> getAllRecipients() const;
    QList<QVariantMap> getAllBloodRequests() const;

    // ---------- User Management ----------
    bool deleteUser(int userId);

    bool setDonorEligibility(
        int userId,
        bool eligible
    );

    // ---------- Blood Request Management ----------
    bool updateRequestStatus(
        int requestId,
        const QString& status
    );

    bool deleteRequest(int requestId);

    // ---------- Dashboard Statistics ----------
    int getTotalDonors() const;
    int getTotalRecipients() const;
    int getTotalRequests() const;
    int getPendingRequests() const;
};

#endif // ADMINMANAGER_H