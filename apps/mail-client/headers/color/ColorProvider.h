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

    Q_PROPERTY(QColor background READ Background NOTIFY ColorsChanged)
    Q_PROPERTY(QColor surface READ Surface NOTIFY ColorsChanged)
    Q_PROPERTY(QColor button READ Button NOTIFY ColorsChanged)
    Q_PROPERTY(QColor outline READ Outline NOTIFY ColorsChanged)
    Q_PROPERTY(QColor primaryText READ PrimaryText NOTIFY ColorsChanged)
    Q_PROPERTY(QColor secondaryText READ SecondaryText NOTIFY ColorsChanged)
    Q_PROPERTY(QColor highlight READ Highlight NOTIFY ColorsChanged)
    Q_PROPERTY(QColor border READ Border NOTIFY ColorsChanged)
    Q_PROPERTY(QColor transparent READ Transparent NOTIFY ColorsChanged)
public:

    explicit ColorProvider(QObject* parent = nullptr);

    QColor Background() const;
    QColor Surface() const;
    QColor Button() const;
    QColor Outline() const;
    QColor PrimaryText() const;
    QColor SecondaryText() const;
    QColor Highlight() const;
    QColor Border() const;
    QColor Transparent() const;
    Q_INVOKABLE bool LoadScheme(const QString& path);
    Q_INVOKABLE bool SetTheme(Theme theme);
    Q_INVOKABLE bool SetTheme(const QString& theme_name);

    signals:
        void ColorsChanged();
private:

    void UpdateFromModel();

    ColorModel m_model;
    QColor m_background;
    QColor m_surface;
    QColor m_button;
    QColor m_outline;
    QColor m_primary_text;
    QColor m_secondary_text;
    QColor m_highlight;
    QColor m_border;
    QColor m_transparent;
};

} // namespace ISXMail