let sequences = [];

document.addEventListener('DOMContentLoaded', () => {
    console.log('[INIT] DOM loaded');
    const seqType = document.getElementById('seqType');
    const elemTypeGroup = document.getElementById('elemTypeGroup');
    if (!seqType || !elemTypeGroup) {
        alert('Ошибка: не найдены элементы формы. Проверьте HTML.');
        return;
    }
    seqType.addEventListener('change', () => {
        elemTypeGroup.style.display = seqType.value === 'bit' ? 'none' : 'flex';
    });
});

function updateSequencesList() {
    console.log('[UPDATE] Updating list, sequences:', sequences);
    const select = document.getElementById('seqId');
    if (!select) {
        console.error('[ERROR] Select element not found');
        return;
    }
    const currentSelected = select.value;
    select.innerHTML = '<option value="">-- Выберите --</option>';
    sequences.forEach(seq => {
        const opt = document.createElement('option');
        opt.value = seq.id;
        const type = seq.type || seq.container_type || '?';
        const elemType = seq.elemType || seq.elem_type || '?';
        const length = seq.length ?? 0;
        opt.textContent = `#${seq.id} (${type} | ${elemType}) [${length} эл.]`;
        select.appendChild(opt);
    });
    if (currentSelected && sequences.some(s => s.id == currentSelected)) {
        select.value = currentSelected;
    }
    console.log('[UPDATE] List updated, options count:', select.options.length);
}

async function apiCall(url, method = 'GET', body = null) {
    console.log(`[API] Calling ${method} ${url}`, body);
    try {
        const opts = { method, headers: { 'Content-Type': 'application/json' } };
        if (body) opts.body = JSON.stringify(body);
        const res = await fetch(url, opts);
        console.log(`[API] Response status:`, res.status);
        const data = await res.json();
        console.log(`[API] Response data:`, data);
        return data;
    } catch (e) {
        console.error(`[API] Exception:`, e);
        alert('Ошибка соединения: ' + e.message + '\n\nУбедитесь, что Flask сервер запущен (python app.py)');
        return { error: 'Ошибка соединения с сервером. Проверьте консоль (F12).' };
    }
}

function showResult(title, data = null) {
    console.log('[SHOW] showResult called with title:', title, 'data:', data);
    const el = document.getElementById('result');
    if (!el) {
        console.error('[ERROR] Result element not found!');
        alert('Ошибка: элемент result не найден в HTML');
        return;
    }
    
    const showJson = document.getElementById('showJson').checked;
    el.innerHTML = `<div class="result-header">${title}</div>`;

    if (!data) {
        el.innerHTML += `<div class="status error">⚠ Нет данных для отображения</div>`;
        console.log('[SHOW] No data, showing error');
        return;
    }

    if (showJson) {
        el.innerHTML += `<pre class="json-view">${JSON.stringify(data, null, 2)}</pre>`;
        console.log('[SHOW] Showing JSON');
        return;
    }

    if (data.error) {
        el.innerHTML += `<div class="status error">⚠ ${data.error}</div>`;
        console.log('[SHOW] Showing error:', data.error);
        return;
    }

    if (data.items !== undefined) {
        el.innerHTML += `<div class="seq-container">${renderSequenceItems(data.items)}</div>`;
        console.log('[SHOW] Showing sequence items');
        return;
    }

    if (data.result !== undefined) {
        el.innerHTML += `<div class="status success">✅ Значение: <strong>${data.result}</strong></div>`;
        console.log('[SHOW] Showing result value');
        return;
    }

    el.innerHTML += `<div class="status success">✅ Операция выполнена</div>`;
    console.log('[SHOW] Showing success');
}

function renderSequenceItems(items) {
    if (!items || items.length === 0) return '<div class="empty-state">Пустая последовательность</div>';
    let html = '<div class="items-row">';
    items.forEach((val, i) => {
        html += `<div class="item-box" style="animation-delay:${i*0.04}s">
                    <span class="item-idx">[${i}]</span>
                    <span class="item-val">${val}</span>
                 </div>`;
        if (i < items.length - 1) html += '<span class="conn-arrow">→</span>';
    });
    html += '</div>';
    return html;
}

