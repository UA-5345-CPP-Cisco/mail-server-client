#include "headers/color/ColorModel.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonValue>

namespace ISXMail {

// CONSTRUCTORS
ColorModel::ColorModel()
{
	m_colors[Role::Background]    = QColor("#FFFFFF");
	m_colors[Role::Surface]       = QColor("#F9FAFB");
	m_colors[Role::Button]        = QColor("#101828");
	m_colors[Role::Outline]       = QColor("#E5E7EB");
	m_colors[Role::Hover]         = QColor("#1f2937");
	m_colors[Role::PrimaryText]   = QColor("#101828");
	m_colors[Role::SecondaryText] = QColor("#6A7282");
	m_colors[Role::Highlight]     = QColor("#F3F4F6");
	m_colors[Role::Border]        = QColor("#E5E7EB");
	m_colors[Role::Transparent]   = QColor(Qt::transparent);
}

// METHODS
bool ColorModel::LoadFromFile(const QString& path)
{
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
	{
		return false;
	}

	const QJsonDocument document = QJsonDocument::fromJson(file.readAll());
	if (!document.isObject())
	{
		return false;
	}

	return LoadFromJson(document.object());
}

bool ColorModel::LoadFromJson(const QJsonObject& obj)
{
	bool loaded = false;

	for (auto it = obj.begin(); it != obj.end(); ++it)
	{
		const Role role = RoleFromString(it.key());

		if (role == Role::Unknown || !it.value().isString())
		{
			continue;
		}

		const QColor color = FromHex(it.value().toString());

		if (!color.isValid())
		{
			continue;
		}

		m_colors[role] = color;
		loaded = true;
	}

	return loaded;
}

QColor ColorModel::Color(Role role) const
{
	return m_colors.value(role, QColor());
}

QColor ColorModel::Color(const QString& role_name) const
{
	return Color(RoleFromString(role_name));
}

void ColorModel::SetColor(Role role, const QColor& color)
{
	m_colors[role] = color;
}

QString ColorModel::RoleToString(Role role)
{
	switch (role)
	{
	case Role::Background:    return "background";
	case Role::Surface:       return "surface";
	case Role::Button:        return "button";
	case Role::Outline:       return "outline";
	case Role::PrimaryText:   return "primaryText";
	case Role::SecondaryText: return "secondaryText";
	case Role::Highlight:     return "highlight";
	case Role::Border:        return "border";
	case Role::Transparent:   return "transparent";
	default:                  return "unknown";
	}
}

ColorModel::Role ColorModel::RoleFromString(const QString& name)
{
	const QString key = name.trimmed().toLower();

	if (key == "background") return Role::Background;
	if (key == "surface") return Role::Surface;
	if (key == "button") return Role::Button;
	if (key == "outline") return Role::Outline;
	if (key == "hover") return Role::Hover;
	if (key == "primarytext" || key == "primary" || key == "text") return Role::PrimaryText;
	if (key == "secondarytext" || key == "secondary") return Role::SecondaryText;
	if (key == "highlight" || key == "hover") return Role::Highlight;
	if (key == "border") return Role::Border;
	if (key == "transparent") return Role::Transparent;

	return Role::Unknown;
}

QColor ColorModel::FromHex(const QString& hex)
{
	return QColor(hex.trimmed());
}

}