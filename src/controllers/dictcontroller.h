#ifndef DICTCONTROLLER_H
#define DICTCONTROLLER_H

#include <QObject>
#include "qqml.h"
#include "../models/question_response_entry.h"
#include <QString>
#include <unordered_set>
#include "../repositories/dict_repo_interface.h"
#include <memory>
#include "../models/question_response_entries_set.h"
#include "../models/dict_repo_enum.h"

/**
 * @brief The DictController class
 * This controller is the responsible of holding the database in memory,
 * and synchronizing it with the real database
 */
class DictController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int num_rows READ num_rows WRITE set_num_rows NOTIFY num_rowsChanged FINAL)
    Q_PROPERTY(int num_not_checked_questions READ num_not_checked_questions WRITE set_num_not_checked_questions NOTIFY num_not_checked_questionsChanged FINAL)
    Q_PROPERTY(int num_not_checked_responses READ num_not_checked_responses WRITE set_num_not_checked_responses NOTIFY num_not_checked_responsesChanged FINAL)
    Q_PROPERTY(QString dict_file_name READ get_file_name WRITE setdict_file_name NOTIFY dict_file_nameChanged FINAL)
    QML_ELEMENT
public:
    explicit DictController(QObject *parent = nullptr);
    QVariantMap selectRandomQuestion();
    QVariantMap selectRandomResponse();
    Q_INVOKABLE QVariantList getAllRecords(QString search_term);
    // hint_index is important for performance
    // hint_index = -1 means that the question/response is located at the last position of the vector
    Q_INVOKABLE void checkQuestion(int id, int hint_index);
    Q_INVOKABLE void checkResponse(int id, int hint_index);
    Q_INVOKABLE void uncheckQuestion(int id);
    Q_INVOKABLE void uncheckResponse(int id);
    Q_INVOKABLE void checkQuestionInDatabase(int id);
    Q_INVOKABLE void checkResponseInDatabase(int id);
    Q_INVOKABLE void uncheckQuestionInDatabase(int id);
    Q_INVOKABLE void uncheckResponseInDatabase(int id);
    Q_INVOKABLE void resetDict();
    Q_INVOKABLE void init();
    Q_INVOKABLE bool editQuestionResponse(int id, const QString new_question, const QString new_response);
    Q_INVOKABLE bool removeEntry(int id);
    Q_INVOKABLE QString get_file_name_without_extension(QString file_name) const;
    Q_INVOKABLE bool insertNewEntry(const QString question, const QString response, bool isQuestionChecked = false, bool isResponseChecked = false);
    // TODO : add new method
    void overrideDict(const std::vector<QVariantMap>& dict_rows);
    QuestionResponseEntriesSet getCheckedQuestionsAndResponses();

    void change_dict(QString dict_name, DictRepoEnum dict_type);

    int num_rows() const;
    void set_num_rows(int newNum_rows);

    int num_not_checked_questions() const;
    void set_num_not_checked_questions(int newNum_not_checked_questions);

    int num_not_checked_responses() const;
    void set_num_not_checked_responses(int newNum_not_checked_responses);

    QString get_file_name() const;
    void setdict_file_name(const QString &newDict_file_name);

    QByteArray get_dict_content_binary() const;
signals:
    void fileProcessingLineWarning(const QString &output);
    void num_rowsChanged();
    void num_not_checked_questionsChanged();
    void num_not_checked_responsesChanged();
    void error(const QString &message);

    void dict_file_nameChanged();

private:
    std::unique_ptr<DictRepoInterface> m_dict_repo;
    std::vector<QuestionResponseEntry> m_not_checked_qsts;
    std::vector<QuestionResponseEntry> m_not_checked_rsps;

    void initInternalMemory();
    int m_num_rows;
    int m_num_not_checked_questions;
    int m_num_not_checked_responses;
    QString m_dict_file_name;
};

#endif // DICTCONTROLLER_H
