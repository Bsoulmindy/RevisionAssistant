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
    QML_ELEMENT
public:
    explicit DictController(QObject *parent = nullptr);
    QVariantMap selectRandomQuestion();
    QVariantMap selectRandomResponse();
    Q_INVOKABLE QVariantList getAllRecords();
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
    // TODO : add new method
    void overrideDict(const std::vector<QVariantMap>& dict_rows);
    QuestionResponseEntriesSet getCheckedQuestionsAndResponses();

    int num_rows() const;
    void set_num_rows(int newNum_rows);

    int num_not_checked_questions() const;
    void set_num_not_checked_questions(int newNum_not_checked_questions);

    int num_not_checked_responses() const;
    void set_num_not_checked_responses(int newNum_not_checked_responses);

signals:
    void fileProcessingLineWarning(const QString &output);
    void num_rowsChanged();
    void num_not_checked_questionsChanged();
    void num_not_checked_responsesChanged();
    void error(const QString &message);

private:
    std::unique_ptr<DictRepoInterface> m_dict_repo;
    std::vector<QuestionResponseEntry> m_not_checked_qsts;
    std::vector<QuestionResponseEntry> m_not_checked_rsps;

    void initInternalMemory();
    int m_num_rows;
    int m_num_not_checked_questions;
    int m_num_not_checked_responses;
};

#endif // DICTCONTROLLER_H
