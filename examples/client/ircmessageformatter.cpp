/*
 * Copyright (C) 2008-2013 The Communi Project
 *
 * This example is free, and not covered by the LGPL license. There is no
 * restriction applied to their modification, redistribution, using and so on.
 * You can study them, modify them, use them in your own program - either
 * completely or partially.
 */

#include "ircmessageformatter.h"

#include <IrcTextFormat>
#include <IrcSession>
#include <IrcSender>
#include <QTime>
#include <Irc>

QString IrcMessageFormatter::formatMessage(IrcMessage* message)
{
    QString formatted;
    switch (message->type()) {
        case IrcMessage::Join:
            formatted = formatJoinMessage(static_cast<IrcJoinMessage*>(message));
            break;
        case IrcMessage::Names:
            formatted = formatNamesMessage(static_cast<IrcNamesMessage*>(message));
            break;
        case IrcMessage::Nick:
            formatted = formatNickMessage(static_cast<IrcNickMessage*>(message));
            break;
        case IrcMessage::Part:
            formatted = formatPartMessage(static_cast<IrcPartMessage*>(message));
            break;
        case IrcMessage::Private:
            formatted = formatPrivateMessage(static_cast<IrcPrivateMessage*>(message));
            break;
        case IrcMessage::Quit:
            formatted = formatQuitMessage(static_cast<IrcQuitMessage*>(message));
            break;
        default:
            break;
    }
    return formatMessage(formatted);
}

QString IrcMessageFormatter::formatMessage(const QString& message)
{
    if (!message.isEmpty()) {
        QString formatted = QObject::tr("[%1] %2").arg(QTime::currentTime().toString(), message);
        if (message.startsWith("!"))
            formatted = QObject::tr("<font color='gray'>%1</font>").arg(formatted);
        else if (message.startsWith("*"))
            formatted = QObject::tr("<font color='maroon'>%1</font>").arg(formatted);
        else if (message.startsWith("["))
            formatted = QObject::tr("<font color='indianred'>%1</font>").arg(formatted);
        return formatted;
    }
    return QString();
}

QString IrcMessageFormatter::formatJoinMessage(IrcJoinMessage* message)
{
    const QString sender = message->sender().name();
    if (message->flags() & IrcMessage::Own)
        return QObject::tr("! You have joined %1 as %2").arg(message->channel(), sender);
    else
        return QObject::tr("! %1 has joined %2").arg(sender, message->channel());
}

QString IrcMessageFormatter::formatNamesMessage(IrcNamesMessage* message)
{
    return QObject::tr("! %1 has %2 users").arg(message->channel()).arg(message->names().count());
}

QString IrcMessageFormatter::formatNickMessage(IrcNickMessage* message)
{
    const QString sender = message->sender().name();
    return QObject::tr("! %1 has changed nick to %2").arg(sender, message->nick());
}

QString IrcMessageFormatter::formatPartMessage(IrcPartMessage* message)
{
    const QString sender = message->sender().name();
    if (message->reason().isEmpty())
        return QObject::tr("! %1 has left %2").arg(sender, message->channel());
    else
        return QObject::tr("! %1 has left %2 (%3)").arg(sender, message->channel(), message->reason());
}

QString IrcMessageFormatter::formatPrivateMessage(IrcPrivateMessage* message)
{
    const QString sender = message->sender().name();
    const QString content = IrcTextFormat().toHtml(message->message());
    if (message->isAction())
        return QObject::tr("* %1 %2").arg(sender, content);
    else
        return QObject::tr("&lt;%1&gt; %2").arg(sender,content);
}

QString IrcMessageFormatter::formatQuitMessage(IrcQuitMessage* message)
{
    const QString sender = message->sender().name();
    if (message->reason().isEmpty())
        return QObject::tr("! %1 has quit").arg(sender);
    else
        return QObject::tr("! %1 has quit (%2)").arg(sender, message->reason());
}
