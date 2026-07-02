#include "headers/users/CurrentUser.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

namespace ISXCurrentUser
{

CurrentUser::CurrentUser(QObject* parent) :
	QObject(parent),
    m_is_authorized(false),
    m_username(""),
    m_email(""),
    m_avatar_path("")
{
}

CurrentUser& CurrentUser::GetInstance()
{
	static CurrentUser instance;
	return instance;
}

bool CurrentUser::is_authorized() const
{
	return m_is_authorized;
}

QString CurrentUser::username() const
{
	return m_username;
}

QString CurrentUser::email() const
{
	return m_email;
}

QString CurrentUser::avatar_path() const
{
	return m_avatar_path;
}

void CurrentUser::Authorize(
	const QString& username,
	const QString& email,
	const QString& avatar_path
)
{
	if (m_username == username && m_email == email && m_avatar_path == avatar_path && m_is_authorized)
	{
		return;
	}

	m_username = username;
	m_email = email;
	m_is_authorized = true;

	if (!avatar_path.isEmpty())
	{
		UpdateAvatarPath(avatar_path);
	}
	else
	{
        m_avatar_path = "";
		emit profileChanged();
	}

	emit authorizationChanged();
}

void CurrentUser::Logout()
{
	if (!m_is_authorized)
	{
		return;
	}

	m_username.clear();
	m_email.clear();
    m_avatar_path.clear();
	m_is_authorized = false;

	emit profileChanged();
	emit authorizationChanged();
}

void CurrentUser::UpdateAvatarPath(const QString& new_path)
{
    QString clean_path = new_path;

    if (clean_path.startsWith("file:///"))
    {
        clean_path = clean_path.mid(8);
    }
#ifdef Q_OS_WIN
    else if (clean_path.startsWith("file://"))
    {
        clean_path = clean_path.mid(7);
    }
#endif

    if (clean_path.isEmpty() || !QFile::exists(clean_path))
    {
        return;
    }

    const QString app_data_dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir;

    if (!dir.exists(app_data_dir))
    {
        dir.mkpath(app_data_dir);
    }

    const QString target_avatar_path = app_data_dir + "/current_user_avatar.png";

    if (QFile::exists(target_avatar_path))
    {
        QFile::remove(target_avatar_path);
    }

    if (QFile::copy(clean_path, target_avatar_path))
    {
        m_avatar_path = target_avatar_path;
        SaveAvatarToSqlite(target_avatar_path);
    }
    else
    {
        m_avatar_path = clean_path;
    }

    emit profileChanged();
}

void CurrentUser::SaveAvatarToSqlite(const QString& local_path)
{
    Q_UNUSED(local_path)
}

void CurrentUser::SaveAvatarFromSqlite(const QString& local_path)
{
}

}
