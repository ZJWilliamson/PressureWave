#include "city_selector_widget.hpp"

#include "city_model.hpp"
CitySelectorWidget::CitySelectorWidget(QWidget * parent) : QWidget(parent) {
	ui.setupUi(this);

	cities.push_back(CityData(LONDON, "London", QGeoCoordinate(0, 0)));
	cities.push_back(CityData(BERLIN, "Berlin", QGeoCoordinate(0, 0)));
	cities.push_back(CityData(AMSTERDAM, "Amsterdam", QGeoCoordinate(0, 0)));
	cities.push_back(CityData(NEW_YORK, "New York", QGeoCoordinate(0, 0)));
	cities.push_back(CityData(MOSCOW, "Moscow", QGeoCoordinate(0, 0)));
	cities.push_back(CityData(BEJING, "Bejing", QGeoCoordinate(0, 0)));
	cities.push_back(CityData(CAPE_TOWN, "Cape Town", QGeoCoordinate(0, 0)));

	for (uint i = 0; i < CITIES_MAX; i++)
	{
		CityData cityData = getCity((CityType)i);
		ui.cityCombo->addItem(cityData.cityName, QVariant::fromValue(cityData));
	}
	connect(ui.cityCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onCityComboChanged(int)));

	connect(CityModel::get(), SIGNAL(valueChanged(int, QVariant)), this, SLOT(onCityModelChanged(int, QVariant)));
	setModel(CityModel::get());
	CityModel::get()->initialise();
}

CitySelectorWidget::~CitySelectorWidget() {
	
}

void CitySelectorWidget::onCityModelChanged(int propertyId, QVariant value)
{
	switch (propertyId)
	{
		case CityModel::City:
		{
			CityData cityData = value.value<CityData>();

			int index = cityData.city;
			if (index >= 0)
			{
				ui.cityCombo->blockSignals(true);
				ui.cityCombo->setCurrentIndex(index);
				ui.cityCombo->blockSignals(false);
			}
		}
	}
}

void CitySelectorWidget::onCityComboChanged(int index)
{
	CityData cityData = ui.cityCombo->itemData(index, Qt::UserRole).value<CityData>();
	CityModel::get()->setValue(CityModel::City, QVariant::fromValue(cityData));
}

void CitySelectorWidget::setModel(PropertyModel * model)
{
	CityData cityData = model->value(CityModel::City).value<CityData>();

	int index = ui.cityCombo->findData(QVariant::fromValue(cityData), Qt::UserRole);
	if (index >= 0)
	{
		ui.cityCombo->blockSignals(true);
		ui.cityCombo->setCurrentIndex(index);
		ui.cityCombo->blockSignals(false);
	}
}