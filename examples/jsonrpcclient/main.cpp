#include <QCoreApplication>
#include <QxtNetwork/QxtJSONRpcClient>
#include <QxtNetwork/QxtJSONRpcCall>
#include <QDebug>
#include <QTextStream>
#include <QStringList>
#include <QxtCore/QxtSignalWaiter>


static const int inc = 2;

void format(QTextStream &s, int indention, QVariant v)
{
    switch (v.type()) {
        case QVariant::Invalid:
            s << "null\n";
            break;
        case QVariant::StringList:
        case QVariant::Hash:
        case QVariant::List:
            {
                QVariantList m = v.toList();
                if (m.isEmpty()) {
                    s << "[ ]\n";
                    return;
                }
                s << "[\n";
                foreach(QVariant i, m) {
                    s << QString(indention + inc, ' ');
                    format(s, indention + inc, i);
                }
                s << QString(indention - inc, ' ') <<  "]\n";
            }
            break;
        case QVariant::Map:
            {
                indention += inc;
                QVariantMap m = v.toMap();
                QMapIterator<QString, QVariant> i(m);
                s << "{\n";
                while (i.hasNext()) {
                    i.next();
                    s << QString(indention, ' ') << i.key() << ": ";
                    format(s, indention + inc, i.value());
                }
                s << QString(indention - inc, ' ') <<  "}\n";
            }
            break;
        case QVariant::Int:
        case QVariant::String:
        default:
            s << v.toString() << "\n";

    }
};


int main(int argc, char ** argv){
    QCoreApplication app(argc,argv);

    if (app.arguments().count() < 3) {
        qDebug()<<"usage: " << qPrintable(app.arguments().at(0)) <<  "url method [arguments] \n";
        return 1;
    }


    QxtJSONRpcClient client;
    client.setServiceUrl(app.arguments().at(1));

    QxtJSONRpcCall * call = client.call(app.arguments()[2],QVariant(app.arguments().mid(3)).toList());
    QxtSignalWaiter w(call,SIGNAL(finished()));
    w.wait();

    QTextStream s(stdout);
    format(s, 0, call->result());
    s << "\n";
}

