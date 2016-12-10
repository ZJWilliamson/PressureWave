#include "network_manager.hpp"
#include "weatherapp.h"
#include "city_model.hpp"

#include <QLoggingCategory>
#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkSession>
#include <QUrlQuery>
#include <QDebug>
#include <QSignalMapper>
#include <QUrl>
#include <QUrlQuery>
#include <QSignalMapper>
#include <qjsondocument.h>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>

Q_LOGGING_CATEGORY(requestsLog_B, "wapp.requests")

NetworkManager * NetworkManager::networkManager = 0;

NetworkManager* NetworkManager::get()
{
	if (!networkManager)
	{
		networkManager = new NetworkManager();
	}
	return networkManager;
}

NetworkManager::NetworkManager()
{
	nam = new QNetworkAccessManager(this);

	QNetworkConfigurationManager ncm;
	ns = new QNetworkSession(ncm.defaultConfiguration(), this);
	connect(ns, SIGNAL(opened()), this, SLOT(networkSessionOpened()));
	// the session may be already open. if it is, run the slot directly
	if (ns->isOpen())
	{
		this->networkSessionOpened();
	}
	// tell the system we want network
	ns->open();

	delayedCityRequestTimer.setSingleShot(true);
	delayedCityRequestTimer.setInterval(1000); // 1 s
	requestNewWeatherTimer.setSingleShot(false);
	requestNewWeatherTimer.setInterval(20 * 60 * 1000); // 20 min
	throttle.invalidate();
	app_ident = QStringLiteral("36496bad1955bf3365448965a42b9eac");


	//geoReplyMapper = new QSignalMapper(this);
	weatherReplyMapper = new QSignalMapper(this);
	//forecastReplyMapper = new QSignalMapper(this);

	//connect(geoReplyMapper, SIGNAL(mapped(QObject*)),
	//	this, SLOT(handleGeoNetworkData(QObject*)));
	connect(weatherReplyMapper, SIGNAL(mapped(QObject*)),
		this, SLOT(handleWeatherNetworkData(QObject*)));
	//connect(forecastReplyMapper, SIGNAL(mapped(QObject*)),
	//	this, SLOT(handleForecastNetworkData(QObject*)));
	connect(&delayedCityRequestTimer, SIGNAL(timeout()),
		this, SLOT(queryCity()));
	connect(&requestNewWeatherTimer, SIGNAL(timeout()),
		this, SLOT(refreshWeather()));
	requestNewWeatherTimer.start();

	connect(CityModel::get(), SIGNAL(valueChanged(int, QVariant)), this, SLOT(onCityModelChanged(int, QVariant)));
	throttle.invalidate();
}

void NetworkManager::onCityModelChanged(int propertyId, QVariant value)
{
	switch (propertyId)
	{
	case CityModel::City:
	{
		CityData cityData = value.value<CityData>();
		coord = cityData.coord;
		queryCity();
		break;
	}
	default:
	{
		break;
	}
	}
}

void NetworkManager::networkSessionOpened()
{
	//src = QGeoPositionInfoSource::createDefaultSource(this);

	/*if (src) {
	useGps = true;
	connect(src, SIGNAL(positionUpdated(QGeoPositionInfo)),
	this, SLOT(positionUpdated(QGeoPositionInfo)));
	connect(src, SIGNAL(error(QGeoPositionInfoSource::Error)),
	this, SLOT(positionError(QGeoPositionInfoSource::Error)));
	src->startUpdates();
	}
	else {*/
	/*useGps = false;		//TODO: INCLUDE BACK IN
	city = "Brisbane";
	emit cityChanged();
	queryCity();*/
	//this->refreshWeather();
	//}
}
//don't update more often then once a minute
//to keep load on server low

