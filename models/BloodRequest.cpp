#include "BloodRequest.h"

BloodRequest::BloodRequest()
    : requestId(-1),
      requesterId(-1),
      bloodGroup(""),
      unitsRequired(0),
      hospital(""),
      city(""),
      requestDate(QDate::currentDate()),
      status("Pending")
{
}

BloodRequest::BloodRequest(int requestId,
                           int requesterId,
                           QString bloodGroup,
                           int unitsRequired,
                           QString hospital,
                           QString city,
                           QDate requestDate,
                           QString status)
    : requestId(requestId),
      requesterId(requesterId),
      bloodGroup(bloodGroup),
      unitsRequired(unitsRequired),
      hospital(hospital),
      city(city),
      requestDate(requestDate),
      status(status)
{
}

int BloodRequest::getRequestId() const
{
    return requestId;
}

int BloodRequest::getRequesterId() const
{
    return requesterId;
}

QString BloodRequest::getBloodGroup() const
{
    return bloodGroup;
}

int BloodRequest::getUnitsRequired() const
{
    return unitsRequired;
}

QString BloodRequest::getHospital() const
{
    return hospital;
}

QString BloodRequest::getCity() const
{
    return city;
}

QDate BloodRequest::getRequestDate() const
{
    return requestDate;
}

QString BloodRequest::getStatus() const
{
    return status;
}

void BloodRequest::setBloodGroup(QString bloodGroup)
{
    this->bloodGroup = bloodGroup;
}

void BloodRequest::setUnitsRequired(int unitsRequired)
{
    this->unitsRequired = unitsRequired;
}

void BloodRequest::setHospital(QString hospital)
{
    this->hospital = hospital;
}

void BloodRequest::setCity(QString city)
{
    this->city = city;
}

void BloodRequest::setRequestDate(QDate requestDate)
{
    this->requestDate = requestDate;
}

void BloodRequest::setStatus(QString status)
{
    this->status = status;
}