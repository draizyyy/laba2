#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include <cctype>
#include <algorithm>

#include "sequences/sequence.hpp"
#include "core/dynamic_array.hpp"
#include "core/linked_list.hpp"
#include "exceptions.hpp"
#include "option.hpp"
#include "sequences/array_sequence.hpp"
#include "sequences/list_sequence.hpp"
#include "sequences/bit_sequence.hpp"
#include <httplib.h>

using namespace myLib;

class JsonValue {
public:
    enum Type { NULL_T, BOOL_T, NUMBER_T, STRING_T, ARRAY_T, OBJECT_T };
    Type type = NULL_T;
    
    bool b = false;
    double n = 0.0;
    std::string s;
    std::vector<JsonValue> arr;
    std::map<std::string, JsonValue> obj;

    JsonValue() = default;
    JsonValue(int v) : type(NUMBER_T), n(v) {}
    JsonValue(double v) : type(NUMBER_T), n(v) {}
    JsonValue(const std::string& v) : type(STRING_T), s(v) {}
    JsonValue(const char* v) : type(STRING_T), s(v) {}
    JsonValue(bool v) : type(BOOL_T), b(v) {}

    static JsonValue object() { JsonValue j; j.type = OBJECT_T; return j; }
    static JsonValue array() { JsonValue j; j.type = ARRAY_T; return j; }

    JsonValue& operator[](const std::string& key) {
        if (type != OBJECT_T) type = OBJECT_T;
        return obj[key];
    }
    
    void push_back(const JsonValue& item) {
        if (type != ARRAY_T) type = ARRAY_T;
        arr.push_back(item);
    }

    std::string dump(bool root = true) const {
        std::ostringstream oss;
        switch (type) {
            case NULL_T: oss << "null"; break;
            case BOOL_T: oss << (b ? "true" : "false"); break;
            case NUMBER_T: oss << n; break;
            case STRING_T: oss << "\"" << s << "\""; break;
            case ARRAY_T: {
                oss << "[";
                for (size_t i = 0; i < arr.size(); ++i) {
                    oss << arr[i].dump(false) << (i + 1 < arr.size() ? "," : "");
                }
                oss << "]";
                break;
            }
            case OBJECT_T: {
                oss << "{";
                size_t count = 0;
                for (auto const& [key, val] : obj) {
                    oss << "\"" << key << "\":" << val.dump(false);
                    if (++count < obj.size()) oss << ",";
                }
                oss << "}";
                break;
            }
        }
        return oss.str();
    }

    std::string asString() const {
        if (type == STRING_T) return s;
        if (type == NUMBER_T) {
            std::ostringstream oss;
            oss << n;
            return oss.str();
        }
        if (type == BOOL_T) return b ? "true" : "false";
        if (type == NULL_T) return "null";
        return "";
    }

    static JsonValue parseSimpleDict(const std::string& body) {
        JsonValue result = JsonValue::object();
        std::string clean = body;
        clean.erase(remove_if(clean.begin(), clean.end(), [](char c){ return c=='{' || c=='}' || c=='\"'; }), clean.end());
        std::istringstream stream(clean);
        std::string pair;
        while (getline(stream, pair, ',')) {
            size_t delim = pair.find(':');
            if (delim != std::string::npos) {
                std::string key = pair.substr(0, delim);
                std::string val = pair.substr(delim + 1);
                key.erase(0, key.find_first_not_of(" \t\n\r"));
                val.erase(0, val.find_first_not_of(" \t\n\r"));
                
                if (val == "true") result[key] = true;
                else if (val == "false") result[key] = false;
                else if (!val.empty() && isdigit(val[0])) result[key] = std::stod(val);
                else result[key] = val;
            }
        }
        return result;
    }
};

static int Square(int x) { return x * x; }
static bool IsEven(int x) { return x % 2 == 0; }
static bool IsPositive(int x) { return x > 0; }
static int SumFunc(int acc, int x) { return acc + x; }
static int ProdFunc(int acc, int x) { return acc * x; }
static bool IsDivBy3(int x) { return x % 3 == 0; }

