#include "city_model.hpp"

CityModel * CityModel::cityModel = 0;

CityModel * CityModel::get()
{
	if (!cityModel)
	{
		cityModel = new CityModel();
	}
	return cityModel;
}

CityModel::CityModel()
{
	city = getCity(MOSCOW);
}

void CityModel::initialise()
{
	setValue(City, QVariant::fromValue(getCity(MOSCOW)));
}

QVariant CityModel::value(int propertyId) const
{
	switch (propertyId)
	{
		case City:
		{
			return QVariant::fromValue(city);
			break;
		}
		default:
		{
			return 0;
		}
	}
}

void CityModel::setValue(int propertyId, const QVariant value)
{
	switch (propertyId)
	{
		case City:
		{
			city = value.value<CityData>();
			emit valueChanged(City, value);
			break;
		}
		default:
		{
			break;
		}
	}
}

void CityModel::refresh()
{
	//?
}