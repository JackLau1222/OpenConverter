#include "process_parameter.h"
// #include "moc_process_parameter.h"

ProcessParameter::ProcessParameter(QObject *parent) : QObject(parent) {
    processNumber = 0;

    timeRequired = 0;
}

void ProcessParameter::set_Process_Number(int64_t frameNumber,
                                          int64_t frameTotalNumnber) {
    double result = (double)frameNumber / (double)frameTotalNumnber;
    if (processNumber != result) {
        processNumber = result;
        emit update_Process_Number(result);
    }
}

double ProcessParameter::get_Process_Number() { return processNumber; }

void ProcessParameter::set_Time_Required() {
    // TODO
}

int ProcessParameter::get_Time_Required() {
    // TODO
}

ProcessParameter get_Process_Parmeter() {
    // TODO
}

ProcessParameter::~ProcessParameter() {}
