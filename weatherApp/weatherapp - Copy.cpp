#include "weatherapp.h"
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkSession>
#include <qnetworkconfigmanager.h>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <qgeoserviceprovider.h>
#include <qgeopositioninfosource.h>
#include <QLoggingCategory>
#include <QUrl>
#include <QUrlQuery>
#include <QSignalMapper>
#include <qjsondocument.h>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
//Design Spec...
//Step 1: Hook into yahoo weather API and display any kind of information
//Step 2: Display pressures
//Step 3: Create data structures to store and display data, save locations in QSettings and allow multiple locations
//Step 4: 

Q_LOGGING_CATEGORY(requestsLog, "wapp.requests")

#define ZERO_KELVIN 273.15

weatherapp::weatherapp(QWidget *parent)
	: QMainWindow(parent), buttonState(true),
	src(NULL),
	nam(NULL),
	ns(NULL),
	useGps(true),
	nErrors(0),
	minMsBeforeNewRequest(baseMsBeforeNewRequest)
{
	ui.setupUi(this);

	delayedCityRequestTimer.setSingleShot(true);
	delayedCityRequestTimer.setInterval(1000); // 1 s
	requestNewWeatherTimer.setSingleShot(false);
	requestNewWeatherTimer.setInterval(20 * 60 * 1000); // 20 min
	throttle.invalidate();
	app_ident = QStringLiteral("36496bad1955bf3365448965a42b9eac");

	coord.setLatitude(51.5074);
	coord.setLongitude(0.1278);

	geoReplyMapper = new QSignalMapper(this);
	weatherReplyMapper = new QSignalMapper(this);
	forecastReplyMapper = new QSignalMapper(this);

	connect(geoReplyMapper, SIGNAL(mapped(QObject*)),
		this, SLOT(handleGeoNetworkData(QObject*)));
	connect(weatherReplyMapper, SIGNAL(mapped(QObject*)),
		this, SLOT(handleWeatherNetworkData(QObject*)));
	connect(forecastReplyMapper, SIGNAL(mapped(QObject*)),
		this, SLOT(handleForecastNetworkData(QObject*)));
	connect(&delayedCityRequestTimer, SIGNAL(timeout()),
		this, SLOT(queryCity()));
	connect(&requestNewWeatherTimer, SIGNAL(timeout()),
		this, SLOT(refreshWeather()));
	requestNewWeatherTimer.start();

	throttle.invalidate();
	connect(ui.testButton, SIGNAL(clicked()), this, SLOT(onTestButtonClicked()));

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
}

weatherapp::~weatherapp()
{
	
}

void weatherapp::onTestButtonClicked()
{
    QString text;
	if (buttonState)
	{
		text = "test one two three";
	}
	else
	{
		text = "test four five six";
	}
	buttonState = !buttonState;
	ui.testLabel->setText(text);
}

void weatherapp::networkSessionOpened()
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
		useGps = false;
		city = "London";
		emit cityChanged();
		queryCity();
		//this->refreshWeather();
	//}
}

void weatherapp::positionUpdated(QGeoPositionInfo gpsPos)
{
	/*coord = gpsPos.coordinate();

	if (!(useGps))
		return;
	*/
	queryCity();
}

void weatherapp::positionError(QGeoPositionInfo gpsPos)
{

	int  breakpoint = 5;
}
void weatherapp::queryCity()
{
	//don't update more often then once a minute
	//to keep load on server low
	if (throttle.isValid() && throttle.elapsed() < minMsBeforeNewRequest) {
		qDebug(requestsLog) << "delaying query of city";
		if (!delayedCityRequestTimer.isActive())
			delayedCityRequestTimer.start();
		return;
	}
	qDebug(requestsLog) << "requested query of city";
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
	qDebug(requestsLog) << "submitting request";

	QNetworkReply *rep = nam->get(QNetworkRequest(url));
	// connect up the signal right away
	geoReplyMapper->setMapping(rep, rep);
	connect(rep, SIGNAL(finished()),
		geoReplyMapper, SLOT(map()));
}

void weatherapp::handleGeoNetworkData(QObject *replyObj)
{
	QNetworkReply *networkReply = qobject_cast<QNetworkReply*>(replyObj);
	if (!networkReply) {
		//hadError(false); // should retry?
		return;
	}

	if (!networkReply->error()) {
		nErrors = 0;
		if (!throttle.isValid())
			throttle.start();
		minMsBeforeNewRequest = baseMsBeforeNewRequest;
		//convert coordinates to city name
		QJsonDocument document = QJsonDocument::fromJson(networkReply->readAll());

		QJsonObject jo = document.object();
		QJsonValue jv = jo.value(QStringLiteral("name"));

		const QString newCity = jv.toString();
		qCDebug(requestsLog) << "got city: " << newCity;
		if (city != newCity) {
			city = newCity;
			emit cityChanged();
			refreshWeather();
		}
	}
	else {
		//hadError(true);
	}
	networkReply->deleteLater();
}

void weatherapp::refreshWeather()
{
	if (city.isEmpty()) {
		qCDebug(requestsLog) << "refreshing weather skipped (no city)";
		//return;
	}
	qCDebug(requestsLog) << "refreshing weather";
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

void weatherapp::handleWeatherNetworkData(QObject *replyObj)
{
	qCDebug(requestsLog) << "got weather network data";
	QNetworkReply *networkReply = qobject_cast<QNetworkReply*>(replyObj);
	if (!networkReply)
		return;

	if (!networkReply->error()) {
		foreach(WeatherData *inf, forecast)
			delete inf;
		forecast.clear();

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
				ui.testLabel->setText(tempObject.value(QStringLiteral("description")).toString());
				//now.setWeatherIcon(tempObject.value("icon").toString());
			}
			if (obj.contains(QStringLiteral("main"))) {
				val = obj.value(QStringLiteral("main"));
				tempObject = val.toObject();
				val = tempObject.value(QStringLiteral("temp"));
				ui.tempLabel->setText(niceTemperatureString(val.toDouble()));
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

QString weatherapp::niceTemperatureString(double t)
{
	return QString::number(qRound(t - ZERO_KELVIN)) + QChar(0xB0);
}