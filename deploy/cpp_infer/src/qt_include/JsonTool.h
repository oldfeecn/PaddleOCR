#pragma once
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <QDir>
#include <QtXml> //Ҳ����include <QDomDocument>
#include <QByteArray>
class JsonTool
{
public:
    JsonTool();
    ~JsonTool();
    static bool wirteJson(QString qwirteJson);
    static bool readJson(QString qwirteJson);
    static QString base64encryption(QString src);//base64����
    static QString base64Uncrypt(QString src); //base64����

    static QString UpperAndLowerencryption(QString src);//��Сд����
    static QString UpperAndLowerUncrypt(QString src); //��Сд����
    static QString toUpperAndLower(QString src);//��Сд�ӽ����㷨

    static QString XORencryption(QString src, QString solt);//�����ܼ���
    static QString toXORUncrypt(QString src, QString solt);//������
    static QString toXOREncryptUncrypt(QString src, const QChar key);//���ӽ����㷨
     //�����ַ���תʵ��
    static QString ReverseStr(QString str);//�ַ���ת
    static QString myRencryption(QString src, QString solt);
    static QString myEncryptUncrypt(QString src, QString solt);
    //дxml
    static bool WriteXml(QString xmlFilePath);
    //дxml
    static bool ReadXml(QString xmlFilePath);

private:

};

JsonTool::JsonTool()
{
}

JsonTool::~JsonTool()
{
}

inline bool JsonTool::wirteJson(QString qwirteJson)
{
    // �Զ�д��ʽ����Ŀ¼�µ�1.json�ļ��������ļ�����������Զ�����

    QFileInfo fileaaa(QDir::currentPath() + "/" + qwirteJson);
    if (fileaaa.exists() == false)
    {
        QFile file(QDir::currentPath() + "/" + qwirteJson);
        if (!file.open(QIODevice::ReadWrite)) {
            qDebug() << "File open error";
            return false;
        }
        else {
            qDebug() << "File open!";
        }
        // ʹ��QJsonArray����ֵ����д���ļ�
        QJsonArray jsonArray;
        QJsonObject jsonObject;
        jsonObject.insert(QString("hostName"), myRencryption(QString("localhost"), QString("oldfee")));
        jsonObject.insert(QString("databaseName"), myRencryption(QString("cfsl"), QString("oldfee")));
        jsonObject.insert(QString("username"), myRencryption(QString("root"), QString("oldfee")));
        jsonObject.insert(QString("password"), myRencryption(QString("CFSLoldfee.cn@345"), QString("oldfee")));
        jsonObject.insert(QString("databaseType"), myRencryption(QString("QMYSQL"), QString("oldfee")));
        jsonObject.insert("qq", QString("3457127"));
        jsonArray.append(jsonObject);
        QJsonDocument jsonDoc;
        jsonDoc.setArray(jsonArray);
        file.write(jsonDoc.toJson());
        file.close();
        return true;
    }
    else {
        return true;
    }
}
inline bool JsonTool::readJson(QString qwirteJson)
{
    QFileInfo fileaaa(QDir::currentPath() + "/" + qwirteJson);
    if (fileaaa.exists() == false)
    {
        wirteJson(qwirteJson);
    }
    else {
        QFile loadFile(qwirteJson);
        if (!loadFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "could't open projects json";
            return false;
        }
        else {
            QByteArray allData = loadFile.readAll();
            loadFile.close();
            QJsonParseError json_error;
            QJsonDocument parse_doucment = QJsonDocument::fromJson(allData, &json_error);
            if (json_error.error == QJsonParseError::NoError) {

                if (parse_doucment.isArray()) {

                    QJsonArray array = parse_doucment.array();
                    int size = array.size();
                    for (int i = 0; i < size; i++) {
                        QJsonValue value = array.at(i);

                        if (value.isString()) {
                            QString name = value.toString();//���ֱ�ʾ���ǽ��ַ�����ʽ��ȫ�����
                            qDebug() << "name:" << name;
                        }
                        else if (value.isObject()) {
                            QJsonObject::const_iterator it = value.toObject().constBegin();
                            QJsonObject::const_iterator end = value.toObject().constEnd();
                            while (it != end)
                            {

                                QString key = it.key();
                                key = myEncryptUncrypt(key, "oldfee");
                                //QString value = myEncryptUncrypt(it.value().toString(),"oldfee");
                                qDebug() << key << "= " << "" << endl;
                                it++;
                            }

                        }

                    }
                }



            }
        }
    }
    return true;
}
inline QString JsonTool::base64encryption(QString str)
{
    QByteArray text = str.toLocal8Bit();
    QByteArray by = text.toBase64();
    return by;
}
inline QString JsonTool::base64Uncrypt(QString src)
{
    QByteArray text = src.toLocal8Bit();
    QByteArray by = text.fromBase64(text);
    QString str = QString::fromLocal8Bit(by); // ע�⣺�����Ǵ��������õ�
    return str;
}
inline QString JsonTool::UpperAndLowerencryption(QString src)
{
    QByteArray text = src.toLocal8Bit();
    QByteArray by = text.toBase64();
    QString str = QString(by);
    str = toUpperAndLower(str); // ��Сд����
    return str;
}
inline QString JsonTool::UpperAndLowerUncrypt(QString src)
{
    QString str = toUpperAndLower(src); // ��Сд����
    QByteArray text = str.toLocal8Bit();
    QByteArray by = text.fromBase64(text);
    str = QString::fromLocal8Bit(by); // ע�⣺�����Ǵ��������õ�
    return str;
}
inline QString JsonTool::toUpperAndLower(QString src)
{
    for (int i = 0; i < src.count(); i++) {
        QChar curC = src.at(i);
        if (curC.isUpper()) {
            curC = curC.toLower();
        }
        else if (curC.isLower()) {
            curC = curC.toUpper();
        }
        src[i] = curC;
    }
    return src;
}

