#include <QTemporaryFile>
#include <gtest/gtest.h>

#include "users/AccountListModel.h"
#include "users/CurrentUser.h"

class CurrentUserTest : public ::testing::Test
{
  protected:
  void SetUp() override
  {
    auto& user = ISXCurrentUser::CurrentUser::GetInstance();
    user.Authorize("TestUser", "random@gmail.com", "");
  }

  void TearDown() override
  {
    auto& user = ISXCurrentUser::CurrentUser::GetInstance();
    user.Logout();
  }
};

TEST(NonClassTests, TestDefaultAuthorize)
{
  auto& user = ISXCurrentUser::CurrentUser::GetInstance();
  user.Logout();
  auto& user_after_logout = ISXCurrentUser::CurrentUser::GetInstance();
  EXPECT_FALSE(user_after_logout.is_authorized());
  EXPECT_EQ(user_after_logout.username(), "");
  EXPECT_EQ(user_after_logout.email(), "");
  EXPECT_EQ(user_after_logout.avatar_path(), "");
  user_after_logout.Logout();
}

TEST(NonClassTests, TestLogout)
{
  auto& user = ISXCurrentUser::CurrentUser::GetInstance();
  user.Authorize("bob", "bob_top@gmail.com", "");
  user.Logout();
  ASSERT_FALSE(user.is_authorized());
  EXPECT_EQ(user.username(), "");
  EXPECT_EQ(user.email(), "");
}

TEST_F(CurrentUserTest, InvalidPathAvatar)
{
  auto& user = ISXCurrentUser::CurrentUser::GetInstance();
  user.UpdateAvatarPath("this//path//is//not//exist");
  EXPECT_EQ(user.avatar_path(), "");
}

TEST_F(CurrentUserTest, NormalAvatarPath)
{
  auto& user = ISXCurrentUser::CurrentUser::GetInstance();
  QTemporaryFile temp_file;
  ASSERT_TRUE(temp_file.open());
  QString avatar_path = temp_file.fileName();
  user.UpdateAvatarPath(avatar_path);
  EXPECT_TRUE(user.avatar_path().endsWith("current_user_avatar.png"));
}

TEST_F(CurrentUserTest, StripsFilePrefixFromAvatarPath)
{
  auto& user = ISXCurrentUser::CurrentUser::GetInstance();
  QTemporaryFile temp_file;
  ASSERT_TRUE(temp_file.open());
  QString path_with_prefix = "file:///" + temp_file.fileName();
  user.UpdateAvatarPath(path_with_prefix);
  EXPECT_TRUE(user.avatar_path().endsWith("current_user_avatar.png"));
}