async function createSequence() {
    console.log('[CREATE] createSequence called');
    try {
        const typeEl = document.getElementById('seqType');
        const elemTypeEl = document.getElementById('elemType');
        const itemsEl = document.getElementById('items');
        
        if (!typeEl || !elemTypeEl || !itemsEl) {
            alert('Ошибка: не найдены поля формы');
            return;
        }
        
        const type = typeEl.value;
        const elemType = elemTypeEl.value;
        const raw = itemsEl.value;
        
        console.log('[CREATE] Values:', { type, elemType, raw });
        
        const items = raw ? raw.split(',').map(s => s.trim()).filter(s => s !== '') : [];
        console.log('[CREATE] Parsed items:', items);

        const data = await apiCall('/api/create', 'POST', { type, elemType, items });
        console.log('[CREATE] API response:', data);

        if (!data) {
            alert('Сервер вернул пустой ответ');
            showResult('Ошибка создания', { error: 'Пустой ответ от сервера' });
            return;
        }

        if (data.error) {
            alert('Ошибка от сервера: ' + data.error);
            showResult('Ошибка создания', data);
            return;
        }

        if (!data.id) {
            alert('Сервер не вернул ID. Ответ: ' + JSON.stringify(data));
            showResult('Ошибка сервера', { error: 'Ответ без ID: ' + JSON.stringify(data) });
            return;
        }

        data.type = type;
        data.elemType = elemType;
        sequences.push(data);
        console.log('[CREATE] Added to sequences:', sequences);
        
        updateSequencesList();
        showResult(`Последовательность #${data.id} создана (тип: ${elemType})`, data);
        itemsEl.value = '';
        console.log('[CREATE] Success');
    } catch (e) {
        console.error('[CREATE] Exception:', e);
        alert('Критическая ошибка в createSequence: ' + e.message);
        showResult('Критическая ошибка', { error: e.message });
    }
}

async function append() {
    const id = document.getElementById('seqId').value;
    const item = document.getElementById('item').value;
    if (!id) return showResult('Ошибка', { error: 'Сначала выберите последовательность из списка' });
    const data = await apiCall(`/api/${id}/append`, 'POST', { item });
    if (data.error) return showResult('Ошибка добавления', data);
    updateLocalState(id, data);
    updateSequencesList();
    showResult(`Элемент добавлен в конец #${id}`, data);
}

async function prepend() {
    const id = document.getElementById('seqId').value;
    const item = document.getElementById('item').value;
    if (!id) return showResult('Ошибка', { error: 'Сначала выберите последовательность из списка' });
    const data = await apiCall(`/api/${id}/prepend`, 'POST', { item });
    if (data.error) return showResult('Ошибка добавления', data);
    updateLocalState(id, data);
    updateSequencesList();
    showResult(`Элемент добавлен в начало #${id}`, data);
}

async function insert() {
    const id = document.getElementById('seqId').value;
    let index = document.getElementById('index').value.trim();
    const item = document.getElementById('item').value;
    if (!id) return showResult('Ошибка', { error: 'Сначала выберите последовательность из списка' });
    if (index === '' || isNaN(index)) index = '0';
    const data = await apiCall(`/api/${id}/insert`, 'POST', { index, item });
    if (data.error) return showResult('Ошибка вставки', data);
    updateLocalState(id, data);
    updateSequencesList();
    showResult(`Элемент вставлен в #${id}`, data);
}

async function get() {
    const id = document.getElementById('seqId').value;
    let index = document.getElementById('index').value.trim();
    if (!id) return showResult('Ошибка', { error: 'Сначала выберите последовательность из списка' });
    if (index === '' || isNaN(index)) index = '0';
    const data = await apiCall(`/api/${id}/get?index=${index}`);
    if (data.error) return showResult('Ошибка получения', data);
    showResult(`Элемент по индексу ${index} из #${id}`, data);
}

async function getFirst() {
    const id = document.getElementById('seqId').value;
    if (!id) return showResult('Ошибка', { error: 'Сначала выберите последовательность из списка' });
    const data = await apiCall(`/api/${id}/first`);
    if (data.error) return showResult('Ошибка получения', data);
    showResult(`Первый элемент из #${id}`, data);
}

async function getLast() {
    const id = document.getElementById('seqId').value;
    if (!id) return showResult('Ошибка', { error: 'Сначала выберите последовательность из списка' });
    const data = await apiCall(`/api/${id}/last`);
    if (data.error) return showResult('Ошибка получения', data);
    showResult(`Последний элемент из #${id}`, data);
}

async function showSequence() {
    const id = document.getElementById('seqId').value;
    if (!id) return showResult('Ошибка', { error: 'Сначала выберите последовательность из списка' });
    const data = await apiCall(`/api/${id}/show`);
    if (data.error) return showResult('Ошибка отображения', data);
    showResult(`Содержимое последовательности #${id}`, data);
}

function updateLocalState(id, newData) {
    const seq = sequences.find(s => s.id == id);
    if (seq) { seq.length = newData.length; seq.items = newData.items; }
}