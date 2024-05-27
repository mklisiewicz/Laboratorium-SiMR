#include "interface.h"


interface::interface(QWidget *parent) : QWidget(parent) {
    setupUI();
}


void interface::setupUI() {
    // Create the frame for displaying the image
    imageFrame = new QFrame(this);
    imageFrame->setFrameShape(QFrame::StyledPanel);
    imageFrame->setFixedSize(400, 300);  // Set a fixed size for the image frame

    // Create a label to display the image
    imageLabel = new QLabel(imageFrame);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setText("Image will be displayed here");
    imageLabel->setFixedSize(400, 300);

    // Create combo boxes for selecting parameters
    parameterComboBox1 = new QComboBox(this);
    parameterComboBox2 = new QComboBox(this);

    // Add some example items to the combo boxes
    parameterComboBox1->addItems({"1", "2", "3"});
    parameterComboBox2->addItems({"1", "2", "3"});

    // Create a button to run the Python script
    runButton = new QPushButton("Run Python Script", this);
    connect(runButton, &QPushButton::clicked, this, &interface::runPythonScript);

    outputConsole = new QTextEdit(this);
    outputConsole->setReadOnly(true);

    // Layout for the image frame
    QVBoxLayout *frameLayout = new QVBoxLayout(imageFrame);
    frameLayout->addWidget(imageLabel);
    imageFrame->setLayout(frameLayout);

    // Layout for the combo boxes and button
    QHBoxLayout *comboBoxLayout = new QHBoxLayout;
    comboBoxLayout->addWidget(parameterComboBox1);
    comboBoxLayout->addWidget(parameterComboBox2);
    comboBoxLayout->addWidget(runButton);

    // Main layout for the widget
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(imageFrame);
    mainLayout->addLayout(comboBoxLayout);
    mainLayout->addWidget(outputConsole);

    setLayout(mainLayout);

    // Initialize QProcess
    process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, &interface::handleProcessOutput);
    connect(process, &QProcess::readyReadStandardError, this, &interface::handleProcessError);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &interface::displayPlot);

}

void interface::runPythonScript() {
    QString param1 = parameterComboBox1->currentText();
    QString param2 = parameterComboBox2->currentText();
    QStringList arguments;
    arguments << param1 << param2;

    QString scriptPath = QCoreApplication::applicationDirPath() + "/scripts/differential_solver.py";
    process->start("python", QStringList() << scriptPath << arguments);
}

void interface::callPythonFunction(const QString &functionName, const QVariantList &args) {
    Py_Initialize();

    // Convert arguments to Python objects
    PyObject *pArgs = PyTuple_New(args.size());
    for (int i = 0; i < args.size(); ++i) {
        PyTuple_SetItem(pArgs, i, PyUnicode_FromString(args[i].toString().toUtf8().constData()));
    }

    // Import the Python module
    PyObject *pModule = PyImport_ImportModule("differential_solver");
    if (pModule) {
        // Get the Python function
        PyObject *pFunc = PyObject_GetAttrString(pModule, functionName.toUtf8().constData());
        if (pFunc && PyCallable_Check(pFunc)) {
            // Call the Python function with arguments
            PyObject *pResult = PyObject_CallObject(pFunc, pArgs);
            if (pResult) {
                // Function executed successfully
                displayPlot();
                Py_DECREF(pResult);
            } else {
                // Error in function execution
                outputConsole->append("<span style='color:red'>Error: Python function execution failed.</span>");
            }
        } else {
            // Error in getting the function
            outputConsole->append("<span style='color:red'>Error: Python function not found or not callable.</span>");
        }
    } else {
        // Error in importing module
        outputConsole->append("<span style='color:red'>Error: Python module not found.</span>");
    }

    // Release Python objects and finalize Python interpreter
    Py_DECREF(pArgs);
    Py_XDECREF(pModule);
    Py_Finalize();
}

void interface::displayPlot() {
    QPixmap pixmap("plot.png");
    imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio));
}

void interface::handleProcessOutput() {
    QByteArray output = process->readAllStandardOutput();
    outputConsole->append(output);
}

void interface::handleProcessError() {
    QByteArray error = process->readAllStandardError();
    outputConsole->append("<span style='color:red'>" + QString(error) + "</span>");
}

