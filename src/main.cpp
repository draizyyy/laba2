#include "core/dynamic_array.hpp"
#include "core/linked_list.hpp"
#include "sequences/array_sequence.hpp"
#include "sequences/list_sequence.hpp"
#include "sequences/bit_sequence.hpp"
#include "option.hpp"
#include "exceptions.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

using namespace myLib;

struct SequenceWrapper {
    std::string container_type;
    std::string elem_type;
    void* ptr;
};

std::map<int, SequenceWrapper> sequences;
int nextId = 1;

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    return s.substr(start, s.find_last_not_of(" \t\r\n") - start + 1);
}

std::string escapeJson(const std::string& s) {
    std::string res;
    for (char c : s) {
        if (c == '"') res += "\\\"";
        else if (c == '\\') res += "\\\\";
        else res += c;
    }
    return res;
}

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> res;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delim)) {
        std::string t = trim(token);
        if (!t.empty()) res.push_back(t);
    }
    return res;
}

std::string toJson(int id, Sequence<int>* seq) {
    std::stringstream ss;
    ss << "{\"id\":" << id << ",\"length\":" << seq->GetLength() << ",\"items\":[";
    for (int i = 0; i < seq->GetLength(); ++i) {
        if (i > 0) ss << ",";
        ss << seq->Get(i);
    }
    ss << "]}";
    return ss.str();
}

std::string toJson(int id, Sequence<double>* seq) {
    std::stringstream ss;
    ss << "{\"id\":" << id << ",\"length\":" << seq->GetLength() << ",\"items\":[";
    for (int i = 0; i < seq->GetLength(); ++i) {
        if (i > 0) ss << ",";
        ss << seq->Get(i);
    }
    ss << "]}";
    return ss.str();
}

std::string toJson(int id, Sequence<std::string>* seq) {
    std::stringstream ss;
    ss << "{\"id\":" << id << ",\"length\":" << seq->GetLength() << ",\"items\":[";
    for (int i = 0; i < seq->GetLength(); ++i) {
        if (i > 0) ss << ",";
        ss << "\"" << escapeJson(seq->Get(i)) << "\"";
    }
    ss << "]}";
    return ss.str();
}

std::string toJson(int id, BitSequence<uint8_t>* seq) {
    std::stringstream ss;
    ss << "{\"id\":" << id << ",\"length\":" << seq->GetLength() << ",\"items\":[";
    for (int i = 0; i < seq->GetLength(); ++i) {
        if (i > 0) ss << ",";
        ss << static_cast<int>(seq->Get(i).GetValue());
    }
    ss << "]}";
    return ss.str();
}

