#include "AdminManager.h"

// ---------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------

AdminManager::AdminManager()
{
    ready = database.openDatabase()
            && database.createTables();
}

// ---------------------------------------------------------------
// Database Status
// ---------------------------------------------------------------

bool AdminManager::isReady() const
{
    return ready;
}

QString AdminManager::getLastError() const
{
    return database.getLastError();
}

// ---------------------------------------------------------------
// Admin Authentication
// ---------------------------------------------------------------

bool AdminManager::verifyAdmin(
    const QString& email,
    const QString& password) const
{
    if (!ready)
    {
        return false;
    }

    QString role;
    int userId = 0;

    if (!database.authenticateUser(
            email,
            password,
            role,
            userId))
    {
        return false;
    }

    return role == "Admin";
}

// ---------------------------------------------------------------
// View Records
// ---------------------------------------------------------------

QList<Donor> AdminManager::getAllDonors() const
{
    return database.getAllDonors();
}

QList<Recipient> AdminManager::getAllRecipients() const
{
    return database.getAllRecipients();
}

QList<QVariantMap> AdminManager::getAllBloodRequests() const
{
    return database.getAllBloodRequests();
}

// ---------------------------------------------------------------
// User Management
// ---------------------------------------------------------------

bool AdminManager::deleteUser(int userId)
{
    // Admin accounts cannot be removed
    // from the dashboard.
    if (database.getUserRole(userId) == "Admin")
    {
        return false;
    }

    return database.deleteUser(userId);
}

bool AdminManager::setDonorEligibility(
    int userId,
    bool eligible)
{
    return database.updateDonorEligibility(
        userId,
        eligible
    );
}

// ---------------------------------------------------------------
// Blood Request Management
// ---------------------------------------------------------------

bool AdminManager::updateRequestStatus(
    int requestId,
    const QString& status)
{
    return database.updateBloodRequestStatus(
        requestId,
        status
    );
}

bool AdminManager::deleteRequest(int requestId)
{
    return database.deleteBloodRequest(requestId);
}

// ---------------------------------------------------------------
// Dashboard Statistics
// ---------------------------------------------------------------

int AdminManager::getTotalDonors() const
{
    return database.countUsersByRole("Donor");
}

int AdminManager::getTotalRecipients() const
{
    return database.countUsersByRole("Recipient");
}

int AdminManager::getTotalRequests() const
{
    return database.countBloodRequests();
}

int AdminManager::getPendingRequests() const
{
    return database.countBloodRequests("Pending");
}