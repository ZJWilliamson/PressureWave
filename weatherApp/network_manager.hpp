#ifndef NETWORK_MANAGER_HPP
#define NETWORK_MANAGER_HPP

#include <QObject>
#include <QElapsedTimer>
#include <QGeoCoordinate>
#include <QTimer>
#include <QVariant>

class QNetworkAccessManager;
class QNetworkSession;
class QSignalMapper;

class NetworkManager : public QObject
{
	Q_OBJECT
public:
	static NetworkManager* get();

protected slots:
	void			queryCity();
	void			refreshWeather();
	void			onCityModelChanged(int, QVariant);
	void			handleWeatherNetworkData(QObject *replyObj);

private:
					NetworkManager();
	void			networkSessionOpened();
	QString			niceTemperatureString(double t);

private:
	QNetworkAccessManager * nam;
	QNetworkSession *		ns;
	QElapsedTimer			throttle;
	QSignalMapper *weatherReplyMapper, *forecastReplyMapper;
	QGeoCoordinate			coord;

	int						nErrors;
	int						minMsBeforeNewRequest;
	static const int		baseMsBeforeNewRequest = 5 * 1000; // 5 s, increased after each missing answer up to 10x
	QTimer					delayedCityRequestTimer;
	QTimer					requestNewWeatherTimer;
	QString					app_ident;
	QString					city;

	static NetworkManager *		networkManager;
};

#endif