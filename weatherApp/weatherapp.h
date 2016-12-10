#ifndef WEATHERAPP_H
#define WEATHERAPP_H

#include <QtWidgets/QMainWindow>
#include "ui_weatherapp.h"
#include <QGeoCoordinate>
#include <QElapsedTimer>
#include <QGeoPositionInfo>
#include <QTimer>
class QNetworkAccessManager;
class QNetworkSession;
class QGeoPositionInfoSource;
class QSignalMapper;
#include <QtQml/QQmlListProperty>

#define ZERO_KELVIN 273.15

class WeatherData : public QObject {
	Q_OBJECT
		/*Q_PROPERTY(QString dayOfWeek
			READ dayOfWeek WRITE setDayOfWeek
			NOTIFY dataChanged)
		Q_PROPERTY(QString weatherIcon
			READ weatherIcon WRITE setWeatherIcon
			NOTIFY dataChanged)
		Q_PROPERTY(QString weatherDescription
			READ weatherDescription WRITE setWeatherDescription
			NOTIFY dataChanged)
		Q_PROPERTY(QString temperature
			READ temperature WRITE setTemperature
			NOTIFY dataChanged)*/

public:
	//explicit WeatherData(QObject *parent = 0);
	//WeatherData(const WeatherData &other);

	/*QString dayOfWeek() const;
	QString weatherIcon() const;
	QString weatherDescription() const;
	QString temperature() const;

	void setDayOfWeek(const QString &value);
	void setWeatherIcon(const QString &value);
	void setWeatherDescription(const QString &value);
	void setTemperature(const QString &value);*/

signals:
	void dataChanged();
private:
	QString m_dayOfWeek;
	QString m_weather;
	QString m_weatherDescription;
	QString m_temperature;
};

//Q_DECLARE_METATYPE(WeatherData)

class weatherapp : public QMainWindow
{
	Q_OBJECT

public:
	weatherapp(QWidget *parent = 0);
	~weatherapp();
	QString niceTemperatureString(double t);
	//bool hasValidCity() const;
	//bool hasValidWeather() const;
	//void setUseGps(bool value);
	//void hadError(bool tryAgain);
	WeatherData *weather() const;
	QList<WeatherData*> forecast;
	WeatherData now;
signals:
	void readyChanged();
	void useGpsChanged();
	//void weatherChanged();
	void cityChanged();

protected slots:
	void onTestButtonClicked();

private:
	static const int baseMsBeforeNewRequest = 5 * 1000; // 5 s, increased after each missing answer up to 10x
	Ui::weatherappClass ui;
	bool buttonState;
	bool useGps;
	QString longitude, latitude;
	QString city;
	QGeoCoordinate coord;
	QElapsedTimer throttle;
	int nErrors;
	int minMsBeforeNewRequest;
	QTimer delayedCityRequestTimer;
	QTimer requestNewWeatherTimer;
	QString app_ident;
};

#endif // WEATHERAPP_H