template <typename T> T FromString(const std::string& str);
template <> int FromString<int>(const std::string& str) { return str.empty() ? 0 : std::stoi(str); }
template <> double FromString<double>(const std::string& str) { return str.empty() ? 0.0 : std::stod(str); }
template <> std::string FromString<std::string>(const std::string& str) { return str; }
template <> Bit<uint8_t> FromString<Bit<uint8_t>>(const std::string& str) { return Bit<uint8_t>(str.empty() ? 0 : std::stoi(str)); }

template <typename T> std::string ToString(const T& val) {
    std::ostringstream oss; oss << val; return oss.str();
}

template <> std::string ToString<Bit<uint8_t>>(const Bit<uint8_t>& val) {
    return std::to_string(val.GetValue());
}

class ISequenceWrapper {
public:
    virtual ~ISequenceWrapper() = default;
    virtual void Append(const std::string& val) = 0;
    virtual void Prepend(const std::string& val) = 0;
    virtual void InsertAt(const std::string& val, int index) = 0;
    virtual size_t GetLength() const = 0;
    virtual std::string GetFirst() const = 0;
    virtual std::string GetLast() const = 0;
    virtual std::string Get(int index) const = 0;
    virtual JsonValue GetAllAsJsonArray() const = 0;

    virtual JsonValue Map(const std::string& func) { throw std::runtime_error("Не поддерживается"); }
    virtual JsonValue Where(const std::string& func) { throw std::runtime_error("Не поддерживается"); }
    virtual std::string Reduce(const std::string& func, const std::string& initVal) { throw std::runtime_error("Не поддерживается"); }
    virtual std::string GetFirstPred(const std::string& func) { throw std::runtime_error("Не поддерживается"); }
    virtual std::string GetLastPred(const std::string& func) { throw std::runtime_error("Не поддерживается"); }
};

template <typename T>
class SequenceWrapper : public ISequenceWrapper {
private:
    std::unique_ptr<Sequence<T>> seq;
public:
    SequenceWrapper(Sequence<T>* ptr) : seq(ptr) {}

    void Append(const std::string& val) override { seq->Append(FromString<T>(val)); }
    void Prepend(const std::string& val) override { seq->Prepend(FromString<T>(val)); }
    void InsertAt(const std::string& val, int index) override { seq->InsertAt(FromString<T>(val), index); }
    size_t GetLength() const override { return seq->GetLength(); }
    std::string GetFirst() const override { return ToString(seq->GetFirst()); }
    std::string GetLast() const override { return ToString(seq->GetLast()); }
    std::string Get(int index) const override { return ToString(seq->Get(index)); }

    JsonValue GetAllAsJsonArray() const override {
        JsonValue arr = JsonValue::array();
        for (size_t i = 0; i < seq->GetLength(); ++i) {
            arr.push_back(JsonValue(ToString(seq->Get(i))));
        }
        return arr;
    }

    JsonValue Map(const std::string& func) override {
        if constexpr (std::is_same_v<T, int>) {
            JsonValue arr = JsonValue::array();
            if (func == "Square") {
                for (size_t i = 0; i < seq->GetLength(); ++i) {
                    arr.push_back(Square(seq->Get(i)));
                }
            } else {
                throw std::runtime_error("Не поддерживается с данной функцией: " + func);
            }
            return arr;
        }
        throw std::runtime_error("Map поддерживает только тип int");
    }

    JsonValue Where(const std::string& func) override {
        if constexpr (std::is_same_v<T, int>) {
            JsonValue arr = JsonValue::array();
            bool (*pred)(int) = nullptr;
            if (func == "IsEven") pred = IsEven;
            else if (func == "IsPositive") pred = IsPositive;
            else if (func == "IsDivBy3") pred = IsDivBy3;
            else throw std::runtime_error("Не поддерживается с данной функцией: " + func);
            
            for (size_t i = 0; i < seq->GetLength(); ++i) {
                int val = seq->Get(i);
                if (pred(val)) {
                    arr.push_back(val);
                }
            }
            return arr;
        }
        throw std::runtime_error("Where поддерживает только тип int");
    }

