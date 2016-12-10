#pragma once
#ifndef CITY_MODEL_HPP
#define CITY_MODEL_HPP

#include "property_model.hpp"

#include <QGeoCoordinate>

namespace
{
	enum CityType
	{
		LONDON,
		BERLIN,
		AMSTERDAM,
		BEJING,
		NEW_YORK,
		MOSCOW,
		CAPE_TOWN,

		CITIES_MAX
	};

	struct CityData
	{
		CityType city;
		QString cityName;
		QGeoCoordinate coord;
		CityData() : cityName(""), coord(0, 0) {}
		CityData(CityType city, QString cityName, QGeoCoordinate coord) : city(city), cityName(cityName), coord(coord) {}
	};

	static const CityData & getCity(CityType cityType)
	{
		static const std::vector<CityData> cities =
		{
			CityData(LONDON, "London", QGeoCoordinate(0, 0)),
			CityData(BERLIN, "Berlin", QGeoCoordinate(0, 0)),
			CityData(AMSTERDAM, "Amsterdam", QGeoCoordinate(0, 0)),
			CityData(BEJING, "Bejing", QGeoCoordinate(0, 0)),
			CityData(NEW_YORK, "New York", QGeoCoordinate(0, 0)),
			CityData(MOSCOW, "Moscow", QGeoCoordinate(0, 0)),
			CityData(CAPE_TOWN, "Cape Town", QGeoCoordinate(0, 0)),
		};
		return cities[cityType];
	};
};

Q_DECLARE_METATYPE(CityData);


class CityModel : public PropertyModel
{
	Q_OBJECT

public:
	static CityModel * get();

	enum //property id tags
	{
		City
	};

	void initialise();
	QVariant value(int propertyId) const;
	void setValue(int propertyId, const QVariant value);

signals:
	void valueChanged(int, QVariant);

private slots:
	void refresh();

private:
	CityModel();
	CityData city;

	static CityModel * cityModel;
};

#endif