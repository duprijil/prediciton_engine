import ctypes
from flask import *

_dll = ctypes.CDLL('libengine.so')
engine = _dll.LoanPrediction_new()
_dll.LoanPrediction_loadWeights(engine)

app = Flask(__name__)

@app.route('/')
def hello_world():
    return 'Hey, we have Flask in a Docker container!'

@app.route('/json')
def json_get():
    data = request.get_json()
    result = _dll.LoanPrediction_predict(engine, ctypes.c_double(data['annual_inc']), ctypes.c_double(data['funded_amnt']), ctypes.c_double(data['funded_amnt_inv']), ctypes.c_double(data['installment']), ctypes.c_double(data['int_rate']) )
    return chr(result)

app.run(debug=True, host='0.0.0.0')
