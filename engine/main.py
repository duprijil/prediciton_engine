import ctypes
from flask import Flask, request
import json
_dll = ctypes.CDLL('/app/engine/libengine.so')
print('Hello')
engine = _dll.LoanPrediction_new()
print('HHeelloo')
_dll.LoanPrediction_loadWeights(engine)

app = Flask(__name__, static_url_path='/static')

@app.route('/')
def hello_world():
    return app.send_static_file('index.html')

@app.route('/scripts.js')
def js():
    return app.send_static_file('scripts.js')

@app.route('/style.css')
def css():
    return app.send_static_file('style.css')

@app.route('/json', methods=['POST'])
def json_get():
    data = request.get_json()
    #print(data)
    for key in data:
        data[key] = float(data[key])
    result = _dll.LoanPrediction_predict(engine, ctypes.c_double(data['annual_inc']), ctypes.c_double(data['funded_amnt']), ctypes.c_double(data['funded_amnt_inv']), ctypes.c_double(data['installment']), ctypes.c_double(data['int_rate']) )
    return chr(result)

app.run(debug=False, host='0.0.0.0')
