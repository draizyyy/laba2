#include <iostream>
#include <string>
#include <sstream>
#include "httplib.h"
#include "nlohmann/json.hpp"
#include "sequences/sequence.hpp"
#include "core/dynamic_array.hpp"
#include "core/linked_list.hpp"
#include "exceptions.hpp"
#include "option.hpp"
#include "sequences/array_sequence.hpp"
#include "sequences/list_sequence.hpp"
#include "sequences/bit_sequence.hpp"

using json = nlohmann::json;
using namespace myLib;

static int Square(int x) { return x * x; }
static bool IsEven(int x) { return x % 2 == 0; }
static bool IsPositive(int x) { return x > 0; }
static int SumFunc(int acc, int x) { return acc + x; }
static int ProdFunc(int acc, int x) { return acc * x; }
static bool IsDivBy3(int x) { return x % 3 == 0; }

Sequence<int>* activeIntSeq = nullptr;
Sequence<double>* activeDoubleSeq = nullptr;
Sequence<std::string>* activeStringSeq = nullptr;
BitSequence<uint8_t>* activeBitSeq = nullptr;
std::string currentSeqType = "";
std::string currentDataType = "";

void ClearCurrent() {
    delete activeIntSeq; activeIntSeq = nullptr;
    delete activeDoubleSeq; activeDoubleSeq = nullptr;
    delete activeStringSeq; activeStringSeq = nullptr;
    delete activeBitSeq; activeBitSeq = nullptr;
}

json GetAllElements() {
    json arr = json::array();
    if (currentDataType == "int" && activeIntSeq) {
        for (size_t i = 0; i < activeIntSeq->GetLength(); ++i) arr.push_back(activeIntSeq->Get(i));
    } else if (currentDataType == "double" && activeDoubleSeq) {
        for (size_t i = 0; i < activeDoubleSeq->GetLength(); ++i) arr.push_back(activeDoubleSeq->Get(i));
    } else if (currentDataType == "string" && activeStringSeq) {
        for (size_t i = 0; i < activeStringSeq->GetLength(); ++i) arr.push_back(activeStringSeq->Get(i));
    } else if (currentDataType == "uint8" && activeBitSeq) {
        for (size_t i = 0; i < activeBitSeq->GetLength(); ++i) arr.push_back(static_cast<int>(activeBitSeq->Get(i).GetValue()));
    }
    return arr;
}

