#pragma once

#include <string>
#include <unordered_map>
#include <shared_mutex>
#include <optional>
#include<QObject>
#include <QString>
#include<map>

using TranslatoeMap = std::map <QString, QString>;
#define TR(x) Translator::getInstance().Tr(x)
class Translator : public QObject
{
    Q_OBJECT
public:     
    static Translator &getInstance(); 
    ~Translator() = default;
    QString Tr(const QString& value);
	TranslatoeMap translator_map_;
   
}; 