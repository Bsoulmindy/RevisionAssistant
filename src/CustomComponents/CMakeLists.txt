qt_add_library(custom_components STATIC)
qt_add_qml_module(custom_components
    URI "CustomComponents"
    VERSION 1.0
    QML_FILES
        PrimaryText.qml
        CompleteButton.qml
        QrPageListViewDelegate.qml
        DictEditListViewDelegate.qml
        FileSelectionStatus.qml
	InfoDialog.qml
	QML_FILES FileOptionListDelegate.qml
)
