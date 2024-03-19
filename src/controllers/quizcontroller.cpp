#include "quizcontroller.h"
#include "../exceptions/run_out_of_entries.h"

QuizController::QuizController(QObject *parent)
    : QObject{parent}
{
    QuestionResponseEntry entry(0, "", "", false, false);
    m_current_output = entry.getMap();
}

void QuizController::next_output()
{
    emit outputDismissed(m_current_output);
    setisLastOutputMarked(m_current_output["isChecked"].toBool());
    m_lastOutputId = m_current_output["id"].toInt();
    init();
}

void QuizController::mark_output()
{
    if(m_isQtoR) {
        m_dict_controller->checkQuestion(m_current_output["id"].toInt(), m_current_output["id_memory"].toInt());
    } else {
        m_dict_controller->checkResponse(m_current_output["id"].toInt(), m_current_output["id_memory"].toInt());
    }
    m_current_output["id_memory"] = -1; // The hint is not available anymore
    m_current_output["isChecked"] = true;
}

void QuizController::unmark_output()
{
    if(m_isQtoR) {
        m_dict_controller->uncheckQuestion(m_current_output["id"].toInt());
    } else {
        m_dict_controller->uncheckResponse(m_current_output["id"].toInt());
    }

    m_current_output["isChecked"] = false;
    emit current_outputChanged();
}

void QuizController::unmark_last_output()
{
    if(m_isQtoR) {
        m_dict_controller->uncheckQuestion(m_lastOutputId);
    } else {
        m_dict_controller->uncheckResponse(m_lastOutputId);
    }

    setisLastOutputMarked(false);
}

void QuizController::init()
{
    try {
        m_current_output = m_isQtoR ? m_dict_controller->selectRandomQuestion() : m_dict_controller->selectRandomResponse();
    } catch (RunOutOfEntries& e) {
        emit finished(e.what());
        return;
    }

    m_current_output["isChecked"] = m_isQtoR ? m_current_output["isCheckedQuestion"] : m_current_output["isCheckedResponse"];
    emit current_outputChanged();
}

QVariantMap QuizController::current_output() const
{
    return m_current_output;
}

void QuizController::setCurrent_output(const QVariantMap &newCurrent_output)
{
    if (m_current_output == newCurrent_output)
        return;
    m_current_output = newCurrent_output;
    emit current_outputChanged();
}

bool QuizController::isQtoR() const
{
    return m_isQtoR;
}

void QuizController::setisQtoR(bool newIsQtoR)
{
    if (m_isQtoR == newIsQtoR)
        return;
    m_isQtoR = newIsQtoR;
    emit isQtoRChanged();
}

DictController *QuizController::dict_controller() const
{
    return m_dict_controller;
}

void QuizController::setDict_controller(DictController *newDict_controller)
{
    if (m_dict_controller == newDict_controller)
        return;
    m_dict_controller = newDict_controller;
    emit dict_controllerChanged();
}

bool QuizController::isLastOutputMarked() const
{
    return m_isLastOutputMarked;
}

void QuizController::setisLastOutputMarked(bool newIsLastOutputMarked)
{
    if (m_isLastOutputMarked == newIsLastOutputMarked)
        return;
    m_isLastOutputMarked = newIsLastOutputMarked;
    emit isLastOutputMarkedChanged();
}