inline QString JsonTool::XORencryption(QString src, QString solt)
{
    QByteArray text = src.toLocal8Bit();
    QByteArray by = text.toBase64();
    QString str = QString(by);
    // ������(Կ�׼���) --- ע�⣺�������key�ǿ���ָ��Ϊ�����ַ��ģ���Ӧ�ģ�����ҲҪͬ���ַ����У�
    foreach(QChar qc, solt) {
        str = toXOREncryptUncrypt(str, qc);
    }
    return str;
}
inline QString JsonTool::toXORUncrypt(QString src, QString solt)
{
    QString str = src;
    foreach(QChar qc, solt) {
        str = toXOREncryptUncrypt(str, qc);
    }
    QByteArray text = str.toLocal8Bit();
    QByteArray by = text.fromBase64(text);
    str = QString::fromLocal8Bit(by); // ע�⣺�����Ǵ��������õ�
    return str;
}
// �ӽ��ܶ��ô˷���
inline QString JsonTool::toXOREncryptUncrypt(QString src, const QChar key)
{
    for (int i = 0; i < src.count(); i++) {
        src[i] = src.at(i).toLatin1() ^ key.toLatin1();
    }
    return src;
}

inline QString JsonTool::ReverseStr(QString str)
{
    int i = 0, j = str.size() - 1;
    QChar c;
    while (i < j)
    {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
        ++i;
        --j;
    }
    return str;
}

inline QString JsonTool::myRencryption(QString src, QString solt)
{
    QString str = XORencryption(src, solt);
    str = UpperAndLowerencryption(str);
    str = base64encryption(str);
    return str;
}

inline QString JsonTool::myEncryptUncrypt(QString src, QString solt)
{
    QString str = base64Uncrypt(src);
    str = UpperAndLowerUncrypt(str);
    //solt = ReverseStr(solt);
    str = toXORUncrypt(str, solt);
    return str;
}

inline bool JsonTool::WriteXml(QString xmlFilePath)
{
    //�򿪻򴴽��ļ�
    QFile file(QDir::currentPath() + "/" + xmlFilePath); //���·��������·������Դ·��������
    if (!file.open(QFile::WriteOnly | QFile::Truncate)) //������QIODevice��Truncate��ʾ���ԭ��������
        return false;
    QDomDocument doc;
    //д��xmlͷ��
    QDomProcessingInstruction instruction; //���Ӵ�������
    instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    //���Ӹ��ڵ�
    QDomElement root = doc.createElement("library");
    doc.appendChild(root);


    QDomElement qq = doc.createElement("qq");
    QDomText text; //�������ű�ǩ�м��ֵ
    text = doc.createTextNode("3457127");
    qq.appendChild(text);
    doc.appendChild(qq);

    QDomElement pwd = doc.createElement("pwd");

    text = doc.createTextNode("pwd");
    pwd.appendChild(text);
    doc.appendChild(pwd);

    //������ļ�
    QTextStream out_stream(&file);
    doc.save(out_stream, 4); //����4��
    file.close();
    return false;
}

inline bool JsonTool::ReadXml(QString xmlFilePath)
{
    //�򿪻򴴽��ļ�
    QFile file(QDir::currentPath() + "/" + xmlFilePath); //���·��������·������Դ·������
    if (!file.open(QFile::ReadOnly))
    {
        WriteXml(xmlFilePath);
    }
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement(); //���ظ��ڵ�
    qDebug() << root.nodeName();
    QDomNode node = root.firstChild(); //��õ�һ���ӽڵ�
    while (!node.isNull())  //����ڵ㲻��
    {
        if (node.isElement()) //����ڵ���Ԫ��
        {
            QDomElement e = node.toElement(); //ת��ΪԪ�أ�ע��Ԫ�غͽڵ����������ݽṹ����ʵ���
            qDebug() << e.tagName() << " " << e.attribute("id") << " " << e.attribute("time"); //��ӡ��ֵ�ԣ�tagName��nodeName��һ������
            QDomNodeList list = e.childNodes();
            for (int i = 0; i < list.count(); i++) //������Ԫ�أ�count��size��������,�����ڱ�ǩ������
            {
                QDomNode n = list.at(i);
                if (node.isElement())
                    qDebug() << n.nodeName() << ":" << n.toElement().text();
            }
        }
        node = node.nextSibling(); //��һ���ֵܽڵ�,nextSiblingElement()����һ���ֵ�Ԫ�أ������
    }

    return true;
}

