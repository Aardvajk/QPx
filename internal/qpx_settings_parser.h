#ifndef QPX_SETTINGS_PARSER_H
#define QPX_SETTINGS_PARSER_H

#include <QString>

namespace QPx
{

class Settings;

}

void qpx_settings_parse(QPx::Settings &root, const QString &source);
QString qpx_settings_write(const QPx::Settings &root);

#endif // QPX_SETTINGS_PARSER_H
