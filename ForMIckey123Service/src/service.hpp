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

#ifndef SERVICE_H_
#define SERVICE_H_

#include <QObject>
#include <bb/multimedia/MediaPlayer>
#include <bb/platform/NotificationDialog>
#include <bb/platform/NotificationResult>

namespace bb {
    class Application;
    namespace platform {
        class Notification;
    }
    namespace system {
        class InvokeManager;
        class InvokeRequest;
        class SystemDialog;
    }
    namespace multimedia {
        class MediaPlayer;
    }
}

class Service: public QObject
{
    Q_OBJECT
public:
    Service();
    virtual ~Service();
private slots:
    void handleInvoke(const bb::system::InvokeRequest &);
    void onTimeout();
    void onDialogFinished(bb::platform::NotificationResult::Type type);

private:
    void triggerNotification();

    bb::platform::Notification * m_notify;
    bb::system::InvokeManager * m_invokeManager;

    bb::multimedia::MediaPlayer *mp;
    bb::platform::NotificationDialog* m_dialog;
};

#endif /* SERVICE_H_ */
