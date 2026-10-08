#ifndef REPORTMANAGER_H
#define REPORTMANAGER_H

#include <QString>
#include <QList>
#include <QVariantMap>

#include "../database/DatabaseManager.h"

class ReportManager
{
private:
    DatabaseManager database;

    QString exportDirectory;
    QString logDirectory;

    bool ensureDirectories() const;

public:
    ReportManager();

    bool isReady() const;
    QString getLastError() const;

    // CSV report
    bool generateAdminReport(
        const QString& fileName = "admin_report.csv");

    // System log
    bool logAction(
        const QString& action,
        const QString& details);

    // Convenience logging methods
    bool logAdminLogin(const QString& email);
    bool logUserDeleted(int userId);
    bool logRequestStatusChanged(
        int requestId,
        const QString& status);
    bool logRequestDeleted(int requestId);

    QString getExportDirectory() const;
    QString getLogDirectory() const;
};

#endif // REPORTMANAGER_H