const char* HTML_CONTENT = R"HTML(
<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <title>Sequence Web UI</title>
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
    <h2>Sequence Interface</h2>
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
            <option value="uint8">uint8 (for BitSequence)</option>
        </select>
        <button onclick="apiCall('create')">Создать последовательность</button>
    </div>
    <div class="panel">
        <input type="text" id="inputValue" placeholder="Значение">
        <input type="number" id="inputIndex" placeholder="Индекс (опционально)">
        <div class="grid">
            <button onclick="apiCall('prepend')">Добавить в начало</button>
            <button onclick="apiCall('append')">Добавить в конец</button>
            <button onclick="apiCall('insertAt')">Добавить по индексу</button>
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
            <button onclick="apiCall('getFirstPred')">GetFirst (Pred)</button>
            <button onclick="apiCall('getLastPred')">GetLast (Pred)</button>
        </div>
    </div>
    <h3>Все элементы (Итератор):</h3>
    <div id="allElements" class="output">[]</div>
    <h3>Результат выполнения:</h3>
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
        json j = json::parse(req.body);
        json response;
        response["status"] = "ok";
        std::string action = j["action"];
        std::string seqType = j["seqType"];
        std::string dataType = j["dataType"];
        std::string valueStr = j["value"];
        int index = j["index"];
        std::string func = j["func"];

        try {
            if (action == "create") {
                ClearCurrent();
                currentSeqType = seqType;
                currentDataType = dataType;
                if (dataType == "int") {
                    if (seqType == "ArraySequence") activeIntSeq = new ArraySequence<int>();
                    else if (seqType == "ListSequence") activeIntSeq = new ListSequence<int>();
                } else if (dataType == "double") {
                    if (seqType == "ArraySequence") activeDoubleSeq = new ArraySequence<double>();
                    else if (seqType == "ListSequence") activeDoubleSeq = new ListSequence<double>();
                } else if (dataType == "string") {
                    if (seqType == "ArraySequence") activeStringSeq = new ArraySequence<std::string>();
                    else if (seqType == "ListSequence") activeStringSeq = new ListSequence<std::string>();
                } else if (dataType == "uint8" && seqType == "BitSequence") {
                    activeBitSeq = new BitSequence<uint8_t>();
                }
                response["result"] = "Created: " + seqType + "<" + dataType + ">";
            } else {
                if (action == "append") {
                    if (currentDataType == "int") activeIntSeq->Append(std::stoi(valueStr));
                    else if (currentDataType == "double") activeDoubleSeq->Append(std::stod(valueStr));
                    else if (currentDataType == "string") activeStringSeq->Append(valueStr);
                    else if (currentDataType == "uint8") activeBitSeq->Append(Bit<uint8_t>(std::stoi(valueStr)));
                    response["result"] = "Appended";
                } else if (action == "prepend") {
                    if (currentDataType == "int") activeIntSeq->Prepend(std::stoi(valueStr));
                    else if (currentDataType == "double") activeDoubleSeq->Prepend(std::stod(valueStr));
                    else if (currentDataType == "string") activeStringSeq->Prepend(valueStr);
                    else if (currentDataType == "uint8") activeBitSeq->Prepend(Bit<uint8_t>(std::stoi(valueStr)));
                    response["result"] = "Prepended";
                } else if (action == "insertAt") {
                    if (currentDataType == "int") activeIntSeq->InsertAt(std::stoi(valueStr), index);
                    else if (currentDataType == "double") activeDoubleSeq->InsertAt(std::stod(valueStr), index);
                    else if (currentDataType == "string") activeStringSeq->InsertAt(valueStr, index);
                    else if (currentDataType == "uint8") activeBitSeq->InsertAt(Bit<uint8_t>(std::stoi(valueStr)), index);
                    response["result"] = "Inserted";
                } else if (action == "getLength") {
                    if (currentDataType == "int") response["result"] = activeIntSeq->GetLength();
                    else if (currentDataType == "double") response["result"] = activeDoubleSeq->GetLength();
                    else if (currentDataType == "string") response["result"] = activeStringSeq->GetLength();
                    else if (currentDataType == "uint8") response["result"] = activeBitSeq->GetLength();
                } else if (action == "getFirst") {
                    if (currentDataType == "int") response["result"] = activeIntSeq->GetFirst();
                    else if (currentDataType == "double") response["result"] = activeDoubleSeq->GetFirst();
                    else if (currentDataType == "string") response["result"] = activeStringSeq->GetFirst();
                    else if (currentDataType == "uint8") response["result"] = static_cast<int>(activeBitSeq->GetFirst().GetValue());
                } else if (action == "getLast") {
                    if (currentDataType == "int") response["result"] = activeIntSeq->GetLast();
                    else if (currentDataType == "double") response["result"] = activeDoubleSeq->GetLast();
                    else if (currentDataType == "string") response["result"] = activeStringSeq->GetLast();
                    else if (currentDataType == "uint8") response["result"] = static_cast<int>(activeBitSeq->GetLast().GetValue());
                } else if (action == "get") {
                    if (currentDataType == "int") response["result"] = activeIntSeq->Get(index);
                    else if (currentDataType == "double") response["result"] = activeDoubleSeq->Get(index);
                    else if (currentDataType == "string") response["result"] = activeStringSeq->Get(index);
                    else if (currentDataType == "uint8") response["result"] = static_cast<int>(activeBitSeq->Get(index).GetValue());
                } else if (currentDataType == "int") {
                    if (action == "map") {
                        json arr = json::array();
                        if (currentSeqType == "ArraySequence") {
                            auto* seq = static_cast<ArraySequence<int>*>(activeIntSeq);
                            Sequence<int>* res = nullptr;
                            if (func == "Square") res = seq->Map(Square);
                            if (res) { for (size_t i = 0; i < res->GetLength(); ++i) arr.push_back(res->Get(i)); delete res; }
                        } else if (currentSeqType == "ListSequence") {
                            auto* seq = static_cast<ListSequence<int>*>(activeIntSeq);
                            Sequence<int>* res = nullptr;
                            if (func == "Square") res = seq->Map(Square);
                            if (res) { for (size_t i = 0; i < res->GetLength(); ++i) arr.push_back(res->Get(i)); delete res; }
                        }
                        response["result"] = arr;
                    } else if (action == "where") {
                        json arr = json::array();
                        Sequence<int>* res = nullptr;
                        if (currentSeqType == "ArraySequence") {
                            auto* seq = static_cast<ArraySequence<int>*>(activeIntSeq);
                            if (func == "IsEven") res = seq->Where(IsEven);
                            else if (func == "IsPositive") res = seq->Where(IsPositive);
                            else if (func == "IsDivBy3") res = seq->Where(IsDivBy3);
                        } else if (currentSeqType == "ListSequence") {
                            auto* seq = static_cast<ListSequence<int>*>(activeIntSeq);
                            if (func == "IsEven") res = seq->Where(IsEven);
                            else if (func == "IsPositive") res = seq->Where(IsPositive);
                            else if (func == "IsDivBy3") res = seq->Where(IsDivBy3);
                        }
                        if (res) { for (size_t i = 0; i < res->GetLength(); ++i) arr.push_back(res->Get(i)); delete res; }
                        response["result"] = arr;
                    } else if (action == "reduce") {
                        int initVal = valueStr.empty() ? 0 : std::stoi(valueStr);
                        Sequence<int>* res = nullptr;
                        if (currentSeqType == "ArraySequence") {
                            auto* seq = static_cast<ArraySequence<int>*>(activeIntSeq);
                            if (func == "SumFunc") res = seq->Reduce(SumFunc, initVal);
                            else if (func == "ProdFunc") res = seq->Reduce(ProdFunc, initVal);
                        } else if (currentSeqType == "ListSequence") {
                            auto* seq = static_cast<ListSequence<int>*>(activeIntSeq);
                            if (func == "SumFunc") res = seq->Reduce(SumFunc, initVal);
                            else if (func == "ProdFunc") res = seq->Reduce(ProdFunc, initVal);
                        }
                        if (res) { response["result"] = res->GetFirst(); delete res; }
                    } else if (action == "getFirstPred" || action == "getLastPred") {
                        bool (*pred)(int) = nullptr;
                        if (func == "IsEven") pred = IsEven;
                        else if (func == "IsPositive") pred = IsPositive;
                        else if (func == "IsDivBy3") pred = IsDivBy3;
                        if (currentSeqType == "ArraySequence") {
                            auto* seq = static_cast<ArraySequence<int>*>(activeIntSeq);
                            auto opt = action == "getFirstPred" ? seq->GetFirst(pred) : seq->GetLast(pred);
                            if (opt.HasValue()) response["result"] = opt.GetValue();
                            else response["result"] = "Not found";
                        } else if (currentSeqType == "ListSequence") {
                            auto* seq = static_cast<ListSequence<int>*>(activeIntSeq);
                            auto opt = action == "getFirstPred" ? seq->GetFirst(pred) : seq->GetLast(pred);
                            if (opt.HasValue()) response["result"] = opt.GetValue();
                            else response["result"] = "Not found";
                        }
                    }
                } else {
                    response["result"] = "Predicates not supported for this type in demo";
                }
            }
        } catch (const std::exception& e) {
            response["status"] = "error";
            response["message"] = e.what();
        }
        response["all"] = GetAllElements();
        res.set_content(response.dump(), "application/json");
    });

    svr.listen("0.0.0.0", 8080);
    return 0;
}