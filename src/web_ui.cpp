#include <WebServer.h>
extern WebServer server;

void handleRoot() {
    server.send(200, "text/html",
R"=====(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>TennoPrinter</title>

<!-- three.js + OrbitControls -->
<script src="https://unpkg.com/three@0.125.0/build/three.min.js"></script>
<script src="https://unpkg.com/three@0.125.0/examples/js/controls/OrbitControls.js"></script>

<style>
:root{--bg:#0b0d10;--card:#0f1316;--accent:#00d4ff;--muted:#9aa6b2;--panel:#0b0f13;}
html,body{height:100%;margin:0;font-family:Inter,Segoe UI,Arial,sans-serif;background:var(--bg);color:#e6eef3;}
.header{padding:16px 24px;background:linear-gradient(180deg,rgba(0,0,0,0.25),transparent);border-bottom:1px solid rgba(255,255,255,0.03);display:flex;align-items:center;gap:12px;}
.logo{font-weight:700;color:var(--accent);font-size:22px;text-shadow:0 0 8px rgba(0,212,255,0.08);}
.container{display:grid;grid-template-columns:420px 1fr;gap:18px;max-width:1200px;margin:22px auto;padding:0 18px;}
.card{background:var(--card);padding:14px;border-radius:12px;box-shadow:0 6px 18px rgba(0,0,0,0.6);border:1px solid rgba(255,255,255,0.03);}
.title{color:var(--accent);font-size:16px;margin:0 0 8px;}
.table{width:100%;border-collapse:collapse;}
.table th,.table td{padding:8px 6px;text-align:left;border-bottom:1px solid rgba(255,255,255,0.02);font-size:13px;color:#dfeaf2;}
.table th{color:var(--accent);font-weight:600;}
.actions button{margin-right:6px;background:transparent;border:1px solid rgba(255,255,255,0.03);color:var(--accent);padding:6px 8px;border-radius:8px;cursor:pointer;}
.actions button:hover{background:var(--accent);color:#071018;}
.uploadRow{display:flex;gap:8px;align-items:center;}
.progress{width:100%;background:#081014;height:12px;border-radius:8px;overflow:hidden;margin-top:8px;border:1px solid rgba(255,255,255,0.02);}
.bar{height:100%;width:0%;background:linear-gradient(90deg,var(--accent),#5fe9ff);box-shadow:0 0 12px rgba(0,212,255,0.06);}
#console{background:#031016;border-radius:8px;padding:10px;height:180px;overflow:auto;font-family:monospace;color:#9ef2ff;border:1px solid rgba(0,212,255,0.06);}
.previewWrap{display:flex;flex-direction:column;gap:12px;height:700px;}
#preview{flex:1;background:#000;border-radius:8px;border:1px solid rgba(255,255,255,0.03);}
.select,.btn{padding:8px 10px;border-radius:8px;background:transparent;border:1px solid rgba(255,255,255,0.03);color:var(--accent);cursor:pointer;}
.btn:hover{background:var(--accent);color:#061016;}
.footer{margin-top:18px;font-size:12px;color:var(--muted);text-align:center;}
.small{font-size:13px;color:var(--muted);}

.drop-area {
    border: 2px dashed rgba(0,212,255,0.3);
    border-radius: 8px;
    padding: 40px;
    text-align: center;
    margin-bottom: 12px;
    cursor: pointer;
    background: rgba(0,212,255,0.05);
    transition: all 0.3s ease;
}
.drop-area:hover {
    background: rgba(0,212,255,0.1);
    border-color: var(--accent);
}
.drop-area.highlight {
    background: rgba(0,212,255,0.15);
    border-color: var(--accent);
}
.file-info {
    margin-top: 10px;
    padding: 10px;
    background: rgba(0,212,255,0.05);
    border-radius: 8px;
    border: 1px solid rgba(0,212,255,0.1);
}
.tab-container {
    display: flex;
    gap: 8px;
    margin-bottom: 12px;
}
.tab {
    padding: 8px 16px;
    background: transparent;
    border: 1px solid rgba(255,255,255,0.03);
    border-radius: 8px;
    color: var(--muted);
    cursor: pointer;
}
.tab.active {
    background: rgba(0,212,255,0.1);
    color: var(--accent);
    border-color: var(--accent);
}
.tab-content {
    display: none;
}
.tab-content.active {
    display: block;
}
</style>
</head>

<body>

<div class="header">
  <img style="width: 6vh; border-radius: 100px;" src="https://avatars.githubusercontent.com/u/131226056?v=4" alt="">
  <div class="logo">TennoPrinter</div><div>
  </div>
</div>

<div class="container">

  <div>
    <div class="card">
      <div class="tab-container">
        <div class="tab active" onclick="switchTab('local')"> Locales</div>
        <div class="tab" onclick="switchTab('server')"> ESP32 SD</div>
      </div>
      
      <div id="localTab" class="tab-content active">
        <div class="title">Archivos Locales</div>
        <button class="btn" onclick="loadLocalFiles()">Actualizar Lista</button>
        <table class="table" style="margin-top:8px">
          <thead><tr><th>Nombre</th><th>Tamaño</th><th>Acciones</th></tr></thead>
          <tbody id="localFileList"><tr><td colspan="3">No hay archivos cargados</td></tr></tbody>
        </table>
      </div>
      
      <div id="serverTab" class="tab-content">
        <div class="title">Archivos en SD del ESP32</div>
        <button class="btn" onclick="loadServerFiles()">Actualizar</button>
        <table class="table" style="margin-top:8px">
          <thead><tr><th>Nombre</th><th>Tamaño</th><th>Acciones</th></tr></thead>
          <tbody id="serverFileList"><tr><td colspan="3">Cargando...</td></tr></tbody>
        </table>
      </div>
    </div>

    <div class="card" style="margin-top:12px;">
      <div class="title">⬆ Subir Archivo al ESP32</div>
      
      <div id="dropArea" class="drop-area" onclick="document.getElementById('fileInput').click()">
        <div style="color: var(--accent); margin-bottom: 8px; font-size: 18px;">
           Arrastra y suelta archivos aquí
        </div>
        <div class="small">o haz clic para seleccionar archivos G-code</div>
        <div class="small" style="margin-top: 5px;">(.gcode, .txt, .gco, .nc)</div>
      </div>
      
      <div class="uploadRow">
        <input id="fileInput" type="file" accept=".gcode,.txt,.gco,.nc,.gc" 
               style="display: none;" multiple onchange="handleFileSelect(this.files)">
        <button class="btn" onclick="document.getElementById('fileInput').click()">Seleccionar</button>
        <button class="btn" onclick="uploadToESP32()">Subir al ESP32</button>
        <button class="btn" onclick="clearLocalFiles()">Limpiar</button>
      </div>
      
      <div id="fileInfo" class="file-info" style="display: none;">
        <div style="display: flex; justify-content: space-between; align-items: center;">
          <div>
            <div class="small"><strong id="selectedFileName">Ningún archivo seleccionado</strong></div>
            <div class="small" id="selectedFileSize"></div>
          </div>
          <div>
            <button class="btn" onclick="viewSelectedFile()" style="padding: 4px 8px; font-size: 12px;">Ver 3D</button>
            <button class="btn" onclick="printSelectedFile()" style="padding: 4px 8px; font-size: 12px;">🖨 Imprimir</button>
          </div>
        </div>
      </div>
      
      <div class="progress">
        <div id="uploadBar" class="bar"></div>
      </div>
    </div>

    <div class="card" style="margin-top:12px;">
      <div class="title"> Consola</div>
      <div id="console"></div>
    </div>
  </div>

  <div>
    <div class="card previewWrap">
      <div style="display:flex;justify-content:space-between;align-items:center;">
        <div class="title">Preview 3D</div>
        <div>
          <select id="fileSelect" class="select" onchange="onSelectFile()">
            <option value=''>Seleccionar archivo...</option>
          </select>
          <button class="btn" id="toggleRotate" onclick="toggleRotate()">Pausar</button>
        </div>
      </div>

      <div id="preview"></div>

      <div style="display:flex;gap:12px;align-items:center;">
        <div style="flex:1;">
          <div class="small">Puntos: <span id="pointsText">0</span></div>
          <div class="small">Tamaño: <span id="sizeText">0 x 0 x 0 mm</span></div>
          <div class="progress">
            <div id="progressBar" class="bar"></div>
          </div>
        </div>
        <div style="width:180px;text-align:right;">
          <button class="btn" onclick="deleteCurrentFile()">🗑 Eliminar</button>
          <button class="btn" onclick="printCurrentFile()">🖨 Imprimir</button>
        </div>
      </div>

    </div>

    <div class="footer">G-code convertido a líneas 3D en el navegador - Conectado al ESP32</div>
  </div>

</div>

<script>
function e(q){return document.querySelector(q);}

let localFiles = [];
let serverFiles = [];
let currentFile = null;
let currentFileSource = 'local';
let currentFileName = '';

function switchTab(tabName) {
    document.querySelectorAll('.tab').forEach(tab => {
        tab.classList.remove('active');
    });
    document.querySelectorAll('.tab-content').forEach(content => {
        content.classList.remove('active');
    });
    
    if (tabName === 'local') {
        document.querySelector('.tab:nth-child(1)').classList.add('active');
        e('#localTab').classList.add('active');
    } else {
        document.querySelector('.tab:nth-child(2)').classList.add('active');
        e('#serverTab').classList.add('active');
        loadServerFiles();
    }
}

function handleFileSelect(files) {
    if (!files || files.length === 0) return;
    
    const dropArea = e('#dropArea');
    dropArea.classList.remove('highlight');
    
    for (let i = 0; i < files.length; i++) {
        const file = files[i];
        
        const existingIndex = localFiles.findIndex(f => f.name === file.name && f.size === file.size);
        if (existingIndex === -1) {
            localFiles.push(file);
            console.log("Archivo agregado:", file.name, file.size);
        }
    }
    
    updateLocalFileList();
    updateFileSelect();
    
    if (localFiles.length > 0) {
        showFileInfo(localFiles[0], 'local');
    }
}

function showFileInfo(file, source = 'local') {
    const fileInfo = e('#fileInfo');
    const fileName = e('#selectedFileName');
    const fileSize = e('#selectedFileSize');
    
    fileName.textContent = file.name;
    fileSize.textContent = formatFileSize(file.size);
    fileInfo.style.display = 'block';
    
    currentFile = file;
    currentFileSource = source;
    currentFileName = file.name;
}

function updateLocalFileList() {
    const tbody = e('#localFileList');
    tbody.innerHTML = '';
    
    if (localFiles.length === 0) {
        tbody.innerHTML = "<tr><td colspan='3'>No hay archivos cargados</td></tr>";
        return;
    }
    
    localFiles.forEach((file, index) => {
        const tr = document.createElement('tr');
        tr.innerHTML = `
            <td>${file.name}</td>
            <td>${formatFileSize(file.size)}</td>
            <td class="actions">
                <button onclick="viewLocalFile(${index})">3D</button>
                <button onclick="downloadLocalFile(${index})">Descargar</button>
                <button onclick="removeLocalFile(${index})">Quitar</button>
            </td>
        `;
        tbody.appendChild(tr);
    });
}

function removeLocalFile(index) {
    localFiles.splice(index, 1);
    updateLocalFileList();
    updateFileSelect();
    
    if (localFiles.length > 0) {
        showFileInfo(localFiles[0], 'local');
    } else {
        e('#fileInfo').style.display = 'none';
        currentFile = null;
        currentFileSource = 'local';
        currentFileName = '';
    }
}

function clearLocalFiles() {
    localFiles = [];
    updateLocalFileList();
    updateFileSelect();
    e('#fileInfo').style.display = 'none';
    currentFile = null;
    currentFileSource = 'local';
    currentFileName = '';
}

function loadLocalFiles() {
    updateLocalFileList();
    updateFileSelect();
}

function downloadLocalFile(index) {
    if (index < 0 || index >= localFiles.length) return;
    
    const file = localFiles[index];
    const url = URL.createObjectURL(file);
    const a = document.createElement('a');
    a.href = url;
    a.download = file.name;
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
    
    appendConsole(`Archivo ${file.name} descargado\n`);
}

function loadServerFiles() {
    console.log("Solicitando lista de archivos del ESP32...");
    fetch('/files')
        .then(r => r.json())
        .then(arr => {
            console.log("Respuesta /files:", arr);
            serverFiles = arr;
            updateServerFileList();
        })
        .catch(err => {
            console.error("ERROR /files:", err);
            appendConsole("Error cargando archivos del ESP32\n");
            e('#serverFileList').innerHTML = "<tr><td colspan='3'>Error de conexión</td></tr>";
        });
}

function updateServerFileList() {
    const tbody = e('#serverFileList');
    tbody.innerHTML = '';
    
    if (serverFiles.length === 0) {
        tbody.innerHTML = "<tr><td colspan='3'>No hay archivos en la SD</td></tr>";
        return;
    }
    
    serverFiles.forEach(file => {
        const tr = document.createElement('tr');
        tr.innerHTML = `
            <td>${file.name}</td>
            <td>${formatFileSize(file.size)}</td>
            <td class="actions">
                <button onclick="viewServerFile('${file.name}')">3D</button>
                <button onclick="downloadServerFile('${file.name}')">Descargar</button>
                <button onclick="deleteServerFile('${file.name}')">Borrar</button>
                <button onclick="printServerFile('${file.name}')">🖨</button>
            </td>
        `;
        tbody.appendChild(tr);
    });
}

function viewServerFile(filename) {
    console.log("Cargando G-code del ESP32:", filename);
    appendConsole(`Cargando ${filename} del ESP32...\n`);
    e('#progressBar').style.width = '0%';
    
    fetch('/raw?file=' + encodeURIComponent(filename))
        .then(r => {
            if (!r.ok) throw new Error('Error ' + r.status);
            return r.text();
        })
        .then(txt => {
            console.log("G-code recibido. Longitud:", txt.length);
            appendConsole(`Archivo leído. Tamaño: ${formatFileSize(txt.length)}\n`);
            appendConsole("Parseando G-code...\n");
            
            currentFileSource = 'server';
            currentFileName = filename;
            
            buildFromGcode(txt, filename);
        })
        .catch(err => {
            console.error("ERROR /raw:", err);
            appendConsole("Error cargando archivo del ESP32\n");
        });
}

function downloadServerFile(filename) {
    const url = '/raw?file=' + encodeURIComponent(filename);
    const a = document.createElement('a');
    a.href = url;
    a.download = filename;
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    
    appendConsole(`Descargando ${filename} del ESP32\n`);
}

function deleteServerFile(filename) {
    if (!confirm(`¿Borrar ${filename} de la SD?`)) return;
    
    fetch('/delete?file=' + encodeURIComponent(filename))
        .then(() => {
            console.log("Archivo borrado:", filename);
            appendConsole(`${filename} borrado de la SD\n`);
            loadServerFiles();
        })
        .catch(err => {
            console.error("ERROR en delete:", err);
            appendConsole("Error borrando archivo\n");
        });
}

function printServerFile(filename) {
    console.log("Enviando archivo a imprimir:", filename);
    fetch('/print?file=' + encodeURIComponent(filename))
        .then(() => {
            appendConsole(`Imprimiendo ${filename}...\n`);
        })
        .catch(err => {
            console.error("ERROR /print:", err);
            appendConsole("Error enviando a imprimir\n");
        });
}

function uploadToESP32() {
    if (!currentFile || currentFileSource !== 'local') {
        alert("Selecciona un archivo local primero");
        return;
    }
    
    console.log("Subiendo archivo al ESP32:", currentFile.name, currentFile.size);
    
    const fd = new FormData();
    fd.append('file', currentFile);
    
    const xhr = new XMLHttpRequest();
    xhr.open('POST', '/upload');
    
    xhr.upload.onprogress = (ev) => {
        if (ev.lengthComputable) {
            let p = (ev.loaded / ev.total) * 100;
            e('#uploadBar').style.width = p + '%';
            console.log("Progreso upload:", p.toFixed(1), "%");
        }
    };
    
    xhr.onload = () => {
        console.log("Upload completado:", xhr.status);
        e('#uploadBar').style.width = '100%';
        appendConsole(`${currentFile.name} subido al ESP32\n`);
        
        setTimeout(() => {
            e('#uploadBar').style.width = '0%';
            loadServerFiles();
            switchTab('server');
        }, 1000);
    };
    
    xhr.onerror = () => {
        console.error("ERROR en upload");
        e('#uploadBar').style.width = '0%';
        appendConsole("Error subiendo archivo al ESP32\n");
    };
    
    xhr.send(fd);
}

function updateFileSelect() {
    const select = e('#fileSelect');
    select.innerHTML = '<option value="">Seleccionar archivo...</option>';
    
    localFiles.forEach((file, index) => {
        const option = document.createElement('option');
        option.value = `local:${index}`;
        option.textContent = `📁 ${file.name}`;
        select.appendChild(option);
    });
    
    serverFiles.forEach(file => {
        const option = document.createElement('option');
        option.value = `server:${file.name}`;
        option.textContent = `📂 ${file.name}`;
        select.appendChild(option);
    });
}

function onSelectFile() {
    const value = e('#fileSelect').value;
    if (!value) return;
    
    const [source, identifier] = value.split(':');
    
    if (source === 'local') {
        const index = parseInt(identifier);
        viewLocalFile(index);
    } else if (source === 'server') {
        viewServerFile(identifier);
    }
}

function viewLocalFile(index) {
    if (index < 0 || index >= localFiles.length) return;
    
    const file = localFiles[index];
    showFileInfo(file, 'local');
    readAndParseGcode(file);
}

function viewSelectedFile() {
    if (currentFile && currentFileSource === 'local') {
        readAndParseGcode(currentFile);
    } else if (currentFileSource === 'server') {
        viewServerFile(currentFileName);
    }
}

function printSelectedFile() {
    if (currentFileSource === 'local') {
        if (confirm("Subir archivo al ESP32 e imprimir?")) {
            uploadToESP32();
            setTimeout(() => {
                printServerFile(currentFile.name);
            }, 2000);
        }
    } else if (currentFileSource === 'server') {
        printServerFile(currentFileName);
    }
}

function readAndParseGcode(file) {
    if (!file) {
        appendConsole("Error: No hay archivo seleccionado\n");
        return;
    }
    
    appendConsole(`Cargando archivo: ${file.name}\n`);
    e('#progressBar').style.width = '0%';
    
    const reader = new FileReader();
    
    reader.onload = function(evt) {
        e('#progressBar').style.width = '100%';
        const gcodeText = evt.target.result;
        appendConsole(`Archivo leído. Tamaño: ${formatFileSize(gcodeText.length)}\n`);
        appendConsole("Parseando G-code...\n");
        
        buildFromGcode(gcodeText, file.name);
        
        setTimeout(() => {
            e('#progressBar').style.width = '0%';
        }, 1000);
    };
    
    reader.onerror = function() {
        appendConsole("Error al leer el archivo\n");
        e('#progressBar').style.width = '0%';
    };
    
    reader.readAsText(file);
}

document.addEventListener('DOMContentLoaded', function() {
    const dropArea = e('#dropArea');
    
    ['dragenter', 'dragover', 'dragleave', 'drop'].forEach(eventName => {
        dropArea.addEventListener(eventName, preventDefaults, false);
        document.body.addEventListener(eventName, preventDefaults, false);
    });
    
    dropArea.addEventListener('dragenter', handleDragOver, false);
    dropArea.addEventListener('dragover', handleDragOver, false);
    dropArea.addEventListener('dragleave', handleDragLeave, false);
    dropArea.addEventListener('drop', handleDrop, false);
    
    loadServerFiles();
});

function preventDefaults(e) {
    e.preventDefault();
    e.stopPropagation();
}

function handleDragOver(e) {
    e('#dropArea').classList.add('highlight');
}

function handleDragLeave(e) {
    e('#dropArea').classList.remove('highlight');
}

function handleDrop(e) {
    e('#dropArea').classList.remove('highlight');
    
    if (e.dataTransfer.files.length) {
        handleFileSelect(e.dataTransfer.files);
    }
}

let scene, camera, renderer, lineGroup, controls;
let autoRotate = true;

function init3D() {
    console.log("Inicializando Three.js...");
    const container = e('#preview');
    container.innerHTML = '';

    const W = container.clientWidth;
    const H = Math.max(400, container.clientHeight);

    renderer = new THREE.WebGLRenderer({antialias: true});
    renderer.setSize(W, H);
    renderer.setClearColor(0x000000, 1);
    container.appendChild(renderer.domElement);

    scene = new THREE.Scene();
    camera = new THREE.PerspectiveCamera(45, W/H, 0.1, 10000);
    camera.position.set(0, 200, 200);

    controls = new THREE.OrbitControls(camera, renderer.domElement);
    controls.enableDamping = true;

    scene.add(new THREE.AmbientLight(0xffffff, 0.9));

    lineGroup = new THREE.Group();
    scene.add(lineGroup);

    scene.add(new THREE.GridHelper(400, 40, 0x10323a, 0x0b1a1d));

    window.addEventListener('resize', onResize);
    animate();

    console.log("Three.js listo.");
}

function onResize() {
    const c = e('#preview');
    if(!renderer) return;
    const W = c.clientWidth;
    const H = Math.max(400, c.clientHeight);
    renderer.setSize(W, H);
    camera.aspect = W/H;
    camera.updateProjectionMatrix();
    camera.position.set(0, 200, 200);
}

function animate() {
    requestAnimationFrame(animate);
    controls.update();
    renderer.render(scene, camera);
}

function buildFromGcode(txt, filename = "archivo.gcode") {
    console.log("Parseando G-code...");
    if(!scene) init3D();
    if(lineGroup) scene.remove(lineGroup);

    lineGroup = new THREE.Group();
    scene.add(lineGroup);

    const lines = txt.split('\n');
    console.log("Líneas totales:", lines.length);
    
    e('#progressBar').style.width = '0%';

    let coords = [];
    let last = {x: 0, y: 0, z: 0};

    let minX = 1e9, minY = 1e9, minZ = 1e9;
    let maxX = -1e9, maxY = -1e9, maxZ = -1e9;

    let lineCount = 0;
    for(let L of lines){
        L = L.trim();
        if(!L) continue;
        
        lineCount++;
        if (lineCount % 1000 === 0) {
            const progress = (lineCount / lines.length) * 100;
            e('#progressBar').style.width = progress + '%';
        }

        if (/G0\s|G1\s|G0F|G1F|^G0$|^G1$/i.test(L)) {
            let x = null, y = null, z = null;

            const regex = /([XYZ])\s*(-?\d+(\.\d+)?)/gi;
            let m;
            while ((m = regex.exec(L)) !== null) {
                const axis = m[1].toUpperCase();
                const val = parseFloat(m[2]);
                if (axis === 'X') x = val;
                if (axis === 'Y') y = val;
                if (axis === 'Z') z = val;
            }

            if (x === null && y === null && z === null) continue;

            if (x === null) x = last.x;
            if (y === null) y = last.y;
            if (z === null) z = last.z;

            coords.push({x, y, z});
            last = {x, y, z};

            if (x < minX) minX = x;
            if (y < minY) minY = y;
            if (z < minZ) minZ = z;
            if (x > maxX) maxX = x;
            if (y > maxY) maxY = y;
            if (z > maxZ) maxZ = z;
        }
    }

    e('#progressBar').style.width = '100%';
    console.log("Puntos gcode encontrados:", coords.length);
    
    e('#pointsText').textContent = coords.length.toLocaleString();
    const dimX = (maxX - minX).toFixed(1);
    const dimY = (maxY - minY).toFixed(1);
    const dimZ = (maxZ - minZ).toFixed(1);
    e('#sizeText').textContent = `${dimX} x ${dimY} x ${dimZ} mm`;

    if (coords.length === 0) {
        appendConsole("No se encontraron coordenadas G0/G1 en el archivo\n");
        return;
    }

    const maxPoints = 120000;
    let step = 1;
    if(coords.length > maxPoints){
        step = Math.ceil(coords.length / maxPoints);
        console.log("Reduciendo puntos. Step =", step);
    }

    const geometry = new THREE.BufferGeometry();
    const positions = new Float32Array(Math.floor(coords.length / step) * 6);
    let idx = 0;

    for(let i = 1; i < coords.length; i += step){
        const a = coords[i-1], b = coords[i];
        positions[idx++] = a.x; positions[idx++] = a.y; positions[idx++] = a.z;
        positions[idx++] = b.x; positions[idx++] = b.y; positions[idx++] = b.z;
    }

    geometry.setAttribute('position', new THREE.BufferAttribute(positions, 3));
    const mat = new THREE.LineBasicMaterial({color: 0x00d4ff});
    const line = new THREE.LineSegments(geometry, mat);
    lineGroup.add(line);

    const dimXsize = maxX - minX;
    const dimYsize = maxY - minY;
    const dimZsize = maxZ - minZ;
    const maxDim = Math.max(dimXsize, dimYsize, dimZsize, 1);

    const scale = 160 / maxDim;
    lineGroup.scale.set(scale, scale, scale);

    console.log("Escala final:", scale);
    console.log("Dimensiones:", dimXsize.toFixed(1), dimYsize.toFixed(1), dimZsize.toFixed(1));

    appendConsole(`Preview listo. Puntos: ${coords.length.toLocaleString()}\n`);
    appendConsole(`Dimensiones: ${dimXsize.toFixed(1)} x ${dimYsize.toFixed(1)} x ${dimZsize.toFixed(1)} mm\n`);
    camera.position.set(0, 200, 200);
}

function formatFileSize(bytes) {
    if (bytes === 0) return '0 Bytes';
    const k = 1024;
    const sizes = ['Bytes', 'KB', 'MB', 'GB'];
    const i = Math.floor(Math.log(bytes) / Math.log(k));
    return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + ' ' + sizes[i];
}

function appendConsole(t) {
    const c = e('#console');
    c.innerHTML += t.replace(/\n/g, "<br>");
    c.scrollTop = c.scrollHeight;
}

function deleteCurrentFile() {
    if (currentFileSource === 'server' && currentFileName) {
        deleteServerFile(currentFileName);
    } else if (currentFileSource === 'local' && currentFile) {
        if (confirm(`¿Quitar ${currentFile.name} de la lista local?`)) {
            const index = localFiles.indexOf(currentFile);
            if (index !== -1) {
                removeLocalFile(index);
            }
        }
    }
}

function printCurrentFile() {
    if (currentFileSource === 'server' && currentFileName) {
        printServerFile(currentFileName);
    } else if (currentFileSource === 'local' && currentFile) {
        printSelectedFile();
    }
}

window.addEventListener('load', () => {
    console.log("Iniciando interfaz...");
    init3D();
    appendConsole("TennoPrinter listo - Conectado al ESP32\n");
    appendConsole("Usa las pestañas para cambiar entre archivos locales y del ESP32\n");
});
</script>
</body>
</html>
)====="
    );
}
