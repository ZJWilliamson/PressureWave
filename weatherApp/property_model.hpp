#pragma once
#ifndef PROPERTY_MODEL
#define PROPERTY_MODEL

#include <QObject>
#include <QVariant>

class PropertyModel : public QObject
{
	Q_OBJECT

public:
	virtual QVariant value(int propertyId) const = 0;
	virtual void setValue(int propertyId, QVariant value) { Q_UNUSED(propertyId); }

signals:
	void valueChanged(int propertyId, QVariant value);

protected:
	PropertyModel() {};
};

#endif