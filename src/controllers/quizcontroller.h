#ifndef QUIZCONTROLLER_H
#define QUIZCONTROLLER_H

#include <QObject>
#include "qqml.h"
#include "dictcontroller.h"

class QuizController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(DictController* dict_controller READ dict_controller WRITE setDict_controller NOTIFY dict_controllerChanged)
    Q_PROPERTY(QVariantMap current_output READ current_output WRITE setCurrent_output NOTIFY current_outputChanged FINAL)
    Q_PROPERTY(bool isQtoR READ isQtoR WRITE setisQtoR NOTIFY isQtoRChanged FINAL)
    Q_PROPERTY(bool isLastOutputMarked READ isLastOutputMarked WRITE setisLastOutputMarked NOTIFY isLastOutputMarkedChanged FINAL)
    QML_ELEMENT
public:
    explicit QuizController(QObject *parent = nullptr);

    Q_INVOKABLE void next_output();
    Q_INVOKABLE void mark_output();
    Q_INVOKABLE void unmark_output();
    Q_INVOKABLE void unmark_last_output();
    Q_INVOKABLE void init();

    QVariantMap current_output() const;
    void setCurrent_output(const QVariantMap &newCurrent_output);

    bool isQtoR() const;
    void setisQtoR(bool newIsQtoR);

    DictController *dict_controller() const;
    void setDict_controller(DictController *newDict_controller);

    bool isLastOutputMarked() const;
    void setisLastOutputMarked(bool newIsLastOutputMarked);

signals:
    void current_outputChanged();
    void outputDismissed(QVariantMap output);
    void isQtoRChanged();
    void dict_controllerChanged();
    void finished(QString message);

    void isLastOutputMarkedChanged();

private:
    QVariantMap m_current_output;
    bool m_isQtoR;
    DictController *m_dict_controller = nullptr;
    bool m_isLastOutputMarked = false;
    int m_lastOutputId = 0;
};

#endif // QUIZCONTROLLER_H
