#include "EGlobalVariable.h"
#include <QApplication>
#include <QtWidgets>

Q_GLOBAL_STATIC(EGlobalVariable, rule)
// �������Ա�ʶ,����ֵ,DD����,HideCode����
//  使用 QMap 初始化并填充数据
QMap<QString, QList<int>> EGlobalVariable::keyMap = {
    {"q", {81, 301, 20}},
    {"w", {87, 302, 26}},
    {"e", {69, 303, 8}},
    {"r", {82, 304, 21}},
    {"t", {84, 305, 23}},
    {"y", {89, 306, 28}},
    {"u", {85, 307, 24}},
    {"i", {73, 308, 12}},
    {"o", {79, 309, 18}},
    {"p", {80, 310, 19}},
    {"a", {65, 401, 4}},
    {"s", {83, 402, 22}},
    {"d", {68, 403, 7}},
    {"f", {70, 404, 9}},
    {"g", {71, 405, 10}},
    {"h", {72, 406, 11}},
    {"j", {74, 407, 13}},
    {"k", {75, 408, 14}},
    {"l", {76, 409, 15}},
    {"z", {90, 501, 29}},
    {"x", {88, 502, 27}},
    {"c", {67, 503, 6}},
    {"v", {86, 504, 25}},
    {"b", {66, 505, 5}},
    {"n", {78, 506, 17}},
    {"m", {77, 507, 16}},
    {"ESC", {27, 100, 41}},
    {"F1", {112, 101, 58}},
    {"F2", {113, 102, 59}},
    {"F3", {114, 103, 60}},
    {"F4", {115, 104, 61}},
    {"F5", {116, 105, 62}},
    {"F6", {117, 106, 63}},
    {"F7", {118, 107, 64}},
    {"F8", {119, 108, 65}},
    {"F9", {120, 109, 66}},
    {"F10", {121, 110, 67}},
    {"F11", {122, 111, 68}},
    {"F12", {123, 112, 69}},
    {"~", {192, 200, 53}},
    {"Tab", {9, 300, 43}},
    {"Capslock", {20, 400, 57}},
    {"L_SHIFT", {160, 500, 225}},
    {"L_CTRL", {162, 600, 224}},
    {"L_WIN", {91, 601, 227}},
    {"L_ALT", {164, 602, 226}},
    {"SPACE", {32, 603, 42}},
    {"R_SHIFT", {161, 511, 229}},
    {"R_CTRL", {163, 607, 228}},
    {"R_Mouse", {93, 606, 231}},
    {"R_ALT", {165, 604, 230}},
    {"Enter", {13, 313, 40}},
    {"[", {219, 311, 47}},
    {"]", {221, 312, 48}},
    {";", {186, 410, 51}},
    {"'", {222, 411, 52}},
    {"comma", {188, 508, 54}},
    {".", {190, 509, 55}},
    {"↙", {191, 510, 56}},
    {"1", {49, 201, 30}},
    {"2", {50, 202, 31}},
    {"3", {51, 203, 32}},
    {"4", {52, 204, 33}},
    {"5", {53, 205, 34}},
    {"6", {54, 206, 35}},
    {"7", {55, 207, 36}},
    {"8", {56, 208, 37}},
    {"9", {57, 209, 38}},
    {"0", {48, 210, 39}},
    {"-", {189, 211, 86}},
    {"=", {187, 212, 46}},
    {"↘", {220, 213, 49}},
    {"Delete", {8, 214, 76}},
    {"Print", {44, 700, 70}},
    {"Scroll", {145, 701, 71}},
    {"Pause", {19, 702, 72}},
    {"Ins", {45, 703, 73}},
    {"Home", {36, 704, 74}},
    {"Up", {33, 705, 75}},
    {"Del", {46, 706, 76}},
    {"End", {35, 707, 77}},
    {"Down", {34, 708, 78}},
    {"↑", {38, 709, 82}},
    {"↓", {40, 711, 81}},
    {"←", {37, 710, 80}},
    {"→", {39, 712, 79}},
    {"Num", {144, 810, 83}},
    {"num/", {111, 811, 84}},
    {"num*", {106, 812, 85}},
    {"num-", {109, 813, 86}},
    {"num7", {103, 807, 95}},
    {"num8", {104, 808, 96}},
    {"num9", {105, 809, 97}},
    {"num+", {107, 814, 87}},
    {"num4", {100, 804, 92}},
    {"num5", {101, 805, 93}},
    {"num6", {102, 806, 94}},
    {"num1", {97, 801, 89}},
    {"num2", {98, 802, 90}},
    {"num3", {99, 803, 91}},
    {"num0", {96, 800, 98}},
    {"num_Del", {110, 816, 99}},
    {"num_Enter", {13, 815, 88}}};
