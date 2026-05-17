#pragma once

#include "JException.h"
#include "Utils.h"
#include <concepts>

#include "common_backend.h"

// #include <array>
// #include <any>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QMap>
#include <QRegularExpression>

template<typename T>
concept IsQtJson = std::is_base_of_v<QJsonValue, T> || std::is_base_of_v<QJsonArray, T>
                   || std::is_base_of_v<QJsonObject, T> || std::is_base_of_v<QJsonValueRef, T>;

namespace jsonata
{
template<>
struct json_bridge_impl<QJsonValue,void>
{
        using is_json_bridge_type = void; // Tag to satisfy the concept

        static QJsonValue create(auto &&value)
        {
            using V = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<V, std::nullptr_t>) {
                return QJsonValue(QJsonValue::Null);
            } else if constexpr (std::is_same_v<V, std::string> || std::is_same_v<V, const char *>) {
                // Handle both std::string and literal "strings"
                if constexpr (std::is_same_v<V, const char *>)
                    return QJsonValue(QString(value));
                else
                    return QJsonValue(QString::fromStdString(value));
            } else if constexpr (std::is_integral_v<V> && !std::is_same_v<V, bool>) {
                // Handle all integers (int, long, long long, size_t, etc.)
                // qlonglong is the safest sink for integral types in Qt
                return QJsonValue(static_cast<qlonglong>(value));
            }
            // Handle floating point
            else if constexpr (std::is_floating_point_v<V>) {
                return QJsonValue(static_cast<double>(value));
            } else if constexpr (std::is_constructible_v<QJsonValue, V>) {
                return value;
            } else {
                static_assert(false, "Unsupported type for QJsonValue creation");
            }
        }

        static QJsonValue create() { return QJsonValue(); }

        static size_t size(const QJsonValue &value)
        {
            return value.isArray() ? value.toArray().count() : 0;
        }

        static bool isEmpty(const QJsonValue &value) {
            return value.isNull() ||
                   (value.isObject() && value.toObject().empty()) ||
                   (value.isArray() && value.toArray().empty());
        }

        static bool isObject(const QJsonValue &value) { return value.isObject(); }

        static bool isArray(const QJsonValue &value) { return value.isArray(); }

        static bool isNumber(const QJsonValue &value) { return value.isDouble() ; }
        static bool isInteger(const QJsonValue &value)
        {
            return value.isDouble() && value.toVariant().typeId() == QMetaType::LongLong;
        }

        static bool isFloat(const QJsonValue &value)
        {
            return value.isDouble() && value.toVariant().typeId() == QMetaType::Double;
        }

        static bool isUnsignedInteger(const QJsonValue &value)
        {
            if (!value.isDouble()) {
                return false;
            }
            QVariant V = value.toVariant();
            return V.typeId() == QMetaType::LongLong && V.toLongLong() >= 0;
        }

        static bool isBool(const QJsonValue &value) { return value.isBool(); }

        static bool isString(const QJsonValue &value) { return value.isString(); }

        static bool isNull(const QJsonValue &value) { return value.isNull(); }

        static bool EQ(const QJsonValue &v1,const QJsonValue & v2) { return v1 == v2; }

        static bool contains(const QJsonValue &value, const std::string &k)
        {
            return value.isObject() && value.toObject().contains(QString::fromStdString(k));
        }

        static QJsonValue at(const QJsonValue &value, const std::string &k)
        {
            if (!value.isObject()) {
                throw jsonata::JException("Value is not an object");
            }
            return value.toObject()[QString::fromStdString(k)];
        }

        static QJsonValue at(const QJsonValue &value, int k)
        {
            if (!value.isArray()) {
                throw jsonata::JException("Value is not an array");
            }
            return value.toArray()[k];
        }

        template<typename outType>
        static outType get(const QJsonValue &value)
        {
            return value.toVariant().template value<outType>();
        }

        template<>
        std::string get(const QJsonValue &value)
        {
            return get<QString>(value).toStdString();
        }

        static QJsonValue array() { return QJsonArray(); }

        static QJsonValue object() { return QJsonObject(); }

        static void pushBack(QJsonValue &val, auto value)
        {
            QJsonArray a = val.toArray();
            a.push_back(value);
            val = a;
        }

