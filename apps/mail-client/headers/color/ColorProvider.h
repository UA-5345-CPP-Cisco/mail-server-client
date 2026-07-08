#pragma once

#include "headers/color/ColorModel.h"

#include <QObject>
#include <QString>
#include <QColor>

namespace ISXMail
{

class ColorProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QColor background READ Background NOTIFY colorsChanged)
    Q_PROPERTY(QColor surface READ Surface NOTIFY colorsChanged)
    Q_PROPERTY(QColor button READ Button NOTIFY colorsChanged)
    Q_PROPERTY(QColor outline READ Outline NOTIFY colorsChanged)
    Q_PROPERTY(QColor hover READ Hover NOTIFY colorsChanged)
    Q_PROPERTY(QColor primaryText READ PrimaryText NOTIFY colorsChanged)
    Q_PROPERTY(QColor secondaryText READ SecondaryText NOTIFY colorsChanged)
    Q_PROPERTY(QColor highlight READ Highlight NOTIFY colorsChanged)
    Q_PROPERTY(QColor border READ Border NOTIFY colorsChanged)
    Q_PROPERTY(QColor buttonSpecialText READ ButtonSpecialText NOTIFY colorsChanged)
    Q_PROPERTY(QColor selected READ Selected NOTIFY colorsChanged)
    Q_PROPERTY(QColor buttonSpecial READ ButtonSpecial NOTIFY colorsChanged)
    Q_PROPERTY(QColor buttonSpecialHover READ ButtonSpecialHover NOTIFY colorsChanged)
    Q_PROPERTY(QColor avatar READ Avatar NOTIFY colorsChanged)
    Q_PROPERTY(QColor transparent READ Transparent NOTIFY colorsChanged)
public:

    explicit ColorProvider(QObject* parent = nullptr);

    QColor Background() const;
    QColor Surface() const;
    QColor Button() const;
    QColor Outline() const;
    QColor PrimaryText() const;
    QColor SecondaryText() const;
    QColor Hover() const;
    QColor Highlight() const;
    QColor ButtonSpecialText() const;
    QColor Selected() const;
    QColor Border() const;
    QColor ButtonSpecial() const;
    QColor ButtonSpecialHover() const;
    QColor Avatar() const;
    QColor Transparent() const;
    Q_INVOKABLE bool LoadScheme(const QString& path);
    Q_INVOKABLE bool SetTheme(Theme theme);
    Q_INVOKABLE bool SetTheme(const QString& theme_name);

    signals:
        void colorsChanged();
private:

    void updateFromModel();

    ColorModel m_model;
    QColor m_background;
    QColor m_surface;
    QColor m_button;
    QColor m_outline;
    QColor m_hover;
    QColor m_button_special;
    QColor m_button_special_hover;
    QColor m_avatar;
    QColor m_primary_text;
    QColor m_secondary_text;
    QColor m_highlight;
    QColor m_border;
    QColor m_button_special_text;
    QColor m_selected;
    QColor m_transparent;
};

} // namespace ISXMail