    std::string Reduce(const std::string& func, const std::string& initValStr) override {
        if constexpr (std::is_same_v<T, int>) {
            int initVal = FromString<int>(initValStr);
            int result = initVal;
            
            if (func == "SumFunc") {
                for (size_t i = 0; i < seq->GetLength(); ++i) {
                    result = SumFunc(result, seq->Get(i));
                }
            } else if (func == "ProdFunc") {
                for (size_t i = 0; i < seq->GetLength(); ++i) {
                    result = ProdFunc(result, seq->Get(i));
                }
            } else {
                throw std::runtime_error("Не поддерживается с данной функцией: " + func);
            }
            return ToString(result);
        }
        throw std::runtime_error("Reduce поддерживает только тип int");
    }

    std::string GetFirstPred(const std::string& func) override {
        if constexpr (std::is_same_v<T, int>) {
            bool (*pred)(int) = nullptr;
            if (func == "IsEven") pred = IsEven;
            else if (func == "IsPositive") pred = IsPositive;
            else if (func == "IsDivBy3") pred = IsDivBy3;
            else throw std::runtime_error("Не поддерживается с данной функцией: " + func);

            for (size_t i = 0; i < seq->GetLength(); ++i) {
                int val = seq->Get(i);
                if (pred(val)) {
                    return ToString(val);
                }
            }
            return "Не найдено";
        }
        throw std::runtime_error("Предикаты поддерживаются только для типа int");
    }

    std::string GetLastPred(const std::string& func) override {
        if constexpr (std::is_same_v<T, int>) {
            bool (*pred)(int) = nullptr;
            if (func == "IsEven") pred = IsEven;
            else if (func == "IsPositive") pred = IsPositive;
            else if (func == "IsDivBy3") pred = IsDivBy3;
            else throw std::runtime_error("Не поддерживается с данной функцией: " + func);

            for (int i = static_cast<int>(seq->GetLength()) - 1; i >= 0; --i) {
                int val = seq->Get(i);
                if (pred(val)) {
                    return ToString(val);
                }
            }
            return "Не найдено";
        }
        throw std::runtime_error("Предикаты поддерживаются только для типа int");
    }
};

template <>
class SequenceWrapper<Bit<uint8_t>> : public ISequenceWrapper {
private:
    std::unique_ptr<BitSequence<uint8_t>> seq;
    static constexpr size_t bitsPerElement = 8;
public:
    SequenceWrapper(BitSequence<uint8_t>* ptr) : seq(ptr) {}

    void Append(const std::string& val) override {
        int v = std::stoi(val.empty() ? "0" : val);
        seq->Append(Bit<uint8_t>(v));
    }
    void Prepend(const std::string& val) override {
        int v = std::stoi(val.empty() ? "0" : val);
        seq->Prepend(Bit<uint8_t>(v));
    }
    void InsertAt(const std::string& val, int index) override {
        int v = std::stoi(val.empty() ? "0" : val);
        (*seq)[index] = (v != 0);
    }
    size_t GetLength() const override { return seq->GetLength() * bitsPerElement; }
    std::string GetFirst() const override { return std::to_string(static_cast<bool>((*seq)[0])); }
    std::string GetLast() const override { 
        size_t len = seq->GetLength();
        if (len == 0) return "Пусто";
        return std::to_string(static_cast<bool>((*seq)[len * bitsPerElement - 1])); 
    }
    std::string Get(int index) const override { return std::to_string(static_cast<bool>((*seq)[index])); }

    JsonValue GetAllAsJsonArray() const override {
        JsonValue arr = JsonValue::array();
        size_t total = seq->GetLength() * bitsPerElement;
        for (size_t i = 0; i < total; ++i) {
            arr.push_back(JsonValue(static_cast<bool>((*seq)[i]) ? 1 : 0));
        }
        return arr;
    }

