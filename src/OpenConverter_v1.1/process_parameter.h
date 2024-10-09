#ifndef PROCESSPARAMETER_H
#define PROCESSPARAMETER_H

#include <QObject>

/* inherit the QObject to use slot and signals */
class ProcessParameter : public QObject
{
private:
    Q_OBJECT

public:
    explicit ProcessParameter(QObject *parent = nullptr);
    ~ProcessParameter();

    void set_Process_Number(int64_t frameNumber, int64_t frameTotalNumnber);

    double get_Process_Number();

    void set_Time_Required();

    int get_Time_Required();

    ProcessParameter get_Process_Parmeter();

signals:
    void update_Process_Number(double result);


private:
    double processNumber;

    int timeRequired;

};

#endif // PROCESSPARAMETER_H
