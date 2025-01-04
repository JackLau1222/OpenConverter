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

void ProcessParameter::set_Process_Number(int64_t processNumber) {
    if (processNumber > 0)
        emit update_Process_Number(processNumber);
}

double ProcessParameter::get_Process_Number() { return processNumber; }

void ProcessParameter::set_Time_Required(double timeRequired) {
    if (timeRequired > 0) {
        this->timeRequired = timeRequired;
        emit update_Time_Required(this->timeRequired);
    }
}

double ProcessParameter::get_Time_Required() { return timeRequired; }

ProcessParameter get_Process_Parmeter() {
    // TODO
}

ProcessParameter::~ProcessParameter() {}