    JsonValue Map(const std::string& func) override { throw std::runtime_error("Не поддерживается"); }
    JsonValue Where(const std::string& func) override { throw std::runtime_error("Не поддерживается"); }
    std::string Reduce(const std::string& func, const std::string& initVal) override { throw std::runtime_error("Не поддерживается"); }
    std::string GetFirstPred(const std::string& func) override { throw std::runtime_error("Не поддерживается"); }
    std::string GetLastPred(const std::string& func) override { throw std::runtime_error("Не поддерживается"); }
};

std::unique_ptr<ISequenceWrapper> activeSequence = nullptr;

std::string ProcessApiRequest(const std::string& requestBody) {
    JsonValue request = JsonValue::parseSimpleDict(requestBody);
    JsonValue response = JsonValue::object();
    response["status"] = "ok";

    std::string action = request["action"].s;
    std::string seqType = request["seqType"].s;
    std::string dataType = request["dataType"].s;
    std::string valueStr = request["value"].asString();
    int index = static_cast<int>(request["index"].n);
    std::string func = request["func"].s;

    try {
        if (action == "create") {
            if (dataType == "int") {
                if (seqType == "ArraySequence") activeSequence = std::make_unique<SequenceWrapper<int>>(new ArraySequence<int>());
                else if (seqType == "ListSequence") activeSequence = std::make_unique<SequenceWrapper<int>>(new ListSequence<int>());
            } else if (dataType == "double") {
                if (seqType == "ArraySequence") activeSequence = std::make_unique<SequenceWrapper<double>>(new ArraySequence<double>());
                else if (seqType == "ListSequence") activeSequence = std::make_unique<SequenceWrapper<double>>(new ListSequence<double>());
            } else if (dataType == "string") {
                if (seqType == "ArraySequence") activeSequence = std::make_unique<SequenceWrapper<std::string>>(new ArraySequence<std::string>());
                else if (seqType == "ListSequence") activeSequence = std::make_unique<SequenceWrapper<std::string>>(new ListSequence<std::string>());
            } else if (dataType == "uint8" && seqType == "BitSequence") {
                activeSequence = std::make_unique<SequenceWrapper<Bit<uint8_t>>>(new BitSequence<uint8_t>());
            } else {
                throw std::runtime_error("Неподдерживаемая комбинация последовательности и типа данных");
            }
            response["result"] = "Создано: " + seqType + "<" + dataType + ">";
        } 
        else if (activeSequence) {
            if (action == "append") { activeSequence->Append(valueStr); response["result"] = "Добавлено в конец"; }
            else if (action == "prepend") { activeSequence->Prepend(valueStr); response["result"] = "Добавлено в начало"; }
            else if (action == "insertAt") { activeSequence->InsertAt(valueStr, index); response["result"] = "Установлено по индексу"; }
            else if (action == "getLength") response["result"] = static_cast<double>(activeSequence->GetLength());
            else if (action == "getFirst") response["result"] = activeSequence->GetFirst();
            else if (action == "getLast") response["result"] = activeSequence->GetLast();
            else if (action == "get") response["result"] = activeSequence->Get(index);
            else if (action == "map") response["result"] = activeSequence->Map(func);
            else if (action == "where") response["result"] = activeSequence->Where(func);
            else if (action == "reduce") response["result"] = activeSequence->Reduce(func, valueStr);
            else if (action == "getFirstPred") response["result"] = activeSequence->GetFirstPred(func);
            else if (action == "getLastPred") response["result"] = activeSequence->GetLastPred(func);
        } else {
            throw std::runtime_error("Последовательность ещё не создана");
        }
    } catch (const std::exception& e) {
        response["status"] = "error";
        response["message"] = std::string(e.what());
    }

    if (activeSequence) {
        response["all"] = activeSequence->GetAllAsJsonArray();
    } else {
        response["all"] = JsonValue::array();
    }

    return response.dump();
}

