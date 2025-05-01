#include "builder/include/open_converter.h"

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    QCommandLineParser parser;
    parser.setApplicationDescription("OpenConverter");
    parser.addHelpOption();

    QCommandLineOption inOpt({"i","in"},  "Input file",  "input");
    QCommandLineOption outOpt({"o","out"}, "Output file", "output");
    QCommandLineOption videoCodecOpt("vcodec", "Video codec", "libx264");
    QCommandLineOption audioCodecOpt("acodec", "Audio codec", "aac");
    QCommandLineOption presetOpt("preset", "Preset name", "preset", "medium");

    parser.addOptions({ inOpt, outOpt, videoCodecOpt, audioCodecOpt, presetOpt });
    parser.process(app);

    if (parser.isSet(inOpt) || parser.isSet(outOpt)) {
        EncodeParameter *encodeParameter = new EncodeParameter;
        ProcessParameter *processParameter = new ProcessParameter;
        Converter *converter = new Converter(processParameter, encodeParameter);
        QString inFile  = parser.value(inOpt);
        QString outFile = parser.value(outOpt);
        encodeParameter->set_Video_Codec_Name(parser.value(videoCodecOpt).toStdString());
        encodeParameter->set_Audio_Codec_Name(parser.value(audioCodecOpt).toStdString());
        // Dispatch to core and exit
        converter->convert_Format(inFile, outFile);
        // TODO: catch the error and handle it
        return 0;
    }

    OpenConverter w;
    w.setWindowTitle("Open Converter");
    w.show();
    return app.exec();
}
