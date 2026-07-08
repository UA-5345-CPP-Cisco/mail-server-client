#pragma once

#include <QColor>
#include <QHash>
#include <QString>
#include <QJsonObject>

namespace ISXMail
{
enum class Theme
{
    Light,
    Dark
};
class ColorModel
{
public:
    enum class Role
    {
        Background,
        Surface,
        Button,
        Outline,
        ButtonSpecial,
        ButtonSpecialHover,
        Avatar,
        Hover,
        PrimaryText,
        SecondaryText,
        Highlight,
        Border,
        Transparent,
        Unknown
    };
    ColorModel();

    bool LoadFromFile(const QString& path);
    bool LoadFromJson(const QJsonObject& obj);
    QColor Color(Role role) const;
    QColor Color(const QString& role_name) const;
    void SetColor(Role role, const QColor& color);

    static QString RoleToString(Role r);
    static Role RoleFromString(const QString& name);
    static QColor FromHex(const QString& hex);
private:
    // DATA MEMBERS
    QHash<Role, QColor> m_colors;
};
}