const char* HTML_CONTENT = R"HTML(
<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <title>Веб-интерфейс последовательностей</title>
    <style>
        body { font-family: sans-serif; margin: 20px; background: #f4f4f9; color: #333; }
        .container { max-width: 800px; margin: 0 auto; background: #fff; padding: 20px; border-radius: 8px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }
        .panel { padding: 15px; border: 1px solid #ddd; margin-bottom: 15px; border-radius: 4px; }
        select, input, button { padding: 8px; margin: 5px 0; border: 1px solid #ccc; border-radius: 4px; }
        button { background: #007bff; color: white; border: none; cursor: pointer; }
        button:hover { background: #0056b3; }
        .output { background: #e9ecef; padding: 10px; border-radius: 4px; font-family: monospace; white-space: pre-wrap; min-height: 20px; margin-bottom: 15px;}
        .grid { display: grid; grid-template-columns: repeat(auto-fill, minmax(180px, 1fr)); gap: 10px; }
    </style>
</head>
<body>
<div class="container">
    <h2>Интерфейс последовательностей</h2>
    <div class="panel">
        <select id="seqType">
            <option value="ArraySequence">ArraySequence</option>
            <option value="ListSequence">ListSequence</option>
            <option value="BitSequence">BitSequence</option>
        </select>
        <select id="dataType">
            <option value="int">int</option>
            <option value="double">double</option>
            <option value="string">string</option>
            <option value="uint8">uint8 (для BitSequence)</option>
        </select>
        <button onclick="apiCall('create')">Создать последовательность</button>
    </div>
    <div class="panel">
        <input type="text" id="inputValue" placeholder="Значение (0 или 1 для битов)">
        <input type="number" id="inputIndex" placeholder="Индекс (опционально)">
        <div class="grid">
            <button onclick="apiCall('prepend')">Добавить в начало</button>
            <button onclick="apiCall('append')">Добавить в конец</button>
            <button onclick="apiCall('insertAt')">Установить по индексу</button>
            <button onclick="apiCall('getLength')">Размер</button>
            <button onclick="apiCall('getFirst')">Начало</button>
            <button onclick="apiCall('getLast')">Конец</button>
            <button onclick="apiCall('get')">По индексу</button>
        </div>
    </div>
    <div class="panel">
        <select id="predicateFunc">
            <option value="Square">Square</option>
            <option value="IsEven">IsEven</option>
            <option value="IsPositive">IsPositive</option>
            <option value="SumFunc">SumFunc</option>
            <option value="ProdFunc">ProdFunc</option>
            <option value="IsDivBy3">IsDivBy3</option>
        </select>
        <div class="grid">
            <button onclick="apiCall('map')">Map</button>
            <button onclick="apiCall('reduce')">Reduce</button>
            <button onclick="apiCall('where')">Where</button>
            <button onclick="apiCall('getFirstPred')">Первый по предикату</button>
            <button onclick="apiCall('getLastPred')">Последний по предикату</button>
        </div>
    </div>
    <h3>Все элементы:</h3>
    <div id="allElements" class="output">[]</div>
    <h3>Результат:</h3>
    <div id="resultOutput" class="output"></div>
</div>
<script>
async function apiCall(action) {
    const payload = {
        action: action,
        seqType: document.getElementById('seqType').value,
        dataType: document.getElementById('dataType').value,
        value: document.getElementById('inputValue').value,
        index: parseInt(document.getElementById('inputIndex').value) || 0,
        func: document.getElementById('predicateFunc').value
    };
    try {
        const response = await fetch('/api', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(payload)
        });
        const data = await response.json();
        if (data.status === "error") {
            document.getElementById('resultOutput').innerText = "Ошибка: " + data.message;
        } else {
            document.getElementById('resultOutput').innerText = JSON.stringify(data.result, null, 2);
            document.getElementById('allElements').innerText = JSON.stringify(data.all, null, 2);
        }
    } catch (e) {
        document.getElementById('resultOutput').innerText = "Сетевая ошибка: " + e;
    }
}
</script>
</body>
</html>
)HTML";

int main() {
    httplib::Server svr;

    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(HTML_CONTENT, "text/html");
    });

    svr.Post("/api", [](const httplib::Request& req, httplib::Response& res) {
        std::string jsonResponse = ProcessApiRequest(req.body);
        res.set_content(jsonResponse, "application/json");
    });

    std::cout << "Сервер слушает 0.0.0.0:8080\n";
    svr.listen("0.0.0.0", 8080);

    return 0;
}