int main() {
    std::string command;
    while (std::getline(std::cin, command)) {
        try {
            command = trim(command);
            if (command.empty()) continue;
            if (command == "EXIT") break;
            
            std::istringstream iss(command);
            std::string cmd;
            iss >> cmd;
            
            if (cmd == "CREATE") {
                std::string container, elem_type, itemsStr;
                iss >> container >> elem_type;
                std::getline(iss, itemsStr);
                itemsStr = trim(itemsStr);
                
                if (container == "bit") {
                    elem_type = "bit";
                    if (itemsStr.empty()) {
                        BitSequence<uint8_t>* seq = new BitSequence<uint8_t>();
                        sequences[nextId] = { "bit", "bit", seq };
                        std::cout << toJson(nextId, seq) << std::endl;
                        nextId++;
                        continue;
                    }
                    auto parts = split(itemsStr, ',');
                    std::vector<int> items;
                    for (auto& p : parts) items.push_back(std::stoi(p));
                    Bit<uint8_t>* bits = new Bit<uint8_t>[items.size()];
                    for (size_t i = 0; i < items.size(); ++i) bits[i] = Bit<uint8_t>(static_cast<uint8_t>(items[i]));
                    BitSequence<uint8_t>* seq = new BitSequence<uint8_t>(bits, static_cast<int>(items.size()));
                    delete[] bits;
                    sequences[nextId] = { "bit", "bit", seq };
                    std::cout << toJson(nextId, seq) << std::endl;
                    nextId++;
                    continue;
                }
                
                if (itemsStr.empty()) {
                    if (elem_type == "int") {
                        void* seq = (container == "array") ? static_cast<void*>(new ArraySequence<int>()) : static_cast<void*>(new ListSequence<int>());
                        sequences[nextId] = { container, "int", seq };
                        std::cout << toJson(nextId, static_cast<Sequence<int>*>(seq)) << std::endl;
                    } else if (elem_type == "double") {
                        void* seq = (container == "array") ? static_cast<void*>(new ArraySequence<double>()) : static_cast<void*>(new ListSequence<double>());
                        sequences[nextId] = { container, "double", seq };
                        std::cout << toJson(nextId, static_cast<Sequence<double>*>(seq)) << std::endl;
                    } else if (elem_type == "string") {
                        void* seq = (container == "array") ? static_cast<void*>(new ArraySequence<std::string>()) : static_cast<void*>(new ListSequence<std::string>());
                        sequences[nextId] = { container, "string", seq };
                        std::cout << toJson(nextId, static_cast<Sequence<std::string>*>(seq)) << std::endl;
                    } else {
                        std::cout << "{\"error\":\"Неизвестный тип элементов\"}" << std::endl;
                        continue;
                    }
                    nextId++;
                    continue;
                }
                
                auto parts = split(itemsStr, ',');
                
                if (elem_type == "int") {
                    std::vector<int> items;
                    for (auto& p : parts) items.push_back(std::stoi(p));
                    void* seq = (container == "array") ? static_cast<void*>(new ArraySequence<int>(items.data(), static_cast<int>(items.size()))) : static_cast<void*>(new ListSequence<int>(items.data(), static_cast<int>(items.size())));
                    sequences[nextId] = { container, "int", seq };
                    std::cout << toJson(nextId, static_cast<Sequence<int>*>(seq)) << std::endl;
                }
                else if (elem_type == "double") {
                    std::vector<double> items;
                    for (auto& p : parts) items.push_back(std::stod(p));
                    void* seq = (container == "array") ? static_cast<void*>(new ArraySequence<double>(items.data(), static_cast<int>(items.size()))) : static_cast<void*>(new ListSequence<double>(items.data(), static_cast<int>(items.size())));
                    sequences[nextId] = { container, "double", seq };
                    std::cout << toJson(nextId, static_cast<Sequence<double>*>(seq)) << std::endl;
                }
                else if (elem_type == "string") {
                    std::vector<std::string> items = parts;
                    void* seq = (container == "array") ? static_cast<void*>(new ArraySequence<std::string>(items.data(), static_cast<int>(items.size()))) : static_cast<void*>(new ListSequence<std::string>(items.data(), static_cast<int>(items.size())));
                    sequences[nextId] = { container, "string", seq };
                    std::cout << toJson(nextId, static_cast<Sequence<std::string>*>(seq)) << std::endl;
                }
                else {
                    std::cout << "{\"error\":\"Неизвестный тип элементов\"}" << std::endl;
                    continue;
                }
                nextId++;
            }
            
            else if (cmd == "APPEND" || cmd == "PREPEND" || cmd == "INSERTAT") {
                int id; iss >> id;
                if (sequences.find(id) == sequences.end()) { std::cout << "{\"error\":\"Последовательность не найдена\"}" << std::endl; continue; }
                auto& w = sequences[id];
                
                if (w.container_type == "bit") {
                    int val; iss >> val;
                    auto* seq = static_cast<BitSequence<uint8_t>*>(w.ptr);
                    Bit<uint8_t> b(static_cast<uint8_t>(val));
                    if (cmd == "APPEND") seq->Append(b);
                    else if (cmd == "PREPEND") seq->Prepend(b);
                    else { int idx; iss >> idx; seq->InsertAt(b, idx); }
                    std::cout << toJson(id, seq) << std::endl;
                    continue;
                }
                
                std::string valStr;
                int idx = -1;
                if (cmd == "INSERTAT") {
                    iss >> idx;
                    std::getline(iss, valStr);
                } else {
                    std::getline(iss, valStr);
                }
                valStr = trim(valStr);
                
                if (valStr.empty() && w.elem_type != "string") {
                    std::cout << "{\"error\":\"Пустое значение для числового типа\"}" << std::endl;
                    continue;
                }
                
                if (w.elem_type == "int") {
                    auto* seq = static_cast<Sequence<int>*>(w.ptr);
                    int val = std::stoi(valStr);
                    if (cmd == "APPEND") seq->Append(val);
                    else if (cmd == "PREPEND") seq->Prepend(val);
                    else seq->InsertAt(val, idx);
                    std::cout << toJson(id, seq) << std::endl;
                } else if (w.elem_type == "double") {
                    auto* seq = static_cast<Sequence<double>*>(w.ptr);
                    double val = std::stod(valStr);
                    if (cmd == "APPEND") seq->Append(val);
                    else if (cmd == "PREPEND") seq->Prepend(val);
                    else seq->InsertAt(val, idx);
                    std::cout << toJson(id, seq) << std::endl;
                } else if (w.elem_type == "string") {
                    auto* seq = static_cast<Sequence<std::string>*>(w.ptr);
                    if (cmd == "APPEND") seq->Append(valStr);
                    else if (cmd == "PREPEND") seq->Prepend(valStr);
                    else seq->InsertAt(valStr, idx);
                    std::cout << toJson(id, seq) << std::endl;
                }
            }
            
            else if (cmd == "GET" || cmd == "GETFIRST" || cmd == "GETLAST") {
                int id; iss >> id;
                if (sequences.find(id) == sequences.end()) { std::cout << "{\"error\":\"Последовательность не найдена\"}" << std::endl; continue; }
                auto& w = sequences[id];
                
                if (w.container_type == "bit") {
                    auto* seq = static_cast<BitSequence<uint8_t>*>(w.ptr);
                    try {
                        int res = 0;
                        if (cmd == "GET") { int idx; iss >> idx; res = static_cast<int>(seq->Get(idx).GetValue()); }
                        else if (cmd == "GETFIRST") res = static_cast<int>(seq->GetFirst().GetValue());
                        else res = static_cast<int>(seq->GetLast().GetValue());
                        std::cout << "{\"result\":" << res << "}" << std::endl;
                    } catch (...) { std::cout << "{\"error\":\"Индекс выходит за границы или последовательность пуста\"}" << std::endl; }
                    continue;
                }
                
                if (w.elem_type == "string") {
                    auto* seq = static_cast<Sequence<std::string>*>(w.ptr);
                    try {
                        std::string res;
                        if (cmd == "GET") { int idx; iss >> idx; res = seq->Get(idx); }
                        else if (cmd == "GETFIRST") res = seq->GetFirst();
                        else res = seq->GetLast();
                        std::cout << "{\"result\":\"" << escapeJson(res) << "\"}" << std::endl;
                    } catch (...) { std::cout << "{\"error\":\"Индекс выходит за границы или последовательность пуста\"}" << std::endl; }
                } else if (w.elem_type == "int") {
                    auto* seq = static_cast<Sequence<int>*>(w.ptr);
                    try {
                        if (cmd == "GET") { int idx; iss >> idx; std::cout << "{\"result\":" << seq->Get(idx) << "}" << std::endl; }
                        else if (cmd == "GETFIRST") std::cout << "{\"result\":" << seq->GetFirst() << "}" << std::endl;
                        else std::cout << "{\"result\":" << seq->GetLast() << "}" << std::endl;
                    } catch (...) { std::cout << "{\"error\":\"Индекс выходит за границы или последовательность пуста\"}" << std::endl; }
                } else if (w.elem_type == "double") {
                    auto* seq = static_cast<Sequence<double>*>(w.ptr);
                    try {
                        if (cmd == "GET") { int idx; iss >> idx; std::cout << "{\"result\":" << seq->Get(idx) << "}" << std::endl; }
                        else if (cmd == "GETFIRST") std::cout << "{\"result\":" << seq->GetFirst() << "}" << std::endl;
                        else std::cout << "{\"result\":" << seq->GetLast() << "}" << std::endl;
                    } catch (...) { std::cout << "{\"error\":\"Индекс выходит за границы или последовательность пуста\"}" << std::endl; }
                }
            }
            
            else if (cmd == "SHOW") {
                int id; iss >> id;
                if (sequences.find(id) == sequences.end()) { std::cout << "{\"error\":\"Последовательность не найдена\"}" << std::endl; continue; }
                auto& w = sequences[id];
                
                if (w.container_type == "bit") {
                    std::cout << toJson(id, static_cast<BitSequence<uint8_t>*>(w.ptr)) << std::endl;
                } else if (w.elem_type == "int") {
                    std::cout << toJson(id, static_cast<Sequence<int>*>(w.ptr)) << std::endl;
                } else if (w.elem_type == "double") {
                    std::cout << toJson(id, static_cast<Sequence<double>*>(w.ptr)) << std::endl;
                } else if (w.elem_type == "string") {
                    std::cout << toJson(id, static_cast<Sequence<std::string>*>(w.ptr)) << std::endl;
                }
            }
            
            else {
                std::cout << "{\"error\":\"Неизвестная команда\"}" << std::endl;
            }
        }
        catch (const std::exception&) {
            std::cout << "{\"error\":\"Неверный индекс или формат данных\"}" << std::endl;
        }
        catch (...) {
            std::cout << "{\"error\":\"Неизвестная ошибка\"}" << std::endl;
        }
    }
    
    for (auto& [id, w] : sequences) {
        if (w.elem_type == "int") {
            if (w.container_type == "array") delete static_cast<ArraySequence<int>*>(w.ptr);
            else delete static_cast<ListSequence<int>*>(w.ptr);
        } else if (w.elem_type == "double") {
            if (w.container_type == "array") delete static_cast<ArraySequence<double>*>(w.ptr);
            else delete static_cast<ListSequence<double>*>(w.ptr);
        } else if (w.elem_type == "string") {
            if (w.container_type == "array") delete static_cast<ArraySequence<std::string>*>(w.ptr);
            else delete static_cast<ListSequence<std::string>*>(w.ptr);
        } else if (w.elem_type == "bit") {
            delete static_cast<BitSequence<uint8_t>*>(w.ptr);
        }
    }
    return 0;
}