        template<typename action>
        static void mutateForAll(QJsonValue &from, action a)
        {
            using fromType = std::decay_t<decltype(from)>;
            if (from.isArray()) {
                int idx = 0;
                bool changed = false;
                QJsonArray arr = from.toArray();
                for (auto el : arr) {
                    if (a(std::to_string(idx++), el)) {
                        arr[idx - 1] = el;
                        changed = true;
                    }
                }
                if (changed) {
                    from = arr;
                }
            } else if (from.isObject()) {
                bool changed = false;
                QJsonObject obj = from.toObject();
                for (auto [key, value] : obj.asKeyValueRange()) {
                    QJsonValue J(value);
                    if (a(key.toString().toStdString(), J)) {
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
        static void forAll(const QJsonValue &from, action a)
        {
            if (from.isArray()) {
                int idx = 0;
                for (const auto &el : from.toArray()) {
                    a(std::to_string(idx++), QJsonValue(el));
                }
            } else if (from.isObject()) {
                QJsonObject obj = from.toObject();
                for (auto [key, value] : obj.asKeyValueRange()) {
                    a(key.toString().toStdString(), QJsonValue(value));
                }
            }
        }

        template<typename keyType, typename valueType>
        static void appendValue(QJsonValue &to, const keyType &k, const valueType &v)
        {
            if (to.isArray()) {
                auto x = to.toArray();
                if constexpr (std::is_convertible_v<valueType, std::string>) {
                    x.push_back(QString::fromStdString(v));
                } else if constexpr (!std::is_convertible_v<valueType, std::string>) {
                    x.push_back(v);
                }
                to = x;
            } else {
                auto x = to.toObject();
                if constexpr (std::is_convertible_v<valueType, std::string>) {
                    x[QString::fromStdString(k)] = QString::fromStdString(v);
                } else if constexpr (!std::is_convertible_v<valueType, std::string>) {
                    x[QString::fromStdString(k)] = v;
                }
                to = x;
            }
        }

        template<typename keyType, typename valueType>
        static void set(QJsonValue &to, const keyType &k, const valueType &v)
        {
            if constexpr (std::is_same_v<keyType, QString>) {
                auto x = to.toObject();
                if constexpr (std::is_convertible_v<valueType, std::string>) {
                    x[k] = QString::fromStdString(v);
                } else {
                    x[k] = v;
                }
                to = x;
            } else if constexpr (std::is_convertible_v<keyType, std::string>) {
                auto x = to.toObject();
                if constexpr (std::is_convertible_v<valueType, std::string>) {
                    x[QString::fromStdString(k)] = QString::fromStdString(v);
                } else {
                    x[QString::fromStdString(k)] = v;
                }
                to = x;
            } else if constexpr (std::is_convertible_v<keyType, int>) {
                auto x = to.toArray();
                if constexpr (std::is_convertible_v<valueType, std::string>) {
                    x[k] = QString::fromStdString(v);
                } else {
                    x[k] = v;
                }
                to = x;
            }
        }

        static std::string dump(const QJsonValue &v)
        {
            return QString::fromUtf8(QJsonValue(v).toJson(QJsonValue::JsonFormat::Compact)).toStdString();
        }

        static QJsonValue parse(const std::string &s)
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
            return r;
        }

        // for simple types
        static bool getPropertyValueOfType(const QJsonValue &root,
                                           const std::string &propertyName,
                                           auto &propertyValue)
        {
            using propertyType = std::decay_t<decltype(propertyValue)>;

            QJsonObject obj = root.toObject();
            QJsonValue E = obj.value(QString::fromStdString(propertyName));

            if( E.isUndefined() ) {
                // propertyname does not exist
                return false;
            }

            if constexpr (std::is_base_of_v<std::string, propertyType>) {
                // property must be string
                if (E.isString()) {
                    propertyValue = E.toString().toStdString();
                    return true;
                }
            } else if constexpr (std::is_same_v<propertyType, bool>) {
                if (E.isBool()) {
                    propertyValue = E.toBool();
                    return true;
                }
            } else if constexpr (IsQtJson<propertyType>) {
                propertyValue = E;
                return true;
            } else {
                // This triggers ONLY if none of the above branches are taken
                static_assert(sizeof(propertyType) == 0, "cannot get unknown propertytype");
            }

            return false;
        }

        static bool getPropertyValueOfType(const QJsonValue &root,
                                           const std::string &propertyName,
                                           TaggedProperty<QJsonValue,AsArray> propertyValue)
        {
            QJsonObject obj = root.toObject();
            QJsonValue E = obj.value(QString::fromStdString(propertyName));

            if( E.isUndefined() ) {
                // propertyname does not exist
                return false;
            }

            if (E.isArray()) {
                // value MUST be an array
                propertyValue.value = E.toArray();
                return true;
            }
            return false;
        }

};

}
