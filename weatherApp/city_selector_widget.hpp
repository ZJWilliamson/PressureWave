#pragma once
#ifndef CITY_SELECTOR_WIDGET
#define CITY_SELECTOR_WIDGET
#include <QGeoCoordinate>
#include <QWidget>

#include "ui_city_selector_widget.h"
#include "city_model.hpp"

class PropertyModel;

class CitySelectorWidget : public QWidget {
	Q_OBJECT

public:
	CitySelectorWidget(QWidget * parent = Q_NULLPTR);
	~CitySelectorWidget();

protected slots:
	void onCityComboChanged(int);
	void onCityModelChanged(int, QVariant);

private:
	void setModel(PropertyModel *);

	std::vector<CityData> cities;
	CityData selectedCity;
	Ui::city_selector_widget ui;
};

#endif