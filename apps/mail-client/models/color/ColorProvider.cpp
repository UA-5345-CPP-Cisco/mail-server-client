#include "headers/color/ColorProvider.h"
#include "logger/Logger.h"

#include <QFile>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>
namespace ISXMail {

// CONSTRUCTORS
ColorProvider::ColorProvider(QObject* parent)
	: QObject(parent)
{
	if (!LoadThemeFromFile(":/pngs/assets/settings.json"))
	{
		SetTheme(Theme::Light);
	}
}

// METHODS
QColor ColorProvider::Background() const
{
	return m_background;
}

QColor ColorProvider::Surface() const
{
	return m_surface;
}

QColor ColorProvider::Button() const
{
	return m_button;
}
QColor ColorProvider::ButtonSpecialText() const
{
	return m_button_special_text;
}

QColor ColorProvider::Selected() const
{
	return m_selected;
}
QColor ColorProvider::Hover() const
{
	return m_hover;
}

QColor ColorProvider::Outline() const
{
	return m_outline;
}

QColor ColorProvider::PrimaryText() const
{
	return m_primary_text;
}

QColor ColorProvider::SecondaryText() const
{
	return m_secondary_text;
}

QColor ColorProvider::Highlight() const
{
	return m_highlight;
}

QColor ColorProvider::Border() const
{
	return m_border;
}
QColor ColorProvider::Avatar() const
{
	return m_avatar;
}

QColor ColorProvider::ButtonSpecial() const
{
	return m_button_special;
}

QColor ColorProvider::ButtonSpecialHover() const
{
	return m_button_special_hover;
}

QColor ColorProvider::Transparent() const
{
	return m_transparent;
}

bool ColorProvider::LoadScheme(const QString& path)
{
	if (!m_model.LoadFromFile(path))
	{
		Logging::Logger::Instance().Log(Logging::LogLevel::Info, "ColorProvider::LoadScheme: Failed to find path");
		return false;
	}

	updateFromModel();
	emit colorsChanged();

	return true;
}

bool ColorProvider::SetTheme(Theme theme)
{
	QString path;
	switch (theme)
	{
	case Theme::Light:
		path = ":/pngs/assets/LightColorScheme.json";
		break;

	case Theme::Dark:
		path = ":/pngs/assets/DarkColorScheme.json";
		break;
	}
	SaveThemeToFile(":/pngs/assets/settings.json", theme);

	if (!m_model.LoadFromFile(path))
	{
		Logging::Logger::Instance().Log(Logging::LogLevel::Info, "ColorProvider::SetTheme: Failed to load theme");
		return false;
	}

	updateFromModel();
	emit colorsChanged();
	Logging::Logger::Instance().Log(Logging::LogLevel::Info, "ColorProvider::SetTheme: Theme loaded successfully");
	return true;
}

bool ColorProvider::SetTheme(const QString& theme_name)
{
	if (theme_name.trimmed().toLower() == "dark")
	{
		return SetTheme(Theme::Dark);
	}
	else if (theme_name.trimmed().toLower() == "light")
	{
		return SetTheme(Theme::Light);
	}
	return false;
}

void ColorProvider::updateFromModel()
{
	m_background     		= m_model.Color(ColorModel::Role::Background);
	m_surface        		= m_model.Color(ColorModel::Role::Surface);
	m_button         		= m_model.Color(ColorModel::Role::Button);
	m_outline        		= m_model.Color(ColorModel::Role::Outline);
	m_primary_text   		= m_model.Color(ColorModel::Role::PrimaryText);
	m_hover          		= m_model.Color(ColorModel::Role::Hover);
	m_button_special 		= m_model.Color(ColorModel::Role::ButtonSpecial);
	m_button_special_hover	= m_model.Color(ColorModel::Role::ButtonSpecialHover);
	m_avatar				= m_model.Color(ColorModel::Role::Avatar);
	m_button_special_text	= m_model.Color(ColorModel::Role::ButtonSpecialText);
	m_selected				= m_model.Color(ColorModel::Role::Selected);
	m_secondary_text 		= m_model.Color(ColorModel::Role::SecondaryText);
	m_highlight      		= m_model.Color(ColorModel::Role::Highlight);
	m_border         		= m_model.Color(ColorModel::Role::Border);
	m_transparent    		= m_model.Color(ColorModel::Role::Transparent);
}

bool ColorProvider::LoadThemeFromFile(const QString& file_path)
{
	QFile file(file_path);

	if (!file.open(QIODevice::ReadOnly))
		return false;

	const auto document = QJsonDocument::fromJson(file.readAll());

	if (!document.isObject())
		return false;

	const QString theme =
		document.object().value("theme").toString("Light");

	return SetTheme(theme);
}
bool ColorProvider::SaveThemeToFile(const QString& file_path,
										Theme theme)
{
	QFile file(file_path);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return false;

	QJsonObject object;
	object["theme"] =
		theme == Theme::Dark ? "Dark" : "Light";

	file.write(QJsonDocument(object).toJson());

	return true;
}

}