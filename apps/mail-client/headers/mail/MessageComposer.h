#pragma once

#include <QObject>
#include <QString>


namespace ISXMail {

    class MessageComposer : public QObject
    {
        Q_OBJECT

    public:
        explicit MessageComposer(QObject* parent = nullptr);

        Q_INVOKABLE bool SendMailMessage(const QString& sender_name,
                                         const QString& sender_email,
                                         const QString& recipient_email,
                                         const QString& subject,
                                         const QString& body,
                                         bool is_inbox = false);
        Q_INVOKABLE bool SaveDraft(const QString& sender_name,
                                   const QString& sender_email,
                                   const QString& recipient_email,
                                   const QString& subject,
                                   const QString& body);


    };

} // namespace ISXMail
