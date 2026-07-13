#pragma once

#include <QSortFilterProxyModel>

#include "EmailListModel.h"

namespace ISXMail {

class EmailPageProxy : public QSortFilterProxyModel
{
  Q_OBJECT
  Q_PROPERTY(int currentPage READ CurrentPage NOTIFY currentPageChanged)
  Q_PROPERTY(int pageCount READ PageCount NOTIFY pageCountChanged)
  Q_PROPERTY(int totalEmailsCount READ TotalEmailsCount NOTIFY totalEmailsCountChanged)
  Q_PROPERTY(QString pageAmountText READ PageAmountText NOTIFY pageAmountTextChanged)
  public:
  explicit EmailPageProxy(QObject* parent = nullptr);

  Q_INVOKABLE void NextPage();
  Q_INVOKABLE void PrevPage();

  Q_INVOKABLE bool SetEmailData(int proxy_row, const QVariant& value, int role);
  Q_INVOKABLE bool SetStarred(int proxy_row, bool starred);

  Q_INVOKABLE void RemoveEmailData(int proxy_row);

  int CurrentPage() const;
  int PageCount() const;
  int TotalEmailsCount() const;
  QString PageAmountText() const;

  void setSourceModel(QAbstractItemModel* source_model) override;
  signals:
  void currentPageChanged();
  void pageCountChanged();
  void totalEmailsCountChanged();
  void pageAmountTextChanged();

  protected:
  bool filterAcceptsRow(int source_row, const QModelIndex&) const override;

  private:
  void RecalcValues();

  int m_current_page = 0;
  int m_page_count = 0;
  int m_emails_count = 0;
  static constexpr int s_per_page = 8;
};
}