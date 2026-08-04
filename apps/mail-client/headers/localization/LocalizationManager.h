#pragma once

#include <QObject>
#include <QString>
#include <QTranslator>

class QQmlEngine;

namespace ISXMail
{

class LocalizationManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString currentLanguage READ CurrentLanguage NOTIFY languageChanged)
    Q_PROPERTY(QString effectiveLocale READ EffectiveLocale NOTIFY languageChanged)

public:
    explicit LocalizationManager(QQmlEngine* engine, QObject* parent = nullptr);

    QString CurrentLanguage() const;
    QString EffectiveLocale() const;

    Q_INVOKABLE bool SetLanguage(const QString& language_code);

signals:
    void languageChanged();

private:
    bool ApplyLanguage(const QString& language_code, bool persist);

    QQmlEngine* m_engine = nullptr;
    QTranslator m_translator;
    QString m_current_language;
    QString m_effective_locale;
};

} // namespace ISXMail
