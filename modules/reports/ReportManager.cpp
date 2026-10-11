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

bool ReportManager::generateAdminReport(const QString& fileName)
{
    if (!database.isOpen() && !database.openDatabase())
        return false;

    if (!ensureDirectories())
        return false;

    QFile file(exportDirectory + "/" + fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);

    auto csv = [](QString value) {
        value.replace("\"", "\"\"");
        return "\"" + value + "\"";
    };

    out << "ADMIN REPORT\n";
    out << "Generated At," 
        << csv(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
        << "\n\n";

    out << "SUMMARY\n";
    out << "Total Donors,Total Recipients,Total Requests,Pending Requests\n";
    out << database.countUsersByRole("Donor") << ","
        << database.countUsersByRole("Recipient") << ","
        << database.countBloodRequests() << ","
        << database.countBloodRequests("Pending") << "\n\n";

    out << "DONORS\n";
    out << "User ID,Name,Phone,Email,Blood Group,City,Eligible\n";
    for (const Donor& d : database.getAllDonors()) {
        out << d.getUserId() << ","
            << csv(d.getName()) << ","
            << csv(d.getPhone()) << ","
            << csv(d.getEmail()) << ","
            << csv(d.getBloodGroup()) << ","
            << csv(d.getCity()) << ","
            << csv(d.isEligible() ? "Yes" : "No") << "\n";
    }

    out << "\nRECIPIENTS\n";
    out << "User ID,Name,Phone,Email,Required Blood,City\n";
    for (const Recipient& r : database.getAllRecipients()) {
        out << r.getUserId() << ","
            << csv(r.getName()) << ","
            << csv(r.getPhone()) << ","
            << csv(r.getEmail()) << ","
            << csv(r.getRequiredBloodGroup()) << ","
            << csv(r.getCity()) << "\n";
    }

    out << "\nBLOOD REQUESTS\n";
    out << "Request ID,Recipient ID,Recipient,Blood Group,City,Units,Status,Created At\n";
    for (const QVariantMap& r : database.getAllBloodRequests()) {
        out << r.value("requestId").toInt() << ","
            << r.value("recipientId").toInt() << ","
            << csv(r.value("recipientName").toString()) << ","
            << csv(r.value("bloodGroup").toString()) << ","
            << csv(r.value("city").toString()) << ","
            << r.value("units").toInt() << ","
            << csv(r.value("status").toString()) << ","
            << csv(r.value("createdAt").toString()) << "\n";
    }

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