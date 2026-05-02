from flask import Flask, render_template, request, jsonify
import subprocess
import json
import os
import threading

app = Flask(__name__)
EXECUTABLE = os.path.abspath(os.path.join(os.path.dirname(__file__), 'build', 'Release', 'laba2_app.exe'))

class CppBridge:
    def __init__(self, exe_path):
        self.process = None
        self.lock = threading.Lock()
        
        if not os.path.exists(exe_path):
            raise FileNotFoundError(f"Исполняемый файл не найден: {exe_path}")
            
        try:
            self.process = subprocess.Popen(
                [exe_path],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                encoding='utf-8',
                errors='replace',
                bufsize=1
            )
        except Exception as e:
            raise RuntimeError(f"Не удалось запустить процесс C++: {e}")

    def send(self, cmd):
        with self.lock:
            if self.process is None or self.process.poll() is not None:
                return {"error": "Процесс C++ завершился. Перезапустите сервер."}
            try:
                self.process.stdin.write(cmd + '\n')
                self.process.stdin.flush()
                line = self.process.stdout.readline()
                if not line:
                    return {"error": "Процесс C++ неожиданно закрыл вывод."}
                return json.loads(line)
            except Exception as e:
                return {"error": "Ошибка взаимодействия с модулем C++."}

try:
    bridge = CppBridge(EXECUTABLE)
    print(f"[OK] Модуль C++ запущен. Путь: {EXECUTABLE}")
except Exception as e:
    print(f"[ERROR] {e}")
    bridge = None

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/create', methods=['POST'])
def create():
    if not bridge:
        return jsonify({"error": "Модуль C++ не загружен. Проверьте консоль сервера."}), 500
    data = request.json
    seq_type = data['type']
    elem_type = data.get('elemType', 'int')
    items = data.get('items', [])
    
    if seq_type == 'bit':
        elem_type = 'bit'
        
    items_str = ','.join(map(str, items))
    cmd = f"CREATE {seq_type} {elem_type} {items_str}"
    res = bridge.send(cmd)
    if 'error' not in res:
        res['type'] = seq_type
        res['elemType'] = elem_type
    return jsonify(res)

@app.route('/api/<int:sid>/append', methods=['POST'])
def append(sid):
    if not bridge: return jsonify({"error": "Модуль C++ не активен."}), 500
    return jsonify(bridge.send(f"APPEND {sid} {request.json['item']}"))

@app.route('/api/<int:sid>/prepend', methods=['POST'])
def prepend(sid):
    if not bridge: return jsonify({"error": "Модуль C++ не активен."}), 500
    return jsonify(bridge.send(f"PREPEND {sid} {request.json['item']}"))

@app.route('/api/<int:sid>/insert', methods=['POST'])
def insert(sid):
    if not bridge: return jsonify({"error": "Модуль C++ не активен."}), 500
    idx = request.json.get('index', 0) or 0
    return jsonify(bridge.send(f"INSERTAT {sid} {idx} {request.json['item']}"))

@app.route('/api/<int:sid>/get', methods=['GET'])
def get_item(sid):
    if not bridge: return jsonify({"error": "Модуль C++ не активен."}), 500
    idx = request.args.get('index', '0') or '0'
    if not idx.lstrip('-').isdigit():
        idx = '0'
    return jsonify(bridge.send(f"GET {sid} {idx}"))

@app.route('/api/<int:sid>/first', methods=['GET'])
def get_first(sid):
    if not bridge: return jsonify({"error": "Модуль C++ не активен."}), 500
    return jsonify(bridge.send(f"GETFIRST {sid}"))

@app.route('/api/<int:sid>/last', methods=['GET'])
def get_last(sid):
    if not bridge: return jsonify({"error": "Модуль C++ не активен."}), 500
    return jsonify(bridge.send(f"GETLAST {sid}"))

@app.route('/api/<int:sid>/show', methods=['GET'])
def show_sequence(sid):
    if not bridge: return jsonify({"error": "Модуль C++ не активен."}), 500
    return jsonify(bridge.send(f"SHOW {sid}"))

if __name__ == '__main__':
    app.run(debug=True, port=5000)