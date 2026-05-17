#pragma once

#include "JException.h"
#include "Utils.h"
#include <concepts>

#include "common_backend.h"

// #include <array>
// #include <any>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariant>
#include <QMap>
#include <QRegularExpression>

template<typename T>
concept IsQVariant = std::is_base_of_v<QVariant, T>;

namespace jsonata
{
    template<>
    struct json_bridge_impl<QVariant,void>
    {
            using is_json_bridge_type = void; // Tag to satisfy the concept

            static QVariant create(auto &&value)
            {
                using V = std::decay_t<decltype(value)>;

                if constexpr (std::is_same_v<V, std::nullptr_t>) {
                    return QVariant();
                } else if constexpr (std::is_same_v<V, std::string> || std::is_same_v<V, const char *>) {
                    // Handle both std::string and literal "strings"
                    if constexpr (std::is_same_v<V, const char *>)
                        return QVariant(QString(value));
                    else
                        return QVariant(QString::fromStdString(value));
                } else if constexpr (std::is_integral_v<V> && !std::is_same_v<V, bool>) {
                    // Handle all integers (int, long, long long, size_t, etc.)
                    // qlonglong is the safest sink for integral types in Qt
                    return QVariant(static_cast<qlonglong>(value));
                }
                // Handle floating point
                else if constexpr (std::is_floating_point_v<V>) {
                    return QVariant(static_cast<double>(value));
                } else if constexpr (std::is_constructible_v<QVariant, V>) {
                    return value;
                } else {
                    static_assert(false, "Unsupported type for QVariant creation");
                }
            }

            static QVariant create() { return QVariant(); }

            static size_t size(const QVariant &value)
            {
                return isArray(value) ? value.toList().size() : 0;
            }

            static bool isEmpty(const QVariant &value) {
                return value.isNull() ||
                       ( isObject( value ) && value.toMap().empty()) ||
                       ( isArray( value ) && value.toList().empty());
            }

            static bool isObject(const QVariant &value) { return isOfType( value, QMetaType::QVariantMap ); }

            static bool isArray(const QVariant &value) { return isOfType( value, QMetaType::QVariantList ); }

            static bool isNumber(const QVariant &value) {
                switch (value.typeId()) {
                    case QMetaType::Int:
                    case QMetaType::Long:
                    case QMetaType::LongLong:
                    case QMetaType::ULong:
                    case QMetaType::UInt:
                    case QMetaType::ULongLong:
                    case QMetaType::Double:
                    case QMetaType::Float:
                        return true;
                    default :
                        return false;
                }
            }
            static bool isInteger(const QVariant &value)
            {
                switch (value.typeId()) {
                    case QMetaType::Int:
                    case QMetaType::Long:
                    case QMetaType::LongLong:
                    case QMetaType::ULong:
                    case QMetaType::UInt:
                    case QMetaType::ULongLong:
                        return true;
                    default :
                        return false;
                }
            }

            static bool isFloat(const QVariant &value)
            {
                switch (value.typeId()) {
                    case QMetaType::Double:
                    case QMetaType::Float:
                        return true;
                    default :
                        return false;
                }
            }

            static bool isUnsignedInteger(const QVariant &value)
            {
                switch (value.typeId()) {
                    case QMetaType::ULong:
                    case QMetaType::UInt:
                    case QMetaType::ULongLong:
                        return true;
                    default :
                        return false;
                }
            }

            static bool isBool(const QVariant &value) { return isOfType( value, QMetaType::Bool ); }

            static bool isString(const QVariant &value) { return isOfType( value, QMetaType::QString ); }

            static bool isNull(const QVariant &value) { return value.isNull(); }

            static bool EQ(const QVariant &v1,const QVariant & v2) { return v1 == v2; }

            static bool contains(const QVariant &value, const std::string &k)
            {
                return isObject(value) && value.toMap().contains(QString::fromStdString(k));
            }

            static QVariant at(const QVariant &value, const std::string &k)
            {
                if (!isObject(value)) {
                    throw jsonata::JException("Value is not an object");
                }
                return value.toMap()[QString::fromStdString(k)];
            }

            static QVariant at(const QVariant &value, int k)
            {
                if (!isArray(value)) {
                    throw jsonata::JException("Value is not an array");
                }
                return value.toList()[k];
            }

            template<typename outType>
            static outType get(const QVariant &value)
            {
                return value.template value<outType>();
            }

            template<>
            std::string get(const QVariant &value)
            {
                return get<QString>(value).toStdString();
            }

            static QVariant array() { return QVariantList(); }

            static QVariant object() { return QVariantMap(); }

            static void pushBack(QVariant &val, auto value)
            {
                QVariantList a = val.toList();
                a.push_back(value);
                val = a;
            }

            template<typename action>
            static void mutateForAll(QVariant &from, action a)
            {
                using fromType = std::decay_t<decltype(from)>;
                if (isArray(from)) {
                    int idx = 0;
                    bool changed = false;
                    auto arr = from.toList();
                    for (auto el : arr) {
                        if (a(std::to_string(idx++), el)) {
                            arr[idx - 1] = el;
                            changed = true;
                        }
                    }
                    if (changed) {
                        from = arr;
                    }
                } else if (isObject(from)) {
                    bool changed = false;
                    auto obj = from.toMap();
                    for (auto [key, value] : obj.asKeyValueRange()) {
                        QVariant J(value);
                        if (a(key.toStdString(), J)) {
                            value = J;
                            changed = true;
                        }
                    }
                    if (changed) {
                        from = obj;
                    }
                }
            }

