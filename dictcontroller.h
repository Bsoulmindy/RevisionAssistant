#ifndef DICTCONTROLLER_H
#define DICTCONTROLLER_H

#include <QObject>
#include "qqml.h"
#include <QSqlDatabase>
#include <QString>
#include <unordered_set>

/**
 * @brief The DictController class
 * This controller is the responsible of holding the database in memory,
 * and synchronizing it with the real database
 */
class DictController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double progressionQst READ progressionQst WRITE setProgressionQst NOTIFY progressionQstChanged FINAL)
    Q_PROPERTY(double progressionRsp READ progressionRsp WRITE setprogressionRsp NOTIFY progressionRspChanged FINAL)
    QML_ELEMENT
public:
    explicit DictController(QObject *parent = nullptr);
    Q_INVOKABLE QVariantMap selectRandomQuestion();
    Q_INVOKABLE QVariantMap selectRandomResponse();
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
    std::pair<std::unordered_set<QString>, std::unordered_set<QString>> getCheckedQuestionsAndResponses();

    double progressionDict() const;
    void setprogressionDict(double newProgressionDict);

    double progressionQst() const;
    void setProgressionQst(double newProgressionQst);

    double progressionRsp() const;
    void setprogressionRsp(double newProgressionRsp);

signals:
    void progressionQstChanged();
    void progressionRspChanged();
    void fileProcessingLineWarning(const QString &output);

private:
    QSqlDatabase m_database;
    int m_num_rows = 0;
    std::vector<QVariantMap> m_not_checked_qsts;
    std::vector<QVariantMap> m_not_checked_rsps;
    double m_progressionQst = 0;
    double m_progressionRsp = 0;

    void initInternalMemory();
};

#endif // DICTCONTROLLER_H
