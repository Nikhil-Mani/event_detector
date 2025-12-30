from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/')
def hello_world():
    return '<h1>Hello World! This is my first Python Website.</h1>'


@app.route('/post', methods=['POST'])
def recv_data():
    data = request.get_json()
    if data is None:
        return jsonify({"error": "Invalid JSON"}), 400
    print(data)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5001, debug=True)