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

#include "network_manager.hpp"
//Design Spec...
//Step 1: Hook into yahoo weather API and display any kind of information
//Step 2: Display pressures
//Step 3: Create data structures to store and display data, save locations in QSettings and allow multiple locations
//Step 4: 

Q_LOGGING_CATEGORY(requestsLog, "wapp.requests")

weatherapp::weatherapp(QWidget *parent)
	: QMainWindow(parent), 
	buttonState(true),
	useGps(true),
	nErrors(0),
	minMsBeforeNewRequest(baseMsBeforeNewRequest)
{
	ui.setupUi(this);

	coord.setLatitude(51.5074);
	coord.setLongitude(0.1278);

	NetworkManager *theManager = NetworkManager::get();
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
