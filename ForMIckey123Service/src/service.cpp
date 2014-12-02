/*
 * Copyright (c) 2013-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "service.hpp"

#include <bb/Application>
#include <bb/platform/Notification>
#include <bb/platform/NotificationDefaultApplicationSettings>
#include <bb/system/InvokeManager>
#include <bb/system/SystemUiButton>
#include <bb/system/SystemUiError>

#include <QTimer>

using namespace bb::platform;
using namespace bb::system;

Service::Service() :
        QObject(),
        m_notify(new Notification(this)),
        m_invokeManager(new InvokeManager(this))
{
    qDebug() << "Headless started!";

    m_dialog = new bb::platform::NotificationDialog();
    SystemUiButton *yesButton = new SystemUiButton("Snooze");
    SystemUiButton *noButton = new SystemUiButton("Turn off");
    m_dialog->appendButton(yesButton);
    m_dialog->appendButton(noButton);

    connect(m_dialog, SIGNAL(finished(bb::platform::NotificationResult::Type)), this, SLOT(onDialogFinished(bb::platform::NotificationResult::Type)));

    mp = new bb::multimedia::MediaPlayer(this);
    QString audio = "asset:///Music/Alarmsound.wav";
    mp->setSourceUrl(audio);
    mp->setRepeatMode(bb::multimedia::RepeatMode::Track);

    m_invokeManager->connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
            this, SLOT(handleInvoke(const bb::system::InvokeRequest&)));

    NotificationDefaultApplicationSettings settings;
    settings.setPreview(NotificationPriorityPolicy::Allow);
    settings.apply();

    bb::system::InvokeRequest request;
    request.setTarget("com.example.ForMIckey123");
    request.setAction("bb.action.START");
    m_notify->setInvokeRequest(request);
}

Service::~Service()
{
    qDebug() << "Headless stopped!";
}

void Service::handleInvoke(const bb::system::InvokeRequest & request)
{
    if (request.action().compare("com.example.ForMIckey123Service.SETALARM") == 0) {
        QTimer::singleShot(2000, this, SLOT(onTimeout()));
    }
    if (request.action().compare("com.example.ForMIckey123Service.SHUTDOWN") == 0) {
        qDebug() << "Shutdown";
        bb::Application::instance()->quit();
    }
}

void Service::onTimeout()
{
    qDebug() << "Ring the alarm!";

    Notification::clearEffectsForAll();
    Notification::deleteAllFromInbox();

    m_notify->setTitle("ForMIckey123 Service");
    m_notify->setBody("WAKE UP!!!");
    m_notify->notify();

    mp->play();

    m_dialog->setTitle("WAKE UP!!!");
    m_dialog->setBody("What do you want to do with this alarm?");
    m_dialog->show();
}

void Service::onDialogFinished(bb::platform::NotificationResult::Type type)
{
    qDebug() << "onDialogFinished() :" << type;
    switch (type) {
        case bb::platform::NotificationResult::ButtonSelection: {
            qDebug() << "ButtonSelection";
            break;
        }
        case bb::platform::NotificationResult::Error: {
            qDebug() << "Error";
            break;
        }
        case bb::platform::NotificationResult::None: {
            qDebug() << "None";
            break;
        }
        default: {
            qDebug() << "UNDEFINED";
        }
    }

    qDebug() << m_dialog->error();
    switch (m_dialog->error()) {
        case bb::platform::NotificationError::InvalidRequest: {
            qDebug() << "InvalidRequest";
            break;
        }
        case bb::platform::NotificationError::NoButtons: {
            qDebug() << "NoButtons";
            break;
        }
        case bb::platform::NotificationError::None: {
            qDebug() << "None";
            break;
        }
        case bb::platform::NotificationError::ServiceUnavailable: {
            qDebug() << "ServiceUnavailable";
            break;
        }
        case bb::platform::NotificationError::Unknown: {
            qDebug() << "Unknown";
            break;
        }
        default: {
            qDebug() << "UNDEFINED";
        }
    }

    SystemUiButton *button = m_dialog->buttonSelection();
    if (button->label() == "Snooze") {
        qDebug() << "Snooze";

        mp->stop();

        // Snooze in 5 second
        QTimer::singleShot(5000, this, SLOT(onTimeout()));
    }
    else {
        if (button->label() == "Turn off") {
            qDebug() << "Turn off";

            mp->stop();
        }
    }
}