            template<typename action>
            static void forAll(const QVariant &from, action a)
            {
                if (isArray(from)) {
                    int idx = 0;
                    for (const auto &el : from.toList()) {
                        a(std::to_string(idx++), QVariant(el));
                    }
                } else if (isObject(from)) {
                    auto obj = from.toMap();
                    for (auto [key, value] : obj.asKeyValueRange()) {
                        a(key.toStdString(), QVariant(value));
                    }
                }
            }

            template<typename keyType, typename valueType>
            static void appendValue(QVariant &to, const keyType &k, const valueType &v)
            {
                if (isArray(to)) {
                    auto x = to.toList();
                    if constexpr (std::is_convertible_v<valueType, std::string>) {
                        x.push_back(QString::fromStdString(v));
                    } else if constexpr (!std::is_convertible_v<valueType, std::string>) {
                        x.push_back(v);
                    }
                    to = x;
                } else {
                    auto x = to.toMap();
                    if constexpr (std::is_convertible_v<valueType, std::string>) {
                        x[QString::fromStdString(k)] = QString::fromStdString(v);
                    } else if constexpr (!std::is_convertible_v<valueType, std::string>) {
                        x[QString::fromStdString(k)] = v;
                    }
                    to = x;
                }
            }

            template<typename keyType, typename valueType>
            static void set(QVariant &to, const keyType &k, const valueType &v)
            {
                if constexpr (std::is_same_v<keyType, QString>) {
                    auto x = to.toMap();
                    if constexpr (std::is_convertible_v<valueType, std::string>) {
                        x[k] = QString::fromStdString(v);
                    } else {
                        x[k] = v;
                    }
                    to = x;
                } else if constexpr (std::is_convertible_v<keyType, std::string>) {
                    auto x = to.toMap();
                    if constexpr (std::is_convertible_v<valueType, std::string>) {
                        x[QString::fromStdString(k)] = QString::fromStdString(v);
                    } else {
                        x[QString::fromStdString(k)] = v;
                    }
                    to = x;
                } else if constexpr (std::is_convertible_v<keyType, int>) {
                    auto x = to.toList();
                    if constexpr (std::is_convertible_v<valueType, std::string>) {
                        x[k] = QString::fromStdString(v);
                    } else {
                        x[k] = v;
                    }
                    to = x;
                }
            }

            static std::string dump(const QVariant &v)
            {
                return QString::fromUtf8(QJsonValue::fromVariant(v).toJson(QJsonValue::JsonFormat::Compact)).toStdString();
            }

            static QVariant parse(const std::string &s)
            {
                auto unescape = [](QString & input ) {
                    // Match \u followed by 4 hex digits
                    static QRegularExpression re("\\\\u([0-9a-fA-F]{4})");
                    QRegularExpressionMatchIterator i = re.globalMatch(input);

                    int offset = 0;
                    while (i.hasNext()) {
                        QRegularExpressionMatch match = i.next();
                        bool ok;
                        ushort code = match.captured(1).toUShort(&ok, 16);

                        if (ok) {
                            QString replacement = QChar(code);
                            input.replace(match.capturedStart() + offset, match.capturedLength(), replacement);
                            // Adjust offset because the string length changed
                            offset += (replacement.length() - match.capturedLength());
                        }
                    }
                };

                QString str = QString::fromStdString( s );
                unescape( str );

                QUtf8StringView sv( str.toUtf8().data() );

                if( ! sv.isValidUtf8() ) {
                    throw jsonata::JException("D3141", 0);
                }

                QJsonParseError Err;
                auto r = QJsonValue::fromJson(str.toUtf8(), &Err);
                if (Err.error != QJsonParseError::NoError) {
                    throw jsonata::JException("D3141", Err.offset, Err.errorString().data());
                }
                return r.toVariant();
            }

            // for simple types
            static bool getPropertyValueOfType(const QVariant &root,
                                               const std::string &propertyName,
                                               auto &propertyValue)
            {
                using propertyType = std::decay_t<decltype(propertyValue)>;

                auto obj = root.toMap();
                QString S = QString::fromStdString(propertyName);
                if( ! obj.contains( S )) {
                    return false;
                }

                QVariant E = obj.value( S );

                if constexpr (std::is_base_of_v<std::string, propertyType>) {
                    // property must be string
                    if ( isString( E ) ) {
                        propertyValue = E.toString().toStdString();
                        return true;
                    }
                } else if constexpr (std::is_same_v<propertyType, bool>) {
                    if (isBool( E ) ) {
                        propertyValue = E.toBool();
                        return true;
                    }
                } else if constexpr (IsQVariant<propertyType>) {
                    propertyValue = E;
                    return true;
                } else {
                    // This triggers ONLY if none of the above branches are taken
                    static_assert(sizeof(propertyType) == 0, "cannot get unknown propertytype");
                }

                return false;
            }

            static bool getPropertyValueOfType(const QVariant &root,
                                               const std::string &propertyName,
                                               TaggedProperty<QVariant,AsArray> propertyValue)
            {
                auto obj = root.toMap();
                QString S = QString::fromStdString(propertyName);

                if( ! obj.contains( S )) {
                    return false;
                }

                QVariant E = obj.value( S );

                if (isArray(E)) {
                    // value MUST be an array
                    propertyValue.value = E.toList();
                    return true;
                }
                return false;
            }

        private :
            static bool isOfType( const QVariant & v, int type ) {
                return v.typeId() == type;
            }

    };

}
