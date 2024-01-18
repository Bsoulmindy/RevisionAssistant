#include "quizcontroller.h"

QuizController::QuizController(QObject *parent)
    : QObject{parent}
{
}

void QuizController::next_output()
{
    emit outputDismissed(m_current_output);
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

void QuizController::init()
{
    m_current_output = m_isQtoR ? m_dict_controller->selectRandomQuestion() : m_dict_controller->selectRandomResponse();
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
