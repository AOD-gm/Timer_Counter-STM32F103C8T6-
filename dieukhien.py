import base64
import hashlib
import hmac
import os
import time
import urllib.parse

import requests
from dotenv import load_dotenv
from flask import Flask, jsonify, render_template_string, request

app = Flask(__name__)
load_dotenv()
CONNECTION_STRING = os.getenv("AZURE_IOT_HUB_CONNECTION_STRING")
DEVICE_ID = "ESP01"


def generate_sas_token(cs, device_id):
    cs_dict = dict(item.split("=", 1) for item in cs.split(";"))
    uri = cs_dict["HostName"]
    key_name = cs_dict["SharedAccessKeyName"]
    key = cs_dict["SharedAccessKey"]
    ttl = int(time.time()) + 3600
    sign_key = f"{urllib.parse.quote_plus(uri)}\n{ttl}"
    signature = base64.b64encode(
        hmac.new(
            base64.b64decode(key), sign_key.encode("utf-8"), hashlib.sha256
        ).digest()
    ).decode("utf-8")
    return (
        f"SharedAccessSignature sr={urllib.parse.quote_plus(uri)}&sig={urllib.parse.quote_plus(signature)}&se={ttl}&skn={key_name}",
        uri,
    )


def send_command_to_azure(command_str):
    try:
        token, hub_name = generate_sas_token(CONNECTION_STRING, DEVICE_ID)
        url = f"https://{hub_name}/devices/{DEVICE_ID}/messages/deviceBound?api-version=2020-03-13"
        headers = {"Authorization": token, "Content-Type": "text/plain"}
        response = requests.post(url, headers=headers, data=command_str)
        if response.status_code == 204:
            return True, f"Đã gửi lệnh thành công!"
        return False, f"Lỗi: {response.status_code}"
    except Exception as e:
        return False, str(e)


HTML_PAGE = """
<!DOCTYPE html>
<html lang="vi">
<head>
    <meta charset="UTF-8"><meta name="viewport" content="width=device-width, initial-scale=1.0"><title>Dashboard IoT</title>
    <style>
        body { font-family: sans-serif; background: #f0f2f5; display: flex; justify-content: center; padding: 20px; }
        .card { background: white; padding: 20px; border-radius: 15px; box-shadow: 0 4px 10px rgba(0,0,0,0.1); width: 100%; max-width: 400px; }
        .status-bar { padding: 10px; border-radius: 8px; text-align: center; font-weight: bold; margin-bottom: 20px; }
        .on { background: #d4edda; color: #155724; }
        .off { background: #f8d7da; color: #721c24; }
        .setting-row { display: flex; align-items: center; gap: 5px; margin-bottom: 15px; }
        input, select { flex: 2; padding: 8px; border: 1px solid #ccc; border-radius: 5px; }
        button { flex: 1; padding: 8px; border: none; border-radius: 5px; background: #007bff; color: white; cursor: pointer; }
    </style>
</head>
<body>
    <div class="card">
        <h2>🚀 ĐIỀU KHIỂN THIẾT BỊ</h2>
        <div id="statusIndicator" class="status-bar off">ĐÈN ĐANG TẮT</div>
        
        <div class="setting-row">
            <button onclick="toggleLight(1)">BẬT</button>
            <button onclick="toggleLight(0)">TẮT</button>
        </div>

        <div class="setting-row">
            <input type="time" id="time"><button onclick="sendSetting('T', 'time')">SET GIỜ</button>
        </div>
        <div class="setting-row">
            <input type="date" id="date"><button onclick="sendSetting('E', 'date')">SET NGÀY</button>
        </div>
        <div class="setting-row">
            <input type="time" id="alarm"><button onclick="sendSetting('A', 'alarm')">SET ALARM</button>
        </div>
        <div class="setting-row">
            <select id="mode">
                <option value="OPTIMISE">Optimise</option>
                <option value="NORMAL">Normal</option>
                <option value="SAVING">Saving</option>
            </select>
            <button onclick="sendSetting('MODE', 'mode')">SET MODE</button>
        </div>
    </div>
    <script>
        function toggleLight(state) {
            let el = document.getElementById('statusIndicator');
            el.className = 'status-bar ' + (state==1 ? 'on' : 'off');
            el.innerText = state==1 ? 'ĐÈN ĐANG SÁNG' : 'ĐÈN ĐANG TẮT';
            sendCommand('D:' + state);
        }
        function sendSetting(type, id) {
            let val = document.getElementById(id).value;
            if (!val) {
                alert("Vui lòng nhập đầy đủ thông tin!");
                return;
            }

            let formattedCmd = "";

            if (type === 'T') {
                formattedCmd = 'T:' + val + ':00'; 
            } 
            else if (type === 'E') {
                let parts = val.split('-'); // parts[0]=2026, parts[1]=05, parts[2]=31
                formattedCmd = 'E:' + parts[2] + parts[1] + parts[0]; 
            } 
            else if (type === 'A') {
                formattedCmd = 'A:' + val + ':00';
            } 
            else {
                formattedCmd = type + ':' + val;
            }

            sendCommand(formattedCmd);
        }
        function sendCommand(cmd) {
            fetch('/api/send', { method: 'POST', headers: {'Content-Type': 'application/json'}, body: JSON.stringify({command: cmd}) });
        }
    </script>
</body>
</html>
"""


@app.route("/")
def home():
    return render_template_string(HTML_PAGE)


@app.route("/api/send", methods=["POST"])
def api_send():
    cmd = request.json.get("command", "")
    is_success, msg = send_command_to_azure(cmd)
    return jsonify({"success": is_success, "message": msg})


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