void NetworkManager::queryCity()
{
	if (throttle.isValid() && throttle.elapsed() < minMsBeforeNewRequest) {
		qDebug(requestsLog_B) << "delaying query of city";
		if (!delayedCityRequestTimer.isActive())
			delayedCityRequestTimer.start();
		return;
	}
	qDebug(requestsLog_B) << "requested query of city";
	throttle.start();
	minMsBeforeNewRequest = (nErrors + 1) * baseMsBeforeNewRequest;

	QString latitude, longitude;
	longitude.setNum(coord.longitude());
	latitude.setNum(coord.latitude());
	
	QUrl url("http://api.openweathermap.org/data/2.5/weather");
	QUrlQuery query;
	query.addQueryItem("lat", latitude);
	query.addQueryItem("lon", longitude);
	query.addQueryItem("mode", "json");
	query.addQueryItem("APPID", app_ident);
	url.setQuery(query);
	qDebug(requestsLog_B) << "submitting request";

	QNetworkReply *rep = nam->get(QNetworkRequest(url));
	// connect up the signal right away
	// ZW: I don't have gps locator on dev laptop. Should manually set coordinates to London and lose geomapper
	/*geoReplyMapper->setMapping(rep, rep);
	connect(rep, SIGNAL(finished()),
		geoReplyMapper, SLOT(map()));*/
}

void NetworkManager::refreshWeather()
{
	if (city.isEmpty()) {
		qDebug(requestsLog_B) << "refreshing weather skipped (no city)";
		//return;
	}
	qDebug(requestsLog_B) << "refreshing weather";
	QUrl url("http://api.openweathermap.org/data/2.5/weather");
	QUrlQuery query;

	query.addQueryItem("q", city);
	query.addQueryItem("mode", "json");
	query.addQueryItem("APPID", app_ident);
	url.setQuery(query);

	QNetworkReply *rep = nam->get(QNetworkRequest(url));
	// connect up the signal right away
	weatherReplyMapper->setMapping(rep, rep);
	connect(rep, SIGNAL(finished()),
		weatherReplyMapper, SLOT(map()));
}


void NetworkManager::handleWeatherNetworkData(QObject *replyObj)
{
	qCDebug(requestsLog_B) << "got weather network data";
	QNetworkReply *networkReply = qobject_cast<QNetworkReply*>(replyObj);
	if (!networkReply)
		return;

	if (!networkReply->error()) {
		/*foreach(WeatherData *inf, forecast)
			delete inf;
		forecast.clear();*/

		QJsonDocument document = QJsonDocument::fromJson(networkReply->readAll());

		if (document.isObject()) {
			QJsonObject obj = document.object();
			QJsonObject tempObject;
			QJsonValue val;

			if (obj.contains(QStringLiteral("weather"))) {
				val = obj.value(QStringLiteral("weather"));
				QJsonArray weatherArray = val.toArray();
				val = weatherArray.at(0);
				tempObject = val.toObject();
				//ui.testLabel->setText(tempObject.value(QStringLiteral("description")).toString());
				//now.setWeatherIcon(tempObject.value("icon").toString());
			}
			if (obj.contains(QStringLiteral("main"))) {
				val = obj.value(QStringLiteral("main"));
				tempObject = val.toObject();
				val = tempObject.value(QStringLiteral("temp"));
				//ui.tempLabel->setText(niceTemperatureString(val.toDouble()));
			}
		}
	}
	networkReply->deleteLater();

	//retrieve the forecast
	QUrl url("http://api.openweathermap.org/data/2.5/forecast/daily");
	QUrlQuery query;

	query.addQueryItem("q", city);
	query.addQueryItem("mode", "json");
	query.addQueryItem("cnt", "5");
	query.addQueryItem("APPID", app_ident);
	url.setQuery(query);

	QNetworkReply *rep = nam->get(QNetworkRequest(url));
	// connect up the signal right away
	forecastReplyMapper->setMapping(rep, rep);
	connect(rep, SIGNAL(finished()), forecastReplyMapper, SLOT(map()));
}

QString NetworkManager::niceTemperatureString(double t)
{
	return QString::number(qRound(t - ZERO_KELVIN)) + QChar(0xB0);
}