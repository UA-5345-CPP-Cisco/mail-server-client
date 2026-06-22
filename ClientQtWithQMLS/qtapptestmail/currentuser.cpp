#include "currentuser.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>

namespace ISXCurrentUser
{
CurrentUser::CurrentUser(QObject* parent)
    : QObject(parent)
    , m_is_authorized(true)
    , m_username("John Doe")
    , m_email("johndoe@gmail.com")
    , m_avatar_path("")
{
}

// METHODS
CurrentUser& CurrentUser::get_instance()
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

void CurrentUser::authorize(const QString& username, const QString& email, const QString& avatar_path)
{
    if (m_username == username && m_email == email && m_avatar_path == avatar_path && m_is_authorized)
    {
        return;
    }

    m_username = username;
    m_email = email;
    m_is_authorized = true;

    // Якщо при авторизації передано новий шлях до аватара — копіюємо його в систему
    if (!avatar_path.isEmpty())
    {
        update_avatar_path(avatar_path);
    }
    else
    {
        m_avatar_path = "";
        emit profileChanged();
    }

    emit authorizationChanged();
}

void CurrentUser::logout()
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

void CurrentUser::update_avatar_path(const QString& new_path)
{
    QString clean_path = new_path;

    // Очищаємо префікси URL, які QML FileDialog додає до локальних шляхів
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

    // 1. Знаходимо стабільну системну папку додатка (працює на Windows, Linux, macOS, Android, iOS)
    QString app_data_dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir dir;
    if (!dir.exists(app_data_dir))
    {
        dir.mkpath(app_data_dir);
    }

    // 2. Формуємо постійний шлях до файлу всередині пісочниці додатка
    QString target_avatar_path = app_data_dir + "/current_user_avatar.png";

    // Якщо стара аватарка існує — видаляємо її перед копіюванням
    if (QFile::exists(target_avatar_path))
    {
        QFile::remove(target_avatar_path);
    }

    // 3. Копіюємо обране зображення
    if (QFile::copy(clean_path, target_avatar_path))
    {
        m_avatar_path = target_avatar_path;
        save_avatar_to_sqlite(target_avatar_path);
    }
    else
    {
        m_avatar_path = clean_path;
    }

    emit profileChanged();
}

void CurrentUser::save_avatar_to_sqlite(const QString& local_path)
{
    // TODO: Додати виконання SQL-запиту:
    // UPDATE users SET avatar_path = :local_path WHERE email = :m_email;
    Q_UNUSED(local_path)
}

void CurrentUser::load_avatar_from_sqlite()
{
    // TODO: Додати виконання SQL-запиту для відновлення шляху після перезапуску додатка:
    // SELECT avatar_path FROM users WHERE email = :m_email;
    // Після зчитування:
    // m_avatar_path = db_result;
    // emit profileChanged();
}

}