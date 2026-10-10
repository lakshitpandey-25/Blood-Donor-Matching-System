#include "ReportManager.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QStandardPaths>

// ---------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------

ReportManager::ReportManager()
{
    database.openDatabase();
    QString appDataPath =
        QStandardPaths::writableLocation(
            QStandardPaths::AppDataLocation);

    exportDirectory =
        appDataPath + "/data/exports";

    logDirectory =
        appDataPath + "/data/logs";

    ensureDirectories();
}

// ---------------------------------------------------------------
// Directory Management
// ---------------------------------------------------------------

bool ReportManager::ensureDirectories() const
{
    QDir exportDir(exportDirectory);
    QDir logDir(logDirectory);

    bool exportOk = exportDir.exists()
                    || exportDir.mkpath(".");

    bool logOk = logDir.exists()
                  || logDir.mkpath(".");

    return exportOk && logOk;
}

// ---------------------------------------------------------------
// Status
// ---------------------------------------------------------------

bool ReportManager::isReady() const
{
    return database.isOpen();
}

QString ReportManager::getLastError() const
{
    return database.getLastError();
}

// ---------------------------------------------------------------
// Generate Admin CSV Report
// ---------------------------------------------------------------

bool ReportManager::generateAdminReport(
    const QString& fileName)
{
    if (!database.isOpen())
    {
        return false;
    }

    if (!ensureDirectories())
    {
        return false;
    }

    QString filePath =
        exportDirectory + "/" + fileName;

    QFile file(filePath);

    if (!file.open(
            QIODevice::WriteOnly |
            QIODevice::Text))
    {
        return false;
    }

    QTextStream out(&file);

    // CSV header
    out << "Report Generated,";
    out << "Total Donors,";
    out << "Total Recipients,";
    out << "Total Requests,";
    out << "Pending Requests\n";

    // Report data
    int totalDonors =
        database.countUsersByRole("Donor");

    int totalRecipients =
        database.countUsersByRole("Recipient");

    int totalRequests =
        database.countBloodRequests();

    int pendingRequests =
        database.countBloodRequests("Pending");

    out << "\""
        << QDateTime::currentDateTime()
               .toString("yyyy-MM-dd HH:mm:ss")
        << "\",";

    out << totalDonors << ",";
    out << totalRecipients << ",";
    out << totalRequests << ",";
    out << pendingRequests << "\n";

    file.close();

    return true;
}

// ---------------------------------------------------------------
// System Logging
// ---------------------------------------------------------------

bool ReportManager::logAction(
    const QString& action,
    const QString& details)
{
    if (!ensureDirectories())
    {
        return false;
    }

    QString filePath =
        logDirectory + "/system_log.txt";

    QFile file(filePath);

    if (!file.open(
            QIODevice::WriteOnly |
            QIODevice::Append |
            QIODevice::Text))
    {
        return false;
    }

    QTextStream out(&file);

    QString timestamp =
        QDateTime::currentDateTime()
            .toString("yyyy-MM-dd HH:mm:ss");

    out << "["
        << timestamp
        << "] "
        << action
        << " - "
        << details
        << "\n";

    file.close();

    return true;
}

// ---------------------------------------------------------------
// Admin Login Log
// ---------------------------------------------------------------

bool ReportManager::logAdminLogin(
    const QString& email)
{
    return logAction(
        "ADMIN_LOGIN",
        "Admin logged in: " + email
    );
}

// ---------------------------------------------------------------
// User Deleted Log
// ---------------------------------------------------------------

bool ReportManager::logUserDeleted(
    int userId)
{
    return logAction(
        "USER_DELETED",
        "User ID: " + QString::number(userId)
    );
}

// ---------------------------------------------------------------
// Request Status Changed Log
// ---------------------------------------------------------------

bool ReportManager::logRequestStatusChanged(
    int requestId,
    const QString& status)
{
    return logAction(
        "REQUEST_STATUS_CHANGED",
        "Request ID: "
        + QString::number(requestId)
        + ", New Status: "
        + status
    );
}

// ---------------------------------------------------------------
// Request Deleted Log
// ---------------------------------------------------------------

bool ReportManager::logRequestDeleted(
    int requestId)
{
    return logAction(
        "REQUEST_DELETED",
        "Request ID: "
        + QString::number(requestId)
    );
}

// ---------------------------------------------------------------
// Directory Getters
// ---------------------------------------------------------------

QString ReportManager::getExportDirectory() const
{
    return exportDirectory;
}

QString ReportManager::getLogDirectory() const
{
    return logDirectory;
}