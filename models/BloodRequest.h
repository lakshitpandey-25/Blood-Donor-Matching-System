#ifndef BLOODREQUEST_H
#define BLOODREQUEST_H

#include <QString>
#include <QDate>

class BloodRequest
{
private:
    int requestId;
    int requesterId;
    QString bloodGroup;
    int unitsRequired;
    QString hospital;
    QString city;
    QDate requestDate;
    QString status;

public:
    BloodRequest();

    BloodRequest(int requestId,
                 int requesterId,
                 QString bloodGroup,
                 int unitsRequired,
                 QString hospital,
                 QString city,
                 QDate requestDate,
                 QString status);

    int getRequestId() const;
    int getRequesterId() const;
    QString getBloodGroup() const;
    int getUnitsRequired() const;
    QString getHospital() const;
    QString getCity() const;
    QDate getRequestDate() const;
    QString getStatus() const;

    void setBloodGroup(QString bloodGroup);
    void setUnitsRequired(int unitsRequired);
    void setHospital(QString hospital);
    void setCity(QString city);
    void setRequestDate(QDate requestDate);
    void setStatus(QString status);
};

#endif