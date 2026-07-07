#include "headers/color/ColorProvider.h"

namespace ISXMail {

// CONSTRUCTORS
ColorProvider::ColorProvider(QObject* parent)
	: QObject(parent)
{
	SetTheme(Theme::Light);
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

QColor ColorProvider::Transparent() const
{
	return m_transparent;
}

bool ColorProvider::LoadScheme(const QString& path)
{
	if (!m_model.LoadFromFile(path))
	{
		return false;
	}

	UpdateFromModel();
	emit colorsChanged();

	return true;
}

bool ColorProvider::SetTheme(Theme theme)
{
	QString path;

	switch (theme)
	{
	case Theme::Light:
		path = ":/assets/LightColorScheme.json";
		break;

	case Theme::Dark:
		path = ":/assets/DarkColorScheme.json";
		break;
	}

	if (!m_model.LoadFromFile(path))
	{
		return false;
	}

	UpdateFromModel();
	emit colorsChanged();

	return true;
}

bool ColorProvider::SetTheme(const QString& theme_name)
{
	if (theme_name.trimmed().toLower() == "dark")
	{
		return SetTheme(Theme::Dark);
	}
	return SetTheme(Theme::Light);
}

void ColorProvider::UpdateFromModel()
{
	m_background     = m_model.Color(ColorModel::Role::Background);
	m_surface        = m_model.Color(ColorModel::Role::Surface);
	m_button         = m_model.Color(ColorModel::Role::Button);
	m_outline        = m_model.Color(ColorModel::Role::Outline);
	m_primary_text   = m_model.Color(ColorModel::Role::PrimaryText);
	m_secondary_text = m_model.Color(ColorModel::Role::SecondaryText);
	m_highlight      = m_model.Color(ColorModel::Role::Highlight);
	m_border         = m_model.Color(ColorModel::Role::Border);
	m_transparent    = m_model.Color(ColorModel::Role::Transparent);
}

}