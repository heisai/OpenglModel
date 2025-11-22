#pragma once

#include <string>
#include <unordered_map>
#include <shared_mutex>
#include <optional>
#include<QObject>
#include<map>
using TranslatoeMap = std::map <QString, QString>;
class Translator : public QObject
{
    Q_OBJECT
public:     
    Translator(); 
    ~Translator() = default;
    void addTranslator(QString key);
private:
    mutable std::shared_mutex m_mutex;
    TranslatoeMap translator_map_;
}; 