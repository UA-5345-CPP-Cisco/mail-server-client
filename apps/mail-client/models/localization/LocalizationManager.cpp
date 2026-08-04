#include "headers/localization/LocalizationManager.h"

#include <QCoreApplication>
#include <QLocale>
#include <QQmlEngine>
#include <QSettings>

namespace ISXMail
{
constexpr auto DefaultLanguage = "en";
constexpr auto SettingsKey = "ui/language";

bool IsSupportedLanguage(const QString& language_code)
{
    return language_code == "en"
        || language_code == "uk"
        || language_code == "pl";
}

LocalizationManager::LocalizationManager(QQmlEngine* engine, QObject* parent)
    : QObject(parent), m_engine(engine)
{
    QSettings settings("ISX", "MailClient");
    const QString saved_language = settings.value(SettingsKey, DefaultLanguage).toString();

    if (!ApplyLanguage(saved_language, false))
    {
        ApplyLanguage(DefaultLanguage, false);
    }
}

QString LocalizationManager::CurrentLanguage() const
{
    return m_current_language;
}

QString LocalizationManager::EffectiveLocale() const
{
    return m_effective_locale;
}

bool LocalizationManager::SetLanguage(const QString& language_code)
{
    return ApplyLanguage(language_code, true);
}

bool LocalizationManager::ApplyLanguage(const QString& language_code, bool persist)
{
    const QString normalized_code = language_code.trimmed().toLower();
    if (!IsSupportedLanguage(normalized_code))
    {
        return false;
    }

    const QLocale locale(normalized_code);

    QCoreApplication::removeTranslator(&m_translator);

    const bool has_translation_catalog = locale.language() == QLocale::Ukrainian
        || locale.language() == QLocale::Polish;
    if (has_translation_catalog)
    {
        if (!m_translator.load(locale, "mail_client", "_", ":/i18n"))
        {
            return false;
        }

        QCoreApplication::installTranslator(&m_translator);
    }

    QLocale::setDefault(locale);
    m_current_language = normalized_code;
    m_effective_locale = locale.name();

    if (persist)
    {
        QSettings settings("ISX", "MailClient");
        settings.setValue(SettingsKey, m_current_language);
    }

    if (m_engine)
    {
        m_engine->setUiLanguage(m_effective_locale);
        m_engine->retranslate();
    }

    emit languageChanged();
    return true;
}

}
