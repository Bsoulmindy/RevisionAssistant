# Revision Assistant

Revision Assistant is an application that gives some random questions (or responses) from a dictionary configured by the user, and the user should answer with the corresponding answer (or question respectively). It also tracks the current progression and gives to the user the possibility to mark a question/response as `completed` so it don't get selected randomly.

Currently, the application is still in Beta and it is cross-platform :

-   Windows 10
-   Linux (Ubuntu 20.04)
-   Android 8
-   [Web](https://revision-assistant.vercel.app) (modern browser with WASM support required)

## Dictionaries

Revision Assistant uses the concept of `Dictionaries` to differentiate from various data sources.

Each `Dict` holds a set of questions with their answers, with their progression (completed or not).

All `Dict`s are independent each other. Users can have multiple dictionnaries at the same time, and add new data set easily without altering other dictionnaries.

## One-To-One Association

Each entry of a `Dict` consists of a single question with a single response. This is a One-To-One Association, which means each question has exactly one response, and that response has exactly that question.

Many-To-Many Association is not currently supported. However, based on your criteria, you can transform it into a One-To-One Association by combining all the 'many' elements from each side. For example, `question1 / question2 | response1 / response2`. Here, the application will display `question1 / question2` entirely, and the user is expected to answer with `response1 / response2`.

## Modes

Currently, the application has 2 modes:

-   `Q to R` Question to Response : The application will select a random question and the user should guess the response.
-   `R to Q` Response to Question : The application will select a random response and the user should guess the question.

## Progression

Initially, all entries (question/response) are marked as `not completed`. When you complete an entry, you can mark it as `completed` so the entry will not be selected randomly. You can undo that at any time.

Since the application has 2 modes (Q to R) or (R to Q), the progression is tracked both ways.

## Quiz Page Interface

The following interface represents an example of a dictionary for learning German, the entries are words from English and German that have similar meaning.

![Quiz page interface](./docs/quizPage.png)

-   1 : Current progress
-   2 : History of previous questions and their responses
-   3 : Current question to answer
-   4 : If you marked a question as `completed` but then you realized that your answer wasn't correct, you can undo this action by clicking on this button
-   5 : If the user knows 99% the answer of the current question, he can marks it as `completed`, next question will be issued automatically.
-   6 : Skip to the next question without marking the current question as `completed`, the question and the response will be shown in the history field.

## New Dictionary Example

Example : German Dict that associate words from German to the convenient words in English with similar meaning.

To accomplish that, we create a `.txt` file, and fill each line with an entry, the line should be divided by a separator (of your choice). The separator will help the application to distinguish which is the question and which is the response.

![Dict  creation example](./docs/dictCreationExample1.png)

In my example, I chose `|` as the separator. Each line must have exactly 1 separator. The lest side of the separator is considered the `question`, and the right side is the `response`.

After creating the text file, you can import it in the application by specifying both the location of the text file and the separator chosen.

# Upcoming Features

-   `CRUD Dict data directly from the application.` Currently, users can only modify progression, but not the data directly in the application.

# Credits

-   [Qt Framework](https://www.qt.io/) for providing an easy way to code GUI applications on multiple platforms
-   [flaticon.com](https://www.flaticon.com/) for logo icons
