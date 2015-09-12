#include "utils.h"
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <iostream>
#include <fstream>

std::string set_to_string(std::set<int> set_to_convert)
{
    std::string ret;

    int i(0);
    for(int value : set_to_convert)
    {
        if(i++ > 0)
            ret.append(",");
        ret.append(std::to_string(value));
    }

    return ret;
}

std::set<int> string_to_set(std::string string_to_convert)
{
    std::set<int> ret;

    if(!string_to_convert.empty())
    {
        QString qstring_to_convert(QString::fromStdString(string_to_convert));

        QStringList values ( qstring_to_convert.split(',') );

        for(QString value : values)
            if(value.size() > 0)
                ret.insert(value.toInt());
    }

    return ret;
}

bool removeRecursively(const QString & path)
{
    QFileInfo file_info(path);
    if(file_info.exists())
    {
        if (file_info.isDir()) {
            QDir target_dir(path);
            return target_dir.removeRecursively();
        } else { // FILE
            QFile file(path);
            return file.remove();
        }
    }

    return true;
}

bool copyRecursively(const QString & src_file_path, const QString & tgt_file_path)
{
    QFileInfo src_file_info(src_file_path);
    if (src_file_info.isDir()) {
        QDir target_dir(tgt_file_path);

        if (!target_dir.mkpath("."))
            return false;
        QDir src_dir(src_file_path);
        QStringList file_names = src_dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach (const QString &file_name, file_names) {
            const QString new_src_file_path ( src_file_path + QDir::separator() + file_name );
            const QString new_tgt_file_path ( tgt_file_path + QDir::separator() + file_name );
            if (!copyRecursively(new_src_file_path, new_tgt_file_path))
                return false;
        }
    } else {
        if (!QFile::copy(src_file_path, tgt_file_path))
            return false;
    }
    return true;
}

bool file_exists(const std::string & path)
{
    std::ifstream file(path);
    return !file.fail();
}