static char *keyArr[103][4]{
    "q", "81", "301", "20",
    "w", "87", "302", "26",
    "e", "69", "303", "8",
    "r", "82", "304", "21",
    "t", "84", "305", "23",
    "y", "89", "306", "28",
    "u", "85", "307", "24",
    "i", "73", "308", "12",
    "o", "79", "309", "18",
    "p", "80", "310", "19",
    "a", "65", "401", "4",
    "s", "83", "402", "22",
    "d", "68", "403", "7",
    "f", "70", "404", "9",
    "g", "71", "405", "10",
    "h", "72", "406", "11",
    "j", "74", "407", "13",
    "k", "75", "408", "14",
    "l", "76", "409", "15",
    "z", "90", "501", "29",
    "x", "88", "502", "27",
    "c", "67", "503", "6",
    "v", "86", "504", "25",
    "b", "66", "505", "5",
    "n", "78", "506", "17",
    "m", "77", "507", "16",
    "ESC", "27", "100", "41",
    "F1", "112", "101", "58",
    "F2", "113", "102", "59",
    "F3", "114", "103", "60",
    "F4", "115", "104", "61",
    "F5", "116", "105", "62",
    "F6", "117", "106", "63",
    "F7", "118", "107", "64",
    "F8", "119", "108", "65",
    "F9", "120", "109", "66",
    "F10", "121", "110", "67",
    "F11", "122", "111", "68",
    "F12", "123", "112", "69",
    "~", "192", "200", "53",
    "Tab", "9", "300", "43",
    "Capslock", "20", "400", "57",
    "L_SHIFT", "160", "500", "225",
    "L_CTRL", "162", "600", "224",
    "L_WIN", "91", "601", "227",
    "L_ALT", "164", "602", "226",
    "SPACE", "32", "603", "42",
    "R_SHIFT", "161", "511", "229",
    "R_CTRL", "163", "607", "228",
    "R_Mouse", "93", "606", "231",
    "R_ALT", "165", "604", "230",
    "Enter", "13", "313", "40",
    "[", "219", "311", "47",
    "]", "221", "312", "48",
    ";", "186", "410", "51",
    "'", "222", "411", "52",
    "comma", "188", "508", "54",
    ".", "190", "509", "55",
    "�L", "191", "510", "56",
    "1", "49", "201", "30",
    "2", "50", "202", "31",
    "3", "51", "203", "32",
    "4", "52", "204", "33",
    "5", "53", "205", "34",
    "6", "54", "206", "35",
    "7", "55", "207", "36",
    "8", "56", "208", "37",
    "9", "57", "209", "38",
    "0", "48", "210", "39",
    "-", "189", "211", "86",
    "=", "187", "212", "46",
    "�K", "220", "213", "49",
    "Delete", "8", "214", "76",
    "Print", "44", "700", "70",
    "Scroll", "145", "701", "71",
    "Pause", "19", "702", "72",
    "Ins", "45", "703", "73",
    "Home", "36", "704", "74",
    "Up", "33", "705", "75",
    "Del", "46", "706", "76",
    "End", "35", "707", "77",
    "Down", "34", "708", "78",
    "��", "38", "709", "82",
    "��", "40", "711", "81",
    "��", "37", "710", "80",
    "��", "39", "712", "79",
    "Num", "144", "810", "83",
    "num/", "111", "811", "84",
    "num*", "106", "812", "85",
    "num-", "109", "813", "86",
    "num7", "103", "807", "95",
    "num8", "104", "808", "96",
    "num9", "105", "809", "97",
    "num+", "107", "814", "87",
    "num4", "100", "804", "92",
    "num5", "101", "805", "93",
    "num6", "102", "806", "94",
    "num1", "97", "801", "89",
    "num2", "98", "802", "90",
    "num3", "99", "803", "91",
    "num0", "96", "800", "98",
    "num_Del", "110", "816", "99",
    "num_Enter", "13", "815", "88"};
EGlobalVariable *EGlobalVariable::instance()
{

    return rule();
}

void EGlobalVariable::test()
{
    // todo
}
/*
 * ����:get_localmachine_ip
 * ����:��ȡ������IP��ַ
 * ����:no
 * ����:QString
 */
QString EGlobalVariable::get_localmachine_ip()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    return ipAddress;
}
QString EGlobalVariable::hookKeyToKeyStr(int val)
{
    WORD arrlength = sizeof(keyArr) / sizeof(keyArr[0]);
    for (size_t i = 0; i < arrlength; i++)
    {
        if (atoi(keyArr[i][1]) == val)
        {
            return (QString(QLatin1String(keyArr[i][0])));
        }
    }
    return "";
}
int EGlobalVariable::KeyToUsbKeyCode(QString keyStr)
{
    WORD arrlength = sizeof(keyArr) / sizeof(keyArr[0]);
    for (size_t i = 0; i < arrlength; i++)
    {
        if (QString(QLatin1String(keyArr[i][0])) == keyStr)
        {
            return atoi(keyArr[i][3]);
        }
    }
    return 0;
}
int EGlobalVariable::KeyToWinKeyCode(QString keyStr)
{
    WORD arrlength = sizeof(keyArr) / sizeof(keyArr[0]);
    for (size_t i = 0; i < arrlength; i++)
    {
        if (QString(QLatin1String(keyArr[i][0])) == keyStr)
        {
            return atoi(keyArr[i][1]);
        }
    }
    return 0;
}
void EGlobalVariable::getScreenWH()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect mm = screen->availableGeometry();
    screen_width = mm.width();
    // screen_height = mm.height();//��ȡ�ĸ߶�ȥ�����·����������߶�(���ﲻ����)
    screen_height = QApplication::desktop()->height();
    ;

    qreal dpiVal = screen->logicalDotsPerInch(); // ��ȡϵͳ����Ŵ���
                                                 // ��ȡϵͳdpi�Ŵ���//��Ļ�ķŴ�ϵ��100% 125% 150% 200%����Ӧ��dpiֵΪ��96 120 144 192.
    if (dpiVal == 96)
    {

        sysDpiVal = 1;
    }
    else if (dpiVal == 120.0)
    {

        sysDpiVal = 1.25;
    }
    else if (dpiVal == 144.0)
    {

        sysDpiVal = 1.5;
    }
    else if (dpiVal == 192.0)
    {

        sysDpiVal = 2.0;
    }
    qDebug() << "w: " << screen_width << " h: " << screen_height;
    qDebug() << "dpiVal: " << sysDpiVal;
}
QUdpSocket *EGlobalVariable::m_receiver = NULL;
DWORD EGlobalVariable::lastTime = 0;
// QQueue<QByteArray>* EGlobalVariable::kmMsgqueue=NULL;
qint8 EGlobalVariable::msgRadioState = 0;